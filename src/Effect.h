#pragma once

enum EFFECT_TYPE
{
	ET_NULL = 0,
	ET_INSTANT = 1,
	ET_KEEP = 2,
	ET_TOGGLE = 3,
	ET_PASSIVE = 4
};

enum EFFECT_STATUS_TYPE
{
	EST_NULL = 0,
	EST_PHYSICAL = 1,
	EST_MAGIC = 2,
	EST_STAT = 3,
	EST_EVENT = 4,
	EST_TRIGGER = 5,
	EST_SKILL = 6,
	EST_HEAL = 7,
	EST_DOT = 8,
	EST_AURA = 9,
	EST_RECALL = 10,
	EST_EST_CONDITION = 11,
	EST_MAX = 12
};

enum EFFECT_STATUS_KIND_TYPE
{
	DSKT_NULL = 0,
	DSKT_PHYSICAL_ATTACK = 10001,
	DSKT_PHYSICAL_MIGHT_ATTACK = 10002,
	DSKT_MAGIC_FIRE = 20001,
	DSKT_MAGIC_WATER = 20002,
	DSKT_MAGIC_WIND = 20003,
	DSKT_MAGIC_EARTH = 20004,
	DSKT_CLASS = 1,
	DSKT_GENDER = 2,
	DSKT_LV = 3,
	DSKT_MONEY = 4,
	DSKT_EXP = 5,
	DSKT_HP = 6,
	DSKT_SP = 7,
	DSKT_CURR_FAME = 8,
	DSKT_BASE_FAME = 9,
	DSKT_ADD_FAME = 10,
	DSKT_RATIO_FAME = 11,
	DSKT_CURR_ATK = 12,
	DSKT_BASE_ATK = 13,
	DSKT_ADD_ATK = 14,
	DSKT_RATIO_ATK = 15,
	DSKT_CURR_LUCK = 16,
	DSKT_BASE_LUCK = 17,
	DSKT_ADD_LUCK = 18,
	DSKT_RATIO_LUCK = 19,
	DSKT_CURR_ELE = 20,
	DSKT_BASE_ELE = 21,
	DSKT_ADD_ELE = 22,
	DSKT_RATIO_ELE = 23,
	DSKT_CURR_DEF = 24,
	DSKT_BASE_DEF = 25,
	DSKT_ADD_DEF = 26,
	DSKT_RATIO_DEF = 27,
	DSKT_CURR_MHP = 28,
	DSKT_BASE_MHP = 29,
	DSKT_ADD_MHP = 30,
	DSKT_RATIO_MHP = 31,
	DSKT_CURR_MSP = 32,
	DSKT_BASE_MSP = 33,
	DSKT_ADD_MSP = 34,
	DSKT_RATIO_MSP = 35,
	DSKT_CURR_WATER = 36,
	DSKT_BASE_WATER = 37,
	DSKT_ADD_WATER = 38,
	DSKT_RATIO_WATER = 39,
	DSKT_CURR_WIND = 40,
	DSKT_BASE_WIND = 41,
	DSKT_ADD_WIND = 42,
	DSKT_RATIO_WIND = 43,
	DSKT_CURR_FIRE = 44,
	DSKT_BASE_FIRE = 45,
	DSKT_ADD_FIRE = 46,
	DSKT_RATIO_FIRE = 47,
	DSKT_CURR_EARTH = 48,
	DSKT_BASE_EARTH = 49,
	DSKT_ADD_EARTH = 50,
	DSKT_RATIO_EARTH = 51,
	DSKT_CURR_RECV_HP = 52,
	DSKT_BASE_RECV_HP = 53,
	DSKT_ADD_RECV_HP = 54,
	DSKT_RATIO_RECV_HP = 55,
	DSKT_CURR_RECV_SP = 56,
	DSKT_BASE_RECV_SP = 57,
	DSKT_ADD_RECV_SP = 58,
	DSKT_RATIO_RECV_SP = 59,
	DSKT_CURR_HEAVY = 60,
	DSKT_BASE_HEAVY = 61,
	DSKT_ADD_HEAVY = 62,
	DSKT_RATIO_HEAVY = 63,
	DSKT_CURR_NORMAL = 64,
	DSKT_BASE_NORMAL = 65,
	DSKT_ADD_NORMAL = 66,
	DSKT_RATIO_NORMAL = 67,
	DSKT_CURR_MP_WATER = 68,
	DSKT_BASE_MP_WATER = 69,
	DSKT_CURR_MP_WIND = 70,
	DSKT_BASE_MP_WIND = 71,
	DSKT_CURR_MP_FIRE = 72,
	DSKT_BASE_MP_FIRE = 73,
	DSKT_CURR_MP_EARTH = 74,
	DSKT_BASE_MP_EARTH = 75,
	DSKT_ADD_MP_ELEMENTAL = 76,
	DSKT_RATIO_MP_ELEMENTAL = 77,
	DSKT_CURR_AP_HEAVY = 78,
	DSKT_BASE_AP_HEAVY = 79,
	DSKT_CURR_AP_NORMAL = 80,
	DSKT_BASE_AP_NORMAL = 81,
	DSKT_ADD_AP_ATK = 82,
	DSKT_RATIO_AP_ATK = 83,
	DSKT_RECOVERY_RATIO_HP = 2001,
	DSKT_RECOVERY_RATIO_SP = 2002,
	DSKT_RECOVERY_HP = 40001,
	DSKT_RECOVERY_SP = 40002,
	DSKT_CONDITION_STUN = 110001,
	DSKT_CONDITION_STUN_CANCEL = 110002,
	DSKT_CONDITION_SLEEP = 110003,
	DSKT_CONDITION_SLEEP_CANCEL = 110004,
	DSKT_CONDITION_SILENCE = 110005,
	DSKT_CONDITION_SILENCE_CANCEL = 110006
};

