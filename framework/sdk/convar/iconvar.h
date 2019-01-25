#ifndef ICONVAR_H
#define ICONVAR_H
#ifdef _WIN32
#pragma once
#endif

class convar;
class iconvar;
class ccommand;
class con_command;
class con_command_base;

class icvr {
public:
	virtual void			set_value(const char* value) = 0;
	virtual void			set_value(float value) = 0;
	virtual void			set_value(int value) = 0;
	virtual const char*		get_name(void)const = 0;
	virtual bool			is_flag_set(int nFlag) const = 0;
};

typedef void(*change_callback_fn)(iconvar* variable, const char* old_value, float fl_old_value);
class icon_command_base_accessor {
public:
	virtual bool register_con_command_base(con_command_base* variable) = 0;
};

class iapp_system {
public:
	virtual bool connect(create_interface_fn factory) = 0;
	virtual void disconnect() = 0;

	virtual void *query_interfaces(const char* interface_name) = 0;

	virtual int initialize() = 0;
	virtual void shutdown(char* reason) = 0;
};

class iconvar : public iapp_system {
public:
	virtual int				allocate_dll_identifier() = 0;

	virtual void			register_con_command(con_command_base* command_base) = 0;
	virtual void			unregister_con_command(con_command_base* command_base) = 0;
	virtual void			unregister_con_commands(int id) = 0;

	virtual const char*		get_command_line_value(const char* var_name) = 0;

	virtual con_command_base		*find_command_base(const char *name) = 0;
	virtual const con_command_base	*find_command_base(const char *name) const = 0;
	virtual convar					*find_variable(const char *var_name) = 0;
	virtual const convar			*find_variable(const char *var_name) const = 0;
	virtual con_command				*find_command(const char *name) = 0;
	virtual const con_command		*find_command(const char *name) const = 0;

	virtual con_command_base		*get_commands(void) = 0;
	virtual const con_command_base	*get_commands(void) const = 0;

	virtual void			install_global_change_callback(change_callback_fn callback) = 0;
	virtual void			remove_global_change_callback(change_callback_fn callback) = 0;
	virtual void			call_global_change_callbacks(convar *var, const char *pOldString, float flOldValue) = 0;

	virtual void			install_console_display_func(void* pDisplayFunc) = 0;
	virtual void			remove_console_display_func(void* pDisplayFunc) = 0;
	virtual void			console_print_colour(const colour_t& clr, const char *pFormat, ...) const = 0;
	virtual void			console_print(const char *pFormat, ...) const = 0;
	virtual void			console_dprint(const char *pFormat, ...) const = 0;

	virtual void			revert_flagged_convars(int nFlag) = 0;

	virtual void			install_convar_query(void *pQuery) = 0;

#if defined( _X360 )
	virtual void			publish_to_vx_console() = 0;
#endif
	virtual bool			is_material_thread_set_allowed() const = 0;
	virtual void			queue_material_thread_set_value(convar *pConVar, const char *pValue) = 0;
	virtual void			queue_material_thread_set_value(convar *pConVar, int nValue) = 0;
	virtual void			queue_material_thread_set_value(convar *pConVar, float flValue) = 0;
	virtual bool			has_queued_material_thread_convar_sets() const = 0;
	virtual int				process_queued_material_thread_convar_sets() = 0;

protected:	class iconvar_iterator_internal;
public:
	class iterator {
	public:
		inline						iterator(iconvar *icvar);
		inline						~iterator(void);
		inline void					set_first(void);
		inline void					next(void);
		inline bool					is_valid(void);
		inline con_command_base		*get(void);
	private:
		iconvar_iterator_internal * m_iterator;
	};

protected:
	class iconvar_iterator_internal {
	public:
		virtual						~iconvar_iterator_internal(){}
		virtual void				set_first(void) = 0;
		virtual void				next(void) = 0;
		virtual	bool				is_valid(void) = 0;
		virtual con_command_base	*get(void) = 0;
	};

	virtual iconvar_iterator_internal	*factory_internal_iterator(void) = 0;
	friend class iterator;
};

class con_command_base
{
public:
	con_command_base(void);
	con_command_base(const char *pName, const char *pHelpString = 0, int flags = 0);

	virtual							~con_command_base(void);
	virtual	bool					is_command(void) const;
	virtual bool					is_flag_set(int flag) const;
	virtual void					add_flags(int flags);
	virtual void					remove_flags(int flags);
	virtual int						get_flags() const;
	virtual const char*				get_name(void) const;
	virtual const char*				get_help_text(void) const;
	virtual bool					is_registered(void) const;
	virtual int						get_dll_identifier() const;
	virtual void					create(const char *pName, const char *pHelpString = 0, int flags = 0);
	virtual void					initialize();

public:
	con_command_base *				m_next;
	bool							m_registered;
	const char*						m_name;
	const char*						m_help_string;
	int								m_flags;

public:
	static con_command_base*			con_command_bases;
	static icon_command_base_accessor*	accessor;
};

class convar : public con_command_base, public iconvar {
public:
	typedef con_command_base base_class;

	convar(const char *pName, const char *pDefaultValue, int flags = 0);

	convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString);
	convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax);
	convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, change_callback_fn callback);
	convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, change_callback_fn callback);

	virtual							~convar(void);

	virtual bool					is_flag_set(int flag) const;
	virtual const char*				get_help_text(void) const;
	virtual bool					is_registered(void) const;
	virtual const char*				get_name(void) const;
	virtual const char*				get_base_name(void) const;
	virtual int						get_split_screen_player_slot() const;
	DWORD							get_colour(void) const;
	virtual void					add_flags(int flags);
	virtual int						get_flags() const;
	virtual	bool					is_command(void) const;
	virtual void					set_value(const char *value);
	virtual void					set_value(float value);
	virtual void					set_value(int value);
	virtual void					set_value(DWORD value);
	virtual void					internal_set_value(const char *value);
	virtual void					internal_set_float_value(float fNewValue);
	virtual void					internal_set_int_value(int nValue);
	virtual void					internal_set_colour_value(DWORD value);
	virtual bool					clamp_value(float& value);
	virtual void					change_string_value(const char *tempVal, float flOldValue);
	virtual void					create(const char *pName, const char *pDefaultValue, int flags = 0,
		const char *pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = false, change_callback_fn callback = 0);

	float					get_float(void) const;
	int						get_int(void) const;
	const char*				get_string(void) const;
	const char*				get_default(void) const;

	void install_change_callback(change_callback_fn callback);

	struct CVValue_t {
		char*	m_pszString;
		int		m_StringLength;
		float	m_fValue;
		int		m_nValue;
	};

	convar*					m_pParent;
	const char	*			m_pszDefaultValue;
	CVValue_t				m_Value;
	bool						m_bHasMin;
	float					m_fMinVal;
	bool						m_bHasMax;
	float					m_fMaxVal;
	change_callback_fn		m_fnChangeCallback;

	char pad_0x0000[0x4]; //0x0000
	convar* pNext; //0x0004 
	__int32 bRegistered; //0x0008 
	char* pszName; //0x000C 
	char* pszHelpString; //0x0010 
	__int32 nFlags; //0x0014 
	char pad_0x0018[0x4]; //0x0018
	convar* pParent; //0x001C 
	char* pszDefaultValue; //0x0020 
	char* strString; //0x0024 
	__int32 StringLength; //0x0028 
	float fValue; //0x002C 
	__int32 nValue; //0x0030 
	__int32 bHasMin; //0x0034 
	float fMinVal; //0x0038 
	__int32 bHasMax; //0x003C 
	float fMaxVal; //0x0040 
	void* fnChangeCallback; //0x0044 
};

enum class ConvarFlags
{
	// The default, no flags at all
	FCVAR_NONE = 0,

	// Command to ConVars and ConCommands
	// ConVar Systems
	FCVAR_UNREGISTERED = (1 << 0), // If this is set, don't add to linked list, etc.
	FCVAR_DEVELOPMENTONLY = (1 << 1), // Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
	FCVAR_GAMEDLL = (1 << 2), // defined by the game DLL
	FCVAR_CLIENTDLL = (1 << 3), // defined by the client DLL
	FCVAR_HIDDEN = (1 << 4), // Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

	// ConVar only
	FCVAR_PROTECTED = (1 << 5),  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
	FCVAR_SPONLY = (1 << 6),  // This cvar cannot be changed by clients connected to a multiplayer server.
	FCVAR_ARCHIVE = (1 << 7),  // set to cause it to be saved to vars.rc
	FCVAR_NOTIFY = (1 << 8),  // notifies players when changed
	FCVAR_USERINFO = (1 << 9),  // changes the client's info string
	FCVAR_CHEAT = (1 << 14), // Only useable in singleplayer / debug / multiplayer & sv_cheats

	FCVAR_PRINTABLEONLY = (1 << 10), // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
	FCVAR_UNLOGGED = (1 << 11), // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
	FCVAR_NEVER_AS_STRING = (1 << 12), // never try to print that cvar

	// It's a ConVar that's shared between the client and the server.
	// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
	//  client, of course )
	// If a change is requested it must come from the console (i.e., no remote client changes)
	// If a value is changed while a server is active, it's replicated to all connected clients
	FCVAR_REPLICATED = (1 << 13), // server setting enforced on clients, TODO rename to FCAR_SERVER at some time
	FCVAR_DEMO = (1 << 16), // record this cvar when starting a demo file
	FCVAR_DONTRECORD = (1 << 17), // don't record these command in demofiles
	FCVAR_RELOAD_MATERIALS = (1 << 20), // If this cvar changes, it forces a material reload
	FCVAR_RELOAD_TEXTURES = (1 << 21), // If this cvar changes, if forces a texture reload

	FCVAR_NOT_CONNECTED = (1 << 22), // cvar cannot be changed by a client that is connected to a server
	FCVAR_MATERIAL_SYSTEM_THREAD = (1 << 23), // Indicates this cvar is read from the material system thread
	FCVAR_ARCHIVE_XBOX = (1 << 24), // cvar written to config.cfg on the Xbox

	FCVAR_ACCESSIBLE_FROM_THREADS = (1 << 25), // used as a debugging tool necessary to check material system thread convars

	FCVAR_SERVER_CAN_EXECUTE = (1 << 28), // the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
	FCVAR_SERVER_CANNOT_QUERY = (1 << 29), // If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
	FCVAR_CLIENTCMD_CAN_EXECUTE = (1 << 30), // IVEngineClient::ClientCmd is allowed to execute this command.

	FCVAR_MATERIAL_THREAD_MASK = (FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD)
};

#endif // !ICONVAR_H