#ifndef PLAYER_H
#define PLAYER_H
#ifdef _WIN32
#pragma once
#endif

class c_base_weapon;

enum tf_cond
{
	TFCond_Slowed					= (1 << 0),		// Toggled when a player is slowed down. 
	TFCond_Zoomed					= (1 << 1),		// Toggled when a player is zoomed. 
	TFCond_Disguising				= (1 << 2),		// Toggled when a Spy is disguising.  
	TFCond_Disguised				= (1 << 3),		// Toggled when a Spy is disguised. 
	TFCond_Cloaked					= (1 << 4),		// Toggled when a Spy is invisible. 
	TFCond_Ubercharged				= (1 << 5),		// Toggled when a player is ÜberCharged. 
	TFCond_TeleportedGlow			= (1 << 6),		// Toggled when someone leaves a teleporter and has glow beneath their feet. 
	TFCond_Taunting					= (1 << 7),		// Toggled when a player is taunting. 
	TFCond_UberchargeFading			= (1 << 8),		// Toggled when the ÜberCharge is fading. 
	TFCond_CloakFlicker				= (1 << 9),		// Toggled when a Spy is visible during cloak. 
	TFCond_Teleporting				= (1 << 10),	// Only activates for a brief second when the player is being teleported; not very useful. 
	TFCond_Kritzkrieged				= (1 << 11),	// Toggled when a player is being crit buffed by the KritzKrieg. 
	TFCond_TmpDamageBonus			= (1 << 12),	// Unknown what this is for. Name taken from the AlliedModders SDK. 
	TFCond_DeadRingered				= (1 << 13),	// Toggled when a player is taking reduced damage from the Deadringer. 
	TFCond_Bonked					= (1 << 14),	// Toggled when a player is under the effects of The Bonk! Atomic Punch. 
	TFCond_Stunned					= (1 << 15),	// Toggled when a player's speed is reduced from airblast or a Sandman ball. 
	TFCond_Buffed					= (1 << 16),	// Toggled when a player is within range of an activated Buff Banner. 
	TFCond_Charging					= (1 << 17),	// Toggled when a Demoman charges with the shield. 
	TFCond_DemoBuff					= (1 << 18),	// Toggled when a Demoman has heads from the Eyelander. 
	TFCond_CritCola					= (1 << 19),	// Toggled when the player is under the effect of The Crit-a-Cola. 
	TFCond_InHealRadius				= (1 << 20),	// Unused condition, name taken from AlliedModders SDK. 
	TFCond_Healing					= (1 << 21),	// Toggled when someone is being healed by a medic or a dispenser. 
	TFCond_OnFire					= (1 << 22),	// Toggled when a player is on fire. 
	TFCond_Overhealed				= (1 << 23),	// Toggled when a player has >100% health. 
	TFCond_Jarated					= (1 << 24),	// Toggled when a player is hit with a Sniper's Jarate. 
	TFCond_Bleeding					= (1 << 25),	// Toggled when a player is taking bleeding damage. 
	TFCond_DefenseBuffed			= (1 << 26),	// Toggled when a player is within range of an activated Battalion's Backup. 
	TFCond_Milked					= (1 << 27),	// Player was hit with a jar of Mad Milk. 
	TFCond_MegaHeal					= (1 << 28),	// Player is under the effect of Quick-Fix charge. 
	TFCond_RegenBuffed				= (1 << 29),	// Toggled when a player is within a Concheror's range. 
	TFCond_MarkedForDeath			= (1 << 30),	// Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated. 
	TFCond_NoHealingDamageBuff		= (1 << 31),	// Unknown what this is used for.
	TFCondEx_SpeedBuffAlly			= (1 << 0),		// Toggled when a player gets hit with the disciplinary action. 
	TFCondEx_HalloweenCritCandy		= (1 << 1),		// Only for Scream Fortress event maps that drop crit candy. 
	TFCondEx_CritCanteen			= (1 << 2),		// Player is getting a crit boost from a MVM canteen.
	TFCondEx_CritDemoCharge			= (1 << 3),		// From demo's shield
	TFCondEx_CritHype				= (1 << 4),		// Soda Popper crits. 
	TFCondEx_CritOnFirstBlood		= (1 << 5),		// Arena first blood crit buff. 
	TFCondEx_CritOnWin				= (1 << 6),		// End of round crits. 
	TFCondEx_CritOnFlagCapture		= (1 << 7),		// CTF intelligence capture crits. 
	TFCondEx_CritOnKill				= (1 << 8),		// Unknown what this is for. 
	TFCondEx_RestrictToMelee		= (1 << 9),		// Unknown what this is for. 
	TFCondEx_DefenseBuffNoCritBlock = (1 << 10),	// MvM Buff.
	TFCondEx_Reprogrammed			= (1 << 11),	// MvM Bot has been reprogrammed.
	TFCondEx_PyroCrits				= (1 << 12),	// Player is getting crits from the Mmmph charge. 
	TFCondEx_PyroHeal				= (1 << 13),	// Player is being healed from the Mmmph charge. 
	TFCondEx_FocusBuff				= (1 << 14),	// Player is getting a focus buff.
	TFCondEx_DisguisedRemoved		= (1 << 15),	// Disguised remove from a bot.
	TFCondEx_MarkedForDeathSilent	= (1 << 16),	// Player is under the effects of the Escape Plan/Equalizer or GRU.
	TFCondEx_DisguisedAsDispenser	= (1 << 17),	// Bot is disguised as dispenser.
	TFCondEx_Sapped					= (1 << 18),	// MvM bot is being sapped.
	TFCondEx_UberchargedHidden		= (1 << 19),	// MvM Related
	TFCondEx_UberchargedCanteen		= (1 << 20),	// Player is receiving ÜberCharge from a canteen.
	TFCondEx_HalloweenBombHead		= (1 << 21),	// Player has a bomb on their head from Merasmus.
	TFCondEx_HalloweenThriller		= (1 << 22),	// Players are forced to dance from Merasmus.
	TFCondEx_BulletCharge			= (1 << 26),	// Player is receiving 75% reduced damage from bullets.
	TFCondEx_ExplosiveCharge		= (1 << 27),	// Player is receiving 75% reduced damage from explosives.
	TFCondEx_FireCharge				= (1 << 28),	// Player is receiving 75% reduced damage from fire.
	TFCondEx_BulletResistance		= (1 << 29),	// Player is receiving 10% reduced damage from bullets.
	TFCondEx_ExplosiveResistance	= (1 << 30),	// Player is receiving 10% reduced damage from explosives.
	TFCondEx_FireResistance			= (1 << 31),	// Player is receiving 10% reduced damage from fire.
	TFCondEx2_Stealthed				= (1 << 0),
	TFCondEx2_MedigunDebuff			= (1 << 1),
	TFCondEx2_StealthedUserBuffFade = (1 << 2),
	TFCondEx2_BulletImmune			= (1 << 3),
	TFCondEx2_BlastImmune			= (1 << 4),
	TFCondEx2_FireImmune			= (1 << 5),
	TFCondEx2_PreventDeath			= (1 << 6),
	TFCondEx2_MVMBotRadiowave		= (1 << 7),
	TFCondEx2_HalloweenSpeedBoost	= (1 << 8),		// Wheel has granted player speed boost.
	TFCondEx2_HalloweenQuickHeal	= (1 << 9),		// Wheel has granted player quick heal.
	TFCondEx2_HalloweenGiant		= (1 << 10),	// Wheel has granted player giant mode.
	TFCondEx2_HalloweenTiny			= (1 << 11),	// Wheel has granted player tiny mode.
	TFCondEx2_HalloweenInHell		= (1 << 12),	// Wheel has granted player in hell mode.
	TFCondEx2_HalloweenGhostMode	= (1 << 13),	// Wheel has granted player ghost mode.
	TFCondEx2_Parachute				= (1 << 16),	// Player has deployed the BASE Jumper.
	TFCondEx2_BlastJumping			= (1 << 17),	// Player has sticky or rocket jumped.
	TFCond_MiniCrits				= (TFCond_Buffed | TFCond_CritCola),
	TFCond_IgnoreStates				= (TFCond_Ubercharged | TFCond_Bonked),
	TFCondEx_IgnoreStates			= (TFCondEx_PyroHeal)
};

