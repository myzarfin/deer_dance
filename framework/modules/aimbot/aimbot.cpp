#include "../../main.h"
float safe_to_aim(c_base_player* local_player, c_base_weapon* weapon)
{
	auto demo_melee = weapon->get_item_definition_index() == WPN_Sword || weapon->get_item_definition_index() == WPN_FestiveEyelander ||
					  weapon->get_item_definition_index() == WPN_Golfclub || weapon->get_item_definition_index() == WPN_ScottsSkullctter || weapon->get_item_definition_index() == WPN_Headless;

	if (weapon->get_slot() == 2 && !demo_melee) { return 8.4f; }
	if (weapon->get_slot() == 2 && demo_melee) { return 12.0f; }
	if (local_player->get_class_num() == TF2_Pyro && weapon->get_slot() == 0) { return 17.0f; }
	return 10000.0f;
}
void clamp_angle(vec3_t &angles) {
	while (angles[0] > 89) { angles[0] -= 180; }
	while (angles[0] < -89) { angles[0] += 180; }
	while (angles[1] > 180) { angles[1] -= 360; }
	while (angles[1] < -180) { angles[1] += 360; }
	while (angles[2] != 0) { angles[2] = 0; }
}

namespace aimbot {
	c_base_player* aimbot::aimbot_target = nullptr;

