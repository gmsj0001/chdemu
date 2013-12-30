#pragma once

#pragma pack(push, 1)


struct _MAP_INFO // 0x6
{
	//_MAP_INFO() { memset(this, 0, sizeof(_MAP_INFO)); }
	WORD uiStageID; // +0x0(0x2)
	WORD uiMapGroupID; // +0x2(0x2)
	WORD uiMapID; // +0x4(0x2)
};

struct _EVENT_INFO // 0x12
{
	//_EVENT_INFO(void) { memset(this, 0, sizeof(_EVENT_INFO)); }
	DWORD id; // +0x0(0x4)
	struct _MAP_INFO mapInfo; // +0x4(0x6)
	float posX; // +0xa(0x4)
	float posY; // +0xe(0x4)
};

struct _MAPEVENT_INFO // 0x694
{
	_MAPEVENT_INFO::_MAPEVENT_INFO(void) { Clear(); }
	void _MAPEVENT_INFO::Clear(void) { memset(this, 0, sizeof(_MAPEVENT_INFO)); }
	void SetEventInfo(struct _EVENT_INFO*);
	//enum MAPEVENT_NPC_TYPE GetNpcType(void);
	DWORD classID; // +0x0(0x4)
	DWORD instanceID; // +0x4(0x4)
	DWORD id; // +0x8(0x4)
	struct _MAP_INFO mapInfo; // +0xc(0x6)
	float posX; // +0x12(0x4)
	float posY; // +0x16(0x4)
	float rectTop; // +0x1a(0x4)
	float rectLeft; // +0x1e(0x4)
	float rectRight; // +0x22(0x4)
	float rectBottom; // +0x26(0x4)
	BYTE eventType; // +0x2a(0x1)
	DWORD eventSubType; // +0x2b(0x4)
	DWORD eventTypeID; // +0x2f(0x4)
	//BYTE iExtraIconType; // +0x33(0x1)
	//DWORD obejctImage; // +0x34(0x4)
	//BYTE iTerrainLayer; // +0x38(0x1)
	//BYTE iFacing; // +0x39(0x1)
	//DWORD stageID; // +0x3a(0x4)
	//struct _REQUIRE_INFO requireInfo[0x2]; // +0x3e(0x30)
	//DWORD iConsumeItemID; // +0x6e(0x4)
	char name[0x11]; // +0x72(0x11)
	//char tipMessage[0x200]; // +0x83(0x200)
	//char errorMessage[0x200]; // +0x283(0x200)
	//char tipMessageOff[0x200]; // +0x483(0x200)
	//char dummy[0x3]; // +0x683(0x3)
	//DWORD iInsDungeunID; // +0x686(0x4)
	BYTE byDynamicCreate; // +0x68a(0x1)
	//DWORD iDynamicFX; // +0x68b(0x4)
	//DWORD ServerChange; // +0x68f(0x4)
	//BYTE iNpcType; // +0x693(0x1)
};

struct EVENTPORTAL_INFO // 0x18
{
	//EVENTPORTAL_INFO() { memset(this, 0, sizeof(EVENTPORTAL_INFO)); }
	DWORD m_iID; // +0x0(0x4)
	int m_iStageID; // +0x4(0x4)
	int m_iMapGroupID; // +0x8(0x4)
	int m_iMapID; // +0xc(0x4)
	float m_fPosX; // +0x10(0x4)
	float m_fPosY; // +0x14(0x4)
};

#pragma pack(pop)

class EventManager : public Singleton<EventManager>
{
public:
	typedef stdext::hash_map<int, _MAPEVENT_INFO*> EventHashMap;
	EventManager() { Init(); }
	void Init();
	void LoadLDTEvent();
	void LoadLDTEventPortal();
	_MAPEVENT_INFO* GetEvent(int iItemID);
	EVENTPORTAL_INFO* GetEventPortal(int iEventID);
	EventHashMap::iterator EventEnumIterBegin() { return m_hmEventInfo.begin(); }
	EventHashMap::iterator EventEnumIterEnd() { return m_hmEventInfo.end(); }
protected:
	stdext::hash_map<int, _MAPEVENT_INFO*> m_hmEventInfo;
	stdext::hash_map<int, EVENTPORTAL_INFO*> m_hmEventPortal;
};

class MapEvent : public GameObject
{
public:
	MapEvent(int iInstanceID, enum CLASS_ID eClass) 
	{
		SetInstanceID(iInstanceID);
		SetClassID(eClass); 
	}
	virtual float GetPosX();
	virtual float GetPosY();
	void SetEventID(int iEventID);
	int GetEventID() { return m_iEventID; }
	_MAPEVENT_INFO* GetEventInfo() { return m_pEventInfo; }
protected:
	_MAPEVENT_INFO* m_pEventInfo;
	int m_iEventID;

};