#include "../main.h"

namespace hooks {
	bool __fastcall	hooks::create_move(iclientmodeshared* ecx, PVOID edx, float input_sample_time, c_user_cmd* command) {
		static auto original_create_move = hook::client_mode_hook->get_original_function<decltype(&hooks::create_move)>(21);
		original_create_move(ecx, edx, input_sample_time, command);

		global::local_player = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(interfaces::engineclient->get_local_player()));
		global::command = command; global::original_command = *command;

		uintptr_t _bp; __asm mov _bp, ebp;
		global::send_packet = (bool*)(***(uintptr_t***)_bp - 1);

		if (global::local_player == nullptr || global::command == nullptr || !global::command->command_number) {
			return false;
		}

		position_adjustment::do_position_adjustment();
		miscellanous::do_miscellanous();
		triggerbot::do_triggerbot();
		anti_aim::do_anti_aim();
		aimbot::do_aimbot();

		if (vars::visuals::thirdperson_angle == 0 && !*global::send_packet) {
			global::antiaim_angle = global::command->viewangles;
			global::real_angle = global::command->viewangles;
		} else if (vars::visuals::thirdperson_angle == 1 && *global::send_packet) {
			global::antiaim_angle = global::command->viewangles;
			global::fakelag_position = global::local_player->get_abs_origin();
			global::fake_angle = global::command->viewangles;
		}

		announcer::get().run();

