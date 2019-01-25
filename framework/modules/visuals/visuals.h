#pragma once

namespace visuals {
	void render_esp();
	void create_player_esp(c_base_player* entity);
	void create_building_esp(c_base_player* entity);
	void create_item_esp(c_base_player* entity);

	bool world_to_screen(vec3_t &origin, vec3_t &screen);
	bool create_bounds(c_base_player* entity, float &x, float &y, float &w, float &h);
}