#ifndef IVDEBUGOVERLAY_H
#define IVDEBUGOVERLAY_H
#ifdef _WIN32
#pragma once
#endif

class ivdebugoverlay
{
public:

	int screen_position(const vec3_t& in, vec3_t& out)
	{
		using original_fn = int(__thiscall*)(PVOID, const vec3_t&, vec3_t&);
		return utilities::call_vfunc<original_fn>(this, 10)(this, in, out);
	}

};

#endif // !IVDEBUGOVERLAY_H