		return false;
	}
	float __fastcall hooks::get_viewmodel_fov(iclientmodeshared* ecx, PVOID edx) {
		static auto original_get_viewmodel_fov = hook::client_mode_hook->get_original_function<decltype(&hooks::get_viewmodel_fov)>(32);
		return original_get_viewmodel_fov(ecx, edx);
	}
	int __fastcall hooks::key_event(ibaseclientdll* ecx, PVOID edx, int event_code, int key_num, const char* current_binding) {
		static auto original_key_event = hook::client_hook->get_original_function<decltype(&hooks::key_event)>(20);
		/*if (event_code == 1) {
			menu::handle_input(key_num);
		}*/
		return original_key_event(ecx, edx, event_code, key_num, current_binding);
	}
	void __fastcall	hooks::paint_traverse(ipanel* ecx, PVOID edx, unsigned int vgui_panel, bool force_repaint, bool allow_force) {
		if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game()) {
			if (vars::visuals::no_scope && !strcmp(interfaces::panel->get_name(vgui_panel), "HudScope")) {
				return;
			}
		}
		
		static auto original_paint_traverse = hook::panel_hook->get_original_function<decltype(&hooks::paint_traverse)>(41);
		original_paint_traverse(ecx, edx, vgui_panel, force_repaint, allow_force);

		static unsigned int vgui_focus_overlay;
		if (vgui_focus_overlay == NULL) {
			const char* szName = interfaces::panel->get_name(vgui_panel);
			if (szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P') {
				vgui_focus_overlay = vgui_panel;
			}
		}

		auto quick_do_fonts = [&]() {
			font::verdana = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::verdana, "Verdana", 14, 400, 0, 0, FONTFLAG_DROPSHADOW);

			font::smallest_pixel = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::smallest_pixel, "Smallest Pixel-7", 10, 100, 0, 0, FONTFLAG_OUTLINE);

			font::tahoma = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::tahoma, "Tahoma", 12, 400, 0, 0, FONTFLAG_OUTLINE);

			font::menu_font = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::menu_font, "Tahoma", 12, 500, 0, 0, FONTFLAG_DROPSHADOW);

			font::tab_font = interfaces::surface->create_font();
			interfaces::surface->set_font_glyph_set(font::tab_font, "linea-basic-10", 36, 0, 0, 0, FONTFLAG_ANTIALIAS);

			interfaces::engineclient->get_screen_size(global::screen_width, global::screen_height);
		};

		static bool once = true;
		if (once) {
			quick_do_fonts();
			CNetVars::get().Initialize();
			once = false;
		}

		if (vgui_focus_overlay == vgui_panel) {
			c_menu::get().draw(vgui_focus_overlay);
			if (vars::miscellanous::sv_pure_bypass) {
				bool check_active = false;
				if (!interfaces::engineclient->is_in_game() && !check_active) {
					void** pure_addr = nullptr;
					if (!pure_addr) {
						pure_addr = *reinterpret_cast<void***>(utilities::find_signature("engine.dll", "A1 ? ? ? ? 56 33 F6 85 C0") + 1);
					} *pure_addr = (void*)0;
					check_active = true;
				}
			}
		}

		if (vgui_panel == interfaces::enginevgui->get_panel(PANEL_CLIENTDLL)) {
			int width, height; interfaces::engineclient->get_screen_size(width, height);
			if (global::screen_width != width || global::screen_height != height) {
				interfaces::engineclient->get_screen_size(global::screen_width, global::screen_height);
				quick_do_fonts();
			}

			visuals::render_esp();
			c_menu::get().draw_radar();
		}
	}
	void __fastcall hooks::draw_model_exeute(imodel_render* ecx, PVOID edx, model_render_info &entity_info, matrix3x4_t* custom_bones) {
		// Fuck DME.
		/*if (interfaces::engineclient->is_in_game() && interfaces::engineclient->is_connected()) {
			if (global::local_player && entity_info.model) {
				std::string model_name = interfaces::model_info->get_model_name(entity_info.model);

				hook::render_hook->un_hook_function(19);

				if (vars::visuals::chams && (model_name.find("models/player") != std::string::npos)) {
					c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(entity_info.entity_index));
					bool is_cloaked = utilities::has_condition(entity, 16) || utilities::has_condition(entity, 8192);
					if (utilities::is_valid_player(entity) && !is_cloaked) {
						colour_t hidden_colour = (entity->get_team_num() == 2) ? colour_t(255, 0, 0) : colour_t(0, 150, 255);
						colour_t visible_colour = (entity->get_team_num() == 2) ? colour_t(255, 225, 0) : colour_t(0, 150, 0);

						if (vars::visuals::chams_xqz) {
							global::hidden->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, true);
							global::hidden->increment_reference_count();
							chams::force_material(hidden_colour, global::hidden);
							interfaces::model_render->draw_model_exeute(edx, entity_info, custom_bones);
						}

						global::visible->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, false);
						global::visible->increment_reference_count();
						chams::force_material(visible_colour, global::visible);
						interfaces::model_render->draw_model_exeute(edx, entity_info, custom_bones);
					}
				}
				else {
					interfaces::model_render->force_material_override(NULL);
				}
			}
		}

		interfaces::model_render->draw_model_exeute(edx, entity_info, custom_bones);
		interfaces::model_render->force_material_override(NULL);
		hook::render_hook->hook_function(hooks::draw_model_exeute, 19);*/
	}
	void __fastcall hooks::frame_stage_notify(ibaseclientdll* ecx, PVOID edx, client_frame_stage_t stage) {
		static auto original_frame_stage = hook::client_hook->get_original_function<decltype(&hooks::frame_stage_notify)>(35);
		if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game() && global::local_player) {
			switch (stage) {
				case FRAME_START: break;
				case FRAME_NET_UPDATE_START: break;
				case FRAME_NET_UPDATE_POSTDATAUPDATE_START: {
					for (int i = 0; i <= interfaces::engineclient->get_max_clients(); i++) {
						c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
						if (!entity || entity->is_dead() || entity->is_dormant() || entity->get_team_num() == global::local_player->get_team_num()) {
							continue;
						}

						if (vars::hvh::auto_pitch_resolver) {
							uintptr_t eye_angles = reinterpret_cast<uintptr_t>(entity) + CNetVars::get().get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
							float entity_angles = *reinterpret_cast<float*>(eye_angles);

							if (entity_angles >= 90) {
								entity_angles = -89;
							} else if (entity_angles <= -90) {
								entity_angles = 89;
							}
						}

						if (aimbot::aimbot_target) {
							DWORD eye_angles = reinterpret_cast<DWORD>(aimbot::aimbot_target) + CNetVars::get().get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
							vec3_t &aimbot_entity = *reinterpret_cast<vec3_t*>(eye_angles);

							if (global::shots_fired > 2) {
								global::shots_fired = 0;
							}

							if (vars::hvh::auto_resolver) {
								switch (global::shots_fired) {
									case 0: aimbot_entity.x = 89.0f;  break;
									case 1: aimbot_entity.x = 0.0f;   break;
									case 2: aimbot_entity.x = -89.0f; break;
									default: break;
								}
							}

							if (vars::hvh::auto_yaw_resolver) {
								aimbot_entity.y = fmod(aimbot_entity.y + 180.0f, 360.0f);
								if (aimbot_entity.y < 0) {
									aimbot_entity.y += 360.0f;
								} aimbot_entity.y -= 180.0f;

								switch (global::shots_fired) {
									case 0: aimbot_entity.y += 180.0f; break;
									case 1: aimbot_entity.y += 90.0f;  break;
									case 2: aimbot_entity.y -= 90.0f;  break;
									default: break;
								}
							}
						}
					}
				} break;
				case FRAME_NET_UPDATE_POSTDATAUPDATE_END: {
					if (vars::visuals::nightmode) {
						static int starting = 100;
						if (starting != vars::visuals::nightmode_value) {
							starting  = vars::visuals::nightmode_value;

							for (material_handle i = interfaces::material_system->first_material(); i != interfaces::material_system->invalid_material(); i = interfaces::material_system->next_material(i)) {
								imaterial* material = interfaces::material_system->get_material(i);
								if (!material || material->is_error_material()) {
									continue;
								}

								float final_nightmode_value = vars::visuals::nightmode_value / 100.0f;
								if (strstr(material->get_texture_group_name(), "StaticProp") || strstr(material->get_texture_group_name(), "World")) {
									material->colour_modulate(final_nightmode_value, final_nightmode_value, final_nightmode_value);
								}
							}
						}
					}
				} break;
				case FRAME_NET_UPDATE_END: break;
				case FRAME_RENDER_START: {
					if (global::in_thirdperson) {
						float &eye_angles_y = *reinterpret_cast<float*>(reinterpret_cast<DWORD>(global::local_player) + CNetVars::get().get_offset("DT_BasePlayer", "pl", "deadflag") + 8);
						float &eye_angles_x = *reinterpret_cast<float*>(reinterpret_cast<DWORD>(global::local_player) + CNetVars::get().get_offset("DT_BasePlayer", "pl", "deadflag") + 4);

						if (global::local_player && !global::local_player->is_dead()) {
							eye_angles_x = global::antiaim_angle.x;
							eye_angles_y = global::antiaim_angle.y;
						}
					}
				} break;
				case FRAME_RENDER_END: break;
			}
		}
		original_frame_stage(ecx, edx, stage);
	}
	void __fastcall hooks::override_view(void* ecx, void* edx, c_view_setup* setup) {
		static auto original_override_view = hook::client_mode_hook->get_original_function<decltype(&hooks::override_view)>(16);
		original_override_view(ecx, edx, setup);

		if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game()) {
			if (global::local_player && !global::local_player->is_dead()) {
				if (utilities::has_condition(global::local_player, 2)) {
					global::is_scoped = true;
				} else {
					global::is_scoped = false;
				}

				if (vars::visuals::no_zoom) {
					if (utilities::has_condition(global::local_player, 2)) {
						global::local_player->set_condition(global::local_player->get_condition() & ~2);
					}
					
					setup->m_fov = vars::visuals::forced_fov;
				} else if (setup->m_fov != 20.0f && setup->m_fov != vars::visuals::forced_fov) {
					setup->m_fov = vars::visuals::forced_fov;
				}
			}
		}
	}
	void __fastcall hooks::scene_end(void* ecx, void* edx) {
		static auto original_scene_end = hook::render_hook->get_original_function<decltype(&hooks::scene_end)>(9);
		original_scene_end(ecx, edx);

		if (!global::local_player || vars::chams::chams_activation == 0) {
			return;
		}

		auto force_material_thanks = [&](imaterial* material, colour_t colour, bool ignorez) {
			material->set_materialvar_flag(MATERIAL_VAR_IGNOREZ, ignorez);
			chams::force_material(colour, material);
		};

		imaterial* visible_mat = nullptr;
		imaterial* hidden_mat = nullptr;
		switch (vars::chams::chams_model) {
			case 0: { // Flat chams
				visible_mat = global::visible_flattened;
				hidden_mat = global::hidden_flattened;
			} break;
			case 1: { // Textured chams
				visible_mat = global::visible_textured;
				hidden_mat = global::hidden_textured;
			} break;
			default: break;
		}
		if (vars::visuals::player_chams) {
			for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
				c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
				if (!entity || entity->is_dead() || entity->is_dormant() || entity->get_client_class()->class_id != 246 || entity == global::local_player) {
					continue;
				}

				if (vars::visuals::player_enemyonly && entity->get_team_num() == global::local_player->get_team_num() && !utilities::is_developer(entity)) {
					continue;
				}

				bool is_teammate = entity->get_team_num() == global::local_player->get_team_num() && !utilities::is_developer(entity);
				colour_t visible_colour = is_teammate ? colour_t(200, 200, 0) : colour_t(0, 150, 25);
				colour_t hidden_colour  = is_teammate ? colour_t(185, 0, 0) : colour_t(0, 100, 200);

				if (utilities::has_condition(entity, 16) || utilities::has_condition(entity, 8192)) {
					continue;
				}

				if (utilities::has_condition(entity, 32) || utilities::has_condition(entity, 16384) || utilities::has_condition(entity, 268435456)) {
					visible_colour = colour_t(125, 125, 125);
					hidden_colour = colour_t(85, 85, 85);

					if (vars::chams::chams_activation == 2) {
						force_material_thanks(hidden_mat, hidden_colour, true);
						entity->draw_model(0x1);
						interfaces::model_render->force_material_override(nullptr);
					}

					force_material_thanks(visible_mat, visible_colour, false);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
					continue;
				}

				if (utilities::has_condition(entity, 2048)) {
					visible_colour = colour_t(255, 150, 0);
					hidden_colour = colour_t(200, 100, 0);

					if (vars::chams::chams_activation == 2) {
						force_material_thanks(hidden_mat, hidden_colour, true);
						entity->draw_model(0x1);
						interfaces::model_render->force_material_override(nullptr);
					}

					force_material_thanks(visible_mat, visible_colour, false);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
					continue;
				}

				if (utilities::is_developer(entity)) {
					visible_colour = colour_t(0, 100, 200);
					hidden_colour = colour_t(200, 0, 200);
				}

				if (vars::chams::chams_activation == 2) {
					force_material_thanks(hidden_mat, hidden_colour, true);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
				}

				force_material_thanks(visible_mat, visible_colour, false);
				entity->draw_model(0x1);
				interfaces::model_render->force_material_override(nullptr);
			}
		}

		if (vars::visuals::building_chams) {
			for (int i = 1; i <= interfaces::entitylist->get_highest_entity_index(); i++) {
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

					bool is_teammate = entity->get_team_num() == global::local_player->get_team_num() && entity_builder != global::local_player && !utilities::is_developer(entity_builder);
					colour_t visible_colour = is_teammate ? colour_t(200, 200, 0) : colour_t(0, 150, 25);
					colour_t hidden_colour = is_teammate ? colour_t(185, 0, 0) : colour_t(0, 100, 200);

					if (vars::visuals::building_enemyonly && is_teammate) {
						continue;
					}

					if (vars::chams::local_player && entity_builder == global::local_player) {
						visible_colour = colour_t(195, 140, 145);
						hidden_colour = colour_t(255, 255, 255);
					}

					if (utilities::is_developer(entity_builder)) {
						visible_colour = colour_t(0, 100, 200);
						hidden_colour = colour_t(200, 0, 200);
					}

					if (entity_builder->is_dead()) {
						visible_colour = colour_t(255, 255, 255);
						hidden_colour = colour_t(255, 255, 255);
					}

					if (vars::chams::chams_activation == 2) {
						force_material_thanks(hidden_mat, hidden_colour, true);
						entity->draw_model(0x1);
						interfaces::model_render->force_material_override(nullptr);
					}

					force_material_thanks(visible_mat, visible_colour, false);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
				}
			}
		}
	
		for (int i = 1; i < interfaces::entitylist->get_highest_entity_index(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant()) {
				continue;
			}

			colour_t visible_colour = colour_t(255, 255, 255);
			colour_t hidden_colour  = colour_t(255, 255, 255);
			bool is_health = false, is_ammo = false, is_projectile = false;
			const char* model_name = entity->get_model_name();
			if ((!strcmp(model_name, "models/items/medkit_small.mdl") ||
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
				!strcmp(model_name, "models/items/plate_robo_sandwich.mdl")) && vars::visuals::world_health_chams) {
				visible_colour = colour_t(0, 200, 0); 
				hidden_colour  = colour_t(0, 150, 0);
				is_health = true;
			}

			if ((!strcmp(model_name, "models/items/ammopack_small.mdl") ||
				!strcmp(model_name, "models/items/ammopack_small_bday.mdl") ||
				!strcmp(model_name, "models/items/ammopack_medium.mdl") ||
				!strcmp(model_name, "models/items/ammopack_medium_bday.mdl") ||
				!strcmp(model_name, "models/items/ammopack_large.mdl") ||
				!strcmp(model_name, "models/items/ammopack_large_bday.mdl") ||
				!strcmp(model_name, "models/items/tf_gift.mdl") ||
				!strcmp(model_name, "models/props_halloween/halloween_gift.mdl")) && vars::visuals::world_ammo_chams) {
				visible_colour = colour_t(255, 255, 255); 
				hidden_colour  = colour_t(200, 200, 200);
				is_ammo = true;
			}

			// TODO: Make weapon chams. Flying Guillotine glows in the hand, why shouldn't it work on every gun?
			if ((!strcmp(model_name, MAD_MILK_MODEL) ||
				!strcmp(model_name, FLYING_GUILLOTINE_MODEL) ||
				!strcmp(model_name, SANDMAN_BASEBALL_MODEL) ||
				!strcmp(model_name, WRAP_ASSASSIN_BAUBLE_MODEL) ||
				!strcmp(model_name, ROCKET_GENERAL_MODEL) ||
				!strcmp(model_name, LASER_MODEL) ||
				!strcmp(model_name, ROCKET_AIRSTRIKE_MODEL) ||
				!strcmp(model_name, FLARES_GENERAL_MODEL) ||
				!strcmp(model_name, PILL_GENERAL_MODEL) ||
				!strcmp(model_name, LOOSE_CANNON_MODEL) ||
				!strcmp(model_name, IRON_BOMBER_MODEL) ||
				!strcmp(model_name, STICKYBOMB_GENERAL_MODEL) ||
				!strcmp(model_name, STICKYBOMB_SCOTTISH_RESISTANCE_MODEL) ||
				!strcmp(model_name, STICKYBOMB_STICKY_JUMPER_MODEL) ||
				!strcmp(model_name, SENTRYGUN_ROCKET_MODEL) ||
				!strcmp(model_name, RESCUE_RANGER_MODEL) ||
				!strcmp(model_name, ARROW_GENERAL_MODEL) ||
				!strcmp(model_name, JARATE_MODEL) ||
				!strcmp(model_name, JARATE_BREAD_MODEL)) && vars::visuals::world_projectile_chams) {
				visible_colour = colour_t(255, 150, 0);
				hidden_colour = colour_t(200, 100, 0);
				is_projectile = true;
			}

			if (is_health || is_ammo || is_projectile) {
				if (vars::chams::chams_activation == 2) {
					force_material_thanks(hidden_mat, hidden_colour, true);
					entity->draw_model(0x1);
					interfaces::model_render->force_material_override(nullptr);
				}

				force_material_thanks(visible_mat, visible_colour, false);
				entity->draw_model(0x1);
				interfaces::model_render->force_material_override(nullptr);
			}
		}
	
		if (vars::chams::local_player && global::in_thirdperson) {
			if (vars::chams::chams_activation == 2) {
				force_material_thanks(visible_mat, utilities::is_developer(global::local_player) ? colour_t(200, 0, 200) : colour_t(195, 140, 145), false);
				global::local_player->draw_model(0x1);
				interfaces::model_render->force_material_override(nullptr);
			}

			force_material_thanks(visible_mat, utilities::is_developer(global::local_player) ? colour_t(200, 0, 200) : colour_t(195, 140, 145), false);
			global::local_player->draw_model(0x1);
			interfaces::model_render->force_material_override(nullptr);
		}
	}
}