enum tfclasses_t
{
	TF2_None = 0,
	TF2_Scout,
	TF2_Sniper,
	TF2_Soldier,
	TF2_Demoman,
	TF2_Medic,
	TF2_Heavy,
	TF2_Pyro,
	TF2_Spy,
	TF2_Engineer
};

enum life_states
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

struct studiohdr_t;
struct mstudiohitboxset_t;
#define MAX_DATATABLES 1024 // must be a power of 2.
#define MAX_DATATABLE_PROPS 4096

#define MAX_ARRAY_ELEMENTS 2048 // a network array should have more that 1024 elements

#define HIGH_DEFAULT -121121.121121f

#define BITS_FULLRES -1    // Use the full resolution of the type being encoded.
#define BITS_WORLDCOORD -2 // Encode as a world coordinate.

#define DT_MAX_STRING_BITS 9
#define DT_MAX_STRING_BUFFERSIZE (1 << DT_MAX_STRING_BITS) // Maximum length of a string that can be sent.

#define STRINGBUFSIZE(className, varName) sizeof (((className *)0)->varName)

// Gets the size of a variable in a class.
#define PROPSIZEOF(className, varName) sizeof (((className *)0)->varName)

// SendProp::m_Flags.
#define SPROP_UNSIGNED (1 << 0) // Unsigned integer data.

#define SPROP_COORD (1 << 1) // If this is set, the float/vector is treated like a world coordinate.
// Note that the bit count is ignored in this case.

#define SPROP_NOSCALE (1 << 2) // For floating point, don't scale into range, just take value as is.

#define SPROP_ROUNDDOWN (1 << 3) // For floating point, limit high value to range minus one bit unit

#define SPROP_ROUNDUP (1 << 4) // For floating point, limit low value to range minus one bit unit

#define SPROP_NORMAL (1 << 5) // If this is set, the vector is treated like a normal (only valid for vectors)

#define SPROP_EXCLUDE (1 << 6) // This is an exclude prop (not excludED, but it points at another prop to be excluded).

#define SPROP_XYZE (1 << 7) // Use XYZ/Exponent encoding for vectors.

#define SPROP_INSIDEARRAY (1 << 8) // This tells us that the property is inside an array, so it shouldn't be put into the
// flattened property list. Its array will point at it when it needs to.

