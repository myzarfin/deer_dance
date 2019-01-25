#ifndef SETUP_H
#define SETUP_H
#ifdef _WIN32
#pragma once
#endif

namespace hooks {
	extern bool __fastcall	create_move(iclientmodeshared* ecx, PVOID edx, float input_sample_time, c_user_cmd* command);
	extern float __fastcall get_viewmodel_fov(iclientmodeshared* ecx, PVOID edx);
	extern int __fastcall	key_event(ibaseclientdll* ecx, PVOID edx, int event_code, int key_num, const char* current_binding);
	extern void __fastcall	paint_traverse(ipanel* ecx, PVOID edx, unsigned int vgui_panel, bool force_repaint, bool allow_force);
	extern void __fastcall	draw_model_exeute(imodel_render* ecx, PVOID edx, model_render_info &entity_info, matrix3x4_t* custom_bones);
	extern void __fastcall  frame_stage_notify(ibaseclientdll* ecx, PVOID edx, client_frame_stage_t stage);
	extern void __fastcall  override_view(void* ecx, void* edx, c_view_setup* setup);
	extern void __fastcall  scene_end(void* ecx, void* edx);
}

typedef void(*netvar_hook_callback)(const CRecvProxyData* data, void* structure, void* out);
namespace netvar_manager_hook {
	void yaw_angles_hook(const CRecvProxyData* data, void* structure, void* out);
	void initialize_netvar_hooks();
	void hook(const char* table_to_hook, const char* prop_to_hook, netvar_hook_callback callback);
}

class resolver : public singleton<resolver>, public igame_event_listener {
public:
	void initialize();
	void fire_game_event(igame_event* event) override;
};

namespace setup
{
	extern void setup_interfaces();
	extern void setup_hooks();
	extern bool ready_to_blast();
}

#define MAD_MILK_MODEL							"models/workshop/weapons/c_models/c_madmilk/c_madmilk.mdl"
#define FLYING_GUILLOTINE_MODEL					"models/workshop_partner/weapons/c_models/c_sd_cleaver/c_sd_cleaver.mdl"
#define SANDMAN_BASEBALL_MODEL					"models/weapons/w_models/w_baseball.mdl"
#define WRAP_ASSASSIN_BAUBLE_MODEL				"models/weapons/c_models/c_xms_festive_ornament.mdl"
#define ROCKET_GENERAL_MODEL					"models/weapons/w_models/w_rocket.mdl"
#define LASER_MODEL								"models/weapons/w_models/w_drg_ball.mdl"
#define ROCKET_AIRSTRIKE_MODEL					"models/weapons/w_models/w_rocket_airstrike/w_rocket_airstrike.mdl"
#define FLARES_GENERAL_MODEL					"models/weapons/w_models_w_flaregun_shell.mdl"
#define PILL_GENERAL_MODEL						"models/weapons/w_models/w_grenade_grenadelauncher.mdl"
#define LOOSE_CANNON_MODEL						"models/weapons/w_models/w_cannonball.mdl"
#define IRON_BOMBER_MODEL						"models/workshop/weapons/c_models/c_quadball/w_quadball_grenade.mdl"
#define STICKYBOMB_GENERAL_MODEL				"models/weapons/w_models/w_stickybomb.mdl"
#define STICKYBOMB_SCOTTISH_RESISTANCE_MODEL	"models/weapons/w_models/w_stickybomb_d.mdl"
#define STICKYBOMB_STICKY_JUMPER_MODEL			"models/weapons/w_models/w_stickybomb2.mdl"
#define SENTRYGUN_ROCKET_MODEL					"models/buildables/sentry3_rockets.mdl"
#define RESCUE_RANGER_MODEL						"models/weapons/w_models/w_repair_claw.mdl"
#define ARROW_GENERAL_MODEL						"models/weapons/w_models/w_arrow.mdl"
#define JARATE_MODEL							"models/weapons/c_models/urinejar.mdl"
#define JARATE_BREAD_MODEL						"models/weapons/c_models/c_breadmonster/c_breadmonster.mdl"

#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_RUNTIME_COMPOSITE				"Runtime Composite"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		    "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

#endif // !SETUP_H