#ifndef ISURFACE_H
#define ISURFACE_H
#ifdef _WIN32
#pragma once
#endif

struct vertex_t
{
	vec2_t	m_Position;
	vec2_t	m_TexCoord;

	vertex_t() {}
	vertex_t(const vec2_t &pos, const vec2_t &coord = vec2_t(0, 0)) {
		m_Position = pos;
		m_TexCoord = coord;
	}
	void initialize(const vec2_t &pos, const vec2_t &coord = vec2_t(0, 0)) {
		m_Position = pos;
		m_TexCoord = coord;
	}
};

class isurface
{
public:

	void draw_set_colour_t(colour_t col)
	{
		using original_fn = void(__thiscall*)(PVOID, colour_t);
		utilities::call_vfunc<original_fn>(this, 10)(this, col);
	}

	void draw_filled_rect(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(PVOID, int, int, int, int);
		utilities::call_vfunc<original_fn>(this, 12)(this, x0, y0, x1, y1);
	}

	void draw_outlined_rect(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(PVOID, int, int, int, int);
		utilities::call_vfunc<original_fn>(this, 14)(this, x0, y0, x1, y1);
	}

	void draw_line(int x0, int y0, int x1, int y1)
	{
		using original_fn = void(__thiscall*)(PVOID, int, int, int, int);
		utilities::call_vfunc<original_fn>(this, 15)(this, x0, y0, x1, y1);
	}

	void draw_set_text_font(unsigned long font)
	{
		using original_fn = void(__thiscall*)(PVOID, unsigned long);
		utilities::call_vfunc<original_fn>(this, 17)(this, font);
	}

	void draw_set_text_colour_t(int r, int g, int b, int a)
	{
		using original_fn = void(__thiscall*)(PVOID, int, int, int, int);
		utilities::call_vfunc<original_fn>(this, 19)(this, r, g, b, a);
	}

	void draw_set_text_pos(int x, int y)
	{
		using original_fn = void(__thiscall*)(PVOID, int, int);
		utilities::call_vfunc<original_fn>(this, 20)(this, x, y);
	}

	void draw_print_text(const wchar_t* text, int text_length)
	{
		using original_fn = void(__thiscall*)(PVOID, const wchar_t*, int, int);
		utilities::call_vfunc<original_fn>(this, 22)(this, text, text_length, 0);
	}

	unsigned long create_font()
	{
		using original_fn = unsigned long(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 66)(this);
	}

	void set_font_glyph_set(unsigned long& font, const char* windows_font_name, int tall, int weight, int blur, int scanlines, int flags)
	{
		using original_fn = void(__thiscall*)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		utilities::call_vfunc<original_fn>(this, 67)(this, font, windows_font_name, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void get_text_size(unsigned long font, const wchar_t* text, int& width, int& tall)
	{
		using original_fn = void(__thiscall*)(PVOID, unsigned long, const wchar_t*, int&, int&);
		utilities::call_vfunc<original_fn>(this, 75)(this, font, text, width, tall);
	}

	void draw_set_texture_rgba(int x, unsigned char const* woah, int eh, int ah, int bh = 0, bool neat = true)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, unsigned char const*, int, int, int, bool);
		utilities::call_vfunc<OriginalFn>(this, 31)(this, x, woah, eh, ah, bh, neat);
	}
	void draw_textured_rect(int x, int y, int width, int height)
	{
		typedef void(__thiscall *OriginalFn)(void*, int, int, int, int);
		utilities::call_vfunc<OriginalFn>(this, 34)(this, x, y, width, height);
	}
	void draw_set_texture(int x)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		utilities::call_vfunc<OriginalFn>(this, 32)(this, x);
	}
	int create_new_texture_id(bool b = true)
	{
		typedef int(__thiscall* OriginalFn)(PVOID, bool);
		return 	utilities::call_vfunc<OriginalFn>(this, 37)(this, b);
	}
	void draw_textured_polygon(int n, vertex_t *vertices, bool bClipVertices = true)
	{
		typedef void(__thiscall *OriginalFn)(PVOID, int, vertex_t*, bool);
		return 	utilities::call_vfunc<OriginalFn>(this, 102)(this, n, vertices, bClipVertices);
	}
	void play_sound(const char* sound_file)
	{
		typedef void(__thiscall *OriginalFn)(PVOID, const char*);
		return 	utilities::call_vfunc<OriginalFn>(this, 78)(this, sound_file);
	}
};

#endif // !ISURFACE_H