#define SPROP_PROXY_ALWAYS_YES (1 << 9) // Set for datatable props using one of the default datatable proxies like
// SendProxy_DataTableToDataTable that always send the data to all clients.

#define SPROP_CHANGES_OFTEN (1 << 10) // this is an often changed field, moved to head of sendtable so it gets a small index

#define SPROP_IS_A_VECTOR_ELEM (1 << 11) // Set automatically if SPROP_VECTORELEM is used.

#define SPROP_COLLAPSIBLE (1 << 12) // Set automatically if it's a datatable with an offset of 0 that doesn't change the pointer
// (ie: for all automatically-chained base classes).
// In this case, it can get rid of this SendPropDataTable altogether and spare the
// trouble of walking the hierarchy more than necessary.

#define SPROP_COORD_MP (1 << 13)              // Like SPROP_COORD, but special handling for multiplayer games
#define SPROP_COORD_MP_LOWPRECISION (1 << 14) // Like SPROP_COORD, but special handling for multiplayer games where the fractional component only gets a 3 bits instead of 5
#define SPROP_COORD_MP_INTEGRAL (1 << 15)     // SPROP_COORD_MP, but coordinates are rounded to integral boundaries
#define SPROP_NUMFLAGBITS_NETWORKED 16

// This is server side only, it's used to mark properties whose SendProxy_* functions encode against gpGlobals->tickcount (the only ones that currently do this are
//  m_flAnimTime and m_flSimulationTime.  MODs shouldn't need to mess with this probably
#define SPROP_ENCODED_AGAINST_TICKCOUNT (1 << 16)

// See SPROP_NUMFLAGBITS_NETWORKED for the ones which are networked
#define SPROP_NUMFLAGBITS 17

// Used by the SendProp and RecvProp functions to disable debug checks on type sizes.
#define SIZEOF_IGNORE -1

// Use this to extern send and receive datatables, and reference them.
#define EXTERN_SEND_TABLE(tableName) \
	namespace tableName {            \
	extern SendTable g_SendTable;    \
	}
#define EXTERN_RECV_TABLE(tableName) \
	namespace tableName {            \
	extern RecvTable g_RecvTable;    \
	}

#define REFERENCE_SEND_TABLE(tableName) tableName::g_SendTable
#define REFERENCE_RECV_TABLE(tableName) tableName::g_RecvTable

class SendProp;

typedef enum {
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array, // An array of the base types (can't be of datatables).
	DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	DPT_Quaternion,
#endif
	DPT_NUMSendPropTypes
} SendPropType;

class DVariant
{
public:
	DVariant()
	{
		m_Type = DPT_Float;
	}
	DVariant(float val)
	{
		m_Type = DPT_Float;
		m_Float = val;
	}

	//	const char *ToString()
	//	{
	//		static char text[128];
	//
	//		switch (m_Type) {
	//		case DPT_Int:
	//			V_snprintf(text, sizeof(text), "%i", m_Int);
	//			break;
	//		case DPT_Float:
	//			V_snprintf(text, sizeof(text), "%.3f", m_Float);
	//			break;
	//		case DPT_Vector:
	//			V_snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f)", m_Vector[0], m_Vector[1], m_Vector[2]);
	//			break;
	//#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
	//		case DPT_Quaternion:
	//			snprintf(text, sizeof(text), "(%.3f,%.3f,%.3f %.3f)",
	//				m_Vector[0], m_Vector[1], m_Vector[2], m_Vector[3]);
	//			break;
	//#endif
	//		case DPT_String:
	//			if (m_pString)
	//				return m_pString;
	//			else
	//				return "NULL";
	//			break;
	//		case DPT_Array:
	//			V_snprintf(text, sizeof(text), "Array");
	//			break;
	//		case DPT_DataTable:
	//			V_snprintf(text, sizeof(text), "DataTable");
	//			break;
	//		default:
	//			V_snprintf(text, sizeof(text), "DVariant type %i unknown", m_Type);
	//			break;
	//		}
	//
	//		return text;
	//	}

	union
	{
		float m_Float;
		long  m_Int;
		char *m_pString;
		void *m_pData; // For DataTables.
#if 0                  // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
		float	m_Vector[4];
#else
		float m_Vector[3];
#endif
	};
	SendPropType m_Type;
};

// This can be used to set the # of bits used to transmit a number between 0 and nMaxElements-1.
inline int NumBitsForCount(int nMaxElements)
{
	int nBits = 0;
	while (nMaxElements > 0) {
		++nBits;
		nMaxElements >>= 1;
	}
	return nBits;
}

#define ADDRESSPROXY_NONE -1

class RecvTable;
class RecvProp;

// This is passed into RecvProxy functions.
class CRecvProxyData
{
public:
	const RecvProp *m_pRecvProp; // The property it's receiving.

	DVariant m_Value; // The value given to you to store.

	int m_iElement; // Which array element you're getting.

	int m_ObjectID; // The object being referred to.
};

//-----------------------------------------------------------------------------
// pStruct = the base structure of the datatable this variable is in (like C_BaseEntity)
// pOut    = the variable that this this proxy represents (like C_BaseEntity::m_SomeValue).
//
// Convert the network-standard-type value in m_Value into your own format in pStruct/pOut.
//-----------------------------------------------------------------------------
typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);

