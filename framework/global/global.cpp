#include "../main.h"

namespace global {
	c_base_player*	global::local_player;
	c_user_cmd*		global::command;
	c_user_cmd		global::original_command;
	bool*			global::send_packet;

	int				global::screen_width;
	int				global::screen_height;

	imaterial*		global::visible_flattened;
	imaterial*		global::hidden_flattened;

	imaterial*		global::visible_textured;
	imaterial*		global::hidden_textured;

	bool			global::is_scoped;
	bool			global::in_thirdperson;

	vec3_t			global::antiaim_angle;
	vec3_t			global::fakelag_position;

	int				global::shots_fired;
	int				global::shots_missed;
	bool			global::stop_resolving;
	vec3_t			global::fake_angle;
	vec3_t			global::real_angle;

	vec3_t			global::enemy_angle_change;
	vec3_t			global::hit_angle_saved;
}
