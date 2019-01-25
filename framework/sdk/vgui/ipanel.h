#ifndef IPANEL_H
#define IPANEL_H
#ifdef _WIN32
#pragma once
#endif

class ipanel
{
public:

	const char* get_name(unsigned int vgui_panel)
	{
		using original_fn = const char*(__thiscall*)(PVOID, unsigned int);
		return utilities::call_vfunc<original_fn>(this, 36)(this, vgui_panel);
	}
	void set_mouse_input_enabled(unsigned int panel, bool state)
	{
		using original_fn = void(__thiscall*)(PVOID, unsigned int, bool);
		utilities::call_vfunc<original_fn>(this, 32)(this, panel, state);
	}
};

#endif // !IPANEL_H