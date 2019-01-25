#include "../../main.h"

namespace triggerbot {
	void triggerbot::do_triggerbot() {
		triggerbot::do_auto_shoot();
		triggerbot::do_auto_airblast();
		triggerbot::do_auto_detonate();
		triggerbot::do_auto_backstab();
	}
	void triggerbot::do_auto_shoot() {
		if (!vars::triggerbot::triggerbot_enabled ||
			(vars::triggerbot::triggerbot_on_key && !utilities::is_variable_key_pressed(vars::triggerbot::triggerbot_key))) {
			return;
		}

		c_base_weapon* weapon = global::local_player->get_active_weapon();
		if (!weapon) {
			return;
		}

		bool item_index = weapon->get_item_definition_index();
		bool is_knife = (global::local_player->get_class_num() == TF2_Spy && weapon->get_slot() == 2);

		if (is_knife) {
			return;
		}

		vec3_t aim_angles, forward;
		interfaces::engineclient->get_view_angles(aim_angles);

		utilities::angle_vectors(aim_angles, &forward);
		forward = forward * 9999 + global::local_player->get_eye_position();

		ray_t ray; trace_t trace;
		trace_filter filt; filt.skip = global::local_player;

		ray.init(global::local_player->get_eye_position(), forward);
		interfaces::enginetrace->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filt, &trace);

		if (!trace.ent || trace.hitgroup < 1 || trace.ent->get_team_num() == global::local_player->get_team_num()) {
			return;
		}

		if (utilities::has_condition(trace.ent, 32) || utilities::has_condition(trace.ent, 256) || utilities::has_condition(trace.ent, 16384)) {
			return;
		}

		if (utilities::has_condition(trace.ent, 16) || utilities::has_condition(trace.ent, 8192)) {
			return;
		}

		auto is_headshot_weapon = [&](c_base_player* local_player, c_base_weapon* local_weapon) {
			if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
				if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
					return true;

			if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
				return true;

			return false;
		};

		if (is_headshot_weapon(global::local_player, weapon) && trace.hitbox != 0) {
			return;
		}