struct EFFECT_STATUS
{
	enum EFFECT_STATUS_TYPE eType;
	enum EFFECT_STATUS_KIND_TYPE eKindType;
	float fDelayTime;
	float fDelayTimeElv;
	int iParameter[6];
};

struct EFFECT_REQUIRE
{
	int iEquipType;
	int iItemKind;
	int64 iMotionType;
	int iStandCheck;
};

struct EffectInfo
{
//public:
	EffectInfo(int iID, enum EFFECT_TYPE eType) { this->m_iID = iID; this->m_eType = eType; }
	void SetRequireInfo(EFFECT_REQUIRE& RequireInfo) { this->m_RequireInfo = RequireInfo; }
	void SetStatusInfo(EFFECT_STATUS& StatusInfo) { this->m_StatusInfo = StatusInfo; }
	EFFECT_REQUIRE* GetRequireInfo() { return &this->m_RequireInfo; }
	EFFECT_STATUS* GetStatusInfo() { return &this->m_StatusInfo; }
	int GetID() { return m_iID; }
//protected:
	int m_iID;
	enum EFFECT_TYPE m_eType;
	//float m_fApplyTime;
	//float m_fApplyTimeElv;
	EFFECT_REQUIRE m_RequireInfo;
	EFFECT_STATUS m_StatusInfo;
};

class EffectManager : public Singleton<EffectManager>
{
public:
	EffectManager() { Init(); }
	EffectInfo* GetEffectInfo(int iID);
	void Init();
	void LoadLDTFile();
protected:
	stdext::hash_map<int, EffectInfo*> m_hmEffectList;
};

class EffectArchive; 

class Effect
{
public:
	Effect();
	Effect(EffectArchive* pkParent, EffectInfo* pkEffectInfo) { m_pkParent = pkParent; m_pkEffectInfo = pkEffectInfo; } 
	void SetUniqueKey(uint uiUniqueKey) { this->m_uiUniqueKey = uiUniqueKey; }
	uint GetUniqueKey() { return this->m_uiUniqueKey; }
	EffectInfo* GetEffectInfo() { return this->m_pkEffectInfo; }
	void ApplyEffectStatus();
	void RemoveEffectStatus();

protected:
	EffectInfo* m_pkEffectInfo;
	EffectArchive* m_pkParent;
	
	float fApplyTime;
	uint m_uiUniqueKey;
};

class Creature;

class EffectArchive
{
public:
	EffectArchive(Creature* pkCreature) { this->m_pkParent = pkCreature; }
	virtual void Update();
	void ApplyEffect(int iID);
	void CheckEffects();
	Creature* GetParent() { return m_pkParent; }
protected:
	std::set<Effect*> m_sEffectList;
	Creature* m_pkParent;
};