// ------------------------------------------------------------------------ //
// ArrayLengthRecvProxies are optionally used to get the length of the
// incoming array when it changes.
// ------------------------------------------------------------------------ //
typedef void(*ArrayLengthRecvProxyFn)(void *pStruct, int objectID, int currentArrayLength);

// NOTE: DataTable receive proxies work differently than the other proxies.
// pData points at the object + the recv table's offset.
// pOut should be set to the location of the object to unpack the data table into.
// If the parent object just contains the child object, the default proxy just does *pOut = pData.
// If the parent object points at the child object, you need to dereference the pointer here.
// NOTE: don't ever return null from a DataTable receive proxy function. Bad things will happen.
typedef void(*DataTableRecvVarProxyFn)(const RecvProp *pProp, void **pOut, void *pData, int objectID);

// This is used to fork over the standard proxy functions to the engine so it can
// make some optimizations.
class CStandardRecvProxies
{
public:
	CStandardRecvProxies();

	RecvVarProxyFn m_Int32ToInt8;
	RecvVarProxyFn m_Int32ToInt16;
	RecvVarProxyFn m_Int32ToInt32;
	RecvVarProxyFn m_FloatToFloat;
	RecvVarProxyFn m_VectorToVector;
};
extern CStandardRecvProxies g_StandardRecvProxies;

class CRecvDecoder;

class RecvProp
{
	// This info comes from the receive data table.
public:
	RecvProp();

	void InitArray(int nElements, int elementStride);

	int GetNumElements() const;
	void SetNumElements(int nElements);

	int GetElementStride() const;
	void SetElementStride(int stride);

	int GetFlags() const;

	const char *GetName() const;
	SendPropType GetType() const;

	RecvTable *GetDataTable() const;
	void SetDataTable(RecvTable *pTable);

	RecvVarProxyFn GetProxyFn() const;
	void SetProxyFn(RecvVarProxyFn fn);

	DataTableRecvVarProxyFn GetDataTableProxyFn() const;
	void SetDataTableProxyFn(DataTableRecvVarProxyFn fn);

	int GetOffset() const;
	void SetOffset(int o);

	// Arrays only.
	RecvProp *GetArrayProp() const;
	void SetArrayProp(RecvProp *pProp);

	// Arrays only.
	void SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy);
	ArrayLengthRecvProxyFn GetArrayLengthProxy() const;

	bool IsInsideArray() const;
	void SetInsideArray();

	// Some property types bind more data to the prop in here.
	const void *GetExtraData() const;
	void SetExtraData(const void *pData);

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char *GetParentArrayPropName();
	void SetParentArrayPropName(const char *pArrayPropName);

public:
	const char *m_pVarName;
	SendPropType m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	RecvVarProxyFn m_ProxyFn;

private:
	bool m_bInsideArray; // Set to true by the engine if this property sits inside an array.

						 // Extra data that certain special property types bind to the property here.
	const void *m_pExtraData;

	// If this is an array (DPT_Array).
	RecvProp *m_pArrayProp;
	ArrayLengthRecvProxyFn m_ArrayLengthProxy;

	DataTableRecvVarProxyFn m_DataTableProxyFn; // For RDT_DataTable.

	RecvTable *m_pDataTable; // For RDT_DataTable.
	int m_Offset;

	int m_ElementStride;
	int m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char *m_pParentArrayPropName;
};

class RecvTable
{
public:
	typedef RecvProp PropType;

	RecvTable();
	RecvTable(RecvProp *pProps, int nProps, const char *pNetTableName);
	~RecvTable();

	void Construct(RecvProp *pProps, int nProps, const char *pNetTableName);

	int GetNumProps();
	RecvProp *GetProp(int i);

	const char *GetName();

	// Used by the engine while initializing array props.
	void SetInitialized(bool bInitialized);
	bool IsInitialized() const;

	// Used by the engine.
	void SetInMainList(bool bInList);
	bool IsInMainList() const;

public:
	// Properties described in a table.
	RecvProp * m_pProps;
	int m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	CRecvDecoder *m_pDecoder;

	const char *m_pNetTableName; // The name matched between client and server.

private:
	bool m_bInitialized;
	bool m_bInMainList;
};

inline int RecvTable::GetNumProps()
{
	return m_nProps;
}

inline RecvProp *RecvTable::GetProp(int i)
{
	// Assert( i >= 0 && i < m_nProps );
	return &m_pProps[i];
}

inline const char *RecvTable::GetName()
{
	return m_pNetTableName;
}

inline void RecvTable::SetInitialized(bool bInitialized)
{
	m_bInitialized = bInitialized;
}

inline bool RecvTable::IsInitialized() const
{
	return m_bInitialized;
}

inline void RecvTable::SetInMainList(bool bInList)
{
	m_bInMainList = bInList;
}

inline bool RecvTable::IsInMainList() const
{
	return m_bInMainList;
}

// ------------------------------------------------------------------------------------------------------ //
// See notes on BEGIN_SEND_TABLE for a description. These macros work similarly.
// ------------------------------------------------------------------------------------------------------ //
#define BEGIN_RECV_TABLE( className, tableName )                                                                                                                                                                                                                                                                                                                                                               \
    BEGIN_RECV_TABLE_NOBASE( className, tableName )                                                                                                                                                                                                                                                                                                                                                            \
    RecvPropDataTable( "baseclass", 0, 0, className::BaseClass::m_pClassRecvTable, DataTableRecvProxy_StaticDataTable ),

