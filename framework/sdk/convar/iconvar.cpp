#include "../../main.h"

con_command_base*		con_command_base::con_command_bases = NULL;
static int				s_nCVarFlag = 0;
static int				s_nDLLIdentifier = -1;
static bool				s_bRegistered = false;

class c_default_accessor : public icon_command_base_accessor {
public:
	virtual bool register_con_command_base(con_command_base* variable) {
		interfaces::convar->register_con_command(variable);
		return true;
	}
};

static c_default_accessor default_accessor;
icon_command_base_accessor* con_command_base::accessor = &default_accessor;

void convar_register(int nCVarFlag, icon_command_base_accessor *pAccessor) {
	if (s_bRegistered) {
		return;
	}

	s_bRegistered = true;
	s_nCVarFlag = nCVarFlag;
	s_nDLLIdentifier = interfaces::convar->allocate_dll_identifier();

	con_command_base* current, *next;

	con_command_base::accessor = pAccessor ? pAccessor : &default_accessor;
	current = con_command_base::con_command_bases;
	while (current) {
		next = current->m_next;
		current->add_flags(s_nCVarFlag);
		current->initialize();
		current = next;
	}

	con_command_base::con_command_bases = NULL;
}

convar::convar(const char *pName, const char *pDefaultValue, int flags) {
	create(pName, pDefaultValue, flags);
}

convar::convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString) {
	create(pName, pDefaultValue, flags, pHelpString);
}

convar::convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax) {
	create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax);
}

convar::convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, change_callback_fn callback) {
	create(pName, pDefaultValue, flags, pHelpString, false, 0.0, false, 0.0, callback);
}

convar::convar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, change_callback_fn callback) {
	create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback);
}

convar::~convar(void) {
	if (m_Value.m_pszString) {
		delete[] m_Value.m_pszString;
		m_Value.m_pszString = NULL;
	}
}

bool convar::is_flag_set(int flag) const {
	return (flag & m_pParent->m_flags) ? true : false;
}

const char *convar::get_help_text(void) const {
	return m_pParent->m_help_string;
}

void convar::add_flags(int flags) {
	m_pParent->m_flags |= flags;
}

int convar::get_flags(void) const {
	return m_pParent->m_flags;
}

bool convar::is_registered(void) const
{
	return m_pParent->m_registered;
}
bool convar::is_command(void) const {
	return false;
}