	void aimbot::do_aimbot() {
		c_base_player* local_player = global::local_player; c_user_cmd* command = global::command;

		vec3_t vec_old_angles = command->viewangles; float fl_old_forwardmove = command->forward_move, fl_old_sidemove = command->side_move;
		if (!vars::aimbot::enabled || !utilities::is_variable_key_pressed(vars::aimbot::aimbot_key) || command->buttons & IN_RELOAD) {
			return;
		}

		c_base_player* entity = get_best_target(local_player, command);
		if (!entity) { return; }

		c_base_weapon* weapon = local_player->get_active_weapon();
		if (!weapon) { return; }

		int item_index = weapon->get_item_definition_index();

		int best_hitbox = aimbot::get_best_hitbox(local_player, entity, false);
		if (best_hitbox == -1) { return; }

		vec3_t  vec_hitbox = utilities::get_hitbox_position(entity, best_hitbox),
				vec_eye_position = local_player->get_eye_position(), vec_eye_angles;

		auto size_of_position_adjustment = position_adjustment::ticks[entity->get_index()].size() - 1;

		if (entity->get_client_class()->class_id == 86 || entity->get_client_class()->class_id == 88 || entity->get_client_class()->class_id == 89) {
			if ((!vars::aimbot::aimbot_sentry && entity->get_client_class()->class_id == 88) ||
				(!vars::aimbot::aimbot_dispenser && entity->get_client_class()->class_id == 86) ||
				(!vars::aimbot::aimbot_teleporters && entity->get_client_class()->class_id == 89)) {
				return;
			}

			vec_hitbox = entity->get_world_space_center();

			aimbot::projectile_prediction(local_player, entity, weapon, vec_hitbox);
			if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), vec_hitbox)) {
				return;
			}
		}
		else if (entity->get_client_class()->class_id == 246) {
			aimbot::projectile_prediction(local_player, entity, weapon, vec_hitbox);
			if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), vec_hitbox)) {
				if (position_adjustment::ticks[entity->get_index()].empty()) { return; }
				if (utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) && !utilities::is_projectile(global::local_player, weapon)) {
					vec_hitbox = position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position;
				}
				else {
					return;
				}
			}

			if (item_index == WPN_Ambassador || item_index == WPN_FestiveAmbassador) {
				if (!(interfaces::globals->curtime - weapon->last_fire_time() >= 0.930)) {
					return;
				}
			}
		}

		aimbot::aimbot_target = entity;

		auto bullet_time = [&](c_base_player* local_player) -> bool {
			if (!local_player) return false;

			c_base_weapon* local_weapon = local_player->get_active_weapon();
			int tick_base = local_player->get_tick_base();
			if (!local_weapon || !tick_base) return false;

			float next_attack = local_weapon->get_next_attack();
			bool can_tickbase = next_attack <= TICKS_TO_TIME(tick_base);

			return can_tickbase;
		};
		auto mathematics = [&](vec3_t hitbox, vec3_t eye_position, vec3_t eye_angles, c_user_cmd* command, c_base_weapon* weapon, bool bullet_time_enabled) -> void {
			if (bullet_time_enabled) {
				if (bullet_time(local_player) && weapon->get_slot() != 2) {
					vec3_t length = hitbox - vec_eye_position;
					utilities::vector_angle(length, vec_eye_angles);
					clamp_angle(vec_eye_angles);

					command->viewangles = vec_eye_angles;
					if (!vars::aimbot::silent_aim) {
						interfaces::engineclient->set_view_angles(command->viewangles);
					}
				}
				else if (weapon->get_slot() == 2) {
					vec3_t length = hitbox - vec_eye_position;
					utilities::vector_angle(length, vec_eye_angles);
					clamp_angle(vec_eye_angles);

					command->viewangles = vec_eye_angles;
					if (!vars::aimbot::silent_aim) {
						interfaces::engineclient->set_view_angles(command->viewangles);
					}
				}
			}
			else {
				vec3_t length = hitbox - vec_eye_position;
				utilities::vector_angle(length, vec_eye_angles);
				clamp_angle(vec_eye_angles);

				command->viewangles = vec_eye_angles;
				if (!vars::aimbot::silent_aim) {
					interfaces::engineclient->set_view_angles(command->viewangles);
				}
			}
		}; 
		
		float server_time = global::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
		bool bullet_time_hehe = !(weapon->get_next_attack() > server_time);

		mathematics(vec_hitbox, vec_eye_position, vec_eye_angles, command, weapon, vars::aimbot::bullet_time);
		aimbot::fix_movement(command, vec_old_angles, fl_old_forwardmove, fl_old_sidemove);

		bool is_knife = (global::local_player->get_class_num() == TF2_Spy && weapon->get_slot() == 2);

		if (vars::aimbot::autoshoot && !is_knife) {
			command->buttons |= IN_ATTACK;
			if (command->buttons & IN_ATTACK && entity->get_client_class()->class_id == 246 && !utilities::is_projectile(global::local_player, weapon)) {
				if (vars::position_adjustment::enabled) {
					if (position_adjustment::ticks[entity->get_index()].empty()) { return; }
					if (vec_hitbox == position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) {
						command->tick_count = TIME_TO_TICKS(position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).simulation_time);
					}
				}
			} else if (command->buttons & IN_ATTACK && bullet_time_hehe) {
				*global::send_packet = false;
			} else {
				*global::send_packet = true;
			}
		}
		else if (vars::aimbot::autoshoot && is_knife) {
			if (weapon->ready_to_backstab()) {
				command->buttons |= IN_ATTACK;
			}

			if (command->buttons & IN_ATTACK && bullet_time_hehe) {
				*global::send_packet = false;
			} else {
				*global::send_packet = true;
			}
		}

		if (bullet_time_hehe && command->buttons & IN_ATTACK && entity->get_client_class()->class_id == 246) {
			global::shots_fired++;
		}
	}
	void aimbot::fix_movement(c_user_cmd* command, vec3_t old_angles, float old_fowardmove, float old_sidemove) {
		float delta = command->viewangles.y - old_angles.y, f1, f2;

		if (old_angles.y < 0.f) {
			f1 = 360.0f + old_angles.y;
		}
		else {
			f1 = old_angles.y;
		}

		if (command->viewangles.y < 0.0f) {
			f2 = 360.0f + command->viewangles.y;
		}
		else {
			f2 = command->viewangles.y;
		}

		if (f2 < f1) {
			delta = abs(f2 - f1);
		}
		else {
			delta = 360.0f - abs(f1 - f2);
		}
		delta = 360.0f - delta;

		command->forward_move = cos(DEG2RAD(delta)) * old_fowardmove + cos(DEG2RAD(delta + 90.f)) * old_sidemove;
		command->side_move = sin(DEG2RAD(delta)) * old_fowardmove + sin(DEG2RAD(delta + 90.f)) * old_sidemove;
	}

	c_base_player* aimbot::get_best_target(c_base_player* local_player, c_user_cmd* command) {
		float desired_fov = vars::aimbot::field_of_view, max_distance = 8192.0f, max_melee_distance = 8192.0f;
		vec3_t local_viewangles = local_player->get_eye_position();
		int best_target_index = -1;

		auto convert_idx_to_player = [&](int idx) -> c_base_player* {
			c_base_player* return_entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(idx));
			if (!return_entity) return nullptr; return return_entity;
		};

		for (int idx = 1; idx < interfaces::entitylist->get_highest_entity_index(); idx++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(idx));
			if (!entity || entity == local_player || !local_player) continue;
			if (entity->is_dormant() || entity->is_dead()) continue;

			if (entity->get_client_class()->class_id == 86 ||
				entity->get_client_class()->class_id == 88 ||
				entity->get_client_class()->class_id == 89 ||
				entity->get_client_class()->class_id == 246) {
				/*bool mp_friendlyfire = interfaces::convar->find_variable("mp_friendlyfire")->get_int() == 1;
				if (mp_friendlyfire == 0 && entity->get_team_num() == local_player->get_team_num() && entity->get_client_class()->class_id == 246) continue;*/

				if (entity->get_team_num() == local_player->get_team_num()) {
					continue;
				}

				c_base_weapon* local_weapon = local_player->get_active_weapon();
				if (!local_weapon) continue;

				auto size_of_position_adjustment = position_adjustment::ticks[entity->get_index()].size() - 1;
				int hitbox = get_best_hitbox(local_player, entity, false);
				if (hitbox == -1) { continue; }

				vec3_t aim_point = utilities::get_hitbox_position(entity, hitbox);
				if (entity->get_client_class()->class_id == 86 || entity->get_client_class()->class_id == 88 || entity->get_client_class()->class_id == 89) {
					if ((!vars::aimbot::aimbot_sentry && entity->get_client_class()->class_id == 88) ||
						(!vars::aimbot::aimbot_dispenser && entity->get_client_class()->class_id == 86) ||
						(!vars::aimbot::aimbot_teleporters && entity->get_client_class()->class_id == 89)) {
						continue;
					}

					aim_point = entity->get_world_space_center();

					if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), aim_point)) {
						continue;
					}
				}
				else if (entity->get_client_class()->class_id == 246) {
					if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), aim_point)) {
						if (position_adjustment::ticks[entity->get_index()].empty()) { continue; }
						if (utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position) && !utilities::is_projectile(global::local_player, local_weapon)) {
							aim_point = position_adjustment::ticks[entity->get_index()].at(size_of_position_adjustment).head_position;
						}
						else {
							continue;
						}
					}

					if (utilities::has_condition(entity, 32) || utilities::has_condition(entity, 16384)) {
						continue;
					}

					if (utilities::has_condition(entity, 16) || utilities::has_condition(entity, 8192)) {
						continue;
					}
				}

				auto get_distance = [&](vec3_t to, vec3_t from) -> float {
					vec3_t delta = to - from;
					float distance = ::sqrt(delta.length());
					if (distance < 1.0f) {
						distance = 1.0f;
					} return distance;
				};

				float new_best_distance = safe_to_aim(local_player, local_weapon);
				float real_distance = get_distance(aim_point, local_player->get_eye_position());

				vec3_t angles, eye_angles, length = aim_point - local_viewangles;
				utilities::vector_angle(aim_point, angles);
				clamp_angle(angles);
				interfaces::engineclient->get_view_angles(eye_angles);

				float distance_to_target = utilities::get_field_of_view(eye_angles, local_viewangles, aim_point);
				if (distance_to_target > desired_fov) {
					continue;
				}

				if (distance_to_target < max_distance && real_distance < new_best_distance) {
					max_distance = distance_to_target;
					new_best_distance = real_distance;
					best_target_index = idx;
				}

				if ((vars::aimbot::prefer_medic && entity->get_class_num() == TF2_Medic) && entity->get_client_class()->class_id == 246) {
					return convert_idx_to_player(best_target_index);
				}
			}
		} 
		return convert_idx_to_player(best_target_index);
	}
	int aimbot::get_best_hitbox(c_base_player* local_player, c_base_player* entity, bool is_a_building) {
		if (!local_player || !entity) return -1;

		auto local_weapon = local_player->get_active_weapon();
		if (!local_weapon) return -1;

		auto is_headshot_weapon = [&](c_base_player* local_player, c_base_weapon* local_weapon) {
			if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
				if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
					return true;

			if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
				return true;

			return false;
		};

		int best_hitbox = 0;
		switch (vars::aimbot::main_hitbox) {
			case 0: best_hitbox = 0; break;
			case 1: best_hitbox = 1; break;
			case 2: is_headshot_weapon(local_player, local_weapon) ? 1 : 3; break;
			case 3: {
				for (int hitbox = 0; hitbox < 17; hitbox++) {
					if (is_headshot_weapon(local_player, local_weapon)) {
						return best_hitbox;
					} else {
						return hitbox;
					}
				}
			} break;
			case 4: {
				for (int hitbox = 1; hitbox < 17; hitbox++) {
					return hitbox;
				}
			} break;
		}

		/*if (!utilities::is_entity_visible(local_player, entity, local_player->get_eye_position(), utilities::get_hitbox_position(entity, best_hitbox))) {
			return -1;
		}*/

		if (utilities::get_hitbox_position(entity, best_hitbox).is_zero()) {
			return -1;
		}

		return best_hitbox;
	}

	vec3_t aimbot::estimate_abs_velocity(c_base_player* entity) {
		typedef void(__thiscall * estimate_abs_velocity_fn)(c_base_player*, vec3_t &);
		static uintptr_t function = utilities::find_signature("client.dll", "E8 ? ? ? ? F3 0F 10 4D ? 8D 85 ? ? ? ? F3 0F 10 45 ? F3 0F 59 C9 56 F3 0F 59 C0 F3 0F 58 C8 0F 2F 0D ? ? ? ? 76 07") + 0x1;
		static uintptr_t estimate = ((*(PDWORD)(function)) + function + 4);

		estimate_abs_velocity_fn vel = (estimate_abs_velocity_fn)estimate;
		vec3_t v; vel(entity, v); return v;
	}

	void aimbot::projectile_prediction(c_base_player* local_player, c_base_player* entity, c_base_weapon* local_weapon, vec3_t& vec_hitbox) {
		auto item_index = local_weapon->get_item_definition_index();
		auto get_speed = [&local_player, &local_weapon, &entity, &item_index]() -> float {
			auto weapon_speed = 0.0f;
			switch (item_index) {
			case WPN_DirectHit:
				weapon_speed = 1980.0f; break;
			case WPN_BotRocketlauncherB:
			case WPN_BotRocketlauncherC:
			case WPN_BotRocketlauncherD:
			case WPN_BotRocketlauncherEG:
			case WPN_BotRocketlauncherES:
			case WPN_BotRocketlauncherG:
			case WPN_BotRocketlauncherR:
			case WPN_BotRocketlauncherS:
			case WPN_FestiveRocketLauncher:
			case WPN_NewRocketLauncher:
			case WPN_RocketLauncher:
			case WPN_Original:
			case WPN_Airstrike:
			case WPN_BlackBox:
				weapon_speed = 1100.0f; break;
			case WPN_FestiveFlaregun:
			case WPN_Flaregun:
				weapon_speed = 2000.0f; break;
			case WPN_SyringeGun:
			case WPN_NewSyringeGun:
			case WPN_Blutsauger:
			case WPN_Overdose:
				weapon_speed = 1000.0f; break;
			case WPN_RescueRanger:
			case WPN_Crossbow:
			case WPN_FestiveCrossbow:
				weapon_speed = 2400.0f; break;
			case WPN_GrenadeLauncher:
			case WPN_NewGrenadeLauncher:
			case WPN_FestiveGrenadeLauncher:
				weapon_speed = 1216.6f; break;
			case WPN_LochNLoad:
				weapon_speed = 1513.3f; break;
			case WPN_LoooseCannon:
				weapon_speed = 1453.9f; break;
			case WPN_IronBomber:
				weapon_speed = 1216.6f; break;
			default: weapon_speed = 0.0f; break;
			}
			return weapon_speed;
		};
		auto get_gravity = [&local_player, &local_weapon, &entity, &item_index]() -> float {
			auto weapon_gravity = 0.0f;
			switch (item_index) {
			case WPN_RescueRanger:
			case WPN_Crossbow:
			case WPN_FestiveCrossbow:
				weapon_gravity = 0.2f; break;
			case WPN_GrenadeLauncher:
			case WPN_NewGrenadeLauncher:
			case WPN_FestiveGrenadeLauncher:
			case WPN_LoooseCannon:
			case WPN_LochNLoad:
			case WPN_IronBomber:
				weapon_gravity = 0.4f; break;
			default: weapon_gravity = 0.0f; break;
			}
			return weapon_gravity;
		};
		auto is_projectile_weapon = [&local_player, &local_weapon, &entity, &item_index]() -> bool {
			auto local_class = local_player->get_class_num(), weapon_slot = local_weapon->get_slot();
			if (local_class == TF2_Demoman || local_class == TF2_Soldier || local_class == TF2_Medic) {
				if (weapon_slot == 0) {
					return true;
				}
			}

			if (local_class == TF2_Engineer) {
				if (weapon_slot == 0) {
					if (item_index == WPN_RescueRanger) {
						return true;
					}
				}
			}

			return false;
		};
		auto distance_to_ground = [&entity]() -> float {
			if (entity->get_flags() & FL_ONGROUND) return 0.0f;
			auto distance_to_ground = [&entity](vec3_t origin) -> float
			{
				trace_t ground_trace; ray_t ray;
				trace_filter filter; filter.skip = entity;
				vec3_t endpos = origin;

				endpos.z -= 8192;
				ray.init(origin, endpos);
				interfaces::enginetrace->trace_ray(ray, MASK_PLAYERSOLID, &filter, &ground_trace);
				return 8192.0f * ground_trace.fraction;
			};

			vec3_t origin = entity->get_abs_origin();
			float v1 = distance_to_ground(origin + vec3_t(10.0f, 10.0f, 0.0f));
			float v2 = distance_to_ground(origin + vec3_t(-10.0f, 10.0f, 0.0f));
			float v3 = distance_to_ground(origin + vec3_t(10.0f, -10.0f, 0.0f));
			float v4 = distance_to_ground(origin + vec3_t(-10.0f, -10.0f, 0.0f));
			return min(v1, min(v2, min(v3, v4)));
		};
		if (is_projectile_weapon() && vars::aimbot::projectile_aimbot) {
			bool on_ground = entity->get_flags() & FL_ONGROUND;
			if (local_player->get_class_num() == TF2_Medic || local_player->get_class_num() == TF2_Engineer || local_player->get_class_num() == TF2_Pyro)
				vec_hitbox = utilities::get_hitbox_position(entity, 4);
			else if (local_player->get_class_num() == TF2_Soldier || local_player->get_class_num() == TF2_Demoman) {
				vec_hitbox = entity->get_abs_origin();
				vec_hitbox[2] += 10.0f;
			}

			auto hitbox_pred = [&local_player, &entity, &on_ground](vec3_t hitbox, float speed, float gravity, float distance_to_ground) -> vec3_t {
				vec3_t result = hitbox, bestpos = result;

				auto vec_velocity = aimbot::estimate_abs_velocity(entity);
				auto medianTime = (local_player->get_eye_position().dist_to(result) / speed), range = 1.5f,
					currenttime = medianTime - range;

				if (currenttime <= 0.0f) currenttime = 0.01f;

				auto besttime = currenttime, mindelta = 65536.0f; auto maxsteps = 300;
				for (int steps = 0; steps < maxsteps; steps++, currenttime += ((float)(2 * range) / (float)maxsteps)) {
					vec3_t curpos = result; curpos += vec_velocity * currenttime;

					if (distance_to_ground > 0.0f) {
						curpos.z -= currenttime * currenttime * 400.0f * (utilities::has_condition(entity, 65536) ? 0.448f : 1.0f);
						if (curpos.z < result.z - distance_to_ground) {
							curpos.z = result.z - distance_to_ground;
						}
					}

					auto rockettime = (local_player->get_eye_position().dist_to(curpos) / speed);
					if (fabs(rockettime - currenttime) < mindelta) {
						besttime = currenttime; bestpos = curpos;
						mindelta = fabs(rockettime - currenttime);
					}
				}
				bestpos.z += (400 * besttime * besttime * gravity);
				return bestpos;
			};

			vec_hitbox = hitbox_pred(vec_hitbox, get_speed(), get_gravity(), distance_to_ground());
		}
	}
}	