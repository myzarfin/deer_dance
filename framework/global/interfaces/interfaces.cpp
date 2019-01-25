#include "../../main.h"

namespace interfaces {
	ibaseclientdll*			interfaces::client;
	icliententitylist*		interfaces::entitylist;
	iclientmodeshared*		interfaces::clientmode;
	imodel_info*			interfaces::model_info;
	imodel_render*			interfaces::model_render;
	imaterial_system*		interfaces::material_system;
	irender_view*			interfaces::render_view;

	ivengineclient*			interfaces::engineclient;
	ienginetrace*			interfaces::enginetrace;
	ivdebugoverlay*			interfaces::debugoverlay;
	ienginevgui*			interfaces::enginevgui;
	ivefx*					interfaces::efx;

	global_vars_base*		interfaces::globals;

	ipanel*					interfaces::panel;
	isurface*				interfaces::surface;

	iconvar*				interfaces::convar;
	igame_event_manager*	interfaces::game_event;
}