#define BEGIN_RECV_TABLE_NOBASE( className, tableName )                                                                                                                                                                                                                                                                                                                                                        \
    template < typename T >                                                                                                                                                                                                                                                                                                                                                                                    \
    int ClientClassInit( T * );                                                                                                                                                                                                                                                                                                                                                                                \
    namespace tableName                                                                                                                                                                                                                                                                                                                                                                                        \
    {                                                                                                                                                                                                                                                                                                                                                                                                          \
    struct ignored;                                                                                                                                                                                                                                                                                                                                                                                            \
    }                                                                                                                                                                                                                                                                                                                                                                                                          \
    template <>                                                                                                                                                                                                                                                                                                                                                                                                \
    int ClientClassInit< tableName::ignored >( tableName::ignored * );                                                                                                                                                                                                                                                                                                                                         \
    namespace tableName                                                                                                                                                                                                                                                                                                                                                                                        \
    {                                                                                                                                                                                                                                                                                                                                                                                                          \
    RecvTable g_RecvTable;                                                                                                                                                                                                                                                                                                                                                                                     \
    int g_RecvTableInit = ClientClassInit( (tableName::ignored *)NULL );                                                                                                                                                                                                                                                                                                                                       \
    }                                                                                                                                                                                                                                                                                                                                                                                                          \
    template <>                                                                                                                                                                                                                                                                                                                                                                                                \
    int ClientClassInit< tableName::ignored >( tableName::ignored * )                                                                                                                                                                                                                                                                                                                                          \
    {                                                                                                                                                                                                                                                                                                                                                                                                          \
        typedef className currentRecvDTClass;                                                                                                                                                                                                                                                                                                                                                                  \
        const char *pRecvTableName = #tableName;                                                                                                                                                                                                                                                                                                                                                               \
        RecvTable &RecvTable = tableName::g_RecvTable;                                                                                                                                                                                                                                                                                                                                                         \
        static RecvProp RecvProps[] = { RecvPropInt( "should_never_see_this", 0, sizeof( int ) ), // It adds a dummy property at the start so you can define "empty" SendTables.

#define END_RECV_TABLE()                                                                                                                                                                                                                                                                                                                                                                                       \
    }                                                                                                                                                                                                                                                                                                                                                                                                          \
    ;                                                                                                                                                                                                                                                                                                                                                                                                          \
    RecvTable.Construct( RecvProps + 1, sizeof( RecvProps ) / sizeof( RecvProp ) - 1, pRecvTableName );                                                                                                                                                                                                                                                                                                        \
    return 1;                                                                                                                                                                                                                                                                                                                                                                                                  \
    }

#define RECVINFO( varName ) #varName, offsetof( currentRecvDTClass, varName ), sizeof( ( (currentRecvDTClass *)0 )->varName )
#define RECVINFO_NAME( varName, remoteVarName ) #remoteVarName, offsetof( currentRecvDTClass, varName ), sizeof( ( (currentRecvDTClass *)0 )->varName )
#define RECVINFO_STRING( varName ) #varName, offsetof( currentRecvDTClass, varName ), STRINGBUFSIZE( currentRecvDTClass, varName )
#define RECVINFO_BASECLASS( tableName ) RecvPropDataTable( "this", 0, 0, &REFERENCE_RECV_TABLE( tableName ) )
#define RECVINFO_ARRAY( varName ) #varName, offsetof( currentRecvDTClass, varName ), sizeof( ( (currentRecvDTClass *)0 )->varName[ 0 ] ), sizeof( ( (currentRecvDTClass *)0 )->varName ) / sizeof( ( (currentRecvDTClass *)0 )->varName[ 0 ] )

// Just specify the name and offset. Used for strings and data tables.
#define RECVINFO_NOSIZE( varName ) #varName, offsetof( currentRecvDTClass, varName )
#define RECVINFO_DT( varName ) RECVINFO_NOSIZE( varName )
#define RECVINFO_DTNAME( varName, remoteVarName ) #remoteVarName, offsetof( currentRecvDTClass, varName )

