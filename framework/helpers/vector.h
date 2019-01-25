#ifndef vec3_t_H
#define vec3_t_H
#ifdef _WIN32
#pragma once
#endif

class vec3_t
{
public:
	float x, y, z;

	vec3_t(void)
	{
		invalidate();
	}

	vec3_t(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}

	vec3_t(float* clr)
	{
		x = clr[0]; y = clr[1]; z = clr[2];
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	bool operator==(const vec3_t& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	bool operator!=(const vec3_t& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	vec3_t& operator+=(const vec3_t& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	vec3_t& operator-=(const vec3_t& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	vec3_t& operator*=(float fl)
	{
		x *= fl; y *= fl; z *= fl;
		return *this;
	}

	vec3_t& operator*=(const vec3_t& v)
	{
		x *= v.x; y *= v.y; z *= v.z;
		return *this;
	}

	vec3_t& operator/=(const vec3_t& v)
	{
		x /= v.x; y /= v.y; z /= v.z;
		return *this;
	}

	vec3_t&	operator+=(float fl)
	{
		x += fl; y += fl; z += fl;
		return *this;
	}

	vec3_t&	operator/=(float fl)
	{
		x /= fl; y /= fl; z /= fl;
		return *this;
	}

	vec3_t&	operator-=(float fl)
	{
		x -= fl; y -= fl; z -= fl;
		return *this;
	}

	vec3_t operator-(void) const
	{
		return vec3_t(-x, -y, -z);
	}

	vec3_t operator+(const vec3_t& v) const
	{
		return vec3_t(x + v.x, y + v.y, z + v.z);
	}

	vec3_t operator-(const vec3_t& v) const
	{
		return vec3_t(x - v.x, y - v.y, z - v.z);
	}

	vec3_t operator*(float fl) const
	{
		return vec3_t(x * fl, y * fl, z * fl);
	}

	vec3_t operator*(const vec3_t& v) const
	{
		return vec3_t(x * v.x, y * v.y, z * v.z);
	}

	vec3_t operator/(float fl) const
	{
		return vec3_t(x / fl, y / fl, z / fl);
	}

	vec3_t operator/(const vec3_t& v) const
	{
		return vec3_t(x / v.x, y / v.y, z / v.z);
	}

	float length(void) const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float length2d(void) const
	{
		return sqrtf(x * x + y * y);
	}

	float dot(const vec3_t& b) const
	{
		return (x * b.x + y * b.y + z * b.z);
	}

	float dist_to(const vec3_t &other) const
	{
		vec3_t delta;

		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length();
	}

	bool is_zero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}

	void invalidate()
	{
		x = y = z = std::numeric_limits<float>::infinity();
	}

	void init(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		this->x = x; this->y = y; this->z = z;
	}

	float length_sqr(void) const
	{
		return (x * x + y * y + z * z);
	}

};

inline void vec3_t_subtract(const vec3_t& a, const vec3_t& b, vec3_t& result)
{
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
}

inline void vec3_t_clear(vec3_t& a)
{
	a.x = a.y = a.z = 0.0f;
}

inline void vec3_t_copy(const vec3_t& src, vec3_t& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

inline void vec3_t_add(const vec3_t& a, const vec3_t& b, vec3_t& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

inline void vec3_t_angles(const vec3_t& forward, vec3_t& angles)
{

	float tmp, yaw, pitch;

	if (forward.y == 0 && forward.x == 0) {

		yaw = 0;

		if (forward.z > 0) {
			pitch = 270;
		} else {
			pitch = 90;
		}

	} else {

		yaw = RAD2DEG(atan2f(forward.y, forward.x));

		if (yaw < 0)
			yaw += 360;

		tmp = forward.length2d();
		pitch = RAD2DEG(atan2f(-forward.z, tmp));

		if (pitch < 0)
			pitch += 360;

	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;

}

class __declspec(align(16))vec3_t_aligned : public vec3_t
{
public:

	inline vec3_t_aligned(void) {};

	inline vec3_t_aligned(float x, float y, float z)
	{
		init(x, y, z);
	}

	explicit vec3_t_aligned(const vec3_t& v)
	{
		init(v.x, v.y, v.z);
	}

	vec3_t_aligned& operator=(const vec3_t& v)
	{
		init(v.x, v.y, v.z);
		return *this;
	}

	float w;

};

#endif // !vec3_t_H