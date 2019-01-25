#ifndef MODEL_INFO_H
#define MODEL_INFO_H
#ifdef _WIN32
#pragma once
#endif

class imaterial_var;
class imaterial;
typedef unsigned short material_handle;

class morph_format_t;
class vertex_format_t;
class mat_property_types_t;

#define MAXSTUDIOSKINS32
enum mat_var_flags
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),
};

struct mstudiobbox_t {
	int					bone;
	int					group;
	vec3_t				bbmin;
	vec3_t				bbmax;
	int					szhitboxnameindex;
	int					unused[8];

	const char* hitbox_name()
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	} mstudiobbox_t() {}

private:
	mstudiobbox_t(const mstudiobbox_t& vOther);
};
struct mstudiohitboxset_t {
	int					sznameindex;
	inline char * const	pszName(void) const { return ((char *)this) + sznameindex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t *get_new_hitbox(int i) const { return (mstudiobbox_t *)(((byte *)this) + hitboxindex) + i; };
};
struct mstudiobone_t
{
	int					sznameindex;
	inline char * const pszName(void) const { return ((char *)this) + sznameindex; }
	int		 			parent;
	int					bonecontroller[6];

	vec3_t				pos;
	float				quat[4];
	vec3_t				rot;
	vec3_t				posscale;
	vec3_t				rotscale;

	float				poseToBone[3][4];
	float				qAlignment[4];
	int					flags;
	int					proctype;
	int					procindex;
	mutable int			physicsbone;
	inline void *pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int					surfacepropidx;
	inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
	int					contents;
	int					unused[8];

	mstudiobone_t() {}
private:
	mstudiobone_t(const mstudiobone_t& vOther);
};
struct studiohdr_t
{
	int			id;
	int			version;
	int			checksum;
	char		name[64];
	int			length;

	vec3_t		eyeposition;
	vec3_t		illumposition;
	vec3_t      hull_min;
	vec3_t      hull_max;
	vec3_t      view_bbmin;
	vec3_t      view_bbmax;

	int         flags;
	int         numbones;
	int         boneindex;

	inline mstudiobone_t *get_bone(int i) const {
		return (mstudiobone_t *)(((byte *)this) + boneindex) + i;
	};

	int         numbonecontrollers;
	int         bonecontrollerindex;
	int         numhitboxsets;
	int         hitboxsetindex;

	mstudiohitboxset_t* get_hitbox_set(int i) const {
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	}

	inline mstudiobbox_t* get_hitbox(int i, int set) const {
		mstudiohitboxset_t const* s = get_hitbox_set(set);
		if (!s) return NULL;
		return s->get_new_hitbox(i);
	}

	inline int get_hitbox_count(int set) const {
		mstudiohitboxset_t const* s = get_hitbox_set(set);
		if (!s) return 0;
		return s->numhitboxes;
	}

	int			numlocalanim;
	int         localanimindex;

	int         numlocalseq;
	int         localseqindex;

	mutable int activitylistversion;
	mutable int eventsindexed;

	int         numtextures;
	int         textureindex;

	int         numcdtextures;
	int         cdtextureindex;

	int         numskinref;
	int         numskinfamilies;
	int         skinindex;

	int         numbodyparts;
	int         bodypartindex;

	int         numlocalattachments;
	int         localattachmentindex;

	int         numlocalnodes;
	int         localnodeindex;
	int         localnodenameindex;

	int         numflexdesc;
	int         flexdescindex;

	int         numflexcontrollers;
	int         flexcontrollerindex;

	int         numflexrules;
	int         flexruleindex;

	int         numikchains;
	int         ikchainindex;

	int         nummouths;
	int         mouthindex;

	int         numlocalposeparameters;
	int         localposeparamindex;

	int         surfacepropindex;

	int         keyvalueindex;
	int         keyvaluesize;


	int         numlocalikautoplaylocks;
	int         localikautoplaylockindex;

	float       mass;
	int         contents;

	int         numincludemodels;
	int         includemodelindex;

	mutable void *virtualModel;

	int         szanimblocknameindex;
	int         numanimblocks;
	int         animblockindex;

