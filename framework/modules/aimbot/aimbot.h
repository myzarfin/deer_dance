#pragma once

namespace aimbot {
	extern c_base_player* aimbot_target;

	void do_aimbot();
	void fix_movement(c_user_cmd* pCmd, vec3_t old_angles, float old_fowardmove, float old_sidemove);

	c_base_player* get_best_target(c_base_player* local_player, c_user_cmd* command);
	int get_best_hitbox(c_base_player* local_player, c_base_player* entity, bool is_a_building);

	vec3_t estimate_abs_velocity(c_base_player* entity);
	void projectile_prediction(c_base_player* local_player, c_base_player* entity, c_base_weapon* local_weapon, vec3_t& vec_hitbox);
}