const char *convar::get_name(void) const {
	return m_pParent->m_name;
}
const char* convar::get_base_name(void) const {
	return m_pParent->m_name;
}
float convar::get_float(void) const {
	return m_pParent->m_Value.m_fValue;
}
int convar::get_int(void) const {
	return m_pParent->m_Value.m_nValue;
}
const char* convar::get_string(void) const {
	return m_pParent->m_Value.m_pszString;
}
int convar::get_split_screen_player_slot(void) const {
	return 0;
}
DWORD convar::get_colour(void) const {
	unsigned char *pColorElement = ((unsigned char *)&m_pParent->m_Value.m_nValue);
	return COLORCODE(pColorElement[0], pColorElement[1], pColorElement[2], pColorElement[3]);
}
void convar::internal_set_value(const char *value) {
	float fNewValue;
	char  tempVal[32];
	char  *val;

	float flOldValue = m_Value.m_fValue;

	val = (char *)value;
	fNewValue = (float)atof(value);

	if (clamp_value(fNewValue)) {
		val = tempVal;
	}

	// Redetermine value
	m_Value.m_fValue = fNewValue;
	m_Value.m_nValue = (int)(fNewValue);

	if (!(m_flags & (int)ConvarFlags::FCVAR_NEVER_AS_STRING)) {
		change_string_value(val, flOldValue);
	}
}
void convar::change_string_value(const char *tempVal, float flOldValue) {
	UNREFERENCED_PARAMETER(flOldValue);
	int len = strlen(tempVal) + 1;

	if (len > m_Value.m_StringLength) {
		if (m_Value.m_pszString) {
			delete[] m_Value.m_pszString;
		}

		m_Value.m_pszString = new char[len];
		m_Value.m_StringLength = len;
	} 
	memcpy(m_Value.m_pszString, tempVal, len); 
}
bool convar::clamp_value(float& value) {
	if (m_bHasMin && (value < m_fMinVal)) {
		value = m_fMinVal;
		return true;
	}

	if (m_bHasMax && (value > m_fMaxVal)) {
		value = m_fMaxVal;
		return true;
	} 

	return false;
}
void convar::internal_set_float_value(float fNewValue) {
	if (fNewValue == m_Value.m_fValue) {
		return;
	}

	clamp_value(fNewValue);

	float flOldValue = m_Value.m_fValue;
	m_Value.m_fValue = fNewValue;
	m_Value.m_nValue = (int)fNewValue;

	if (!(m_flags & (int)ConvarFlags::FCVAR_NEVER_AS_STRING)) {
		char tempVal[32];
		change_string_value(tempVal, flOldValue);
	}
}
void convar::internal_set_int_value(int nValue) {
	if (nValue == m_Value.m_nValue) {
		return;
	}

	float fValue = (float)nValue;
	if (clamp_value(fValue)) {
		nValue = (int)(fValue);
	}

	float flOldValue = m_Value.m_fValue;
	m_Value.m_fValue = fValue;
	m_Value.m_nValue = nValue;

	if (!(m_flags & (int)ConvarFlags::FCVAR_NEVER_AS_STRING))
	{
		char tempVal[32];
		change_string_value(tempVal, flOldValue);
	}
}
void convar::internal_set_colour_value(DWORD cValue) {
	int color = (int)cValue;
	internal_set_int_value(color);
}
void convar::create(const char *pName, const char *pDefaultValue,
	int flags, const char *pHelpString, bool bMin, float fMin,
	bool bMax, float fMax, change_callback_fn callback)
{
	static const char* empty_string = "";

	m_pParent = this;

	// Name should be static data
	m_pszDefaultValue = pDefaultValue ? pDefaultValue : empty_string;

	m_Value.m_StringLength = strlen(m_pszDefaultValue) + 1;
	m_Value.m_pszString = new char[m_Value.m_StringLength];
	memcpy(m_Value.m_pszString, m_pszDefaultValue, m_Value.m_StringLength);

	m_bHasMin = bMin;
	m_fMinVal = fMin;
	m_bHasMax = bMax;
	m_fMaxVal = fMax;

	m_fnChangeCallback = callback;
	m_Value.m_fValue = (float)atof(m_Value.m_pszString);
	m_Value.m_nValue = (int)m_Value.m_fValue;

	base_class::create(pName, pHelpString, flags);
}
void convar::set_value(const char *value)
{
	m_pParent->internal_set_value(value);
}
void convar::set_value(float value)
{
	m_pParent->internal_set_float_value(value);
}
void convar::set_value(int value)
{
	m_pParent->internal_set_int_value(value);
}
void convar::set_value(DWORD value)
{
	m_pParent->internal_set_colour_value(value);
}
const char* convar::get_default(void) const
{
	return m_pParent->m_pszDefaultValue;
}

//-------------------------------------------------------------
// ConCommandBase methods
//-------------------------------------------------------------
con_command_base::con_command_base(void)
{
	m_registered = false;
	m_name = NULL;
	m_help_string = NULL;

	m_flags = 0;
	m_next = NULL;
}
con_command_base::con_command_base(const char *pName, const char *pHelpString /*=0*/, int flags /*= 0*/)
{
	create(pName, pHelpString, flags);
}
con_command_base::~con_command_base(void)
{
}

bool con_command_base::is_command(void) const
{
	return true;
}
int con_command_base::get_dll_identifier() const
{
	return s_nDLLIdentifier;
}
void con_command_base::create(const char *pName, const char *pHelpString /*= 0*/, int flags /*= 0*/)
{
	static const char* empty_string = "";

	m_registered = false;

	m_name = pName;
	m_help_string = pHelpString ? pHelpString : empty_string;

	m_flags = flags;

	if (!(m_flags & (int)ConvarFlags::FCVAR_UNREGISTERED))
	{
		m_next = con_command_bases;
		con_command_bases = this;
	}
	else
	{
		m_next = NULL;
	}

	initialize();
}
void con_command_base::initialize()
{
	if (accessor)
	{
		accessor->register_con_command_base(this);
	}
}
const char* con_command_base::get_name(void) const
{
	return m_name;
}

void convar::install_change_callback(change_callback_fn callback)
{
	assert(!m_pParent->m_fnChangeCallback || !callback);
	m_pParent->m_fnChangeCallback = callback;

	if (m_pParent->m_fnChangeCallback) {
		m_pParent->m_fnChangeCallback(this, m_Value.m_pszString, m_Value.m_fValue);
	}
}

bool con_command_base::is_flag_set(int flag) const
{
	return (flag & m_flags) ? true : false;
}
void con_command_base::add_flags(int flags)
{
	m_flags |= flags;
}
void con_command_base::remove_flags(int flags)
{
	m_flags &= ~flags;
}
int con_command_base::get_flags(void) const
{
	return m_flags;
}
const char* con_command_base::get_help_text(void) const
{
	return m_help_string;
}
bool con_command_base::is_registered(void) const
{
	return m_registered;
}
