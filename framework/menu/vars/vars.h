#ifndef VARS_H
#define VARS_H
#ifdef _WIN32
#pragma once
#endif

namespace vars { 
	namespace menu {
		extern bool menu_active;
		extern int total_items;
		extern int current_pos;
		extern int font_size;
		extern int menu_x;
		extern int menu_y;
	}
	
	namespace aimbot {
		extern bool aimbot_tab;
		extern int aimbot_key;
		extern bool enabled;
		extern int field_of_view;
		extern int main_hitbox;
		extern bool hitscan;
		extern bool autoshoot;
		extern bool silent_aim;
		extern bool bullet_time;
		extern bool prefer_medic;
		extern bool projectile_aimbot;
		extern bool aimbot_sentry;
		extern bool aimbot_dispenser;
		extern bool aimbot_teleporters;
	}

	namespace visuals {
		extern bool visuals_tab;
		extern bool box;
		extern bool health;
		extern bool name;
		extern bool weapon;
		extern bool steam_id;
		extern bool bones;
		extern bool backtrack_visualization;
		extern bool custom_crosshair;
		extern bool spectators;
		extern bool d_lights;
		extern int d_lights_radius;
		extern bool out_of_fov_arrows;
		extern bool nightmode;
		extern int nightmode_value;
		extern bool projectile_prediction;
		extern bool thirdperson;
		extern int thirdperson_angle;
		extern int  forced_fov;
		extern bool no_zoom;
		extern bool no_recoil;
		extern bool no_scope;

		extern int esp_type;

		extern bool player_enemyonly;
		extern bool player_box;
		extern bool player_health;
		extern bool player_name;
		extern bool player_weapon;
		extern bool player_steamid;
		extern bool player_backtrack_visulization;
		extern bool player_chams;

		extern bool building_enemyonly;
		extern bool building_box;
		extern bool building_health;
		extern bool building_name;
		extern bool building_builder;
		extern bool building_chams;

		extern bool world_box;
		extern bool world_name;
		extern bool world_health_chams;
		extern bool world_ammo_chams;
		extern bool world_projectile_chams;

		extern bool radar;
	}

	namespace chams {
		extern int chams_activation;
		extern int chams_model;
		extern bool enemy_only;
		extern bool local_player;
		extern bool health_and_ammo;
		extern bool buildings;
	}

	namespace triggerbot {
		extern bool triggerbot_tab;
		extern bool triggerbot_enabled;
		extern bool triggerbot_on_key;
		extern bool triggerbot_on_backtrack;
		extern int  triggerbot_key;
		extern bool auto_airblast;
		extern bool auto_airblast_on_key;
		extern bool auto_detonate;
		extern bool auto_detonate_on_key;
	}

	namespace position_adjustment { 
		extern bool position_adjustment_tab;
		extern bool enabled;
		extern int	ticks;
	} 

	namespace miscellanous {
		extern bool miscellanous_tab;
		extern int  movement;
		extern bool duck_in_air;
		extern bool taunt_slide;
		extern bool sv_pure_bypass;
	}
	
	namespace chatspam {
		extern bool chatspam_enabled;
		extern int  chatspam_new_lines;
		extern int  chatspam_selection;
	}

	// Holy fuck we be like if aimware and ncc had a baby
	namespace hvh {
		extern int fakelag_mode;
		extern int fakelag_value;

		extern int anti_aim_pitch;
		extern int anti_aim_real_yaw;
		extern int anti_aim_fake_yaw;
		extern int anti_aim_spinbot_speed;

		extern bool auto_pitch_resolver;
		extern bool resolver_key;
		extern int	max_missed_shots;
		extern bool auto_resolver;
		extern bool auto_yaw_resolver;
		extern bool anti_aim_line;
	}

	namespace announcer {
		extern bool enabled;
	}

	namespace example {
		extern bool example_checkbox;
		extern int  example_slider;
		extern int  example_combobox;
	}
}

#endif // !VARS_H