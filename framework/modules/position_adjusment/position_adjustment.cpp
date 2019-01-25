#include "../../main.h"

namespace position_adjustment {
	std::vector<tick_record> position_adjustment::ticks[64];
	void position_adjustment::position_adjustment_start() {
		if (global::local_player == nullptr || global::command == nullptr) return;

		c_base_weapon* local_weapon = global::local_player->get_active_weapon();
		if (!local_weapon) { return; }

		for (int i = 0; i < interfaces::engineclient->get_max_clients(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dormant() || entity->get_life_state() != LIFE_ALIVE) continue;
			if (entity->get_team_num() == global::local_player->get_team_num()) continue;

			// Quick projectile check, no need to do it as it doesn't work.
			if (utilities::is_projectile(global::local_player, local_weapon)) {
				continue;
			}

			ticks[i].insert(ticks[i].begin(), tick_record{ entity->get_simulation_time(), utilities::get_hitbox_position(entity, 0) });

			if (ticks[i].size() > vars::position_adjustment::ticks) { 
				ticks[i].pop_back();
			}
		}
	}
	void position_adjustment::position_adjustment_calc() {
		if (global::local_player == nullptr || global::command == nullptr) return;

		vec3_t view_direction, new_view_direction;
		interfaces::engineclient->get_view_angles(view_direction);
		new_view_direction = utilities::angle_vector(view_direction);

		c_base_weapon* local_weapon = global::local_player->get_active_weapon();
		if (!local_weapon) { return; }

		int best_target_index = -1; float best_field_of_view = FLT_MAX;
		for (int i = 0; i < interfaces::engineclient->get_max_clients(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dormant() || entity->get_life_state() != LIFE_ALIVE) continue;
			if (entity->get_team_num() == global::local_player->get_team_num()) continue;

			if (ticks[i].empty() || utilities::is_projectile(global::local_player, local_weapon)) continue;

			float field_of_view_distance = utilities::distance_point_to_line(entity->get_eye_position(), global::local_player->get_eye_position(), new_view_direction);
			if (field_of_view_distance < best_field_of_view) {
				best_field_of_view = field_of_view_distance;
				best_target_index = i;
			}

			for (unsigned int t = 0; t < ticks[i].size(); t++) {
				if (!entity->setup_bones(ticks[i].at(t).bone_matrix, 128, 256, 0)) {
					continue;
				}
			}
		}

		float final_target_index = -1;
		if (best_target_index != -1) {
			for (unsigned int i = 0; i < ticks[best_target_index].size(); i++) {
				float field_of_view_distance = utilities::distance_point_to_line(ticks[best_target_index].at(i).head_position, global::local_player->get_eye_position(), new_view_direction);
				if (field_of_view_distance < best_field_of_view) {
					best_field_of_view = field_of_view_distance;
					final_target_index = ticks[best_target_index].at(i).simulation_time;
				}
			}

			if (final_target_index != -1) {
				if (global::command->buttons & IN_ATTACK) {
					global::command->tick_count = TIME_TO_TICKS(final_target_index);
				}
			}
		}
	}
	void position_adjustment::do_position_adjustment() {
		if (global::local_player == nullptr || global::command == nullptr || !vars::position_adjustment::enabled) {
			return;
		}

		position_adjustment_start();
		position_adjustment_calc();
	}
};