void RecvProxy_FloatToFloat(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_VectorToVector(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_QuaternionToQuaternion(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt8(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt16(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_StringToString(const CRecvProxyData *pData, void *pStruct, void *pOut);
void RecvProxy_Int32ToInt32(const CRecvProxyData *pData, void *pStruct, void *pOut);

// StaticDataTable does *pOut = pData.
void DataTableRecvProxy_StaticDataTable(const RecvProp *pProp, void **pOut, void *pData, int objectID);

// PointerDataTable does *pOut = *((void**)pData)   (ie: pData is a pointer to the object to decode into).
void DataTableRecvProxy_PointerDataTable(const RecvProp *pProp, void **pOut, void *pData, int objectID);

RecvProp RecvPropFloat(const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE, // Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_FloatToFloat);

RecvProp RecvPropVector(const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE, // Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_VectorToVector);

// This is here so the RecvTable can look more like the SendTable.
#define RecvPropQAngles RecvPropVector

#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!

RecvProp RecvPropQuaternion(
	const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE,	// Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = RecvProxy_QuaternionToQuaternion
);
#endif

RecvProp RecvPropInt(const char *pVarName,
	int offset,
	int sizeofVar = SIZEOF_IGNORE, // Handled by RECVINFO macro, but set to SIZEOF_IGNORE if you don't want to bother.
	int flags = 0,
	RecvVarProxyFn varProxy = 0);

RecvProp RecvPropString(const char *pVarName, int offset, int bufferSize, int flags = 0, RecvVarProxyFn varProxy = RecvProxy_StringToString);

RecvProp RecvPropDataTable(const char *pVarName, int offset, int flags, RecvTable *pTable, DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable);

RecvProp RecvPropArray3(const char *pVarName, int offset, int sizeofVar, int elements, RecvProp pArrayProp, DataTableRecvVarProxyFn varProxy = DataTableRecvProxy_StaticDataTable);

// Use the macro to let it automatically generate a table name. You shouldn't
// ever need to reference the table name. If you want to exclude this array, then
// reference the name of the variable in varTemplate.
RecvProp InternalRecvPropArray(const int elementCount, const int elementStride, const char *pName, ArrayLengthRecvProxyFn proxy);

//
// Use this if you want to completely manage the way the array data is stored.
// You'll need to provide a proxy inside varTemplate that looks for 'iElement'
// to figure out where to store the specified element.
//
#define RecvPropVirtualArray( arrayLengthProxy, maxArrayLength, varTemplate, propertyName ) varTemplate, InternalRecvPropArray( maxArrayLength, 0, #propertyName, arrayLengthProxy )

// Use this and pass the array name and it will figure out the count and stride automatically.
#define RecvPropVariableLengthArray( arrayLengthProxy, varTemplate, arrayName ) varTemplate, InternalRecvPropArray( sizeof( ( (currentRecvDTClass *)0 )->arrayName ) / PROPSIZEOF( currentRecvDTClass, arrayName[ 0 ] ), PROPSIZEOF( currentRecvDTClass, arrayName[ 0 ] ), #arrayName, arrayLengthProxy )

// Use this and pass the array name and it will figure out the count and stride automatically.
#define RecvPropArray( varTemplate, arrayName ) RecvPropVariableLengthArray( 0, varTemplate, arrayName )

// Use this one to specify the element count and stride manually.
#define RecvPropArray2( arrayLengthProxy, varTemplate, elementCount, elementStride, arrayName ) varTemplate, InternalRecvPropArray( elementCount, elementStride, #arrayName, arrayLengthProxy )

// ---------------------------------------------------------------------------------------- //
// Inlines.
// ---------------------------------------------------------------------------------------- //

inline void RecvProp::InitArray(int nElements, int elementStride)
{
	m_RecvType = DPT_Array;
	m_nElements = nElements;
	m_ElementStride = elementStride;
}

inline int RecvProp::GetNumElements() const
{
	return m_nElements;
}

inline void RecvProp::SetNumElements(int nElements)
{
	m_nElements = nElements;
}

inline int RecvProp::GetElementStride() const
{
	return m_ElementStride;
}

inline void RecvProp::SetElementStride(int stride)
{
	m_ElementStride = stride;
}

inline int RecvProp::GetFlags() const
{
	return m_Flags;
}

inline const char *RecvProp::GetName() const
{
	return m_pVarName;
}

inline SendPropType RecvProp::GetType() const
{
	return m_RecvType;
}

inline RecvTable *RecvProp::GetDataTable() const
{
	return m_pDataTable;
}

inline void RecvProp::SetDataTable(RecvTable *pTable)
{
	m_pDataTable = pTable;
}

inline RecvVarProxyFn RecvProp::GetProxyFn() const
{
	return m_ProxyFn;
}

inline void RecvProp::SetProxyFn(RecvVarProxyFn fn)
{
	m_ProxyFn = fn;
}

inline DataTableRecvVarProxyFn RecvProp::GetDataTableProxyFn() const
{
	return m_DataTableProxyFn;
}

inline void RecvProp::SetDataTableProxyFn(DataTableRecvVarProxyFn fn)
{
	m_DataTableProxyFn = fn;
}

inline int RecvProp::GetOffset() const
{
	return m_Offset;
}

inline void RecvProp::SetOffset(int o)
{
	m_Offset = o;
}

inline RecvProp *RecvProp::GetArrayProp() const
{
	return m_pArrayProp;
}

inline void RecvProp::SetArrayProp(RecvProp *pProp)
{
	m_pArrayProp = pProp;
}

inline void RecvProp::SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy)
{
	m_ArrayLengthProxy = proxy;
}

inline ArrayLengthRecvProxyFn RecvProp::GetArrayLengthProxy() const
{
	return m_ArrayLengthProxy;
}

inline bool RecvProp::IsInsideArray() const
{
	return m_bInsideArray;
}

inline void RecvProp::SetInsideArray()
{
	m_bInsideArray = true;
}

inline const void *RecvProp::GetExtraData() const
{
	return m_pExtraData;
}

inline void RecvProp::SetExtraData(const void *pData)
{
	m_pExtraData = pData;
}

inline const char *RecvProp::GetParentArrayPropName()
{
	return m_pParentArrayPropName;
}

inline void RecvProp::SetParentArrayPropName(const char *pArrayPropName)
{
	m_pParentArrayPropName = pArrayPropName;
}
class client_class
{
private:
	BYTE _chPadding[8];
public:
	char* name;
	RecvTable* table;
	client_class* next_class;
	int class_id;
};

template<typename T>
class singleton
{
protected:
	singleton() {}
	~singleton() {}

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;

public:
	static T& get()
	{
		static T instance{};
		return instance;
	}
};
class CNetVars : public singleton<CNetVars>
{
	struct node;
	using map_type = std::unordered_map<std::string, std::shared_ptr<node>>;

	struct node {
		node(DWORD offset) : offset(offset) {}
		map_type nodes;
		DWORD offset;
	};

	map_type nodes;

public:
	void Initialize();

private:
	void populate_nodes(class RecvTable *recv_table, map_type *map);

	/**
	* get_offset_recursive - Return the offset of the final node
	* @map:	Node map to scan
	* @acc:	Offset accumulator
	* @name:	Netvar name to search for
	*
	* Get the offset of the last netvar from map and return the sum of it and accum
	*/
	DWORD get_offset_recursive(map_type &map, int acc, const char *name)
	{
		return acc + map[name]->offset;
	}

	/**
	* get_offset_recursive - Recursively grab an offset from the tree
	* @map:	Node map to scan
	* @acc:	Offset accumulator
	* @name:	Netvar name to search for
	* @args:	Remaining netvar names
	*
	* Perform tail recursion with the nodes of the specified branch of the tree passed for map
	* and the offset of that branch added to acc
	*/
	template<typename ...args_t>
	DWORD get_offset_recursive(map_type &map, int acc, const char *name, args_t ...args)
	{
		const auto &node = map[name];
		return get_offset_recursive(node->nodes, acc + node->offset, args...);
	}

public:
	/**
	* get_offset - Get the offset of a netvar given a list of branch names
	* @name:	Top level datatable name
	* @args:	Remaining netvar names
	*
	* Initiate a recursive search down the branch corresponding to the specified datable name
	*/
	template<typename ...args_t>
	DWORD get_offset(const char *name, args_t ...args)
	{
		const auto &node = nodes[name];
		return get_offset_recursive(node->nodes, node->offset, args...);
	}
};

template<typename T>
class CDynamicNetvar
{
	DWORD off;
	using sT = typename std::remove_reference<T>::type;
public:
	template<typename... args_t>
	CDynamicNetvar(args_t... a)
	{
		// just assume these wont be accessed until after netvars has been inited

		off = CNetVars::get().get_offset(a...);
	}
	template<typename... args_t>
	CDynamicNetvar(int offset, args_t... a)
	{
		off = CNetVars::get().get_offset(a...) + offset;
	}

	T GetValue(PVOID base) const
	{
		return *reinterpret_cast< T * >((DWORD)base + (DWORD)off);
	}
	void SetValue(PVOID base, T val) const
	{
		*reinterpret_cast<sT* >((DWORD)(base)+((DWORD)(off))) = val;
	}
};

#define DYNVAR(name, type, ...) static CDynamicNetvar<type> ##name( __VA_ARGS__ )
#define DYNVAR_RETURN(type, base, ...) DYNVAR(n, type, __VA_ARGS__); return n.GetValue(base)

#define DYNVAR_OFF(name, type, offset, ...) static CDynamicNetvar<type> ##name(offset, __VA_ARGS__)
#define DYNVAR_OFF_RETURN(type, base, offset, ...) DYNVAR_OFF(n, type, offset, __VA_ARGS__); return n.GetValue(base)

class c_base_player
{
public:

	client_networkable* get_client_networkable()
	{
		using original_fn = client_networkable * (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 4)(this);
	}

	client_renderable* get_client_renderable()
	{
		using original_fn = client_renderable * (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 5)(this);
	}

	PVOID get_collideable()
	{
		using original_fn = PVOID(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 3)(this);
	}

	client_class* get_client_class() {
		PVOID networkable = (PVOID)(this + 0x8);
		using original_fn = client_class*(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(networkable, 2)(networkable);
	}

	int get_flags()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x37C);
	}

	int get_team_num()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xB0);
	}

	vec3_t get_velocity()
	{
		return *reinterpret_cast<vec3_t*>(uintptr_t(this) + 0x120);
	}

	int get_class_num()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x154C);
	}

	int get_health()
	{
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 106)(this);
	}

	int get_max_health() {
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 107)(this);
	}

	BYTE get_life_state()
	{
		return *reinterpret_cast<BYTE*>(uintptr_t(this) + 0xA5);
	}

	bool is_dead() {
		return (this->get_life_state() != LIFE_ALIVE);
	}

	int get_condition() {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x18BC);
	}

	void set_condition(int force_condition) {
		*reinterpret_cast<int*>(uintptr_t(this) + 0x18BC) = force_condition;
	}

	float get_simulation_time() {
		return *reinterpret_cast<float*>(uintptr_t(this) + 0x6C);
	}

	vec3_t& get_abs_origin()
	{
		using original_fn = vec3_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 9)(this);
	}

	vec3_t get_eye_position()
	{
		return *reinterpret_cast<vec3_t*>(uintptr_t(this) + 0xFC) + get_abs_origin(); // m_vecViewOffset[0]
	}

	bool is_dormant()
	{
		using original_fn = bool(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_client_networkable(), 8)(get_client_networkable());
	}

	HANDLE get_owner() {
		DYNVAR_RETURN(HANDLE, this, "DT_BaseEntity", "m_hOwnerEntity");
	}

		HANDLE get_builder() {
			DYNVAR_RETURN(HANDLE, this, "DT_BaseObject", "m_hBuilder");
		}

		int get_level() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iUpgradeLevel");
		}

		bool is_sapped() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bHasSapper");
		}

		bool is_carried() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bCarried");
		}

		bool is_placing() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bPlacing");
		}

		bool is_building() {
			DYNVAR_RETURN(bool, this, "DT_BaseObject", "m_bBuilding");
		}

		int get_state() {
			DYNVAR_RETURN(int, this, "DT_ObjectSentrygun", "m_iState");
		}

		float percent_constructed() {
			if (this->is_building()) {
				DYNVAR_RETURN(float, this, "DT_BaseObject", "m_flPercentageConstructed");
			} else {
				return 0.0f;
			}
		}

		int metal_upgraded() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iUpgradeMetal");
		}

		int get_object_health() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iHealth");
		}

		int get_object_max_health() {
			DYNVAR_RETURN(int, this, "DT_BaseObject", "m_iMaxHealth");
		}

	vec3_t& get_collideable_mins()
	{
		using original_fn = vec3_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_collideable(), 1)(get_collideable());
	}

	vec3_t& get_collideable_maxs()
	{
		using original_fn = vec3_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_collideable(), 2)(get_collideable());
	}

	matrix3x4_t& get_rgfl_coordinate_frame()
	{
		using original_fn = matrix3x4_t&(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(get_client_renderable(), 34)(get_client_renderable());
	}

	bool setup_bones(matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float current_time)
	{
		PVOID renderable = (PVOID)(this + 0x4);
		using original_fn = bool(__thiscall*)(PVOID, matrix3x4_t*, int, int, float);
		return utilities::call_vfunc<original_fn>(renderable, 16)(renderable, bone_to_world_out, max_bones, bone_mask, current_time);
	}

	vec3_t get_bone_positon(int bone) {
		matrix3x4_t matrix[128];
		setup_bones(matrix, 128, 0x100, 0);
		return vec3_t(matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3]);
	}

	int draw_model(int flags) {
		using original_fn = int(__thiscall*)(PVOID, int);
		return utilities::call_vfunc<original_fn>(get_client_renderable(), 10)(get_client_renderable(), flags);
	}

	bool is_player_bot() {
		DYNVAR_RETURN(bool, this, "DT_TFPlayer", "m_bIsABot");
	}

	/*void set_abs_origin(vec3_t &vector) {
		using original_fn = void(__thiscall*)(PVOID, vec3_t&);
		static original_fn original = (original_fn)utilities::find_signature("client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07");
		return original(this, vector);
	}*/

	DWORD* get_model() {
		PVOID renderable = (PVOID)(this + 0x4);
		using original_fn = DWORD * (__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(renderable, 9)(renderable);
	}

	int get_index() {
		PVOID networkable = (PVOID)(this + 0x8);
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(networkable, 9)(networkable);
	}
	int get_tick_base() {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x114C);
	}
	void get_render_bounds(vec3_t &min, vec3_t &max) {
		PVOID renderable = (PVOID)(this + 0x4);
		using original_fn = void(__thiscall*)(PVOID, vec3_t&, vec3_t&);
		utilities::call_vfunc<original_fn>(renderable, 20)(renderable, min, max);
	}
	vec3_t get_world_space_center() {
		vec3_t min, max, out; this->get_render_bounds(min, max);
		out = this->get_abs_origin(); out[2] += (min[2] + max[2]) / 2;
		return out;
	}
	int get_pipe_type() {
		DYNVAR_RETURN(int, this, "DT_TFProjectile_Pipebomb", "m_iType");
	}
	HANDLE get_thrower() {
		DYNVAR_RETURN(HANDLE, this, "DT_BaseGrenade", "m_hThrower");
	}
	void force_taunt_cam(bool forced) {
		DYNVAR(n, bool, "DT_TFPlayer", "m_nForceTauntCam");
		n.SetValue(this, forced);
	}
	int get_observer_mode() {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x109C);
	}
	HANDLE get_observer_target() {
		return *reinterpret_cast<HANDLE*>(uintptr_t(this) + 0x10A0);
	}
	void force_taunt_yaw(float forced_yaw) {
		DYNVAR(n, float, "DT_TFPlayer", "m_flTauntYaw");
		n.SetValue(this, forced_yaw);
	}

	c_base_weapon* get_active_weapon();
	const char* get_model_name();
};

class c_base_weapon : public c_base_player {
public:
	char* get_name() {
		using original_fn = char*(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 329)(this);
	}
	char* get_print_name() {
		using original_fn = char*(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 330)(this);
	}
	int get_item_definition_index() {
		DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
	}
	int get_slot() {
		using original_fn = int(__thiscall*)(PVOID);
		return utilities::call_vfunc<original_fn>(this, 327)(this);
	}
	float last_fire_time() {
		DYNVAR_RETURN(float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime");
	}
	float get_next_attack() {
		DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
	}
	float get_next_melee_attack() {
		DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextSecondaryAttack");
	}
	bool ready_to_backstab() {
		DYNVAR_RETURN(bool, this, "DT_TFWeaponKnife", "m_bReadyToBackstab");
	}
};

#endif // !PLAYER_H