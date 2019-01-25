#ifndef MAIN_H
#define MAIN_H
#ifdef _WIN32
#pragma once
#endif

#include <algorithm>
#include <Windows.h>
#include <assert.h>
#include <Psapi.h>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <playsoundapi.h>

#include "helpers/util.h"
#include "helpers/vector2d.h"
#include "helpers/vector.h"
#include "helpers/color.h"
#include "helpers/vmt.h"

#include "sdk/sdk.h"

#include "global/interfaces/interfaces.h"
#include "global/hooks/hooks.h"
#include "global/fonts/fonts.h"
#include "global/draw/draw.h"
#include "global/global.h"

#include "menu/vars/vars.h"
#include "menu/menu.h"

#include "setup/setup.h"

#include "modules/position_adjusment/position_adjustment.h"
#include "modules/miscellanous/miscellanous.h"
#include "modules/triggerbot/triggerbot.h"
#include "modules/announcer/announcer.h"
#include "modules/anti_aim/anti_aim.h"
#include "modules/visuals/visuals.h"
#include "modules/aimbot/aimbot.h"
#include "modules/chams/chams.h"

#include "sounds/sounds.h"

#pragma comment(lib, "Winmm.lib")

#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))
#define vector_ma(v, s, b, o)    ((o)[0] = (v)[0] + (b)[0] * (s), (o)[1] = (v)[1] + (b)[1] * (s), (o)[2] = (v)[2] + (b)[2] * (s))
#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / interfaces::globals->interval_per_tick))
#define TICKS_TO_TIME(t) (interfaces::globals->interval_per_tick * (t))
#define PI 3.14159265358979323846f
#define RADPI 57.295779513082f
#define assert( _exp ) ((void)0)

namespace utilities {
	void vector_transform(const vec3_t& some, const matrix3x4_t& matrix, vec3_t& out);
	float distance_point_to_line(vec3_t point, vec3_t origin, vec3_t direction);
	float get_field_of_view(const vec3_t& angle, const vec3_t& aim_angle);
	float get_field_of_view(vec3_t angle, vec3_t src, vec3_t dst);
	vec3_t angle_vector(vec3_t meme);
	vec3_t get_hitbox_position(c_base_player* entity, int hitbox);
	bool has_condition(c_base_player* entity, int condition);
	void angle_vectors(const vec3_t &angles, vec3_t* forward);

	void make_vector(vec3_t angle, vec3_t& vector);
	vec3_t calc_angle(vec3_t src, vec3_t dst);

	void vector_angle(vec3_t &forward, vec3_t &angles);

	bool is_valid_player(c_base_player* entity);
	bool is_projectile(c_base_player* entity, c_base_weapon* weapon);

	bool is_entity_visible(c_base_player* local_player, c_base_player* entity, vec3_t vec_start, vec3_t vec_end);
	bool is_point_visible(c_base_player* local_player, vec3_t vec_start, vec3_t vec_end);

	inline bool is_developer(c_base_player* entity) {
		player_info_t entity_info;
		if (!interfaces::engineclient->get_player_info(entity->get_index(), &entity_info) || !entity) {
			return false;
		}

		if (entity_info.friends_id == 120731054) {
			return true;
		}

		return false;
	}

	inline bool is_variable_key_pressed(int var) {
		switch (var) {
			case 0:		return true;
			case 1:		return GetAsyncKeyState(VK_LBUTTON);
			case 2:		return GetAsyncKeyState(VK_RBUTTON);
			case 3:		return GetAsyncKeyState(VK_MBUTTON);
			case 4:		return GetAsyncKeyState(VK_XBUTTON1);
			case 5:		return GetAsyncKeyState(VK_XBUTTON2);
			case 6:		return GetAsyncKeyState(VK_SHIFT);
			case 7:		return GetAsyncKeyState(VK_MENU);
		}
		return false;
	}
}