// TODO: Get netvar proxys to work.
namespace netvar_manager_hook {
	void netvar_manager_hook::yaw_angles_hook(const CRecvProxyData* data, void* structure, void* out) {
		float yaw = data->m_Value.m_Float;

		c_base_player* entity = (c_base_player*)structure;
		if (!entity || entity->is_dead() || entity->is_dormant() || entity->get_client_class()->class_id != 246) {
			return;
		}

		if (GetAsyncKeyState(VK_MENU)) {
			vec3_t viewangles; interfaces::engineclient->get_view_angles(viewangles);
			auto at_target_yaw = utilities::calc_angle(entity->get_abs_origin(), global::local_player->get_abs_origin()).y;

			auto normalize_yaw = [&](float yaw) {
				if (yaw > 180) {
					yaw -= (round(yaw / 360) * 360.f);
				} else if (yaw < -180) {
					yaw += (round(yaw / 360) * -360.f);
				} return yaw;
			};

			auto delta = normalize_yaw(viewangles.y - at_target_yaw);
			auto right_delta = at_target_yaw + 90.0f; auto left_delta = at_target_yaw - 90.0f;

			if (delta > 0) {
				yaw += right_delta;
			} else {
				yaw -= left_delta;
			}
		}

		*(float*)(out) = yaw;
	}
	void netvar_manager_hook::initialize_netvar_hooks() {
		netvar_manager_hook::hook("DT_TFPlayer", "m_angEyeAngles[1]", yaw_angles_hook);
	}
	void netvar_manager_hook::hook(const char* table_to_hook, const char* prop_to_hook, netvar_hook_callback callback) {
		client_class* client_class = interfaces::client->get_all_classes();
		while (client_class) {
			const char* table_name = client_class->table->m_pNetTableName;
			if (!strcmp(table_name, table_to_hook)) {
				for (int i = 0; i < client_class->table->m_nProps; i++) {
					RecvProp &prop = client_class->table->m_pProps[i];
					const char* prop_name = prop.m_pVarName;
					if (!strcmp(prop_name, prop_to_hook)) {
						prop.m_ProxyFn = callback;
					}
				}
			}
		}
	}
}

