#include "../main.h"

RECT get_text_size(DWORD font, const char* text, ...) {
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	interfaces::surface->get_text_size(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

void c_menu::checkbox(std::string name, bool* item)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 12;

	static bool pressed = false;

	auto text_size = get_text_size(font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size))
		{
			if (pressed)
				*item = !*item;
			pressed = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size) && !pressed) {
			pressed = true;
		}
	}

	if (*item == true)
		draw::draw_filled_rect(x_offset + 8, y_offset, size, size, colour_t(195, 141, 145, MenuAlpha_Main));
	else
		draw::draw_filled_rect(x_offset + 8, y_offset, size, size, colour_t(50, 50, 50, MenuAlpha_Main));

	draw::draw_outlined_rect(x_offset + 8, y_offset, size, size, colour_t(101, 101, 101, MenuAlpha_Main));
	draw::draw_string(x_offset + 29, y_offset - 2, colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, name.c_str());

	if (c_menu::get().menu_opened) {
		if (mouse_in_params(x_offset + 7, y_offset, size + text_size.right, size)) {
			draw::draw_outlined_rect(x_offset + 9, y_offset + 1, 10, 10, colour_t(101, 101, 101, MenuAlpha_Main));
		}
	}
	y_offset += 16;
}
void c_menu::combobox(int size, std::string name, std::string* itemname, int* item)
{
	if (groupbox_bottom <= y_offset + 41)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 125; // 163

	if (c_menu::get().menu_opened) {
		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, ComboSize, 17) && !click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 25, y_offset + 14, ComboSize, 17)) {
			click_rest = false;
		}

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;

	}

	draw::draw_string(x_offset + 29, y_offset - 2, colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, name.c_str());
	draw::draw_filled_rect(x_offset + 29, y_offset + 14, ComboSize, 17, colour_t(50, 50, 50, MenuAlpha_Main));
	draw::draw_outlined_rect(x_offset + 29, y_offset + 14, ComboSize, 17, colour_t(101, 101, 101, MenuAlpha_Main));

	if (open)
	{
		draw::draw_filled_rect(x_offset + 29, y_offset + 14 + 21, ComboSize, (size * 16), colour_t(50, 50, 50, MenuAlpha_Main));
		draw::draw_outlined_rect(x_offset + 29, y_offset + 14 + 21, ComboSize, (size * 16), colour_t(101, 101, 101, MenuAlpha_Main));

		for (int i = 0; i < size; i++)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 29, y_offset + 34 + i * 16, ComboSize, 16))
				*item = i;

			if (i == *item)
				draw::draw_string(x_offset + 29 + 10, y_offset + 35 + i * 16, colour_t(255, 255, 255, MenuAlpha_Text), font::menu_font, itemname[i].c_str());
			else
				draw::draw_string(x_offset + 29 + 10, y_offset + 35 + i * 16, colour_t(155, 155, 155, MenuAlpha_Text), font::menu_font, itemname[i].c_str());

			if (mouse_in_params(x_offset + 29, y_offset + 34 + i * 16, ComboSize, 16)) {
				draw::draw_string(x_offset + 29 + 10, y_offset + 35 + i * 16, colour_t(200, 200, 200, MenuAlpha_Text), font::menu_font, itemname[i].c_str());
			}
		}
	}

	draw::draw_line(x_offset + 29 + ComboSize - 14, y_offset + 13 + 7, x_offset + 29 + ComboSize - 7, y_offset + 13 + 7, colour_t(101, 101, 101, MenuAlpha_Main));
	draw::draw_line(x_offset + 29 + ComboSize - 14, y_offset + 13 + 9, x_offset + 29 + ComboSize - 7, y_offset + 13 + 9, colour_t(101, 101, 101, MenuAlpha_Main));
	draw::draw_line(x_offset + 29 + ComboSize - 14, y_offset + 13 + 11, x_offset + 29 + ComboSize - 7, y_offset + 13 + 11, colour_t(101, 101, 101, MenuAlpha_Main));

	draw::draw_string(x_offset + 38, y_offset + 15, colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, itemname[*item].c_str());

	if (open) // i added 1 more to 42 bec the space between the main box and the drop down
		y_offset += 42 + (size * 16);
	else
		y_offset += 41;
}
void c_menu::groupbox(int x, int y, int w, int h, std::string name)
{
	draw::draw_outlined_rect(_pos.x + x, _pos.y + y, w, h, colour_t(20, 20, 20, MenuAlpha_Main / 2));
	draw::draw_outlined_rect(_pos.x + (x + 1), _pos.y + (y + 1), w - 2, h - 2, colour_t(70, 70, 70, MenuAlpha_Main));

	RECT text_size = get_text_size(font::menu_font, name.c_str());
	draw::draw_filled_rect((((_pos.x + x) + (w / 2)) - (text_size.right / 2)) - 3, _pos.y + y, text_size.right + 6, 2, colour_t(44, 44, 44, MenuAlpha_Main));

	draw::draw_string((((_pos.x + x) + (w / 2)) - (text_size.right / 2)), (_pos.y + y - 6), colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, name.c_str());

	/*
	Make scrollable!
	*/

	y_offset = (_pos.y + (y + 12));
	x_offset = _pos.x + x;
	groupbox_bottom = (_pos.y + (y + h));
	groupbox_top = (_pos.y + y);
	groupbox_width = (_pos.x + (x + w));
}
void c_menu::slider(int max, std::string name, int* item, std::string suffix, int step)
{
	if (groupbox_bottom <= y_offset + 30)
		return;

	float SliderSize = 125; // 163

	float _pixel_value = max / SliderSize;

	if (c_menu::get().menu_opened) {
		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 29, y_offset + 14, SliderSize + 1, 10))
			*item = abs(mouse_pos().x - (x_offset + 29)) * _pixel_value;


		static bool pressed_subtract = false;
		static bool pressed_add = false;

		/*
		subtract
		*/

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 21, y_offset + 12, 8, 10))
		{
			if (pressed_subtract)
				*item -= step;
			pressed_subtract = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 21, y_offset + 12, 8, 10) && !pressed_subtract)
			pressed_subtract = true;

		/*
		add
		*/

		if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 22 + SliderSize, y_offset + 12, 15, 10))
		{
			if (pressed_add)
				*item += step;
			pressed_add = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(x_offset + 22 + SliderSize, y_offset + 12, 15, 10) && !pressed_add)
			pressed_add = true;

		if (*item > max) {
			*item = max;
		}

		if (*item < 0) {
			*item = 0;
		}

	}
	
	std::string namedisplay;
	namedisplay += name.c_str();
	namedisplay += " (";
	namedisplay += std::to_string(*item);
	if (suffix.size() > 0) {
		namedisplay += " " + suffix;
	}
	namedisplay += ")";

	draw::draw_string(x_offset + 29, y_offset - 3, colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, namedisplay.c_str());

	draw::draw_filled_rect(x_offset + 29, y_offset + 14, SliderSize, 10, colour_t(50, 50, 50, MenuAlpha_Main));

	if (*item)
	{
		if (*item == max) //to fix dumb math stufffff bec pixel size
			draw::draw_filled_rect(x_offset + 29, y_offset + 14, SliderSize, 10, colour_t(195, 141, 145, MenuAlpha_Main));
		else
			draw::draw_filled_rect(x_offset + 29, y_offset + 14, (*item / _pixel_value), 10, colour_t(195, 141, 145, MenuAlpha_Main));
	}

	draw::draw_outlined_rect(x_offset + 29, y_offset + 14, SliderSize, 10, colour_t(101, 101, 101, MenuAlpha_Main));

	draw::draw_string(x_offset + 22, y_offset + 10, colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, "-");
	draw::draw_string(x_offset + 20 + SliderSize + 12, y_offset + 11, colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, "+");

	if (c_menu::get().menu_opened) {
		if (mouse_in_params(x_offset + 29, y_offset + 14, SliderSize + 1, 10)) {
			int _hovering_value = abs(mouse_pos().x - (x_offset + 29)) * _pixel_value;
			draw::draw_filled_rect(x_offset + 29, y_offset + 14, (_hovering_value / _pixel_value), 10, colour_t(101, 101, 101, MenuAlpha_Main));
			draw::draw_string(mouse_pos().x + 12, mouse_pos().y - 5, colour_t(185, 185, 185, MenuAlpha_Text), font::menu_font, 
				(suffix.size() > 0) ? (std::to_string(_hovering_value) + " " + suffix).c_str() : std::to_string(_hovering_value).c_str());
		}
	}

	y_offset += 30;
}

