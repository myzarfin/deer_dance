#ifndef IBASECLIENTDLL_H
#define IBASECLIENTDLL_H
#ifdef _WIN32
#pragma once
#endif

enum client_frame_stage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class ibaseclientdll {
public:
	client_class* get_all_classes(void)
	{
		typedef client_class*(__thiscall* OriginalFn)(PVOID);
		return utilities::call_vfunc<OriginalFn>(this, 8)(this);
	}
};

#endif // !IBASECLIENTDLL_H