bool is_headshot_weapon(c_base_player* local_player, c_base_weapon* local_weapon) {
	if (local_weapon->get_slot() == 0 && local_player->get_class_num() == TF2_Sniper)
		if (local_weapon->get_item_definition_index() != WPN_Huntsman && local_weapon->get_item_definition_index() != WPN_CompoundBow)
			return true;

	if (local_weapon->get_item_definition_index() == WPN_Ambassador || local_weapon->get_item_definition_index() == WPN_FestiveAmbassador)
		return true;

	return false;
}

// TODO: Fix resolver log angles.

void resolver::initialize() {
	interfaces::game_event->add_listener(this, "player_hurt", false);
}

void resolver::fire_game_event(igame_event* event) {
	if (global::local_player) {
		player_info_t local_player_info;
		interfaces::engineclient->get_player_info(interfaces::engineclient->get_local_player(), &local_player_info);

		int local_user_id = local_player_info.user_id;
		if (local_user_id == 0) { return; }

		if (strcmp(event->get_name(), "player_hurt") == 0) {
			int attacker = event->get_int("attacker", 0), user_id = event->get_int("userid", 0);
			if (attacker == local_user_id) {
				if (attacker == user_id) { return; }
				
				c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(user_id));
				if (!entity) { return; }

				c_base_weapon* local_weapon = global::local_player->get_active_weapon();
				if (!local_weapon) { return; }

				int damage_done = event->get_int("damageammount"); 
				if (is_headshot_weapon(global::local_player, local_weapon) && damage_done == (150 || 140 || 13) && global::is_scoped) {
					global::hit_angle_saved = global::enemy_angle_change;
				} 
			}
		}
	}
}