	mutable void *animblockModel;

	int         bonetablebynameindex;

	void        *pVertexBase;
	void        *pIndexBase;

	byte        constdirectionallightdot;

	byte        rootLOD;

	byte        numAllowedRootLODs;

	byte        unused[1];

	int         unused4;

	int         numflexcontrollerui;
	int         flexcontrolleruiindex;
	float       flVertAnimFixedPointScale;
	int         unused3[1];
	int         studiohdr2index;
	int         unused2[1];
};

class imodel_info
{
public:
	DWORD * get_model(int index) {
		using original_fn = DWORD * (__thiscall*)(PVOID, int);
		return utilities::call_vfunc<original_fn>(this, 1)(this, index);
	}
	int	get_model_index(const char* name) {
		using original_fn = int(__thiscall*)(PVOID, const char*);
		return utilities::call_vfunc<original_fn>(this, 2)(this, name);
	}
	const char* get_model_name(const DWORD* model) {
		using original_fn = const char*(__thiscall*)(PVOID, const DWORD*);
		return utilities::call_vfunc<original_fn>(this, 3)(this, model);
	}
	studiohdr_t* get_studio_model(const DWORD* model) {
		using original_fn = studiohdr_t * (__thiscall*)(PVOID, const DWORD*);
		return utilities::call_vfunc<original_fn>(this, 28)(this, model);
	}
};

class imaterial
{
public:
	virtual const char* get_name() const = 0;
	virtual const char* get_texture_group_name() const = 0;
	virtual DWORD get_preview_image_properies(int* width, int* height, DWORD* imageFormat, bool* isTranslucent) const = 0;
	virtual DWORD get_preview_image(unsigned char* data, int width, int height, DWORD imageFormat) const = 0;
	virtual int get_mapping_width() = 0;
	virtual int get_mapping_height() = 0;
	virtual int get_num_animation_frames() = 0;
	virtual bool in_material_page(void) = 0;
	virtual void get_material_offset(float* pOffset) = 0;
	virtual void get_material_scale(float* pScale) = 0;
	virtual imaterial* get_material_page(void) = 0;
	virtual imaterial_var* find_var(const char* varName, bool* found, bool complain = true) = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
	virtual int get_enumeration_id(void) const = 0;
	virtual void get_low_res_colour_sample(float s, float t, float* color) const = 0;
	virtual void recompute_state_snapshot() = 0;
	virtual bool is_translucent() = 0;
	virtual bool is_alpha_tested() = 0;
	virtual bool is_vertex_lit() = 0;
	virtual uint64_t get_vertex_format() const = 0;
	virtual bool has_proxy(void) const = 0;
	virtual bool uses_envcube_map(void) = 0;
	virtual bool needs_tangent_space(void) = 0;
	virtual bool needs_power_of_two_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool needs_full_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool needs_software_skinning(void) = 0;
	virtual void alpha_modulate(float alpha) = 0;
	virtual void colour_modulate(float r, float g, float b) = 0;
	virtual void set_materialvar_flag(mat_var_flags flag, bool on) = 0;
	virtual bool get_materialvar_flag(mat_var_flags flag) = 0;
	virtual void get_reflectivity(vec3_t& reflect) = 0;
	virtual bool get_property_flag(mat_property_types_t type) = 0;
	virtual bool is_two_sided() = 0;
	virtual void set_shader(const char* pShaderName) = 0;
	virtual int get_num_passes(void) = 0;
	virtual int get_texture_memory_bytes(void) = 0;
	virtual void refresh() = 0;
	virtual bool needs_lightmap_blend_alpha(void) = 0;
	virtual bool needs_software_lighting(void) = 0;
	virtual int shader_param_count() const = 0;
	virtual imaterial_var** get_shader_params(void) = 0;
	virtual bool is_error_material() const = 0;
	virtual void unused() = 0;
	virtual float get_alpha_modulation() = 0;
	virtual void get_colour_modulation(float* r, float* g, float* b) = 0;
	virtual bool is_translucent_under_modulation(float fAlphaModulation = 1.0f) const = 0;
	virtual imaterial_var* find_var_fast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void set_shader_and_params(void* pKeyValues) = 0;
	virtual const char* get_shader_name() const = 0;
	virtual void delete_if_unreferenced() = 0;
	virtual bool is_sprite_card() = 0;
	virtual void call_bind_proxy(void* proxyData) = 0;
	virtual void refresh_preserving_materialvars() = 0;
	virtual bool was_reloaded_from_whitelist() = 0;
	virtual bool set_temp_excluded(bool bSet, int nExcludedDimensionLimit) = 0;
	virtual int get_reference_count() const = 0;
};

struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct model_t;
class CStudioHdr;

enum class override_type
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};