void c_menu::draw_radar()
{
	if (!vars::visuals::radar) {
		return;
	}

	static int _drag_x = 300;
	static int _drag_y = 300;
	static int _width = 200;
	static int _height = 200;

	static bool _dragging = false;
	bool _click = false;
	static bool _resizing = false;


	if (GetAsyncKeyState(VK_LBUTTON))
		_click = true;

	vec2_t _mouse_pos = mouse_pos();

	if (_dragging && !_click)
		_dragging = false;

	if (_resizing && !_click)
		_resizing = false;

	if (_dragging && _click)
	{
		_pos_radar.x = _mouse_pos.x - _drag_x;
		_pos_radar.y = _mouse_pos.y - _drag_y;
	}
	
	if (_resizing && _click) {
		_width = _mouse_pos.x - _pos_radar.x;
		_height = _mouse_pos.y - _pos_radar.y;

		if (_width < 200)
			_width = 200;

		if (_height < 200)
			_height = 200;

		if (_width > 600)
			_width = 600;

		if (_height > 600)
			_height = 600;
	}

	if (mouse_in_params(_pos_radar.x, _pos_radar.y, _width, _height) && !(mouse_in_params(_pos_radar.x + 20, _pos_radar.y + 20, _width - 10, _height - 10)))
	{
		_dragging = true;
		_drag_x = _mouse_pos.x - _pos_radar.x;
		_drag_y = _mouse_pos.y - _pos_radar.y;
	}


	if (mouse_in_params(_pos_radar.x, _pos_radar.y,_width, _height) && !(mouse_in_params(_pos_radar.x, _pos_radar.y, _width - 30, _height - 30))) {
		_resizing = true;
	}

	draw::draw_filled_rect(_pos_radar.x, _pos_radar.y, _width, _height, colour_t(50, 50, 50, 255));
	draw::draw_filled_rect(_pos_radar.x, _pos_radar.y + 20, _width, _height - 20, colour_t(46, 46, 46, 255));
	draw::draw_filled_rect(_pos_radar.x, _pos_radar.y + 20, _width, 1, colour_t(195, 141, 145, 255));

	draw::draw_line(_pos_radar.x + _width / 2, _pos_radar.y + _height - 2, _pos_radar.x + _width / 2, _pos_radar.y + 20, colour_t(125, 125, 125, 255));
	draw::draw_line(_pos_radar.x, (_pos_radar.y + _height / 2) + 5, _pos_radar.x + _width, (_pos_radar.y + _height / 2) + 5, colour_t(125, 125, 125, 255));

	draw::draw_outlined_rect(_pos_radar.x, _pos_radar.y, _width, _height, colour_t(0, 0, 0, 255 / 3));

	draw::draw_string(_pos_radar.x + 10, _pos_radar.y + 2, colour_t(180, 180, 180, 255), font::menu_font, "radar");

	if (interfaces::engineclient->is_connected() && interfaces::engineclient->is_in_game()) {
		c_base_player* local_player = global::local_player;
		if (!local_player) { return; }

		vec3_t local_angles; interfaces::engineclient->get_view_angles(local_angles);
		for (int i = 1; i <= interfaces::engineclient->get_max_clients(); i++) {
			c_base_player* entity = reinterpret_cast<c_base_player*>(interfaces::entitylist->get_client_entity(i));
			if (!entity || entity->is_dead() || entity->is_dormant() || entity == global::local_player) {
				continue;
			}

			if (vars::visuals::player_enemyonly && entity->get_team_num() == global::local_player->get_team_num()) {
				continue;
			}

			if (utilities::has_condition(entity, 16) || utilities::has_condition(entity, 8192)) {
				continue;
			}

			vec3_t	entity_position = entity->get_world_space_center(),
				local_position = local_player->get_world_space_center();

			if (entity_position.is_zero() || local_position.is_zero()) {
				continue;
			}

			float	angle = DEG2RAD(local_angles.y);

			float	radar_x = local_position.x - entity_position.x,
				radar_y = local_position.y - entity_position.y;

			float	rotate_x = radar_y * cos(angle) - radar_x * sin(angle),
				rotate_y = radar_x * cos(angle) + radar_y * sin(angle);

			float	final_x = (_pos_radar.x + (_width / 2)) + rotate_x / 10,
				final_y = (_pos_radar.y + 5 + (_height / 2)) + rotate_y / 10;

			if (final_x < _pos_radar.x) {
				final_x = _pos_radar.x;
			}

			if (final_x > _pos_radar.x + _width) {
				final_x = _pos_radar.x + _width - 3;
			}

			if (final_y < _pos_radar.y + 20) {
				final_y = _pos_radar.y + 22;
			}

			if (final_y > _pos_radar.y + _height) {
				final_y = _pos_radar.y + _height - 2;
			}

			draw::draw_filled_rect(final_x, final_y, 2, 2, colour_t(200, 200, 200));
		}
	}
}
vec2_t c_menu::mouse_pos()
{
	POINT mouse_position;
	GetCursorPos(&mouse_position);
	ScreenToClient(FindWindow(0, "Team Fortress 2"), &mouse_position);
	return { static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y) };
}
bool c_menu::mouse_in_params(int x, int y, int x2, int y2) {
	if (mouse_pos().x > x && mouse_pos().y > y && mouse_pos().x < x2 + x && mouse_pos().y < y2 + y)
		return true;
	return false;
}