class tf_matchgroup_description {
public:
	char pad0[4];
	int m_iID;
	char pad1[63];
	bool m_bForceCompetitiveSettings;
};

tf_matchgroup_description* get_match_group_description(int& idx)
{
	typedef tf_matchgroup_description* (_cdecl* get_them_fn)(int&);
	static get_them_fn get_them = (get_them_fn)(utilities::find_signature("client.dll", "55 8B EC 8B 45 08 8B 00 83 F8 FF"));
	return get_them(idx);
}

namespace setup {
	void setup::setup_interfaces() {
		create_interface_fn client	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA("client.dll"), "CreateInterface"));
		create_interface_fn engine	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"));
		create_interface_fn vgui	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA("vgui2.dll"), "CreateInterface"));
		create_interface_fn mat		= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA("vguimatsurface.dll"), "CreateInterface"));
		create_interface_fn mat_sys = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA("materialsystem.dll"), "CreateInterface"));
		create_interface_fn convar	= reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "CreateInterface"));

		interfaces::client			= static_cast<ibaseclientdll*>(client("VClient017", nullptr));
		interfaces::entitylist		= static_cast<icliententitylist*>(client("VClientEntityList003", nullptr));
		interfaces::clientmode		= **(iclientmodeshared***)((*(DWORD**)interfaces::client)[10] + 0x5);
		interfaces::model_info		= static_cast<imodel_info*>(engine("VModelInfoClient006", nullptr));
		interfaces::model_render	= static_cast<imodel_render*>(engine("VEngineModel016", nullptr));
		interfaces::material_system = static_cast<imaterial_system*>(mat_sys("VMaterialSystem081", nullptr));
		interfaces::render_view		= static_cast<irender_view*>(engine("VEngineRenderView014", nullptr));

		interfaces::engineclient = static_cast<ivengineclient*>(engine("VEngineClient013", nullptr));
		interfaces::enginetrace	 = static_cast<ienginetrace*>(engine("EngineTraceClient003", nullptr));
		interfaces::debugoverlay = static_cast<ivdebugoverlay*>(engine("VDebugOverlay003", nullptr));
		interfaces::enginevgui	 = static_cast<ienginevgui*>(engine("VEngineVGui001", nullptr));
		interfaces::efx			 = static_cast<ivefx*>(engine("VEngineEffects001", nullptr));

		interfaces::globals		= *reinterpret_cast<global_vars_base**>(utilities::find_signature("engine.dll", "A1 ? ? ? ? 8B 11 68") + 8);

		interfaces::surface		= static_cast<isurface*>(mat("VGUI_Surface030", nullptr));
		interfaces::panel		= static_cast<ipanel*>(vgui("VGUI_Panel009", nullptr));

		interfaces::convar		= static_cast<iconvar*>(convar("VEngineCvar004", nullptr));
		interfaces::game_event  = static_cast<igame_event_manager*>(engine("GAMEEVENTSMANAGER002", nullptr));

		global::visible_flattened = interfaces::material_system->find_material("dev/glow_color", TEXTURE_GROUP_MODEL); //chams::create_material(false);
		global::hidden_flattened  = interfaces::material_system->find_material("dev/glow_color", TEXTURE_GROUP_MODEL); //chams::create_material(true);

		global::visible_textured = chams::create_material(false, false, false, false);//interfaces::material_system->find_material("shaded", "Model textures");
		global::hidden_textured  = chams::create_material(true, false, false, false);//interfaces::material_system->find_material("shaded", "Model textures");

		interfaces::engineclient->client_command("toggleconsole; clear", true); Sleep(200);
		interfaces::convar->console_print_colour(colour_t(0, 135, 255), "deer_dance framework. developed by cervine.\n");
		interfaces::convar->console_print_colour(colour_t(0, 135, 255), "based on @Urosaurus's :thonk:\n");

		for (int i = 0; i < 12; i++) {
			tf_matchgroup_description* match_description = get_match_group_description(i);
			if (!match_description || match_description->m_iID > 9) { 
				continue; 
			}

			if (match_description->m_bForceCompetitiveSettings) { 
				match_description->m_bForceCompetitiveSettings = false; 
			}
		}

		//resolver::get().initialize();
		announcer::get().initialize();

		if (interfaces::convar->find_variable("cl_flipviewmodels")) {
			if (interfaces::convar->find_variable("cl_flipviewmodels")->m_flags &   (1 << 22)) {
				interfaces::convar->find_variable("cl_flipviewmodels")->m_flags &= ~(1 << 22);
			}
		}
	}
	void setup::setup_hooks() {
		if (interfaces::clientmode != nullptr) {
			hook::client_mode_hook = std::make_unique<vmt_manager>(interfaces::clientmode);
			hook::client_mode_hook->hook_function(reinterpret_cast<bool*>(hooks::create_move), 21);
			//hook::client_mode_hook->hook_function(reinterpret_cast<float*>(hooks::get_viewmodel_fov), 32);
			hook::client_mode_hook->hook_function(reinterpret_cast<float*>(hooks::override_view), 16);
		}
		if (interfaces::client != nullptr) {
			hook::client_hook = std::make_unique<vmt_manager>(interfaces::client);
			//hook::client_hook->hook_function(reinterpret_cast<int*>(hooks::key_event), 20);
			hook::client_hook->hook_function(reinterpret_cast<int*>(hooks::frame_stage_notify), 35);
		}
		if (interfaces::panel != nullptr) {
			hook::panel_hook = std::make_unique<vmt_manager>(interfaces::panel);
			hook::panel_hook->hook_function(reinterpret_cast<void*>(hooks::paint_traverse), 41);
		}
		if (interfaces::render_view != nullptr) {
			hook::render_hook = std::make_unique<vmt_manager>(interfaces::render_view);
			hook::render_hook->hook_function(reinterpret_cast<void*>(hooks::scene_end), 9);
		}
		//if (interfaces::model_render != nullptr) {
		//	hook::render_hook = std::make_unique<vmt_manager>(interfaces::model_render);
		//	hook::render_hook->hook_function(reinterpret_cast<void*>(hooks::draw_model_exeute), 19);
		//}
	}
	bool setup::ready_to_blast() {
		if (interfaces::client == nullptr)
			return true;
		return false;
	}
}	