typedef int model_instance_handle_t;
struct model_render_info {
	vec3_t origin;
	vec3_t angles;
	PVOID*	renderable;
	const DWORD*		model;
	const matrix3x4_t*	model_to_world;
	const matrix3x4_t*	lighting_offset;
	const vec3_t*		lighting_origin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	model_instance_handle_t instance;
	model_render_info() {
		model_to_world  = NULL;
		lighting_offset = NULL;
		lighting_origin = NULL;
	}
};

struct static_prop_render_info
{
	const matrix3x4_t*		model_to_world;
	const DWORD*			model;
	client_renderable*		renderable;
	vec3_t*					lighting_origin;
	short					skin;
	unsigned short			instance;
};

struct studiohwdata_t;
struct studio_decal_handle_t;

struct draw_model_state
{
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pStudioHWData;
	client_renderable*		m_pRenderable;
	const matrix3x4_t		*m_pModelToWorld;
	studio_decal_handle_t*	m_decals;
	int						m_drawFlags;
	int						m_lod;
};

class imodel_render
{
public:
	void force_material_override(imaterial *mat, override_type type = override_type::OVERRIDE_NORMAL) {
		using original_fn = void(__thiscall*)(PVOID, imaterial *, override_type);
		return utilities::call_vfunc<original_fn>(this, 1)(this, mat, type);
	}

	void draw_model_exeute(PVOID state, model_render_info &entity_info, matrix3x4_t* custom_bones) {
		using original_fn = void(__thiscall*)(PVOID, PVOID, model_render_info&, matrix3x4_t*);
		return utilities::call_vfunc<original_fn>(this, 19)(this, state, entity_info, custom_bones);
	}
};

class irender_view
{
public:
	void set_blend(float fl) {
		using original_fn = void(__thiscall*)(PVOID, float);
		return utilities::call_vfunc<original_fn>(this, 4)(this, fl);
	}
	void set_colour_modulation(const float *col) {
		using original_fn = void(__thiscall*)(PVOID, const float *);
		return utilities::call_vfunc<original_fn>(this, 6)(this, col);
	}
};

class key_value;
class imaterial_system
{
public:
	imaterial* find_material(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef imaterial *(__thiscall *OriginalFn)(PVOID, const char *, const char *, bool, const char *);
		return utilities::call_vfunc<OriginalFn>(this, 73)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	imaterial* create_material(char const* pMaterialName, key_value* kv)
	{
		typedef imaterial *(__thiscall *OriginalFn)(PVOID, const char*, key_value*);
		return utilities::call_vfunc<OriginalFn>(this, 72)(this, pMaterialName, kv);
	}

	imaterial* get_material(material_handle h)
	{
		typedef imaterial *(__thiscall *OriginalFn)(PVOID, material_handle);
		return utilities::call_vfunc<OriginalFn>(this, 78)(this, h);
	}

	material_handle first_material()
	{
		typedef material_handle(__thiscall *OriginalFn)(PVOID);
		return utilities::call_vfunc<OriginalFn>(this, 75)(this);
	}

	material_handle invalid_material()
	{
		typedef material_handle(__thiscall *OriginalFn)(PVOID);
		return utilities::call_vfunc<OriginalFn>(this, 77)(this);
	}

	material_handle next_material (material_handle h)
	{
		typedef material_handle(__thiscall *OriginalFn)(PVOID, material_handle);
		return utilities::call_vfunc<OriginalFn>(this, 76)(this, h);
	}
};

#endif // !MODEL_INFO_H