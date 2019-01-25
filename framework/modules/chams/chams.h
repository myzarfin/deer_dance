#pragma once

class ifile_system;
class key_value
{
public:
	int m_iKeyName;
	char *m_sValue;
	wchar_t *m_wsValue;

	union
	{
		int m_iValue;
		float m_flValue;
		void *m_pValue;
		unsigned char m_Color[4];
	};

	char	m_iDataType;
	char	m_bHasEscapeSequences;
	char	m_bEvaluateConditionals;
	char	unused[1];

	key_value *m_pPeer;
	key_value *m_pSub;
	key_value *m_pChain;
};

namespace key_values {
	bool load_from_buffer(key_value* key_valuez, char const* resource_name, const char* buffer, ifile_system* file_system = NULL, const char* path_id = NULL);
	key_value* initialize(key_value*, char* name);
}

namespace chams {
	imaterial* create_material(bool ignorez, bool flat, bool wireframe, bool add_shine);
	void create_our_mats();
	void force_material(colour_t colour, imaterial* material);
}