#include "../../main.h"

float get_move_angle(float speed)
{
	auto move_angle = RAD2DEG(asin(15.f / speed));

	if (!isfinite(move_angle) || move_angle > 90.f)
		move_angle = 90.f;
	else if (move_angle < 0.f)
		move_angle = 0.f;

	return move_angle;
}
float normalize_angle(float angle)
{
	if (angle > 180.f || angle < -180.f)
	{
		auto revolutions = round(abs(angle / 360.f));

		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}
	return angle;
}
void vector_angles(const vec3_t& forward, vec3_t &angles)
{
	if (forward.x == 0.f && forward.y == 0.f)
	{
		angles.x = forward.z > 0.f ? -90.f : 90.f;
		angles.y = 0.f;
	}
	else
	{
		angles.x = RAD2DEG(atan2(-forward.z, forward.length2d()));
		angles.y = RAD2DEG(atan2(forward.y, forward.x));
	}
	angles.z = 0.f;
}

namespace miscellanous {
	void miscellanous::do_miscellanous() {
		miscellanous::do_fakelag();
		miscellanous::do_movement();
		miscellanous::do_thirdperson();
		miscellanous::do_chatspam();
	}
	void miscellanous::do_fakelag() {
		if (vars::hvh::fakelag_value > 0 && vars::hvh::fakelag_mode > 0) {
			bool local_on_ground = global::local_player->get_flags() & FL_ONGROUND;
			c_net_channel* net_channel = interfaces::engineclient->get_net_channel_info();

			if (vars::hvh::fakelag_mode == 1 ||
			   (vars::hvh::fakelag_mode == 2 && !local_on_ground) ||
			   (vars::hvh::fakelag_mode == 3 && local_on_ground)) {
				if (net_channel->m_nChokedPackets < vars::hvh::fakelag_value) {
					*global::send_packet = false;
				}
				else {
					*global::send_packet = true;
				}
			}
		}
	}
	void miscellanous::do_movement() {
		if (!(global::local_player->get_flags() & FL_ONGROUND) && global::command->buttons & IN_JUMP) {
			if (vars::miscellanous::movement == 2) {
				static auto switch_key = 1.f;
				static auto circle_yaw = 0.f;
				static auto old_yaw = 0.f;

				auto velocity = aimbot::estimate_abs_velocity(global::local_player);
				velocity.z = 0.f;

				auto speed = velocity.length2d();
				auto ideal_strafe = get_move_angle(speed);

				switch_key *= -1.f;

				if (GetAsyncKeyState(0x57)) {
					global::command->viewangles.y = -global::command->viewangles.y;
				}
				else if (GetAsyncKeyState(0x41)) {
					global::command->viewangles.y += 90.0f;
				}
				else if (GetAsyncKeyState(0x44)) {
					global::command->viewangles.y -= 90.0f;
				}

				auto yaw_delta = normalize_angle(global::command->viewangles.y - old_yaw);
				auto absolute_yaw_delta = abs(yaw_delta);

				circle_yaw = old_yaw = global::command->viewangles.y;
				if (yaw_delta > 0.f) global::command->side_move = -450.f;
				else if (yaw_delta < 0.f) global::command->side_move = 450.f;

				if (absolute_yaw_delta <= ideal_strafe || absolute_yaw_delta >= 30.f)
				{
					vec3_t velocity_angles;
					vector_angles(velocity, velocity_angles);

					auto velocity_delta = normalize_angle(global::command->viewangles.y - velocity_angles.y);
					auto retrack = 2.f * (ideal_strafe * 2.f);

					if (velocity_delta <= retrack || speed <= 15.f)
					{
						if (-retrack <= velocity_delta || speed <= 15.f)
						{
							global::command->viewangles.y += ideal_strafe * switch_key;
							global::command->side_move = switch_key * 450.f;
						}
						else
						{
							global::command->viewangles.y = velocity_angles.y - retrack;
							global::command->side_move = 450.f;
						}
					}
					else
					{
						global::command->viewangles.y = velocity_angles.y + retrack;
						global::command->side_move = -450.f;
					}
				}
			}
		}
		if (vars::miscellanous::movement > 0) {
			static bool first_jump = false, fake_jump;
			if (global::command->buttons & IN_JUMP) {
				if (!first_jump) {
					first_jump = fake_jump = true;
				}
				else if (!(global::local_player->get_flags() & FL_ONGROUND)) {
					if (fake_jump) {
						fake_jump = false;
					}
					else {
						global::command->buttons &= ~IN_JUMP;
					}
				}
				else {
					fake_jump = true;
				}
			}
			else {
				first_jump = false;
			}
		}

		if (vars::miscellanous::taunt_slide && utilities::has_condition(global::local_player, 128)) {
			float forward = 0.0f, side = 0.0f;

			if (global::command->buttons & IN_FORWARD) { forward += 450; }
			if (global::command->buttons & IN_BACK) { forward -= 450; }
			if (global::command->buttons & IN_MOVELEFT) { side -= 450; }
			if (global::command->buttons & IN_MOVERIGHT) { side += 450; }

			global::command->forward_move = forward;
			global::command->side_move = side;

			vec3_t view_angles;
			interfaces::engineclient->get_view_angles(view_angles);

			global::command->viewangles.y = view_angles[1];
			global::original_command.viewangles.y = view_angles[1];
		}
	}
	void miscellanous::do_thirdperson() {
		static bool thirdperson = false, check = false;
		static bool setting = vars::visuals::thirdperson;
		static bool last_thirdperson_check;

		if (GetAsyncKeyState(0x48)) {
			if (!check) setting = !setting;
			check = true;
		}
		else check = false;
		if (vars::visuals::thirdperson != last_thirdperson_check) {
			setting = vars::visuals::thirdperson;
			last_thirdperson_check = vars::visuals::thirdperson;
		}
		if (vars::visuals::thirdperson) {
			if (setting) {
				global::local_player->force_taunt_cam(true);
				thirdperson = true, global::in_thirdperson = true;
			}
		}
		if (!setting && thirdperson) {
			global::local_player->force_taunt_cam(false);
			thirdperson = false, global::in_thirdperson = false;
		}
	}

	static std::string chatspam_selection[5] = { "blank", "lmaobox", "nullcore", "lithium", "deer_dance" };
	void miscellanous::do_chatspam() {
		static int delay = -1;
		static int current_index;

		if (!vars::chatspam::chatspam_enabled) {
			return;
		}

		if (delay-- > 0) {
			return;
		}

		const std::vector<std::string>* our_spam;
		switch (vars::chatspam::chatspam_selection) {
			/*case 0: our_spam = &blank; break;
			case 1: our_spam = &lmaobox; break;
			case 2: our_spam = &nullcore; break;
			case 3: our_spam = &lithium; break;
			case 4: our_spam = &deer_dance; break;*/
		}

		auto repeat = [&](int n, const char* str) -> std::string {
			std::ostringstream os;
			for (int i = 0; i < n; i++) {
				os << str;
			}
			return os.str();
		};

		if (current_index >= our_spam->size()) {
			current_index = 0;
		}

		std::string current_spam = our_spam->at(current_index);
		std::string our_message = "say ";
		if (vars::chatspam::chatspam_new_lines > 0) {
			our_message.append(" ");
			our_message.append(repeat(vars::chatspam::chatspam_new_lines, "\x0D"));
		}
		our_message.append(current_spam);

		interfaces::engineclient->client_command(our_message.c_str(), true);
		current_index++;

		delay = 12;
	}
}