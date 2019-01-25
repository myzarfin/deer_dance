#include "../../main.h"

float player_alpha[64];
float building_alpha[64];
int spectator_count = 0;

colour_t rainbow() {
	static float rainbow_value = 0.0f;
	rainbow_value += .005;

	if (rainbow_value > 1.0f) {
		rainbow_value = 0.0f;
	}

	return colour_t::from_hsb(rainbow_value, 1.0f, 1.0f);
}

namespace visuals {
	void visuals::render_esp() {
		if (!global::local_player) {
			return;
		}

		if (vars::visuals::no_scope) {
			if (global::is_scoped && !global::local_player->is_dead()) {
				draw::draw_line(global::screen_width / 2, 0, global::screen_width / 2, global::screen_height, colour_t(0, 0, 0, 255));
				draw::draw_line(0, global::screen_height / 2, global::screen_width, global::screen_height / 2, colour_t(0, 0, 0, 255));
			}
		}

		if (vars::visuals::custom_crosshair) {
			int c_w = global::screen_width / 2,
				c_h = global::screen_height / 2;

			interfaces::surface->draw_set_colour_t(colour_t(0, 0, 0));
			interfaces::surface->draw_filled_rect(c_w - 4, c_h - 2, c_w - 4 + 8, c_h - 2 + 4);
			interfaces::surface->draw_filled_rect(c_w - 2, c_h - 4, c_w - 2 + 4, c_h - 4 + 8);

			interfaces::surface->draw_set_colour_t(colour_t(255, 255, 255));
			interfaces::surface->draw_filled_rect(c_w - 3, c_h - 1, c_w - 3 + 6, c_h - 1 + 2);
			interfaces::surface->draw_filled_rect(c_w - 1, c_h - 3, c_w - 1 + 2, c_h - 3 + 6);
		}

		for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead()) {
				continue;
			}
			
			if (!entity->is_dormant()) {
				if (utilities::has_condition(entity, 16) || utilities::has_condition(entity, 8192)) {
					continue;
				}

				if (utilities::has_condition(entity, 4)) {
					entity->set_condition(entity->get_condition() & ~4);
				}

				if (utilities::has_condition(entity, 8)) {
					entity->set_condition(entity->get_condition() & ~8);
				}
			}

			if (entity->is_dormant() && player_alpha[i] > 0) {
				player_alpha[i] -= .2f;
			} else if (!entity->is_dormant() && player_alpha[i] < 255) {
				player_alpha[i] = 255.0f;
			} clamp(player_alpha[i], 0.5f, 255.f);

			if (entity == global::local_player && !global::in_thirdperson) {
				continue;
			}

			if (utilities::is_developer(entity)) {
				create_player_esp(entity);
			}

			if (entity->get_team_num() == global::local_player->get_team_num() && entity != global::local_player) {
				if (vars::visuals::player_enemyonly) {
					continue;
				}
			}