		global::command->buttons |= IN_ATTACK;
	}
	void triggerbot::do_auto_airblast() {
		if (!vars::triggerbot::triggerbot_enabled || 
			!vars::triggerbot::auto_airblast ||
			(vars::triggerbot::auto_airblast_on_key && !GetAsyncKeyState(VK_XBUTTON1))) {
			return;
		}

		if (global::local_player->is_dead() || !global::local_player) {
			return;
		}

		if (global::local_player->get_class_num() != TF2_Pyro || 
			global::local_player->get_active_weapon()->get_slot() != 0) {
			return;
		}

		for (int i = 1; i <= interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			c_base_weapon* weapon = global::local_player->get_active_weapon();

			if (!entity || !weapon) {
				continue;
			}

			if (entity->get_team_num() == global::local_player->get_team_num() || 
				global::local_player->is_dormant()) {
				continue;
			}

			if (!strcmp(entity->get_client_class()->name, "CTFProjectile_SentryRocket")
			 || !strcmp(entity->get_client_class()->name, "CTFProjectile_Arrow")
			 || !strcmp(entity->get_client_class()->name, "CTFProjectile_HealingBolt")
			 || !strcmp(entity->get_client_class()->name, "CTFProjectile_Jar")
			 || !strcmp(entity->get_client_class()->name, "CTFProjectile_JarMilk")
			 || !strcmp(entity->get_client_class()->name, "CTFProjectile_Throwable")
			 || !strcmp(entity->get_client_class()->name, "CTFProjectile_Rocket")) {
				vec3_t local_eye_position = global::local_player->get_eye_position();
				vec3_t entity_position = entity->get_world_space_center();
				vec3_t velocity = aimbot::estimate_abs_velocity(entity);

				c_net_channel* network = interfaces::engineclient->get_net_channel_info();
				if (!network) {
					continue;
				}

				float interpolation =
					interfaces::convar->find_variable("cl_interp_ration")->get_float() /
					interfaces::convar->find_variable("cl_updaterate")->get_float() *
					interfaces::globals->interval_per_tick;

				float average_latency = network->get_average_latency(0) + network->get_average_latency(1);
				float scale = interpolation + average_latency;
				vector_ma(entity_position, scale, velocity, entity_position);

				auto get_vector_distance = [&](vec3_t one, vec3_t two) {
					vec3_t delta = one - two;
					float distance = ::sqrt(delta.length());
					if (distance < 1.0f) {
						distance = 1.0f;
					} return distance;
				};

				float distance = get_vector_distance(local_eye_position, entity_position);
				if (distance > 185.f) {
					continue;
				}

				auto is_projectile_visible = [&](c_base_player* entity, vec3_t start, vec3_t end) {
					trace_t tr; ray_t ray; trace_filter filter;
					ray.init(start, end);
					interfaces::enginetrace->trace_ray(ray, 0x200400B, &filter, &tr);

					if (tr.ent == NULL) {
						if (tr.fraction == 1.0f) {
							return true;
						}
					} return (tr.ent->get_index() == entity->get_index());
				};

				if (!is_projectile_visible(entity, local_eye_position, entity->get_world_space_center())) {
					continue;
				}

				global::command->buttons |= IN_ATTACK2;
			}
		}
	}
	void triggerbot::do_auto_detonate() {
		if (!vars::triggerbot::triggerbot_enabled ||
			!vars::triggerbot::auto_detonate ||
			(vars::triggerbot::auto_detonate_on_key && !utilities::is_variable_key_pressed(vars::triggerbot::triggerbot_key))) {
			return;
		}

		auto is_local_grenade = [&](c_base_player* entity) {
			if (entity->get_client_class()->class_id != 216) return false;
			if (entity->get_pipe_type() != 1) return false;

			auto get_thrower = entity->get_thrower();
			if (!get_thrower) return false;

			auto entity_handle = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(get_thrower);
			if (!entity_handle) return false;

			if (entity_handle != global::local_player) return false;
			return true;
		};

		for (int i = 0; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* first_entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!first_entity) {
				continue;
			}

			vec3_t sticky_position = first_entity->get_world_space_center();
			if (is_local_grenade(first_entity)) {
				for (int player = 0; player < interfaces::engineclient->get_max_clients(); player++) {
					if (i == global::local_player->get_index()) { continue; }
					c_base_player* second_entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(player));
					if (!second_entity || second_entity->is_dormant() || second_entity->is_dead()) {
						continue;
					}
					if (second_entity->get_team_num() == global::local_player->get_team_num()) {
						continue;
					}

					if (utilities::has_condition(second_entity, 32) || utilities::has_condition(second_entity, 16384) || 
						utilities::has_condition(second_entity, 16) || utilities::has_condition(second_entity, 8192)) {
						continue;
					}

					auto get_distance = [&](vec3_t to, vec3_t from) -> float {
						vec3_t delta = to - from;
						float distance = ::sqrt(delta.length());

						if (distance < 1.0f) {
							distance = 1.0f;
						} 
						
						return distance;
					};

					auto is_bomb_visible = [&](c_base_player* first, c_base_player* second) {
						trace_t tr; ray_t ray; trace_filter filter;

						auto first_origin = first->get_abs_origin(),
							 second_origin = second->get_abs_origin();

						filter.skip = first;
						ray.init(first_origin, second_origin);
						interfaces::enginetrace->trace_ray(ray, MASK_SHOT, &filter, &tr);

						return (tr.ent == second);
					};

					if (get_distance(sticky_position, second_entity->get_abs_origin()) < 12.0f) {
						if (is_bomb_visible(first_entity, second_entity)) {
							global::command->buttons |= IN_ATTACK2;
						}
					}
				}
			}
		}
	}
	void triggerbot::do_auto_backstab() {
		if (!vars::triggerbot::triggerbot_enabled) {
			return;
		}

		if (!global::local_player || global::local_player->is_dead()) {
			return;
		}

		c_base_weapon* local_weapon = global::local_player->get_active_weapon();
		if (!local_weapon) {
			return;
		}

		bool item_index = local_weapon->get_item_definition_index();
		bool is_knife = (global::local_player->get_class_num() == TF2_Spy && local_weapon->get_slot() == 2);

		if (is_knife) {
			if (local_weapon->ready_to_backstab()) {
				global::command->buttons |= IN_ATTACK;
			}
		}
	}
}