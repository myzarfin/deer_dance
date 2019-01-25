#ifndef IVENGINECLIENT_H
#define IVENGINECLIENT_H
#ifdef _WIN32
#pragma once
#endif

typedef struct player_info_s
{

	char			name[32];
	int				user_id;
	char			guid[33];
	unsigned long	friends_id;
	char			friends_name[32];
	bool			fake_player;
	bool			ishltv;
	unsigned long	custom_files[4];
	unsigned char	files_downloaded;

} player_info_t;

class ivengineclient
{
public:

	int get_local_player()
	{
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 12)(this);
	}

	bool is_in_game()
	{
		using original_fn = bool(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 26)(this);
	}

	bool is_connected()
	{
		using original_fn = bool(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 27)(this);
	}

	bool is_taking_screenshot()
	{
		using original_fn = bool(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 85)(this);
	}

	void set_view_angles(vec3_t& ang)
	{
		using original_fn = void(__thiscall*)(PVOID, vec3_t&);
		return utilities::call_vfunc<original_fn>(this, 20)(this, ang);
	}

	bool get_player_info(int i, player_info_t* info)
	{
		using original_fn = bool(__thiscall*)(PVOID, int, player_info_t*);
		return utilities::call_vfunc<original_fn>(this, 8)(this, i, info);
	}
	void client_command(const char* command, bool unrestricted) 
	{
		using original_fn = void(__thiscall*)(PVOID, const char*);
		return utilities::call_vfunc<original_fn>(this, unrestricted ? 106 : 7)(this, command);
	}
	int get_max_clients()
	{
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 21)(this);
	}
	void get_view_angles(vec3_t& vector)
	{
		using original_fn = void(__thiscall*)(PVOID, vec3_t& vector);
		utilities::call_vfunc<original_fn>(this, 19)(this, vector);
	}
	c_net_channel* get_net_channel_info() {
		using original_fn = c_net_channel * (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 72)(this);
	}
	void get_screen_size(int &width, int &height) {
		using original_fn = void(__thiscall*)(PVOID, int&, int&);
		return utilities::call_vfunc<original_fn>(this, 5)(this, width, height);
	}
	const matrix3x4_t& world_to_screen_matrix() {
		using original_fn = const matrix3x4_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 36)(this);
	}
};

#endif // !IVENGINECLIENT_H
