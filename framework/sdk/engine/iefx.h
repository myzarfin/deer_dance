#ifndef IEFX_H
#define IEFX_H
#ifdef _WIN32
#pragma once
#endif

struct colour_t_rgb_exp_22
{
	byte r, g, b;
	signed char exponent;
};

struct dlight_t
{

	int					flags;
	vec3_t				origin;
	float				radius;
	colour_t_rgb_exp_22	colour_t;
	float				die;
	float				decay;
	float				minlight;
	int					key;
	int					style;
	vec3_t				direction;
	float				inner_angle;
	float				outer_angle;

	float get_radius() const
	{
		return radius;
	}

	float get_radius_squared() const
	{
		return radius * radius;
	}

	float is_radius_greater_than_zero() const
	{
		return radius > 0.0f;
	}

};

class ivefx
{
public:

	dlight_t* alloc_dlight(int key)
	{
		using original_fn = dlight_t*(__thiscall*)(PVOID, int);
		return utilities::call_vfunc<original_fn>(this, 4)(this, key);
	}

	dlight_t* alloc_elight(int key)
	{
		using original_fn = dlight_t*(__thiscall*)(PVOID, int);
		return utilities::call_vfunc<original_fn>(this, 5)(this, key);
	}

	dlight_t* elight_by_key(int key)
	{
		using original_fn = dlight_t*(__thiscall*)(PVOID, int);
		return utilities::call_vfunc<original_fn>(this, 8)(this, key);
	}

};

#endif // !IEFX_H