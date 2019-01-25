#ifndef IEVENTMANGER_H
#define IEVENTMANGER_H
#ifdef _WIN32
#pragma once
#endif

class igame_event {
public:
	virtual ~igame_event() { };
	virtual const char* get_name() const = 0;

	virtual bool is_reliable() const = 0;
	virtual bool is_local() const = 0;
	virtual bool is_empty(const char* key_name = 0) = 0;

	virtual bool		get_bool(const char* key_name = 0, bool default_valye = false) = 0;
	virtual int			get_int(const char* key_name = 0, int default_valye = 0) = 0;
	virtual float		get_float(const char* key_name = 0, float default_valye = 0.0f) = 0;
	virtual const char* get_string(const char* key_name = 0, const char* default_valye = "") = 0;

	virtual void set_bool  (const char* key_name,  bool			new_value) = 0;
	virtual void set_int   (const char* key_name,  int			new_value) = 0;
	virtual void set_float (const char* key_name,  float		new_value) = 0;
	virtual void set_string(const char* key_namee, const char*  new_value) = 0;
};

class igame_event_listener {
public:
	virtual ~igame_event_listener(void) {};

	virtual void fire_game_event(igame_event *event) = 0;
};

class igame_event_manager {
public:
	bool add_listener(igame_event_listener* listener, const char* name, bool server_sided) {
		using original_fn = bool(__thiscall*)(void*, igame_event_listener*, const char*, bool);
		return utilities::call_vfunc<original_fn>(this, 3)(this, listener, name, server_sided);
	}
};

#endif // !IEVENTMANGER_H