template<class T, class U>
T clamp(T in, U low, U high) {
	if (in <= low) return low;
	if (in >= high) return high;
	return in;
}

enum scoutweapons
{
	//Primary
	WPN_Scattergun = 13,
	WPN_NewScattergun = 200,
	WPN_FAN = 45,
	WPN_Shortstop = 220,
	WPN_SodaPopper = 448,
	WPN_BabyFaceBlaster = 772,
	WPN_BotScattergunS = 799,
	WPN_BotScattergunG = 808,
	WPN_BotScattergunR = 888,
	WPN_BotScattergunB = 897,
	WPN_BotScattergunC = 906,
	WPN_BotScattergunD = 915,
	WPN_BotScattergunES = 964,
	WPN_BotScattergunEG = 973,
	WPN_FestiveFaN = 1078,
	WPN_BackScatter = 1103,
	//Secondary
	WPN_ScoutPistol = 23,
	WPN_NewPistol = 209,
	WPN_Bonk = 46,
	WPN_CritCola = 163,
	WPN_Lugermorph = 160,
	WPN_Milk = 222,
	WPN_Lugermorph2 = 294,
	WPN_Winger = 449,
	WPN_PocketPistol = 773,
	WPN_MutatedMilk = 1121,
	WPN_CAPPER = 30666,
	//Melee
	WPN_Bat = 0,
	WPN_NewBat = 190,
	WPN_Sandman = 44,
	WPN_Fish = 221,
	WPN_Cane = 317,
	WPN_BostonBasher = 325,
	WPN_SunStick = 349,
	WPN_FanOWar = 355,
	WPN_RuneBlade = 452,
	WPN_Saxxy = 423,
	WPN_Atomizer = 450,
	WPN_ConscientiousObjector = 474,
	WPN_UnarmedCombat = 572,
	WPN_WrapAssassin = 648,
	WPN_FestiveBat = 660,
	WPN_FestiveScattergun = 669,
	WPN_FlyingGuillotine1 = 812,
	WPN_FlyingGuillotine2 = 833,
	WPN_FreedomStaff = 880,
	WPN_BatOuttaHell = 939,
	WPN_MemoryMaker = 954,
	WPN_FestiveFish = 999,
	WPN_TheHamShank = 1013,
	WPN_CrossingGuard = 1127,
	WPN_NecroSmasher = 1123,
	WPN_Batsaber = 30667,
};
enum soldierweapons
{
	//Primary
	WPN_RocketLauncher = 18,
	WPN_NewRocketLauncher = 205,
	WPN_DirectHit = 127,
	WPN_BlackBox = 228,
	WPN_RocketJumper = 237,
	WPN_LibertyLauncher = 414,
	WPN_CowMangler = 441,
	WPN_Original = 513,
	WPN_FestiveRocketLauncher = 658,
	WPN_BeggersBazooka = 730,
	WPN_BotRocketlauncherS = 800,
	WPN_BotRocketlauncherG = 809,
	WPN_BotRocketlauncherR = 889,
	WPN_BotRocketlauncherB = 898,
	WPN_BotRocketlauncherC = 907,
	WPN_BotRocketlauncherD = 916,
	WPN_BotRocketlauncherES = 965,
	WPN_BotRocketlauncherEG = 974,
	WPN_FestiveBlackbox = 1085,
	WPN_Airstrike = 1104,
	//Secondary
	WPN_SoldierShotgun = 10,
	WPN_NewShotgun = 199,
	WPN_BuffBanner = 129,
	WPN_BattalionBackup = 226,
	WPN_Concheror = 354,
	WPN_ReserveShooter = 415,
	WPN_RighteousBison = 442,
	WPN_FestiveBuffBanner = 1001,
	WPN_PanicAttack = 1153,
	//Melee
	WPN_Shovel = 6,
	WPN_NewShovel = 196,
	WPN_Equalizer = 128,
	WPN_PainTrain = 154,
	WPN_Katana = 357,
	WPN_MarketGardener = 416,
	WPN_DisciplinaryAction = 447,
	WPN_EscapePlan = 775,
};
enum pyroweapons
{
	//Primary
	WPN_Flamethrower = 21,
	WPN_NewFlamethrower = 208,
	WPN_Backburner = 40,
	WPN_Degreaser = 215,
	WPN_FestiveFlamethrower = 659,
	WPN_Phlogistinator = 594,
	WPN_Rainblower = 741,
	WPN_BotFlamethrowerS = 798,
	WPN_BotFlamethrowerG = 807,
	WPN_BotFlamethrowerR = 887,
	WPN_BotFlamethrowerB = 896,
	WPN_BotFlamethrowerC = 905,
	WPN_BotFlamethrowerD = 914,
	WPN_BotFlamethrowerES = 963,
	WPN_BotFlamethrowerEG = 972,
	WPN_FestiveBackburner = 1146,
	//Secondary
	WPN_PyroShotgun = 12,
	WPN_Flaregun = 39,
	WPN_Detonator = 351,
	WPN_ManMelter = 595,
	WPN_ScorchShot = 740,
	WPN_FestiveFlaregun = 1081,
	//Melee
	WPN_Fireaxe = 2,
	WPN_NewAxe = 192,
	WPN_Axtingusher = 38,
	WPN_HomeWrecker = 153,
	WPN_PowerJack = 214,
	WPN_Backscratcher = 326,
	WPN_VolcanoFragment = 348,
	WPN_Maul = 466,
	WPN_Mailbox = 457,
	WPN_ThirdDegree = 593,
	WPN_Lollychop = 739,
	WPN_NeonAnnihilator1 = 813,
	WPN_NeonAnnihilator2 = 834,
	WPN_FestiveAxtingisher = 1000,
};
enum demomanweapons
{
	//Primary
	WPN_GrenadeLauncher = 19,
	WPN_NewGrenadeLauncher = 206,
	WPN_LochNLoad = 308,
	WPN_LoooseCannon = 996,
	WPN_FestiveGrenadeLauncher = 1007,
	WPN_IronBomber = 1151,
	//Secondary
	WPN_StickyLauncher = 20,
	WPN_NewStickyLauncher = 207,
	WPN_ScottishResistance = 130,
	WPN_StickyJumper = 265,
	WPN_FestiveStickyLauncher = 661,
	WPN_BotStickyS = 797,
	WPN_BotStickyG = 806,
	WPN_BotStickyR = 886,
	WPN_BotStickyB = 895,
	WPN_BotStickyC = 904,
	WPN_BotStickyD = 913,
	WPN_BotStickyES = 962,
	WPN_BotStickyEG = 971,
	WPN_QuickieBombLauncher = 1150,
	//Melee
	WPN_Bottle = 1,
	WPN_NewBottle = 191,
	WPN_Sword = 132,
	WPN_ScottsSkullctter = 172,
	WPN_Fryingpan = 264,
	WPN_Headless = 266,
	WPN_Ullapool = 307,
	WPN_Claidheamhmor = 327,
	WPN_PersainPersuader = 404,
	WPN_Golfclub = 482,
	WPN_ScottishHandshake = 609,
	WPN_GoldenFryingPan = 1071,
	WPN_FestiveEyelander = 1082,
};
enum heavyweapons
{
	//Primary
	WPN_Minigun = 15,
	WPN_NewMinigun = 202,
	WPN_Natascha = 41,
	WPN_IronCurtain = 298,
	WPN_BrassBeast = 312,
	WPN_Tomislav = 424,
	WPN_FestiveMinigun = 654,
	WPN_HuoLongHeatmaker1 = 811,
	WPN_HuoLongHeatmaker2 = 832,
	WPN_BotMinigunS = 793,
	WPN_BotMinigunG = 802,
	WPN_BotMinigunR = 882,
	WPN_BotMinigunB = 891,
	WPN_BotMinigunC = 900,
	WPN_BotMinigunD = 909,
	WPN_Deflector = 850,
	WPN_BotMinigunES = 958,
	WPN_BotMinigunEG = 967,
	//Secondary
	WPN_HeavyShotgun = 11,
	WPN_Sandvich = 42,
	WPN_CandyBar = 159,
	WPN_Steak = 311,
	WPN_Fishcake = 433,
	WPN_FamilyBuisness = 425, //GIVE 'EM THE BUISSNESS, TONY!
	WPN_RobotSandvich = 863,
	WPN_FestiveSandvich = 1002,
	//Melee
	WPN_Fists = 5,
	WPN_NewFists = 195,
	WPN_KGB = 43,
	WPN_GRU = 239,
	WPN_WarriorSpirit = 310,
	WPN_FistsOfSteel = 331,
	WPN_EvictionNotice = 426,
	WPN_ApocoFists = 587,
	WPN_HolidayPunch = 656,
	WPN_FestiveGRU = 1084,
	WPN_BreadBite = 1100,
};
enum engineerweapons
{
	//Primary
	WPN_EngineerShotgun = 9,
	WPN_FrontierJustice = 141,
	WPN_Widowmaker = 527,
	WPN_Pomson = 588,
	WPN_RescueRanger = 997,
	WPN_FestiveFrontierJustice = 1004,
	//Secondary
	WPN_EngineerPistol = 22,
	WPN_Wrangler = 140,
	WPN_ShortCircut = 528,
	WPN_FestiveWrangler = 1086,
	WPN_GeigerCounter = 30668,
	//Melee
	WPN_Wrench = 7,
	WPN_NewWrench = 197,
	WPN_Goldenwrench = 169,
	WPN_SouthernHospitality = 155,
	WPN_Gunslinger = 142,
	WPN_Jag = 329,
	WPN_FestiveWrench = 662,
	WPN_EurekaEffect = 589,
	WPN_BotWrenchS = 795,
	WPN_BotWrenchG = 804,
	WPN_BotWrenchR = 884,
	WPN_BotWrenchB = 893,
	WPN_BotWrenchC = 902,
	WPN_BotWrenchD = 911,
	WPN_BotWrenchES = 960,
	WPN_BotWrenchEG = 969,
	//Misc
	WPN_Builder = 25,
	WPN_Builder2 = 737,
	WPN_Destructor = 26,
	WPN_Toolbox = 28,
};
enum medicweapons
{
	//Primary
	WPN_SyringeGun = 17,
	WPN_NewSyringeGun = 204,
	WPN_Blutsauger = 36,
	WPN_Crossbow = 305,
	WPN_Overdose = 412,
	WPN_FestiveCrossbow = 1079,
	//Secondary
	WPN_Medigun = 29,
	WPN_NewMedigun = 211,
	WPN_Kritzkrieg = 35,
	WPN_QuickFix = 411,
	WPN_FestiveMedigun = 663,
	WPN_MedigunS = 796,
	WPN_MedigunG = 805,
	WPN_MedigunR = 885,
	WPN_MedigunB = 894,
	WPN_MedigunC = 903,
	WPN_MedigunD = 912,
	WPN_Vaccinator = 998,
	WPN_MedigunES = 961,
	WPN_MedigunEG = 970,
	//Melee
	WPN_Bonesaw = 8,
	WPN_NewBonesaw = 198,
	WPN_Ubersaw = 37,
	WPN_Vitasaw = 173,
	WPN_Amputator = 304,
	WPN_Solemnvow = 413,
	WPN_FestiveUbersaw = 1003,
};
enum sniperweapons
{
	//Primary
	WPN_SniperRifle = 14,
	WPN_NewSniperRifle = 201,
	WPN_Huntsman = 56,
	WPN_SydneySleeper = 230,
	WPN_Bazaarbargain = 402,
	WPN_Machina = 526,
	WPN_FestiveSniperRifle = 664,
	WPN_HitmanHeatmaker = 752,
	WPN_BotSniperRifleS = 792,
	WPN_BotSniperRifleG = 801,
	WPN_BotSniperRifleR = 881,
	WPN_BotSniperRifleB = 890,
	WPN_BotSniperRifleC = 899,
	WPN_BotSniperRifleD = 908,
	WPN_AWP = 851,
	WPN_BotSniperRifleES = 957,
	WPN_BotSniperRifleEG = 966,
	WPN_FestiveHuntsman = 1005,
	WPN_CompoundBow = 1092,
	WPN_ClassicSniperRifle = 1098,
	WPN_ShootingStar = 30665,
	//Secondary
	WPN_SMG = 16,
	WPN_NewSMG = 203,
	WPN_Jarate = 58,
	WPN_DarwinDangerShield = 231,
	WPN_CleanersCarbine = 751,
	WPN_FestiveJarate = 1083,
	WPN_SelfAwareBeautyMark = 1105,
	//Melee
	WPN_Kukri = 3,
	WPN_NewKukri = 193,
	WPN_TribalmansShiv = 171,
	WPN_Bushwacka = 232,
	WPN_Shahanshah = 401,
};
enum spyweapons
{
	//Primary
	WPN_Revolver = 24,
	WPN_NewRevolver = 210,
	WPN_Ambassador = 61,
	WPN_BigKill = 161,
	WPN_Letranger = 224,
	WPN_Enforcer = 460,
	WPN_Diamondback = 525,
	WPN_FestiveAmbassador = 1006,
	//Melee
	WPN_Knife = 4,
	WPN_NewKnife = 194,
	WPN_EternalReward = 225,
	WPN_DisguiseKit = 27,
	WPN_Kunai = 356,
	WPN_BigEarner = 461,
	WPN_WangaPrick = 574,
	WPN_SharpDresser = 638,
	WPN_Spycicle = 649,
	WPN_FestiveKnife = 665,
	WPN_BlackRose = 727,
	WPN_BotKnifeS = 794,
	WPN_BotKnifeG = 803,
	WPN_BotKnifeR = 883,
	WPN_BotKnifeB = 892,
	WPN_BotKnifeC = 901,
	WPN_BotKnifeD = 910,
	WPN_BotKnifeES = 959,
	WPN_BotKnifeEG = 968,
	//Watches
	WPN_InivisWatch = 30,
	WPN_DeadRinger = 59,
	WPN_CloakAndDagger = 60,
	WPN_TimePiece = 297,
	//Sapper
	WPN_Sapper = 735,
	WPN_Sapper2 = 736,
	WPN_RedTape1 = 810,
	WPN_RedTape2 = 831,
	WPN_ApSap = 933,
	WPN_FestiveSapper = 1080,
	WPN_SnackAttack = 1102,
};

