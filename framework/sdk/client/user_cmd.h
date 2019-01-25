#ifndef USER_CMD_H
#define USER_CMD_H
#ifdef _WIN32
#pragma once
#endif

// button flags
#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define	IN_ATTACK3		(1 << 25)

// player specific flags
#define	FL_ONGROUND		(1 << 0)
#define FL_DUCKING		(1 << 1)
#define	FL_WATERJUMP	(1 << 2)
#define FL_ONTRAIN		(1 << 3)
#define FL_INRAIN		(1 << 4)
#define FL_FROZEN		(1 << 5)
#define FL_ATCONTROLS	(1 << 6)
#define	FL_CLIENT		(1 << 7)
#define FL_FAKECLIENT	(1 << 8)
#define	FL_INWATER		(1 << 9)

class c_user_cmd
{
public:
	virtual ~c_user_cmd() {};
	int		command_number;
	int		tick_count;
	vec3_t	viewangles;
	float	forward_move;
	float	side_move;
	float	up_move;
	int		buttons;
	byte    impulse;
	int		weapon_select;
	int		weapon_sub_type;
	int		random_seed;
	short	moused_x;
	short	moused_y;
	byte	has_been_predicted;
};

class c_view_setup
{
public:
	uint32_t x; //0x0000 
	uint32_t m_nUnscaledX; //0x0004 
	uint32_t y; //0x0008 
	uint32_t m_nUnscaledY; //0x000C 
	uint32_t width; //0x0010 
	uint32_t m_nUnscaledWidth; //0x0014 
	uint32_t height; //0x0018 
	uint32_t m_eStereoEye; //0x001C 
	uint32_t m_nUnscaledHeight; //0x0020 
	bool m_bOrtho; //0x0024 
	float m_OrthoLeft; //0x0028 
	float m_OrthoTop; //0x002C 
	float m_OrthoRight; //0x0030 
	float m_OrthoBottom; //0x0034 
	float m_fov; //0x0038 
	float m_viewmodel_fov; //0x003C 
	vec3_t origin; //0x0040 
	vec3_t angles; //0x004C 
	float zNear; //0x0058 
	float zFar; //0x005C 
	float viewmodel_zNear; //0x0060 
	float viewmodel_zFar; //0x0064 
	bool m_bRenderToSubrectOfLargerScreen; //0x0068 
	float m_flAspectRatio; //0x006C 
	bool m_bOffCenter; //0x0070 
	float m_flOffCenterTop; //0x0074 
	float m_flOffCenterBottom; //0x0078 
	float m_flOffCenterLeft; //0x007C 
	float m_flOffCenterRight; //0x0080 
	bool m_bDoBloomAndToneMapping; //0x0084 
	bool m_bCacheFullSceneState; //0x0088 
	bool m_bViewToProjectionOverride; //0x008C 
	matrix3x4_t m_ViewToProjection; //0x0090 
};

#endif // !USER_CMD_H