bool test = false;
int testcombo = 0;
int test_slider;
void c_menu::draw(unsigned int panel)
{
	static bool _pressed = true;

	if (!_pressed && GetAsyncKeyState(VK_INSERT))
		_pressed = true;
	else if (_pressed && !GetAsyncKeyState(VK_INSERT))
	{
		_pressed = false;
		menu_opened = !menu_opened;

		interfaces::panel->set_mouse_input_enabled(panel, menu_opened);
	}
	screen_width  = global::screen_width;
	screen_height = global::screen_height;

	static vec2_t _mouse_pos;

	static int _drag_x = 300;
	static int _drag_y = 300;
	static int _width = 480;
	static int _height = 340;

	static bool _dragging = false;
	bool _click = false;
	static bool _resizing = false;
	if (menu_opened)
	{
		MenuAlpha_Main = min(MenuAlpha_Main + 10, 255);
		MenuAlpha_Text = min(MenuAlpha_Text + 10, 255);

		if (GetAsyncKeyState(VK_LBUTTON))
			_click = true;

		vec2_t _mouse_pos = mouse_pos();

		if (_dragging && !_click)
			_dragging = false;

		if (_dragging && _click)
		{
			_pos.x = _mouse_pos.x - _drag_x;
			_pos.y = _mouse_pos.y - _drag_y;
		}

		if (mouse_in_params(_pos.x, _pos.y, _width, _height) && !(mouse_in_params(_pos.x + 20, _pos.y + 20, _width - 10, _height - 10)))
		{
			_dragging = true;
			_drag_x = _mouse_pos.x - _pos.x;
			_drag_y = _mouse_pos.y - _pos.y;
		}
	}
	else {
		MenuAlpha_Main = max(MenuAlpha_Main - 10, 0);
		MenuAlpha_Text = max(MenuAlpha_Text - 10, 0);
	}

	draw::draw_filled_rect(_pos.x, _pos.y, _width, _height, colour_t(50, 50, 50, MenuAlpha_Main));
	draw::draw_filled_rect(_pos.x, _pos.y + 21, 100, _height - 21, colour_t(46, 46, 46, MenuAlpha_Main));
	draw::draw_filled_rect(_pos.x, _pos.y + 20, _width, 1, colour_t(195, 141, 145, MenuAlpha_Main));
	draw::draw_filled_rect(_pos.x + 100, _pos.y + 21, 1, _height - 21, colour_t(37, 37, 37, MenuAlpha_Main));
	draw::draw_outlined_rect(_pos.x, _pos.y, _width, _height, colour_t(0, 0, 0, MenuAlpha_Main / 3));
	draw::draw_string(_pos.x + 10, _pos.y + 2, colour_t(180, 180, 180, MenuAlpha_Text), font::menu_font, "deer_dance for Team Fortress 2");

	int in_sizew = _height - 32;
	static int tab_selected = 0;
	static const int tab_amount = 3;
	static std::string tab_names[tab_amount] = { "T", "H", "P" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size = get_text_size(font::tab_font, tab_names[i].c_str());

		int tab_area[4] = { _pos.x, _pos.y + 20 + i * (in_sizew / tab_amount), 100, (in_sizew / tab_amount) };

		if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_params(tab_area[0], tab_area[1], tab_area[2], tab_area[3]))
			tab_selected = i;

		if (tab_selected == i)
			draw::draw_string(tab_area[0] + (tab_area[2] / 2) - (text_size.right / 2), tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), colour_t(185, 185, 185, MenuAlpha_Text), font::tab_font, tab_names[i].c_str());
		else
			draw::draw_string(tab_area[0] + (tab_area[2] / 2) - (text_size.right / 2), tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2), colour_t(125, 125, 125, MenuAlpha_Text), font::tab_font, tab_names[i].c_str());
	}

	static std::string  thirdperson_anglez[2] = { "real angle", "fake angle" },
						chams_model[2] = { "flattened", "textured" },

						chams_activation[3] = { "none", "vis", "vis + xqz" },
						movement_options[3] = { "none", "autohop", "autohop + strafe" },
						esp_type[3] = { "player", "building", "world" },

						fakelag_modes[4] = { "none", "always", "avoid ground", "on ground" },

						anti_aim_pitch[5] = { "none", "down", "up", "fake-down", "fake-up" },
						anti_aim_real_yaw[5] = { "none", "inverse", "left", "right", "manual (arrows)" },
						anti_aim_fake_yaw[5] = { "none", "inverse", "left", "right", "spinbot" },
						main_hitbox[5] = { "head", "body", "auto", "hitscan", "hitscan prefer body" },
						chatspam_selection[5] = { "blank", "lmaobox", "nullcore", "lithium", "deer_dance" },

						keys[8] = { "always", "mouse1", "mouse2", "mouse3", "mouse4", "mouse5", "shift", "alt" };

	switch (tab_selected) {
		case aimbot_tab: {
			groupbox(106, 31, 180, 186, "aimbot"); {
				checkbox("enabled", &vars::aimbot::enabled);
				slider(180, "field of vew", &vars::aimbot::field_of_view, "deg", 10);
				combobox(5, "main hitbox", main_hitbox, &vars::aimbot::main_hitbox);
				checkbox("autoshoot", &vars::aimbot::autoshoot);
				checkbox("silent_aim", &vars::aimbot::silent_aim);
				checkbox("bullet_time", &vars::aimbot::bullet_time);
				checkbox("prefer_medic", &vars::aimbot::prefer_medic);
				checkbox("projectile_aimbot", &vars::aimbot::projectile_aimbot);
			}

			groupbox(106, 226, 180, 82, "triggerbot"); {
				checkbox("enabled", &vars::triggerbot::triggerbot_enabled);
				checkbox("on key", &vars::triggerbot::triggerbot_on_key);
				checkbox("auto detonate", &vars::triggerbot::auto_detonate);
				checkbox("auto detonate on key", &vars::triggerbot::auto_detonate_on_key);
			}

			groupbox(294, 31, 180, 65, "position adjustment"); {
				checkbox("enabled", &vars::position_adjustment::enabled);
				slider(12, "maximum ticks", &vars::position_adjustment::ticks, "ticks", 1);
			}

			groupbox(294, 105, 180, 66, "targeting"); {
				checkbox("sentrygun", &vars::aimbot::aimbot_sentry);
				checkbox("dispenser", &vars::aimbot::aimbot_dispenser);
				checkbox("teleporters", &vars::aimbot::aimbot_teleporters);
			}

			groupbox(294, 180, 180, 96, "key bindings"); {
				combobox(7, "aimbot key", keys, &vars::aimbot::aimbot_key);
				combobox(7, "triggerbot key", keys, &vars::triggerbot::triggerbot_key);
			}
		} break;
		case visual_tab: {
			groupbox(106, 31, 180, 193, "esp"); {
				combobox(3, "esp type", esp_type, &vars::visuals::esp_type);
				switch (vars::visuals::esp_type) {
					case 0: {
						checkbox("enemy only", &vars::visuals::player_enemyonly);
						checkbox("box", &vars::visuals::player_box);
						checkbox("health", &vars::visuals::player_health);
						checkbox("name", &vars::visuals::player_name);
						checkbox("weapon", &vars::visuals::player_weapon);
						checkbox("steamid", &vars::visuals::player_steamid);
						checkbox("backtrack visualization", &vars::visuals::player_backtrack_visulization);
						checkbox("chams", &vars::visuals::player_chams);
					} break;
					case 1: {
						checkbox("enemy only", &vars::visuals::building_enemyonly);
						checkbox("box", &vars::visuals::building_box);
						checkbox("health", &vars::visuals::building_health);
						checkbox("name", &vars::visuals::building_name);
						checkbox("builder", &vars::visuals::building_builder);
						checkbox("chams", &vars::visuals::building_chams);
					} break;
					case 2: {
						checkbox("box", &vars::visuals::world_box);
						checkbox("name", &vars::visuals::world_name);
						checkbox("health chams", &vars::visuals::world_health_chams);
						checkbox("ammo chams", &vars::visuals::world_ammo_chams);
						checkbox("projectile chams", &vars::visuals::world_projectile_chams);
					}
				}
			}

			groupbox(106, 233, 180, 95, "visual misc"); {
				checkbox("crosshair", &vars::visuals::custom_crosshair);
				checkbox("spectators list", &vars::visuals::spectators);
				checkbox("nightmode", &vars::visuals::nightmode);
				slider(100, "nightmode value", &vars::visuals::nightmode_value, "", 10);
			}

			groupbox(294, 31, 180, 160, "even more misc"); {
				checkbox("thirdperson (h)", &vars::visuals::thirdperson);
				combobox(2, "thirdperson angle", thirdperson_anglez, &vars::visuals::thirdperson_angle);
				checkbox("no_zoom", &vars::visuals::no_zoom);
				checkbox("no_scope", &vars::visuals::no_scope);
				slider(150, "forced_fov", &vars::visuals::forced_fov, "", 10);
				checkbox("radar", &vars::visuals::radar);
			}

			groupbox(294, 200, 180, 116, "coloured models"); {
				combobox(3, "cham activation", chams_activation, &vars::chams::chams_activation);
				combobox(2, "cham model", chams_model, &vars::chams::chams_model);
				checkbox("local player", &vars::chams::local_player);
			}
		} break;
		case misc_tab: {
			groupbox(106, 117, 180, 212, "hvh features"); {
				slider(14, "fakelag value", &vars::hvh::fakelag_value, "packets", 1);
				combobox(4, "fakelag mode", fakelag_modes, &vars::hvh::fakelag_mode);

				combobox(5, "pitch", anti_aim_pitch, &vars::hvh::anti_aim_pitch);
				combobox(5, "real yaw", anti_aim_real_yaw, &vars::hvh::anti_aim_real_yaw);
				combobox(5, "fake yaw", anti_aim_fake_yaw, &vars::hvh::anti_aim_fake_yaw);
			}

			groupbox(106, 31, 180, 77, "miscellanous"); {
				combobox(3, "movement", movement_options, &vars::miscellanous::movement);
				checkbox("taunt sliding", &vars::miscellanous::taunt_slide);
			}

			groupbox(294, 31, 180, 96, "even more hvh features"); {
				slider(10, "spinbot speed", &vars::hvh::anti_aim_spinbot_speed, "", 1);
				checkbox("auto pitch resolver", &vars::hvh::auto_pitch_resolver);
				checkbox("shot based pitch resolver", &vars::hvh::auto_resolver);
				checkbox("shot based yaw resolver (WIP)", &vars::hvh::auto_yaw_resolver);
			}

			groupbox(294, 220, 180, 36, "announcer (windows sound)"); {
				checkbox("enabled", &vars::announcer::enabled);
			}

			groupbox(294, 136, 180, 76, "chatspam"); {
				checkbox("enabled", &vars::chatspam::chatspam_enabled);
				combobox(5, "selection", chatspam_selection, &vars::chatspam::chatspam_selection);
			}
		} break;
		default: break;
	}
}