//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//

#ifndef BSPFLAGS_H
#define BSPFLAGS_H

#ifdef _WIN32
#pragma once
#endif

// contents flags are seperate bits
// a given brush can contribute multiple content bits
// multiple brushes can be in a single leaf

// these definitions also need to be in q_shared.h!

// lower bits are stronger, and will eat weaker brushes completely
#define CONTENTS_EMPTY 0 // No contents

#define CONTENTS_SOLID 0x1  // an eye is never valid in a solid
#define CONTENTS_WINDOW 0x2 // translucent, but not watery (glass)
#define CONTENTS_AUX 0x4
#define CONTENTS_GRATE 0x8 // alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define CONTENTS_SLIME 0x10
#define CONTENTS_WATER 0x20
#define CONTENTS_BLOCKLOS 0x40 // block AI line of sight
#define CONTENTS_OPAQUE 0x80   // things that cannot be seen through (may be non-solid though)
#define LAST_VISIBLE_CONTENTS 0x80

#define ALL_VISIBLE_CONTENTS ( LAST_VISIBLE_CONTENTS | ( LAST_VISIBLE_CONTENTS - 1 ) )

#define CONTENTS_TESTFOGVOLUME 0x100
#define CONTENTS_UNUSED 0x200

// unused
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
#define CONTENTS_UNUSED6 0x400

