#ifndef colour_t_H
#define colour_t_H
#ifdef _WIN32
#pragma once
#endif

class colour_t
{
public:

	colour_t()
	{
		*((int *)this) = 0;
	}

	colour_t(int r, int g, int b)
	{
		set_colour_t(r, g, b, 255);
	}

	colour_t(int _r, int _g, int _b, int _a)
	{
		set_colour_t(_r, _g, _b, _a);
	}

	// set the colour_t
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void set_colour_t(int r, int g, int b, int a = 0)
	{
		_colour_t[0] = (unsigned char)r;
		_colour_t[1] = (unsigned char)g;
		_colour_t[2] = (unsigned char)b;
		_colour_t[3] = (unsigned char)a;
	}

	void get_colour_t(int& r, int& g, int& b, int& a) const
	{
		r = _colour_t[0];
		g = _colour_t[1];
		b = _colour_t[2];
		a = _colour_t[3];
	}

	void set_raw_colour_t(int colour_t32)
	{
		*((int*)this) = colour_t32;
	}

	int get_raw_colour_t() const
	{
		return *((int*)this);
	}

	inline int r() const { return _colour_t[0]; }
	inline int g() const { return _colour_t[1]; }
	inline int b() const { return _colour_t[2]; }
	inline int a() const { return _colour_t[3]; }

	unsigned char& operator[](int index)
	{
		return _colour_t[index];
	}

	const unsigned char& operator[](int index) const
	{
		return _colour_t[index];
	}

	bool operator==(const colour_t& rhs) const
	{
		return (*((int *)this) == *((int*)&rhs));
	}

	bool operator!=(const colour_t& rhs) const
	{
		return !(operator==(rhs));
	}

	colour_t& operator=(const colour_t& rhs)
	{
		set_raw_colour_t(rhs.get_raw_colour_t());
		return *this;
	}

	static colour_t from_hsb(float hue, float saturation, float brightness) {
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1) {
			return colour_t (
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2) {
			return colour_t (
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3) {
			return colour_t (
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4) {
			return colour_t (
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5) {
			return colour_t (
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else {
			return colour_t (
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

private:
	unsigned char _colour_t[4];
};

#endif // !colour_t_H