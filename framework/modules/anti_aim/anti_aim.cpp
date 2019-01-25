#include "../../main.h"

enum {
	down = 1,
	up,
	fake_down,
	fake_up
};

enum {
	inverse = 1,
	left, 
	right,
	manual_aa, 
	edge
};

namespace anti_aim {
	void do_anti_aim() {
		if (!global::local_player || global::local_player->is_dead()) {
			return;
		}

		if (!global::command) {
			return;
		}

		if (utilities::has_condition(global::local_player, 128)) {
			return;
		}

		c_base_weapon* local_weapon = global::local_player->get_active_weapon();
		if (!local_weapon) {
			return;
		}

		float server_time = global::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
		bool bullet_time = !(local_weapon->get_next_attack() > server_time);

		if (global::command->buttons & IN_ATTACK && bullet_time) {
			return;
		}

		float old_side_move = global::command->side_move, old_forward_move = global::command->forward_move;
		vec3_t old_viewangles = global::command->viewangles, temp_angles = global::command->viewangles;
		float spinbot_speed = fmod(interfaces::globals->realtime * vars::hvh::anti_aim_spinbot_speed / 10.0f * 360.0f, 360.0f);

		switch (vars::hvh::anti_aim_pitch) {
			case down:		temp_angles.x =  89.0f;	break;
			case up:		temp_angles.x = -89.0f; break;
			case fake_down: temp_angles.x =  271.0f; break;
			case fake_up:	temp_angles.x = -271.0f; break;
			default: break;
		}

		if (!*global::send_packet) {
			switch (vars::hvh::anti_aim_real_yaw) {
			case inverse:	temp_angles.y = temp_angles.y;	break;
			case left:		temp_angles.y -= 90.0f;			break;
			case right:		temp_angles.y += 90.0f;			break;
			case manual_aa: {
				static bool left_key_pressed = false, right_key_pressed = false, down_key_pressed = false;
				if (GetAsyncKeyState(VK_LEFT) & 1) {
					left_key_pressed = true, right_key_pressed = false, down_key_pressed = false;
				}
				else if (GetAsyncKeyState(VK_RIGHT) & 1) {
					left_key_pressed = false, right_key_pressed = true, down_key_pressed = false;
				}
				else if (GetAsyncKeyState(VK_DOWN) & 1) {
					left_key_pressed = false, right_key_pressed = false, down_key_pressed = true;
				}

				if (left_key_pressed) {
					temp_angles.y -= 90.0f;
				}
				else if (right_key_pressed) {
					temp_angles.y += 90.0f;
				}
				else if (down_key_pressed) {
					temp_angles.y = temp_angles.y;
				}
			} break;
			default: break;
			}
		}

		if (*global::send_packet) {
				switch (vars::hvh::anti_aim_fake_yaw) {
				case 1:	temp_angles.y = temp_angles.y;	break;
				case 2:	temp_angles.y -= 90.0f;			break;
				case 3:	temp_angles.y += 90.0f;			break;
				case 4:	temp_angles.y = spinbot_speed;	break;
				default: break;
			}
		}

		global::command->viewangles = temp_angles;

		aimbot::fix_movement(global::command, old_viewangles, old_forward_move, old_side_move);
	}
}