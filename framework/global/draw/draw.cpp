#include "../../main.h"

namespace draw {
	void draw_outlined_rect(int x, int y, int w, int h, colour_t col) {
		interfaces::surface->draw_set_colour_t(col);
		interfaces::surface->draw_outlined_rect(x, y, x + w, y + h);
	}
	void draw_line(int x, int y, int x1, int y1, colour_t col) {
		interfaces::surface->draw_set_colour_t(col);
		interfaces::surface->draw_line(x, y, x1, y1);
	}
	void draw_filled_rect(int x, int y, int w, int h, colour_t col) {
		interfaces::surface->draw_set_colour_t(col);
		interfaces::surface->draw_filled_rect(x, y, x + w, y + h);
	}
	void draw_string(int x, int y, colour_t col, unsigned long font, const char* text, ...) {
		if (text == NULL) return;

		va_list			va_alist;
		char			buffer[1024];
		wchar_t			string[1024];

		va_start(va_alist, text);
		_vsnprintf(buffer, sizeof(buffer), text, va_alist);
		va_end(va_alist);

		MultiByteToWideChar(CP_UTF8, 0, buffer, 256, string, 256);

		interfaces::surface->draw_set_text_pos(x, y);
		interfaces::surface->draw_set_text_font(font);
		interfaces::surface->draw_set_text_colour_t(col.r(), col.g(), col.b(), col.a());
		interfaces::surface->draw_print_text(string, wcslen(string));
	}
	void draw_string_centered(int x, int y, colour_t col, unsigned long font, const char* text, ...) {
		if (text == NULL) return;

		va_list		va_alist;
		char		buffer[1024];
		wchar_t		string[1024];
		int			w, h;

		va_start(va_alist, text);
		_vsnprintf(buffer, sizeof(buffer), text, va_alist);
		va_end(va_alist);

		MultiByteToWideChar(CP_UTF8, 0, buffer, 256, string, 256);

		interfaces::surface->get_text_size(font, string, w, h);
		interfaces::surface->draw_set_text_pos(x - w / 2, y/* - h / 2*/);
		interfaces::surface->draw_set_text_font(font);
		interfaces::surface->draw_set_text_colour_t(col.r(), col.g(), col.b(), col.a());
		interfaces::surface->draw_print_text(string, wcslen(string));
	}
}