			create_player_esp(entity);
		}

		for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant()) {
				continue;
			}

			if (entity->get_client_class()->class_id == 86 || entity->get_client_class()->class_id == 88 || entity->get_client_class()->class_id == 89) {
				if (entity->get_level() == -1 || entity->is_carried() || entity->is_placing()) {
					continue;
				}

				HANDLE entity_handle = entity->get_builder();
				if (!entity_handle) {
					continue;
				}

				c_base_player* entity_builder = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(entity_handle);
				if (!entity_builder) {
					continue;
				}

				if (entity->get_team_num() == global::local_player->get_team_num() && entity_builder != global::local_player && !utilities::is_developer(entity_builder)) {
					if (vars::visuals::building_enemyonly) {
						continue;
					}
				}

				create_building_esp(entity);
			}
		}

		for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant()) {
				continue;
			}

			create_item_esp(entity);
		}

		spectator_count = 0;
		for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || !entity->is_dead() || entity->is_dormant()) {
				continue;
			}

			if (!global::local_player || global::local_player->is_dead()) {
				continue;
			}

			auto do_spectator_list = [&]() -> void {
				player_info_t entity_info;
				if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info)) {
					return;
				}

				HANDLE obs_targ = entity->get_observer_target();
				if (!obs_targ) {
					return;
				}

				c_base_player* entity_handle = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(obs_targ);
				if (!entity_handle || entity_handle->is_dormant()) {
					return;
				}

				if (entity_handle == global::local_player && !entity_handle->is_dead()) {
					static auto sanitize = [](char * name) -> std::string {
						name[127] = '\0'; std::string tmp(name);
						if (tmp.length() > 20) {
							tmp.erase(20, (tmp.length() - 20));
							tmp.append("..");
						} return tmp;
					};
					
					char* name = entity->is_player_bot() ? ("bot %s", entity_info.name) : entity_info.name;

					std::string final_name = sanitize(name);
					std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);

					draw::draw_string(global::screen_width / 1.37,
									  spectator_count++ * 12, colour_t(255, 255, 255, 255), 
									  font::tahoma, final_name.c_str());
				}
			}; 
			
			if (vars::visuals::spectators) {
				do_spectator_list();
			}
		}
	}
	void visuals::create_player_esp(c_base_player* entity) {
		player_info_t entity_info;
		if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info)) {
			return;
		}

		if (!entity || !global::local_player) {
			return;
		}

		//if (vars::visuals::out_of_fov_arrows) {
		//	
		//	if (entity != global::local_player) {
		//		vec3_t client_viewangles, entity_hitbox = entity->get_world_space_center(), screen; float radius = 300.0f;
		//		int screen_width = 0.0f, screen_height = 0.0f;

		//		//if (world_to_screen(entity_hitbox, screen)) {
		//		//	return;
		//		//}

		//		interfaces::engineclient->get_view_angles(client_viewangles);
		//		interfaces::engineclient->get_screen_size(screen_width, screen_height);

		//		auto calc_angle = [&](vec3_t from, vec3_t to) {
		//			vec3_t return_value, subtracted = to - from;
		//			utilities::vector_angle(subtracted, return_value);
		//			return return_value;
		//		};

		//		const auto screen_center = vec3_t(screen_width / 2.f, screen_height / 2.f, 0);
		//		const auto rot = DEG2RAD(client_viewangles.y - calc_angle(global::local_player->get_eye_position(), entity_hitbox).y - 90);

		//		std::vector<vertex_t> vertices;
		//		vertices.push_back(vertex_t(vec2_t(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
		//		vertices.push_back(vertex_t(vec2_t(screen_center.x + cosf(rot + DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot + DEG2RAD(2)) * (radius - 16))));
		//		vertices.push_back(vertex_t(vec2_t(screen_center.x + cosf(rot - DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot - DEG2RAD(2)) * (radius - 16))));

		//		static auto new_alpha = 0.f; static auto plus_or_minus = false;
		//		if (new_alpha <= 0.f || new_alpha >= 255.f) plus_or_minus = !plus_or_minus;
		//		new_alpha += plus_or_minus ? (255.f * interfaces::globals->frametime) : -(255.f * interfaces::globals->frametime); new_alpha = clamp(new_alpha, 0.f, 255.f);
		//		colour_t arrow_colour = colour_t(255, 255, 255, new_alpha);

		//		if (utilities::is_developer(entity)) {
		//			arrow_colour = colour_t(200, 0, 200, new_alpha);
		//		}

		//		auto draw_textured_polygon = [&](int points, std::vector<vertex_t> vertices, colour_t colour) -> void {
		//			static int texture_id = interfaces::surface->create_new_texture_id(true);
		//			static unsigned char buf[4] = { 255, 255, 255, 255 };
		//			interfaces::surface->draw_set_texture_rgba(texture_id, buf, 1, 1);
		//			interfaces::surface->draw_set_colour_t(colour);
		//			interfaces::surface->draw_set_texture(texture_id);
		//			interfaces::surface->draw_textured_polygon(points, vertices.data());
		//		}; draw_textured_polygon(3, vertices, arrow_colour);
		//	}
		//}

		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();
		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
				right = flb.x,
				left = flb.x,
				bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		float x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		bool dormancy = entity->is_dormant();
		colour_t esp_colour = dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : 
										 colour_t(255, 255, 255, player_alpha[entity->get_index()]);

		if (utilities::is_developer(entity)) {
			esp_colour = colour_t(200, 0, 200);
			draw::draw_string_centered(x + (w / 2), vars::visuals::player_name ? y - 25 : y - 13, esp_colour, font::tahoma, "developer");
		}

		if (vars::visuals::player_box) {
			draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0, player_alpha[entity->get_index()]));
			draw::draw_outlined_rect(x, y, w, h, esp_colour);
		}

		if (vars::visuals::player_health) {
			auto health_to_colour = [&](c_base_player* entity, int current_health) {
				float percentage = (double)current_health / entity->get_max_health() * 100;
				if (percentage > 80) {
					return colour_t(0, 150, 0, player_alpha[entity->get_index()]);
				} else if (percentage <= 80 && percentage > 60) {
					return colour_t(200, 200, 0, player_alpha[entity->get_index()]);
				} else if (percentage <= 60 && percentage > 40) {
					return colour_t(200, 100, 0, player_alpha[entity->get_index()]);
				} else {
					return colour_t(200, 0, 0, player_alpha[entity->get_index()]);
				}	return colour_t(255, 255, 255, player_alpha[entity->get_index()]);
			};

			int health = clamp(entity->get_health(), 0, entity->get_max_health());
			int health_bar = h / entity->get_max_health() * health;
			int health_bar_delta = h - health_bar;

			int green = health * 2.55;
			int red = 255 - green;

			draw::draw_filled_rect	(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80, player_alpha[entity->get_index()]));
			draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0, player_alpha[entity->get_index()]));
			draw::draw_filled_rect	(x - 5, y + health_bar_delta, 2, health_bar, dormancy ? colour_t(91, 91, 91, player_alpha[entity->get_index()]) : health_to_colour(entity, health));
		}

		if (vars::visuals::player_name) {
			static auto sanitize = [](char * name) -> std::string {
				name[127] = '\0'; std::string tmp(name);
				if (tmp.length() > 20) {
					tmp.erase(20, (tmp.length() - 20));
					tmp.append("..");
				} return tmp;
			};

			char* name = entity->is_player_bot() ? ("bot %s", entity_info.name) : entity_info.name;

			std::string final_name = sanitize(name);
			std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);

			draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::tahoma, final_name.c_str());
		}

		if (vars::visuals::player_weapon) {
			c_base_weapon* entity_weapon = entity->get_active_weapon();
			if (!entity_weapon) { return; }
			std::string weapon_name = entity_weapon->get_name();

			auto replace_string = [&](std::string& input, const std::string& found, const std::string& desired) -> void {
				size_t index; index = input.find(found);
				while (index != std::string::npos) {
					input.replace(index, found.size(), desired);
					index = input.find(found, index + desired.size());
				}
			};

			auto trim_start = [&](std::string &string) -> void {
				string.erase(string.begin(), std::find_if(string.begin(), string.end(), [](int ch) {
					return (ch);
				}));
			};

			replace_string(weapon_name, "#", ""); replace_string(weapon_name, "_", " ");
			replace_string(weapon_name, "TF", ""); replace_string(weapon_name, "Weapon", "");
			replace_string(weapon_name, "tf weapon", ""); trim_start(weapon_name);

			draw::draw_string_centered(x + (w / 2) - 3, y + h, esp_colour, font::smallest_pixel, weapon_name.c_str());
		}

		if (vars::visuals::player_steamid) {
			draw::draw_string_centered(x + (w / 2), y + h + (vars::visuals::player_weapon ? 9 : 0), esp_colour, font::smallest_pixel, "%s", entity_info.guid);
		}

		if (vars::visuals::player_backtrack_visulization) {
			if (position_adjustment::ticks[entity->get_index()].empty()) {
				return;
			}

			for (unsigned int t = 0; t < position_adjustment::ticks[entity->get_index()].size(); t++) {
				vec3_t hitbox = position_adjustment::ticks[entity->get_index()].at(t).head_position, screen;
				if (world_to_screen(hitbox, screen)) {
					//draw::draw_string(screen[0], screen[1], esp_colour, font::smallest_pixel, "%i", t);
					draw::draw_outlined_rect(screen[0], screen[1], 2, 2, esp_colour);
					draw::draw_outlined_rect(screen[0] - 1, screen[1] - 1, 3, 3, colour_t(0, 0, 0, player_alpha[entity->get_index()]));
				}
			}
		}

		auto draw_my_epic_crosshair = [&](vec3_t screen) {
			interfaces::surface->draw_set_colour_t(colour_t(0, 0, 0));
			interfaces::surface->draw_filled_rect(screen[0] - 4, screen[1] - 2, screen[0] - 4 + 8, screen[1] - 2 + 4);
			interfaces::surface->draw_filled_rect(screen[0] - 2, screen[1] - 4, screen[0] - 2 + 4, screen[1] - 4 + 8);

			interfaces::surface->draw_set_colour_t(colour_t(255, 255, 255));
			interfaces::surface->draw_filled_rect(screen[0] - 3, screen[1] - 1, screen[0] - 3 + 6, screen[1] - 1 + 2);
			interfaces::surface->draw_filled_rect(screen[0] - 1, screen[1] - 3, screen[0] - 1 + 2, screen[1] - 3 + 6);
		};

		if (vars::visuals::projectile_prediction) {
			c_base_weapon* local_weapon = global::local_player->get_active_weapon();
			if (!local_weapon) {
				return;
			}

			bool is_projectile_weapon = utilities::is_projectile(global::local_player, local_weapon);
			if (is_projectile_weapon) {
				vec3_t predicted = entity->get_world_space_center(), screen;
				aimbot::projectile_prediction(global::local_player, entity, local_weapon, predicted);
				if (world_to_screen(predicted, screen)) {
					draw_my_epic_crosshair(screen);
				}
			}

			if (!is_projectile_weapon) {
				if (position_adjustment::ticks[entity->get_index()].empty()) {
					return;
				}

				vec3_t final_backtrackable_tick = position_adjustment::ticks[entity->get_index()].back().head_position, screen;
				if (world_to_screen(final_backtrackable_tick, screen)) {
					draw_my_epic_crosshair(screen);
				}
			}
		}

		if (vars::visuals::bones) {
			studiohdr_t* studio_model = interfaces::model_info->get_studio_model(entity->get_model());
			if (!studio_model) return;

			static matrix3x4_t bone_to_world[128];
			if (entity->setup_bones(bone_to_world, 128, 256, 0)) {
				for (int i = 0; i < studio_model->numbones; i++) {
					mstudiobone_t* bone = studio_model->get_bone(i);
					if (!bone || !(bone->flags & 256) || bone->parent == -1) {
						continue;
					}

					vec3_t bone_to_world_two = vec3_t(bone_to_world[bone->parent][0][3], bone_to_world[bone->parent][1][3], bone_to_world[bone->parent][2][3]);
					vec3_t bone_to_world_one = vec3_t(bone_to_world[i][0][3], bone_to_world[i][1][3], bone_to_world[i][2][3]); vec3_t bone_position_one, bone_position_two;

					if (!world_to_screen(bone_to_world_one, bone_position_one)) {
						continue;
					}

					if (!world_to_screen(bone_to_world_two, bone_position_two)) {
						continue;
					}

					draw::draw_line((int)bone_position_one.x, (int)bone_position_one.y, (int)bone_position_two.x, (int)bone_position_two.y, esp_colour);
				}
			}
		}

		if (vars::visuals::d_lights) {
			dlight_t* d_light = interfaces::efx->alloc_dlight(entity->get_index());
			d_light->colour_t.r = 0;
			d_light->colour_t.g = 135;
			d_light->colour_t.b = 255;

			d_light->colour_t.exponent = 5;

			d_light->radius = vars::visuals::d_lights_radius;
			d_light->key = entity->get_index();

			d_light->die = interfaces::globals->curtime + 0.05f;
			d_light->decay = d_light->radius / 5.0f;

			d_light->origin = entity->get_abs_origin() + vec3_t(0, 0, 2);
		}
	}
	void visuals::create_building_esp(c_base_player* entity) {
		if (!entity || !global::local_player) {
			return;
		}

		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();
		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
			right = flb.x,
			left = flb.x,
			bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		float x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		/*bool dormancy = entity->is_dormant();
		colour_t esp_colour = dormancy ? colour_t(91, 91, 91) :
										 colour_t(255, 255, 255);*/

		colour_t esp_colour = colour_t(255, 255, 255);

		HANDLE entity_handle = entity->get_builder();
		if (!entity_handle) {
			return;
		}

		c_base_player* entity_builder = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(entity_handle);
		if (!entity_builder) {
			return;
		}

		player_info_t builder;
		if (!interfaces::engineclient->get_player_info(entity_builder->get_index(), &builder)) {
			return;
		}

		if (utilities::is_developer(entity_builder)) {
			esp_colour = colour_t(200, 0, 200);
		}

		auto health_to_colour = [&](c_base_player* entity, int current_health) {
			float percentage = (double)current_health / entity->get_max_health() * 100;
			if (percentage > 80) {
				return colour_t(0, 150, 0);
			}
			else if (percentage <= 80 && percentage > 60) {
				return colour_t(200, 200, 0);
			}
			else if (percentage <= 60 && percentage > 40) {
				return colour_t(200, 100, 0);
			}
			else {
				return colour_t(200, 0, 0);
			}	return colour_t(255, 255, 255);
		};

		switch (entity->get_client_class()->class_id) {
			case 86: {
				if (vars::visuals::building_box) {
					draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x, y, w, h, esp_colour);
				}

				if (vars::visuals::building_health) {
					int health = clamp(entity->get_object_health(), 0, entity->get_object_max_health());
					int health_bar = h / entity->get_object_max_health() * health;
					int health_bar_delta = h - health_bar;

					int green = health * 2.55;
					int red = 255 - green;

					draw::draw_filled_rect(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80));
					draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0));
					draw::draw_filled_rect(x - 5, y + health_bar_delta, 2, health_bar, health_to_colour(entity, health));
				}

				if (utilities::is_developer(entity_builder)) {
					draw::draw_string_centered(x + (w / 2), vars::visuals::building_name ? y - 25 : y - 13, esp_colour, font::tahoma, "developer");
				}

				if (vars::visuals::building_name) {
					draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::tahoma, "dispenser");
				}

				if (vars::visuals::building_builder) {
					static auto sanitize = [](char * name) -> std::string {
						name[127] = '\0'; std::string tmp(name);
						if (tmp.length() > 20) {
							tmp.erase(20, (tmp.length() - 20));
							tmp.append("..");
						} return tmp;
					};

					char* name = builder.name; std::string final_name = sanitize(name);
					std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);

					draw::draw_string_centered(x + (w / 2) - 3, y + h, esp_colour, font::tahoma, final_name.c_str());
				}
			} break;
			case 88: {
				if (vars::visuals::building_box) {
					draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x, y, w, h, esp_colour);
				}

				if (vars::visuals::building_health) {
					int health = clamp(entity->get_object_health(), 0, entity->get_object_max_health());
					int health_bar = h / entity->get_object_max_health() * health;
					int health_bar_delta = h - health_bar;

					int green = health * 2.55;
					int red = 255 - green;

					draw::draw_filled_rect(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80));
					draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0));
					draw::draw_filled_rect(x - 5, y + health_bar_delta, 2, health_bar, health_to_colour(entity, health));
				}

				if (utilities::is_developer(entity_builder)) {
					draw::draw_string_centered(x + (w / 2), vars::visuals::building_name ? y - 25 : y - 13, esp_colour, font::tahoma, "developer");
				}

				if (vars::visuals::building_name) {
					draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::tahoma, "sentrygun");
				}

				if (vars::visuals::building_builder) {
					static auto sanitize = [](char * name) -> std::string {
						name[127] = '\0'; std::string tmp(name);
						if (tmp.length() > 20) {
							tmp.erase(20, (tmp.length() - 20));
							tmp.append("..");
						} return tmp;
					};

					char* name = builder.name; std::string final_name = sanitize(name);
					std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);

					draw::draw_string_centered(x + (w / 2) - 3, y + h, esp_colour, font::tahoma, final_name.c_str());
				}
			} break;
			case 89: {
				if (vars::visuals::building_box) {
					draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
					draw::draw_outlined_rect(x, y, w, h, esp_colour);
				}

				if (vars::visuals::building_health) {
					int health = clamp(entity->get_object_health(), 0, entity->get_object_max_health());
					int health_bar = h / entity->get_object_max_health() * health;
					int health_bar_delta = h - health_bar;

					int green = health * 2.55;
					int red = 255 - green;

					draw::draw_filled_rect(x - 6, y - 1, 4, h + 2, colour_t(80, 80, 80));
					draw::draw_outlined_rect(x - 6, y - 1, 4, h + 2, colour_t(0, 0, 0));
					draw::draw_filled_rect(x - 5, y + health_bar_delta, 2, health_bar, health_to_colour(entity, health));
				}

				if (utilities::is_developer(entity_builder)) {
					draw::draw_string_centered(x + (w / 2), vars::visuals::building_name ? y - 25 : y - 13, esp_colour, font::tahoma, "developer");
				}

				if (vars::visuals::building_name) {
					draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::tahoma, "teleporter");
				}

				if (vars::visuals::building_builder) {
					static auto sanitize = [](char * name) -> std::string {
						name[127] = '\0'; std::string tmp(name);
						if (tmp.length() > 20) {
							tmp.erase(20, (tmp.length() - 20));
							tmp.append("..");
						} return tmp;
					};

					char* name = builder.name; std::string final_name = sanitize(name);
					std::transform(final_name.begin(), final_name.end(), final_name.begin(), tolower);

					draw::draw_string_centered(x + (w / 2), y + h, esp_colour, font::tahoma, final_name.c_str());
				}
			} break;
			default: break;
		}
	}
	void visuals::create_item_esp(c_base_player* entity) {
		if (!entity || !global::local_player) {
			return;
		}

		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();
		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
			right = flb.x,
			left = flb.x,
			bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		float x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		colour_t esp_colour = colour_t(255, 255, 255);

		const char* model_name = entity->get_model_name();
		bool is_ammo = false, is_health = false, is_projectile = false;
		std::string name = "contact cervine";
		if (!strcmp(model_name, "models/items/medkit_small.mdl") ||
			!strcmp(model_name, "models/items/medkit_small_bday.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_medkit_small.mdl") ||
			!strcmp(model_name, "models/items/medkit_medium.mdl") ||
			!strcmp(model_name, "models/items/medkit_medium_bday.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_medkit_medium.mdl") ||
			!strcmp(model_name, "models/items/medkit_large.mdl") ||
			!strcmp(model_name, "models/items/medkit_large_bday.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_medkit_large.mdl") ||
			!strcmp(model_name, "models/items/plate.mdl") ||
			!strcmp(model_name, "models/items/plate_steak.mdl") ||
			!strcmp(model_name, "models/items/plate_robo_sandwich.mdl")) {
			name = "health"; is_health = true;
		}

		if (!strcmp(model_name, "models/items/ammopack_small.mdl") ||
			!strcmp(model_name, "models/items/ammopack_small_bday.mdl") ||
			!strcmp(model_name, "models/items/ammopack_medium.mdl") ||
			!strcmp(model_name, "models/items/ammopack_medium_bday.mdl") ||
			!strcmp(model_name, "models/items/ammopack_large.mdl") ||
			!strcmp(model_name, "models/items/ammopack_large_bday.mdl") ||
			!strcmp(model_name, "models/items/tf_gift.mdl") ||
			!strcmp(model_name, "models/props_halloween/halloween_gift.mdl")) {
			name = "ammo"; is_ammo = true;
		}

		if (!strcmp(model_name, MAD_MILK_MODEL) ||
			!strcmp(model_name, FLYING_GUILLOTINE_MODEL) ||
			!strcmp(model_name, SANDMAN_BASEBALL_MODEL) ||
			!strcmp(model_name, WRAP_ASSASSIN_BAUBLE_MODEL) ||
			!strcmp(model_name, ROCKET_GENERAL_MODEL) ||
			!strcmp(model_name, LASER_MODEL) ||
			!strcmp(model_name, ROCKET_AIRSTRIKE_MODEL) ||
			!strcmp(model_name, FLARES_GENERAL_MODEL) ||
			!strcmp(model_name, PILL_GENERAL_MODEL) ||
			!strcmp(model_name, LOOSE_CANNON_MODEL) ||
			!strcmp(model_name, STICKYBOMB_GENERAL_MODEL) ||
			!strcmp(model_name, STICKYBOMB_SCOTTISH_RESISTANCE_MODEL) ||
			!strcmp(model_name, STICKYBOMB_STICKY_JUMPER_MODEL) ||
			!strcmp(model_name, SENTRYGUN_ROCKET_MODEL) ||
			!strcmp(model_name, RESCUE_RANGER_MODEL) ||
			!strcmp(model_name, ARROW_GENERAL_MODEL) ||
			!strcmp(model_name, JARATE_MODEL) ||
			!strcmp(model_name, JARATE_BREAD_MODEL) ||
			!strcmp(model_name, FLYING_GUILLOTINE_MODEL)) {
			name = "projectile"; is_projectile = true;
		}

		if (is_ammo || is_health || is_projectile) {
			if (vars::visuals::world_box) {
				draw::draw_outlined_rect(x - 1, y - 1, w + 2, h + 2, colour_t(0, 0, 0));
				draw::draw_outlined_rect(x, y, w, h, esp_colour);
			}

			if (vars::visuals::world_name) {
				draw::draw_string_centered(x + (w / 2), y - 13, esp_colour, font::tahoma, name.c_str());
			}
		}
	}

	bool visuals::world_to_screen(vec3_t &origin, vec3_t &screen) {
		const matrix3x4_t& world_to_screen = interfaces::engineclient->world_to_screen_matrix();

		float w = world_to_screen[3][0] * origin[0] + world_to_screen[3][1] * origin[1] + world_to_screen[3][2] * origin[2] + world_to_screen[3][3];
		screen.z = 0;

		if (w > 0.001) {
			float fl1DBw = 1 / w;
			screen.x = (global::screen_width / 2) + (0.5 * ((world_to_screen[0][0] * origin[0] + world_to_screen[0][1] * origin[1] + world_to_screen[0][2] * origin[2] + world_to_screen[0][3]) * fl1DBw) * global::screen_width + 0.5);
			screen.y = (global::screen_height / 2) - (0.5 * ((world_to_screen[1][0] * origin[0] + world_to_screen[1][1] * origin[1] + world_to_screen[1][2] * origin[2] + world_to_screen[1][3]) * fl1DBw) * global::screen_height + 0.5);
			return true;
		}
		return false;
	}
	bool visuals::create_bounds(c_base_player* entity, float &x, float &y, float &w, float &h) {
		const matrix3x4_t& vec_matrix = entity->get_rgfl_coordinate_frame();
		vec3_t vec_minimum = entity->get_collideable_mins(), vec_maximum = entity->get_collideable_maxs();
		vec3_t vec_pointlist[] = {
			vec3_t(vec_minimum.x, vec_minimum.y, vec_minimum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_maximum.y, vec_minimum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_minimum.z),

			vec3_t(vec_maximum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_maximum.y, vec_maximum.z),
			vec3_t(vec_minimum.x, vec_minimum.y, vec_maximum.z),
			vec3_t(vec_maximum.x, vec_minimum.y, vec_maximum.z)
		}, vec_transformed[8];

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 3; j++) {
				vec_transformed[i][j] = vec_pointlist[i].dot((vec3_t&)vec_matrix[j]) + vec_matrix[j][3];
			}
		}

		vec3_t flb, brt, blb, frt, frb, brb, blt, flt;
		if (!world_to_screen(vec_transformed[3], flb) ||
			!world_to_screen(vec_transformed[0], blb) ||
			!world_to_screen(vec_transformed[2], frb) ||
			!world_to_screen(vec_transformed[6], blt) ||
			!world_to_screen(vec_transformed[5], brt) ||
			!world_to_screen(vec_transformed[4], frt) ||
			!world_to_screen(vec_transformed[1], brb) ||
			!world_to_screen(vec_transformed[7], flt))
			return false;

		vec3_t vec_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float	top = flb.y,
				right = flb.x,
				left = flb.x,
				bottom = flb.y;

		for (int i = 0; i < 8; i++) {
			if (left > vec_array[i].x) {
				left = vec_array[i].x;
			}
			if (top < vec_array[i].y) {
				top = vec_array[i].y;
			}
			if (right < vec_array[i].x) {
				right = vec_array[i].x;
			}
			if (bottom > vec_array[i].y) {
				bottom = vec_array[i].y;
			}
		}

		x = left, y = bottom, w = right - left, h = top - bottom;
		x += ((right - left) / 8), w -= ((right - left) / 8) * 2;

		return true;
	}
}