#define CONTENTS_TEAM1 0x800  // per team contents used to differentiate collisions
#define CONTENTS_TEAM2 0x1000 // between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE 0x4000

// remaining contents are non-visible, and don't eat brushes
#define CONTENTS_AREAPORTAL 0x8000

#define CONTENTS_PLAYERCLIP 0x10000
#define CONTENTS_MONSTERCLIP 0x20000

// currents can be added to any other contents, and may be mixed
#define CONTENTS_CURRENT_0 0x40000
#define CONTENTS_CURRENT_90 0x80000
#define CONTENTS_CURRENT_180 0x100000
#define CONTENTS_CURRENT_270 0x200000
#define CONTENTS_CURRENT_UP 0x400000
#define CONTENTS_CURRENT_DOWN 0x800000

#define CONTENTS_ORIGIN 0x1000000 // removed before bsping an entity

#define CONTENTS_MONSTER 0x2000000 // should never be on a brush, only in game
#define CONTENTS_DEBRIS 0x4000000
#define CONTENTS_DETAIL 0x8000000		// brushes to be added after vis leafs
#define CONTENTS_TRANSLUCENT 0x10000000 // auto set if any surface has trans
#define CONTENTS_LADDER 0x20000000
#define CONTENTS_HITBOX 0x40000000 // use accurate hitboxes on trace

// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
#define SURF_LIGHT 0x0001 // value will hold the light strength
#define SURF_SKY2D 0x0002 // don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define SURF_SKY 0x0004   // don't draw, but add to skybox
#define SURF_WARP 0x0008  // turbulent water warp
#define SURF_TRANS 0x0010
#define SURF_NOPORTAL 0x0020 // the surface can not have a portal placed on it
#define SURF_TRIGGER 0x0040  // FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define SURF_NODRAW 0x0080   // don't bother referencing the texture

#define SURF_HINT 0x0100 // make a primary bsp splitter

#define SURF_SKIP 0x0200	  // completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT 0x0400   // Don't calculate light
#define SURF_BUMPLIGHT 0x0800 // calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS 0x1000 // Don't receive shadows
#define SURF_NODECALS 0x2000  // Don't receive decals
#define SURF_NOCHOP 0x4000	// Don't subdivide patches on this surface
#define SURF_HITBOX 0x8000	// surface is part of a hitbox

// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define MASK_ALL ( 0xFFFFFFFF )
// everything that is normally solid
#define MASK_SOLID ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE )
// everything that blocks player movement
#define MASK_PLAYERSOLID ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE )
// blocks npc movement
#define MASK_NPCSOLID ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE )
// water physics in these contents
#define MASK_WATER ( CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME )
// everything that blocks lighting
#define MASK_OPAQUE ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE )
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS ( MASK_OPAQUE | CONTENTS_MONSTER )
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_BLOCKLOS )
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS ( MASK_BLOCKLOS | CONTENTS_MONSTER )
// everything that blocks line of sight for players
#define MASK_VISIBLE ( MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE )
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS ( MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE )
// bullets see these as solid
#define MASK_SHOT ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX )
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE )
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER )
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE )
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE )
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE )
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC ( CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE )
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL ( CONTENTS_WATER | CONTENTS_SLIME )

// UNDONE: This is untested, any moving water
#define MASK_CURRENT ( CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180 | CONTENTS_CURRENT_270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN )

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define MASK_DEADSOLID ( CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_GRATE )

#endif // BSPFLAGS_H


#endif // !MAIN_H