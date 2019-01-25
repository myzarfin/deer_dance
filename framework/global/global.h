#ifndef GLOBAL_H
#define GLOBAL_H
#ifdef _WIN32
#pragma once
#endif

namespace global {
	extern c_base_player*	local_player;
	extern c_user_cmd*		command;
	extern c_user_cmd		original_command;
	extern bool*			send_packet;

	extern int				screen_width;
	extern int				screen_height;

	extern imaterial*		visible_flattened;
	extern imaterial*		hidden_flattened;

	extern imaterial*		visible_textured;
	extern imaterial*		hidden_textured;

	extern bool				is_scoped;
	extern bool				in_thirdperson;
	extern vec3_t           antiaim_angle;
	extern vec3_t			fakelag_position;
	extern int				shots_fired;
	extern int				shots_missed;
	extern bool				stop_resolving;
	extern vec3_t			fake_angle;
	extern vec3_t			real_angle;

	extern vec3_t			enemy_angle_change;
	extern vec3_t			hit_angle_saved;
}

#endif // !GLOBAL_H
