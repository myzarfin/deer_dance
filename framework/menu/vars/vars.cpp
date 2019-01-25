#include "../../main.h"

namespace vars {
	namespace menu {
		bool menu_active	= false;
		int total_items		= 0;
		int current_pos		= 0;
		int font_size		= 15;
		int menu_x			= 250;
		int menu_y			= 250;
	}

	namespace aimbot {
		bool aimbot_tab = false;
		bool enabled = false;
		int  aimbot_key = 0;
		int  field_of_view = 0;
		int  main_hitbox = 0;
		bool hitscan = false;
		bool autoshoot = false;
		bool silent_aim = false;
		bool bullet_time = false;
		bool prefer_medic = false;
		bool projectile_aimbot = false;
		bool aimbot_sentry = false;
		bool aimbot_dispenser = false;
		bool aimbot_teleporters = false;
	}

	namespace visuals {
		bool visuals_tab = false;
		bool box = false;
		bool health = false;
		bool name = false;
		bool weapon = false;
		bool steam_id = false;
		bool bones = false;
		bool backtrack_visualization = false;
		bool custom_crosshair = true;
		bool spectators = false;
		bool d_lights = false;
		int d_lights_radius = 10.0f;
		bool out_of_fov_arrows = true;
		bool nightmode = true;
		int nightmode_value = 35;
		bool projectile_prediction = false;
		bool thirdperson = false;
		int thirdperson_angle = 0;
		int  forced_fov = 90;
		bool no_zoom = false;
		bool no_recoil = false;
		bool no_scope = false;

		int esp_type = 0;

		bool player_enemyonly = false;
		bool player_box = false;
		bool player_health = false;
		bool player_name = false;
		bool player_weapon = false;
		bool player_steamid = false;
		bool player_backtrack_visulization = false;
		bool player_chams = false;

		bool building_enemyonly = false;
		bool building_box = false;
		bool building_health = false;
		bool building_name = false;
		bool building_builder = false;
		bool building_chams = false;

		bool world_box = false;
		bool world_name = false;
		bool world_health_chams = false;
		bool world_ammo_chams = false;
		bool world_projectile_chams = false;

		bool radar = false;
	}

	namespace chams {
		int chams_activation = 0;
		int chams_model = 0;
		bool enemy_only = true;
		bool local_player = false;
		bool health_and_ammo = false;
		bool buildings = false;
	}

	namespace triggerbot {
		bool triggerbot_tab = false;
		bool triggerbot_enabled = false;
		bool triggerbot_on_key = false;
		bool triggerbot_on_backtrack = false;
		int  triggerbot_key = 0;
		bool auto_airblast = false;
		bool auto_airblast_on_key = false;
		bool auto_detonate = false;
		bool auto_detonate_on_key = false;
	}

	namespace position_adjustment {
		bool position_adjustment_tab = false;
		bool enabled = false;
		int	 ticks = 0;
	}

	namespace miscellanous {
		bool miscellanous_tab = false;
		int  movement = 1;
		bool duck_in_air = false;
		bool taunt_slide = true;
		bool sv_pure_bypass = true;
	}

	namespace chatspam {
		bool chatspam_enabled = false;
		int  chatspam_new_lines = 0;
		int  chatspam_selection = 0;
	}

	namespace hvh {
		int fakelag_mode = 0;
		int fakelag_value = 0;

		int anti_aim_pitch = 0;
		int anti_aim_real_yaw = 0;
		int anti_aim_fake_yaw = 0;
		int anti_aim_spinbot_speed = 0;

		bool auto_pitch_resolver = true;
		bool resolver_key = false;
		int  max_missed_shots = 0;
		bool auto_resolver = false;
		bool auto_yaw_resolver = false;
		bool anti_aim_line = false;
	}

	namespace announcer {
		bool enabled = false;
	}


	namespace example {
	    bool example_checkbox = false;
		int  example_slider = 0;
		int  example_combobox = 0;
	}
}