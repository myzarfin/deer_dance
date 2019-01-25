#ifndef INTERFACES_H
#define INTERFACES_H
#ifdef _WIN32
#pragma once
#endif

namespace interfaces {
	extern ibaseclientdll*		client;
	extern icliententitylist*	entitylist;
	extern iclientmodeshared*	clientmode;
	extern imodel_info*			model_info;
	extern imodel_render*		model_render;
	extern imaterial_system*	material_system;
	extern irender_view*		render_view;

	extern ivengineclient*		engineclient;
	extern ienginetrace*		enginetrace;
	extern ivdebugoverlay*		debugoverlay;
	extern ienginevgui*			enginevgui;
	extern ivefx*				efx;

	extern global_vars_base*	globals;

	extern ipanel*				panel;
	extern isurface*			surface;

	extern iconvar*				convar;
	extern igame_event_manager* game_event;
}

#endif // !INTERFACES_H