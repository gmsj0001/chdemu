#include "stdafx.h"

StatusManager* Singleton<StatusManager>::m_pkInstance = NULL;

void StatusManager::Init()
{
	LoadLDTBaseStatus();
	//LoadLDTMobBaseStatus();
}

PLAYER_BASE_STATUS_INFO* StatusManager::GetBaseStatus(uint16 uiLv, uint8 uiClass)
{
	if (uiLv == 0) uiLv = 1;
	stdext::hash_map<int, PLAYER_BASE_STATUS_INFO*>::iterator iter = m_hmBaseStatus.find(uiClass * 1000 + uiLv);
	if (iter == m_hmBaseStatus.end())
		return NULL;
	else
		return iter->second;
}

MOB_BASE_STATUS_INFO* StatusManager::GetMobBaseStatus(int iID)
{
	stdext::hash_map<int, MOB_BASE_STATUS_INFO*>::iterator iter = m_hmMobBaseStatus.find(iID);
	if (iter == m_hmMobBaseStatus.end())
		return NULL;
	else
		return iter->second;
}

void StatusManager::LoadLDTBaseStatus()
{
	LDTFile ldtExp("EXPERIENCE");
	LDTFile ldt("STATUS_LEVEL");
	void* h = ldt.BeginEnum();
	LDTRecord* r;
	while ((r = ldt.EnumRecord(h)) != NULL)
	{
		uint8 uiClass = r->GetInt("_Class");
		uint16 uiLv = r->GetInt("_Lv");
		PLAYER_BASE_STATUS_INFO* pBaseStatus = new PLAYER_BASE_STATUS_INFO();
		LDTRecord* rExp = ldtExp.GetRecord(uiLv);
		pBaseStatus->iExpMin = Convert::Parse<int64>(rExp->GetString("_MinXp"));
		pBaseStatus->iExpMax = Convert::Parse<int64>(rExp->GetString("_MaxXp"));
		rExp->Free();
		pBaseStatus->iStatPoint = r->GetInt("_Stat_Point");
		pBaseStatus->iSkillPoint = r->GetInt("_Skill_Point");
		pBaseStatus->iBaseMHP = r->GetInt("_MOD_BaseMHP");
		pBaseStatus->iBaseMSP = r->GetInt("_MOD_BaseMSP");
		pBaseStatus->iBaseAtkProb = r->GetInt("_Base_Atk_Prob");
		pBaseStatus->iBaseDefProb = r->GetInt("_Base_Def_Prob");
		pBaseStatus->iBaseEleProb = r->GetInt("_Base_Ele_Prob");
		pBaseStatus->iBaseLukProb = r->GetInt("_Base_Luk_Prob");
		pBaseStatus->iBaseGuard = r->GetInt("_Base_Guard");
		pBaseStatus->iBaseAtkMinDam = r->GetInt("_Base_Atk_Min_Dam");
		pBaseStatus->iBaseAtkMaxDam = r->GetInt("_Base_Atk_Max_Dam");
		pBaseStatus->iBaseEleMinDam = r->GetInt("_Base_Ele_Min_Dam");
		pBaseStatus->iBaseEleMaxDam = r->GetInt("_Base_Ele_Max_Dam");
		pBaseStatus->iBaseSpeedX = r->GetInt("_Base_Speed_X");
		pBaseStatus->iBaseSpeedY = r->GetInt("_Base_Speed_Y");
		pBaseStatus->iBaseRopeSpeedX = r->GetInt("_Base_Rope_Speed_X");
		pBaseStatus->iBaseRopeSpeedY = r->GetInt("_Base_Rope_Speed_Y");
		pBaseStatus->iBaseWater = r->GetInt("_BASE_WATER");
		pBaseStatus->iBaseWind = r->GetInt("_BASE_WIND");
		pBaseStatus->iBaseFire = r->GetInt("_BASE_FIRE");
		pBaseStatus->iBaseEarth = r->GetInt("_BASE_EARTH");
		//pBaseStatus->iBaseDark = r->GetInt("_BASE_DARK");
		pBaseStatus->iBaseExpMod = r->GetInt("_BASE_EXP_MOD");
		pBaseStatus->iBaseItemDropMod = r->GetInt("_BASE_ITEM_DROP_MOD");
		pBaseStatus->iBaseDamageAbsolute = r->GetInt("_BASE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseReduceDamageAbsolute = r->GetInt("_BASE_REDUCE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseDefPenetration = r->GetInt("_BASE_DEF_PENETRATION");
		pBaseStatus->iBaseElePenetration = r->GetInt("_BASE_ELE_PENETRATION");
		pBaseStatus->iBaseJumpAtk = r->GetInt("_BASE_JUMP_ATK");
		pBaseStatus->iBaseNormal = r->GetInt("_BASE_NORMAL");
		pBaseStatus->iBaseHeavy = r->GetInt("_BASE_HEAVY");
		pBaseStatus->iBaseCriticalDamage = r->GetInt("_BASE_CRITICAL_DAMAGE");
		pBaseStatus->iBaseEleCriticalDamage = r->GetInt("_BASE_ELE_CRITICAL_DAMAGE");
		pBaseStatus->iBaseHealCriticalAugment = r->GetInt("_BASE_HEAL_CRITICAL_AUGMENT");
		pBaseStatus->iBaseHealCriticalProbability = r->GetInt("_BASE_HEAL_CRITICAL_PROBABILITY");
		pBaseStatus->iClassMHPFactor = r->GetInt("_CLASS_MHP_FACTOR");
		pBaseStatus->iClassMSPFactor = r->GetInt("_CLASS_MSP_FACTOR");
		pBaseStatus->iClassRecvHPFactor = r->GetInt("_CLASS_RECV_HP_FACTOR");
		pBaseStatus->iClassRecvSPFactor = r->GetInt("_CLASS_RECV_SP_FACTOR");
		pBaseStatus->iBasePhyHitRate = r->GetInt("_BASE_PHY_HIT_RATE");
		pBaseStatus->iBaseEleHitRate = r->GetInt("_BASE_ELE_HIT_RATE");
		pBaseStatus->iBasePhyDodgeRate = r->GetInt("_BASE_PHY_DODGE_RATE");
		pBaseStatus->iBaseEleDodgeRate = r->GetInt("_BASE_ELE_DODGE_RATE");
		pBaseStatus->iBaseBackDam = r->GetInt("_BASE_BACK_DAM");
		pBaseStatus->iBaseEleBackDam = r->GetInt("_BASE_ELE_BACK_DAM");
		pBaseStatus->iBaseAtkAddFactor = r->GetInt("_BASE_ATK_ADD_FACTOR");
		pBaseStatus->iBaseAtkRatioFactor = r->GetInt("_BASE_ATK_RATIO_FACTOR");
		pBaseStatus->iBaseEleAddFactor = r->GetInt("_BASE_ELE_ADD_FACTOR");
		pBaseStatus->iBaseEleRatioFactor = r->GetInt("_BASE_ELE_RATIO_FACTOR");
		pBaseStatus->iBaseAmor = r->GetInt("_BASE_AMOR");
		pBaseStatus->iBaseHealHPAdd = r->GetInt("_BASE_HEAL_HP_ADD");
		pBaseStatus->iBaseHealSPAdd = r->GetInt("_BASE_HEAL_SP_ADD");
		pBaseStatus->iBaseHealHPFactor = r->GetInt("_BASE_HEAL_HP_FACTOR");
		pBaseStatus->iBaseHealSPFactor = r->GetInt("_BASE_HEAL_SP_FACTOR");
		pBaseStatus->iBaseEleDamageAbsolute = r->GetInt("_BASE_ELE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseReduceDamageAbsolute = r->GetInt("_BASE_ELE_REDUCE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseWaterMinDam = r->GetInt("_BASE_WATER_MIN_DAM");
		pBaseStatus->iBaseWaterMaxDam = r->GetInt("_BASE_WATER_MAX_DAM");
		pBaseStatus->iBaseWindMinDam = r->GetInt("_BASE_WIND_MIN_DAM");
		pBaseStatus->iBaseWindMaxDam = r->GetInt("_BASE_WIND_MAX_DAM");
		pBaseStatus->iBaseFireMinDam = r->GetInt("_BASE_FIRE_MIN_DAM");
		pBaseStatus->iBaseFireMaxDam = r->GetInt("_BASE_FIRE_MAX_DAM");
		pBaseStatus->iBaseEarthMinDam = r->GetInt("_BASE_EARTH_MIN_DAM");
		pBaseStatus->iBaseEarthMaxDam = r->GetInt("_BASE_EARTH_MAX_DAM");
		//pBaseStatus->iBaseDarkMinDam = r->GetInt("_BASE_DARK_MIN_DAM");
		//pBaseStatus->iBaseDarkMaxDam = r->GetInt("_BASE_DARK_MAX_DAM");
		pBaseStatus->iBaseResWater = r->GetInt("_BASE_RES_WATER");
		pBaseStatus->iBaseResWind = r->GetInt("_BASE_RES_WIND");
		pBaseStatus->iBaseResFire = r->GetInt("_BASE_RES_FIRE");
		pBaseStatus->iBaseResEarth = r->GetInt("_BASE_RES_EARTH");
		//pBaseStatus->iBaseResDark = r->GetInt("_BASE_RES_DARK");
		pBaseStatus->iBaseDropSpeed = r->GetInt("_BASE_DROP_SPEED");
		pBaseStatus->iBaseMaxDropSpeed = r->GetInt("_BASE_MAX_DROP_SPEED");
		this->m_hmBaseStatus[uiClass * 1000 + uiLv] = pBaseStatus;
		r->Free();
	}
	ldt.EndEnum(h);
}

void StatusManager::LoadLDTMobBaseStatus()
{
	LDTFile ldt("MOBBASE");
	void* h = ldt.BeginEnum();
	LDTRecord* r;
	while ((r = ldt.EnumRecord(h)) != NULL)
	{
		int iID = r->GetId();
		MOB_BASE_STATUS_INFO* pBaseStatus = new MOB_BASE_STATUS_INFO();
		pBaseStatus->iBaseMHP = r->GetInt("_MOD_BaseMHP");
		pBaseStatus->iBaseMSP = r->GetInt("_MOD_BaseMSP");
		pBaseStatus->iBaseAtk = r->GetInt("_Base_Atk");
		pBaseStatus->iBaseDef = r->GetInt("_Base_Def");
		pBaseStatus->iBaseEle = r->GetInt("_Base_Ele");
		pBaseStatus->iBaseLuk = r->GetInt("_Base_Luk");
		pBaseStatus->iBaseGuard = r->GetInt("_Base_Guard");
		pBaseStatus->iBaseAtkMinDam = r->GetInt("_Base_Atk_Min_Dam");
		pBaseStatus->iBaseAtkMaxDam = r->GetInt("_Base_Atk_Max_Dam");
		pBaseStatus->iBaseEleMinDam = r->GetInt("_Base_Ele_Min_Dam");
		pBaseStatus->iBaseEleMaxDam = r->GetInt("_Base_Ele_Max_Dam");
		pBaseStatus->iBaseSpeedX = r->GetInt("_Base_Speed_X");
		pBaseStatus->iBaseSpeedY = r->GetInt("_Base_Speed_Y");
		pBaseStatus->iBaseWater = r->GetInt("_Base_Water");
		pBaseStatus->iBaseWind = r->GetInt("_Base_Wind");
		pBaseStatus->iBaseFire = r->GetInt("_Base_Fire");
		pBaseStatus->iBaseEarth = r->GetInt("_Base_Earth");
		//pBaseStatus->iBaseDark = r->GetInt("_Base_DARK");
		pBaseStatus->iBaseDamageAbsolute = r->GetInt("_BASE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseReduceDamageAbsolute = r->GetInt("_BASE_REDUCE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseDefPenetration = r->GetInt("_BASE_DEF_PENETRATION");
		pBaseStatus->iBaseElePenetration = r->GetInt("_BASE_ELE_PENETRATION");
		pBaseStatus->iBaseJumpAtk = r->GetInt("_BASE_JUMP_ATK");
		pBaseStatus->iBaseNormal = r->GetInt("_BASE_NORMAL");
		pBaseStatus->iBaseHeavy = r->GetInt("_BASE_HEAVY");
		pBaseStatus->iBaseCriticalDamage = r->GetInt("_BASE_CRITICAL_DAMAGE");
		pBaseStatus->iBaseEleCriticalDamage = r->GetInt("_BASE_ELE_CRITICAL_DAMAGE");
		pBaseStatus->iBaseHealCriticalAugment = r->GetInt("_BASE_HEAL_CRITICAL_AUGMENT");
		pBaseStatus->iBaseHealCriticalProbability = r->GetInt("_BASE_HEAL_CRITICAL_PROBABILITY");
		pBaseStatus->iClassMHPFactor = r->GetInt("_CLASS_MHP_FACTOR");
		pBaseStatus->iClassMSPFactor = r->GetInt("_CLASS_MSP_FACTOR");
		pBaseStatus->iClassRecvHPFactor = r->GetInt("_CLASS_RECV_HP_FACTOR");
		pBaseStatus->iClassRecvSPFactor = r->GetInt("_CLASS_RECV_SP_FACTOR");
		pBaseStatus->iBasePhyHitRate = r->GetInt("_BASE_PHY_HIT_RATE");
		pBaseStatus->iBaseEleHitRate = r->GetInt("_BASE_ELE_HIT_RATE");
		pBaseStatus->iBasePhyDodgeRate = r->GetInt("_BASE_PHY_DODGE_RATE");
		pBaseStatus->iBaseEleDodgeRate = r->GetInt("_BASE_ELE_DODGE_RATE");
		pBaseStatus->iBaseBackDam = r->GetInt("_BASE_BACK_DAM");
		pBaseStatus->iBaseEleBackDam = r->GetInt("_BASE_ELE_BACK_DAM");
		pBaseStatus->iBaseAtkAddFactor = r->GetInt("_BASE_ATK_ADD_FACTOR");
		pBaseStatus->iBaseAtkRatioFactor = r->GetInt("_BASE_ATK_RATIO_FACTOR");
		pBaseStatus->iBaseEleAddFactor = r->GetInt("_BASE_ELE_ADD_FACTOR");
		pBaseStatus->iBaseEleRatioFactor = r->GetInt("_BASE_ELE_RATIO_FACTOR");
		pBaseStatus->iBaseAmor = r->GetInt("_BASE_AMOR");
		pBaseStatus->iBaseHealHPAdd = r->GetInt("_BASE_HEAL_HP_ADD");
		pBaseStatus->iBaseHealSPAdd = r->GetInt("_BASE_HEAL_SP_ADD");
		pBaseStatus->iBaseHealHPFactor = r->GetInt("_BASE_HEAL_HP_FACTOR");
		pBaseStatus->iBaseHealSPFactor = r->GetInt("_BASE_HEAL_SP_FACTOR");
		pBaseStatus->iBaseEleDamageAbsolute = r->GetInt("_BASE_ELE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseReduceDamageAbsolute = r->GetInt("_BASE_ELE_REDUCE_DAMAGE_ABSOLUTE");
		pBaseStatus->iBaseWaterMinDam = r->GetInt("_BASE_WATER_MIN_DAM");
		pBaseStatus->iBaseWaterMaxDam = r->GetInt("_BASE_WATER_MAX_DAM");
		pBaseStatus->iBaseWindMinDam = r->GetInt("_BASE_WIND_MIN_DAM");
		pBaseStatus->iBaseWindMaxDam = r->GetInt("_BASE_WIND_MAX_DAM");
		pBaseStatus->iBaseFireMinDam = r->GetInt("_BASE_FIRE_MIN_DAM");
		pBaseStatus->iBaseFireMaxDam = r->GetInt("_BASE_FIRE_MAX_DAM");
		pBaseStatus->iBaseEarthMinDam = r->GetInt("_BASE_EARTH_MIN_DAM");
		pBaseStatus->iBaseEarthMaxDam = r->GetInt("_BASE_EARTH_MAX_DAM");
		//pBaseStatus->iBaseDarkMinDam = r->GetInt("_BASE_DARK_MIN_DAM");
		//pBaseStatus->iBaseDarkMaxDam = r->GetInt("_BASE_DARK_MAX_DAM");
		pBaseStatus->iBaseResWater = r->GetInt("_BASE_RES_WATER");
		pBaseStatus->iBaseResWind = r->GetInt("_BASE_RES_WIND");
		pBaseStatus->iBaseResFire = r->GetInt("_BASE_RES_FIRE");
		pBaseStatus->iBaseResEarth = r->GetInt("_BASE_RES_EARTH");
		//pBaseStatus->iBaseResDark = r->GetInt("_BASE_RES_DARK");
		pBaseStatus->iWaterDam = r->GetInt("_WATER_DAM");
		pBaseStatus->iWindDam = r->GetInt("_WIND_DAM");
		pBaseStatus->iFireDam = r->GetInt("_FIRE_DAM");
		pBaseStatus->iEarthDam = r->GetInt("_EARTH_DAM");
		//pBaseStatus->iDarkDam = r->GetInt("_DARK_DAM");
		pBaseStatus->iPhyImmune = r->GetInt("_PHY_IMMUNE");
		pBaseStatus->iEleImmune = r->GetInt("_ELE_IMMUNE");
		pBaseStatus->iWaterImmune = r->GetInt("_WATER_IMMUNE");
		pBaseStatus->iWindImmune = r->GetInt("_WIND_IMMUNE");
		pBaseStatus->iFireImmune = r->GetInt("_FIRE_IMMUNE");
		pBaseStatus->iEarthImmune = r->GetInt("_EARTH_IMMUNE");
		//pBaseStatus->iDarkImmune = r->GetInt("_DARK_IMMUNE");
		pBaseStatus->iBaseDropSpeed = r->GetInt("_BASE_DROP_SPEED");
		pBaseStatus->iBaseMaxDropSpeed = r->GetInt("_BASE_MAX_DROP_SPEED");
		this->m_hmMobBaseStatus[iID] = pBaseStatus;
		r->Free();
	}
	ldt.EndEnum(h);
}

void CreatureStatus::SetNull()
{
	m_strName = "";
	m_iLevel = 0;
	m_iHP = 0;
	m_iSP = 0;
}

void CreatureStatus::Init()
{
	this->SetNull();
}

CreatureStatus::CreatureStatus(Creature* pkParent)
{
	this->Init();
	m_pkParent = pkParent;
}

int CreatureStatus::GetStatusValue(enum CREATURE_STATUS StatusType)
{
	switch (StatusType)
	{
	case STAT_LV: // 3
		return m_iLevel;
	case STAT_HP: // 6
		return m_iHP;
	case STAT_SP: // 7
		return m_iSP;
	case STAT_CURR_ATK: // 12
		return m_Atk.iCurr;
	case STAT_BASE_ATK: // 13
		return m_Atk.iBase;
	case STAT_ADD_ATK: // 14
		return m_Atk.iAdd;
	case STAT_RATIO_ATK: // 15
		return m_Atk.iRatio;
	case STAT_CURR_LUK: // 16
		return m_Luk.iCurr;
	case STAT_BASE_LUK: // 17
		return m_Luk.iBase;
	case STAT_ADD_LUK: // 18
		return m_Luk.iAdd;
	case STAT_RATIO_LUK: // 19
		return m_Luk.iRatio;
	case STAT_CURR_ELE: // 20
		return m_Ele.iCurr;
	case STAT_BASE_ELE: // 21
		return m_Ele.iBase;
	case STAT_ADD_ELE: // 22
		return m_Ele.iAdd;
	case STAT_RATIO_ELE: // 23
		return m_Ele.iRatio;
	case STAT_CURR_DEF: // 24
		return m_Def.iCurr;
	case STAT_BASE_DEF: // 25
		return m_Def.iBase;
	case STAT_ADD_DEF: // 26
		return m_Def.iAdd;
	case STAT_RATIO_DEF: // 27
		return m_Def.iRatio;
	case STAT_CURR_MHP: // 28
		return m_MHP.iCurr;
	case STAT_BASE_MHP: // 29
		return m_MHP.iBase;
	case STAT_ADD_MHP: // 30
		return m_MHP.iAdd;
	case STAT_RATIO_MHP: // 31
		return m_MHP.iRatio;
	case STAT_CURR_MSP: // 32
		return m_MSP.iCurr;
	case STAT_BASE_MSP: // 33
		return m_MSP.iBase;
	case STAT_ADD_MSP: // 34
		return m_MSP.iAdd;
	case STAT_RATIO_MSP: // 35
		return m_MSP.iRatio;
	case STAT_CURR_WATER: // 36
		return m_Water.iCurr;
	case STAT_BASE_WATER: // 37
		return m_Water.iBase;
	case STAT_ADD_WATER: // 38
		return m_Water.iAdd;
	case STAT_RATIO_WATER: // 39
		return m_Water.iRatio;
	case STAT_CURR_WIND: // 40
		return m_Wind.iCurr;
	case STAT_BASE_WIND: // 41
		return m_Wind.iBase;
	case STAT_ADD_WIND: // 42
		return m_Wind.iAdd;
	case STAT_RATIO_WIND: // 43
		return m_Wind.iRatio;
	case STAT_CURR_FIRE: // 44
		return m_Fire.iCurr;
	case STAT_BASE_FIRE: // 45
		return m_Fire.iBase;
	case STAT_ADD_FIRE: // 46
		return m_Fire.iAdd;
	case STAT_RATIO_FIRE: // 47
		return m_Fire.iRatio;
	case STAT_CURR_EARTH: // 48
		return m_Earth.iCurr;
	case STAT_BASE_EARTH: // 49
		return m_Earth.iBase;
	case STAT_ADD_EARTH: // 50
		return m_Earth.iAdd;
	case STAT_RATIO_EARTH: // 51
		return m_Earth.iRatio;
	case STAT_CURR_RECV_HP: // 52
		return m_RecvHP.iCurr;
	case STAT_BASE_RECV_HP: // 53
		return m_RecvHP.iBase;
	case STAT_ADD_RECV_HP: // 54
		return m_RecvHP.iAdd;
	case STAT_RATIO_RECV_HP: // 55
		return m_RecvHP.iRatio;
	case STAT_CURR_RECV_SP: // 56
		return m_RecvSP.iCurr;
	case STAT_BASE_RECV_SP: // 57
		return m_RecvSP.iBase;
	case STAT_ADD_RECV_SP: // 58
		return m_RecvSP.iAdd;
	case STAT_RATIO_RECV_SP: // 59
		return m_RecvSP.iRatio;
	case STAT_CURR_HEAVY: // 60
		return m_Heavy.iCurr;
	case STAT_BASE_HEAVY: // 61
		return m_Heavy.iBase;
	case STAT_ADD_HEAVY: // 62
		return m_Heavy.iAdd;
	case STAT_RATIO_HEAVY: // 63
		return m_Heavy.iRatio;
	case STAT_CURR_NORMAL: // 64
		return m_Normal.iCurr;
	case STAT_BASE_NORMAL: // 65
		return m_Normal.iBase;
	case STAT_ADD_NORMAL: // 66
		return m_Normal.iAdd;
	case STAT_RATIO_NORMAL: // 67
		return m_Normal.iRatio;
	case STAT_CURR_WATER_RES: // 68
		return m_WaterRes.iCurr;
	case STAT_BASE_WATER_RES: // 69
		return m_WaterRes.iBase;
	case STAT_ADD_WATER_RES: // 70
		return m_WaterRes.iAdd;
	case STAT_RATIO_WATER_RES: // 71
		return m_WaterRes.iRatio;
	case STAT_CURR_WIND_RES: // 72
		return m_WindRes.iCurr;
	case STAT_BASE_WIND_RES: // 73
		return m_WindRes.iBase;
	case STAT_ADD_WIND_RES: // 74
		return m_WindRes.iAdd;
	case STAT_RATIO_WIND_RES: // 75
		return m_WindRes.iRatio;
	case STAT_CURR_FIRE_RES: // 76
		return m_FireRes.iCurr;
	case STAT_BASE_FIRE_RES: // 77
		return m_FireRes.iBase;
	case STAT_ADD_FIRE_RES: // 78
		return m_FireRes.iAdd;
	case STAT_RATIO_FIRE_RES: // 79
		return m_FireRes.iRatio;
	case STAT_CURR_EARTH_RES: // 80
		return m_EarthRes.iCurr;
	case STAT_BASE_EARTH_RES: // 81
		return m_EarthRes.iBase;
	case STAT_ADD_EARTH_RES: // 82
		return m_EarthRes.iAdd;
	case STAT_RATIO_EARTH_RES: // 83
		return m_EarthRes.iRatio;
	case STAT_CURR_GUARD: // 84
		return m_Guard.iCurr;
	case STAT_BASE_GUARD: // 85
		return m_Guard.iBase;
	case STAT_ADD_GUARD: // 86
		return m_Guard.iAdd;
	case STAT_RATIO_GUARD: // 87
		return m_Guard.iRatio;
	case STAT_CURR_ATK_MIN_DAM: // 88
		return m_AtkMinDam.iCurr;
	case STAT_BASE_ATK_MIN_DAM: // 89
		return m_AtkMinDam.iBase;
	case STAT_ADD_ATK_MIN_DAM: // 90
		return m_AtkMinDam.iAdd;
	case STAT_RATIO_ATK_MIN_DAM: // 91
		return m_AtkMinDam.iRatio;
	case STAT_CURR_ATK_MAX_DAM: // 92
		return m_AtkMaxDam.iCurr;
	case STAT_BASE_ATK_MAX_DAM: // 93
		return m_AtkMaxDam.iBase;
	case STAT_ADD_ATK_MAX_DAM: // 94
		return m_AtkMaxDam.iAdd;
	case STAT_RATIO_ATK_MAX_DAM: // 95
		return m_AtkMaxDam.iRatio;
	case STAT_CURR_ELE_MIN_DAM: // 96
		return m_EleMinDam.iCurr;
	case STAT_BASE_ELE_MIN_DAM: // 97
		return m_EleMinDam.iBase;
	case STAT_ADD_ELE_MIN_DAM: // 98
		return m_EleMinDam.iAdd;
	case STAT_RATIO_ELE_MIN_DAM: // 99
		return m_EleMinDam.iRatio;
	case STAT_CURR_ELE_MAX_DAM: // 100
		return m_EleMaxDam.iCurr;
	case STAT_BASE_ELE_MAX_DAM: // 101
		return m_EleMaxDam.iBase;
	case STAT_ADD_ELE_MAX_DAM: // 102
		return m_EleMaxDam.iAdd;
	case STAT_RATIO_ELE_MAX_DAM: // 103
		return m_EleMaxDam.iRatio;
	case STAT_CURR_SPEED_X: // 104
		return m_SpeedX.iCurr;
	case STAT_BASE_SPEED_X: // 105
		return m_SpeedX.iBase;
	case STAT_ADD_SPEED_X: // 106
		return m_SpeedX.iAdd;
	case STAT_RATIO_SPEED_X: // 107
		return m_SpeedX.iRatio;
	case STAT_CURR_SPEED_Y: // 108
		return m_SpeedY.iCurr;
	case STAT_BASE_SPEED_Y: // 109
		return m_SpeedY.iBase;
	case STAT_ADD_SPEED_Y: // 110
		return m_SpeedY.iAdd;
	case STAT_RATIO_SPEED_Y: // 111
		return m_SpeedY.iRatio;
	case STAT_CURR_ROPE_SPEED_X: // 112
		return m_RopeSpeedX.iCurr;
	case STAT_BASE_ROPE_SPEED_X: // 113
		return m_RopeSpeedX.iBase;
	case STAT_ADD_ROPE_SPEED_X: // 114
		return m_RopeSpeedX.iAdd;
	case STAT_RATIO_ROPE_SPEED_X: // 115
		return m_RopeSpeedX.iRatio;
	case STAT_CURR_ROPE_SPEED_Y: // 116
		return m_RopeSpeedY.iCurr;
	case STAT_BASE_ROPE_SPEED_Y: // 117
		return m_RopeSpeedY.iBase;
	case STAT_ADD_ROPE_SPEED_Y: // 118
		return m_RopeSpeedY.iAdd;
	case STAT_RATIO_ROPE_SPEED_Y: // 119
		return m_RopeSpeedY.iRatio;
	case STAT_CURR_DAMAGE_ABSOLUTE: // 138
		return m_DamageAbsolute.iCurr;
	case STAT_BASE_DAMAGE_ABSOLUTE: // 139
		return m_DamageAbsolute.iBase;
	case STAT_ADD_DAMAGE_ABSOLUTE: // 140
		return m_DamageAbsolute.iAdd;
	case STAT_RATIO_DAMAGE_ABSOLUTE: // 141
		return m_DamageAbsolute.iRatio;
	case STAT_CURR_REDUCE_DAMAGE_ABSOLUTE: // 142
		return m_ReduceDamageAbsolute.iCurr;
	case STAT_BASE_REDUCE_DAMAGE_ABSOLUTE: // 143
		return m_ReduceDamageAbsolute.iBase;
	case STAT_ADD_REDUCE_DAMAGE_ABSOLUTE: // 144
		return m_ReduceDamageAbsolute.iAdd;
	case STAT_RATIO_REDUCE_DAMAGE_ABSOLUTE: // 145
		return m_ReduceDamageAbsolute.iRatio;
	case STAT_CURR_DEF_PENETRATION: // 146
		return m_DefPenetration.iCurr;
	case STAT_BASE_DEF_PENETRATION: // 147
		return m_DefPenetration.iBase;
	case STAT_ADD_DEF_PENETRATION: // 148
		return m_DefPenetration.iAdd;
	case STAT_RATIO_DEF_PENETRATION: // 149
		return m_DefPenetration.iRatio;
	case STAT_CURR_JUMP_ATK: // 150
		return m_JumpAtk.iCurr;
	case STAT_BASE_JUMP_ATK: // 151
		return m_JumpAtk.iBase;
	case STAT_ADD_JUMP_ATK: // 152
		return m_JumpAtk.iAdd;
	case STAT_RATIO_JUMP_ATK: //153
		return m_JumpAtk.iRatio;
	case STAT_CURR_CRITICAL_DAMAGE: // 154
		return m_CriticalDamage.iCurr;
	case STAT_BASE_CRITICAL_DAMAGE: // 155
		return m_CriticalDamage.iBase;
	case STAT_ADD_CRITICAL_DAMAGE: // 156
		return m_CriticalDamage.iAdd;
	case STAT_RATIO_CRITICAL_DAMAGE: // 157
		return m_CriticalDamage.iRatio;
	case STAT_ADD_CRITICAL_PROBABILITY: // 158
		return m_CriticalProbability.iAdd;
	case STAT_RATIO_CRITICAL_PROBABILITY: // 159
		return m_CriticalProbability.iRatio;
	case STAT_CURR_ELE_CRITICAL_DAMAGE: // 160
		return m_EleCriticalDamage.iCurr;
	case STAT_BASE_ELE_CRITICAL_DAMAGE: // 161
		return m_EleCriticalDamage.iBase;
	case STAT_ADD_ELE_CRITICAL_DAMAGE: // 162
		return m_EleCriticalDamage.iAdd;
	case STAT_RATIO_ELE_CRITICAL_DAMAGE: // 163
		return m_EleCriticalDamage.iRatio;
	case STAT_ADD_ELE_CRITICAL_PROBABILITY: // 164
		return m_EleCriticalProbability.iAdd;
	case STAT_RATIO_ELE_CRITICAL_PROBABILITY: // 165
		return m_EleCriticalProbability.iRatio;
	case STAT_CURR_HEAL_CRITICAL_AUGMENT: // 166
		return m_HealCriticalAugment.iCurr;
	case STAT_BASE_HEAL_CRITICAL_AUGMENT: // 167
		return m_HealCriticalAugment.iBase;
	case STAT_ADD_HEAL_CRITICAL_AUGMENT: // 168
		return m_HealCriticalAugment.iAdd;
	case STAT_RATIO_HEAL_CRITICAL_AUGMENT: // 169
		return m_HealCriticalAugment.iRatio;
	case STAT_CURR_HEAL_CRITICAL_PROBABILITY: // 170
		return m_HealCriticalProbability.iCurr;
	case STAT_BASE_HEAL_CRITICAL_PROBABILITY: // 171
		return m_HealCriticalProbability.iBase;
	case STAT_ADD_HEAL_CRITICAL_PROBABILITY: // 172
		return m_HealCriticalProbability.iAdd;
	case STAT_RATIO_HEAL_CRITICAL_PROBABILITY: // 173
		return m_HealCriticalProbability.iRatio;
	case STAT_CURR_PHY_HIT_RATE: // 174
		return m_PhyHitRate.iCurr;
	case STAT_BASE_PHY_HIT_RATE: // 175
		return m_PhyHitRate.iBase;
	case STAT_ADD_PHY_HIT_RATE: // 176
		return m_PhyHitRate.iAdd;
	case STAT_RATIO_PHY_HIT_RATE: // 177
		return m_PhyHitRate.iRatio;
	case STAT_CURR_ELE_HIT_RATE: // 178
		return m_EleHitRate.iCurr;
	case STAT_BASE_ELE_HIT_RATE: // 179
		return m_EleHitRate.iBase;
	case STAT_ADD_ELE_HIT_RATE: // 180
		return m_EleHitRate.iAdd;
	case STAT_RATIO_ELE_HIT_RATE: // 181
		return m_EleHitRate.iRatio;
	case STAT_CURR_PHY_DODGE_RATE: // 182
		return m_PhyDodgeRate.iCurr;
	case STAT_BASE_PHY_DODGE_RATE: // 183
		return m_PhyDodgeRate.iBase;
	case STAT_ADD_PHY_DODGE_RATE: // 184
		return m_PhyDodgeRate.iAdd;
	case STAT_RATIO_PHY_DODGE_RATE: // 185
		return m_PhyDodgeRate.iRatio;
	case STAT_CURR_ELE_DODGE_RATE: // 186
		return m_EleDodgeRate.iCurr;
	case STAT_BASE_ELE_DODGE_RATE: // 187
		return m_EleDodgeRate.iBase;
	case STAT_ADD_ELE_DODGE_RATE: // 188
		return m_EleDodgeRate.iAdd;
	case STAT_RATIO_ELE_DODGE_RATE: // 189
		return m_EleDodgeRate.iRatio;
	case STAT_CURR_BACK_DAM: // 190
		return m_BackDam.iCurr;
	case STAT_BASE_BACK_DAM: // 191
		return m_BackDam.iBase;
	case STAT_ADD_BACK_DAM: // 192
		return m_BackDam.iAdd;
	case STAT_RATIO_BACK_DAM: // 193
		return m_BackDam.iRatio;
	case STAT_CURR_ELE_BACK_DAM: // 194
		return m_EleBackDam.iCurr;
	case STAT_BASE_ELE_BACK_DAM: // 195
		return m_EleBackDam.iBase;
	case STAT_ADD_ELE_BACK_DAM: // 196
		return m_EleBackDam.iAdd;
	case STAT_RATIO_ELE_BACK_DAM: // 197
		return m_EleBackDam.iRatio;
	case STAT_CURR_ATK_ADD_FACTOR: // 198
		return m_AtkAddFactor.iCurr;
	case STAT_BASE_ATK_ADD_FACTOR: // 199
		return m_AtkAddFactor.iBase;
	case STAT_ADD_ATK_ADD_FACTOR: // 200
		return m_AtkAddFactor.iAdd;
	case STAT_RATIO_ATK_ADD_FACTOR: // 201
		return m_AtkAddFactor.iRatio;
	case STAT_CURR_ATK_RATIO_FACTOR: // 202
		return m_AtkRatioFactor.iCurr;
	case STAT_BASE_ATK_RATIO_FACTOR: // 203
		return m_AtkRatioFactor.iBase;
	case STAT_ADD_ATK_RATIO_FACTOR: // 204
		return m_AtkRatioFactor.iAdd;
	case STAT_RATIO_ATK_RATIO_FACTOR: // 205
		return m_AtkRatioFactor.iRatio;
	case STAT_CURR_ELE_ADD_FACTOR: // 206
		return m_EleAddFactor.iCurr;
	case STAT_BASE_ELE_ADD_FACTOR: // 207
		return m_EleAddFactor.iBase;
	case STAT_ADD_ELE_ADD_FACTOR: // 208
		return m_EleAddFactor.iAdd;
	case STAT_RATIO_ELE_ADD_FACTOR: // 209
		return m_EleAddFactor.iRatio;
	case STAT_CURR_ELE_RATIO_FACTOR: // 210
		return m_EleRatioFactor.iCurr;
	case STAT_BASE_ELE_RATIO_FACTOR: // 211
		return m_EleRatioFactor.iBase;
	case STAT_ADD_ELE_RATIO_FACTOR: // 212
		return m_EleRatioFactor.iAdd;
	case STAT_RATIO_ELE_RATIO_FACTOR: // 213
		return m_EleRatioFactor.iRatio;
	case STAT_CURR_AMOR: // 214
		return m_Amor.iCurr;
	case STAT_BASE_AMOR: // 215
		return m_Amor.iBase;
	case STAT_ADD_AMOR: // 216
		return m_Amor.iAdd;
	case STAT_RATIO_AMOR: // 217
		return m_Amor.iRatio;
	case STAT_WATER_DAM: // 218
		return m_iWaterDam;
	case STAT_WIND_DAM: // 219
		return m_iWindDam;
	case STAT_FIRE_DAM: // 220
		return m_iFireDam;
	case STAT_EARTH_DAM: // 221
		return m_iEarthDam;
	case STAT_CURR_HEAL_HP_ADD: // 222
		return m_HealHPAdd.iCurr;
	case STAT_BASE_HEAL_HP_ADD: // 223
		return m_HealHPAdd.iBase;
	case STAT_ADD_HEAL_HP_ADD: // 224
		return m_HealHPAdd.iAdd;
	case STAT_RATIO_HEAL_HP_ADD: // 225
		return m_HealHPAdd.iRatio;
	case STAT_CURR_HEAL_SP_ADD: // 226
		return m_HealSPAdd.iCurr;
	case STAT_BASE_HEAL_SP_ADD: // 227
		return m_HealSPAdd.iBase;
	case STAT_ADD_HEAL_SP_ADD: // 228
		return m_HealSPAdd.iAdd;
	case STAT_RATIO_HEAL_SP_ADD: // 229
		return m_HealSPAdd.iRatio;
	case STAT_CURR_HEAL_HP_FACTOR: // 230
		return m_HealHPFactor.iCurr;
	case STAT_BASE_HEAL_HP_FACTOR: // 231
		return m_HealHPFactor.iBase;
	case STAT_ADD_HEAL_HP_FACTOR: // 232
		return m_HealHPFactor.iAdd;
	case STAT_RATIO_HEAL_HP_FACTOR: // 233
		return m_HealHPFactor.iRatio;
	case STAT_CURR_HEAL_SP_FACTOR: // 234
		return m_HealSPFactor.iCurr;
	case STAT_BASE_HEAL_SP_FACTOR: // 235
		return m_HealSPFactor.iBase;
	case STAT_ADD_HEAL_SP_FACTOR: // 236
		return m_HealSPFactor.iAdd;
	case STAT_RATIO_HEAL_SP_FACTOR: // 237
		return m_HealSPFactor.iRatio;
	case STAT_CURR_ELE_DAMAGE_ABSOLUTE: // 238
		return m_EleDamageAbsolute.iCurr;
	case STAT_BASE_ELE_DAMAGE_ABSOLUTE: // 239
		return m_EleDamageAbsolute.iBase;
	case STAT_ADD_ELE_DAMAGE_ABSOLUTE: // 240
		return m_EleDamageAbsolute.iAdd;
	case STAT_RATIO_ELE_DAMAGE_ABSOLUTE: // 241
		return m_EleDamageAbsolute.iRatio;
	case STAT_CURR_ELE_REDUCE_DAMAGE_ABSOLUTE: // 242
		return m_EleReduceDamageAbsolute.iCurr;
	case STAT_BASE_ELE_REDUCE_DAMAGE_ABSOLUTE: // 243
		return m_EleReduceDamageAbsolute.iBase;
	case STAT_ADD_ELE_REDUCE_DAMAGE_ABSOLUTE: // 244
		return m_EleReduceDamageAbsolute.iAdd;
	case STAT_RATIO_ELE_REDUCE_DAMAGE_ABSOLUTE: // 245
		return m_EleReduceDamageAbsolute.iRatio;
	case STAT_CURR_WATER_MIN_DAM: // 252
		return m_WaterMinDam.iCurr;
	case STAT_BASE_WATER_MIN_DAM: // 253
		return m_WaterMinDam.iBase;
	case STAT_ADD_WATER_MIN_DAM: // 254
		return m_WaterMinDam.iAdd;
	case STAT_RATIO_WATER_MIN_DAM: // 255
		return m_WaterMinDam.iRatio;
	case STAT_CURR_WATER_MAX_DAM: // 256
		return m_WaterMaxDam.iCurr;
	case STAT_BASE_WATER_MAX_DAM: // 257
		return m_WaterMaxDam.iBase;
	case STAT_ADD_WATER_MAX_DAM: // 258
		return m_WaterMaxDam.iAdd;
	case STAT_RATIO_WATER_MAX_DAM: // 259
		return m_WaterMaxDam.iRatio;
	case STAT_CURR_WIND_MIN_DAM: // 260
		return m_WindMinDam.iCurr;
	case STAT_BASE_WIND_MIN_DAM: // 261
		return m_WindMinDam.iBase;
	case STAT_ADD_WIND_MIN_DAM: // 262
		return m_WindMinDam.iAdd;
	case STAT_RATIO_WIND_MIN_DAM: // 263
		return m_WindMinDam.iRatio;
	case STAT_CURR_WIND_MAX_DAM: // 264
		return m_WindMaxDam.iCurr;
	case STAT_BASE_WIND_MAX_DAM: // 265
		return m_WindMaxDam.iBase;
	case STAT_ADD_WIND_MAX_DAM: // 266
		return m_WindMaxDam.iAdd;
	case STAT_RATIO_WIND_MAX_DAM: // 267
		return m_WindMaxDam.iRatio;
	case STAT_CURR_FIRE_MIN_DAM: // 268
		return m_FireMinDam.iCurr;
	case STAT_BASE_FIRE_MIN_DAM: // 269
		return m_FireMinDam.iBase;
	case STAT_ADD_FIRE_MIN_DAM: // 270
		return m_FireMinDam.iAdd;
	case STAT_RATIO_FIRE_MIN_DAM: // 271
		return m_FireMinDam.iRatio;
	case STAT_CURR_FIRE_MAX_DAM: // 272
		return m_FireMaxDam.iCurr;
	case STAT_BASE_FIRE_MAX_DAM: // 273
		return m_FireMaxDam.iBase;
	case STAT_ADD_FIRE_MAX_DAM: // 274
		return m_FireMaxDam.iAdd;
	case STAT_RATIO_FIRE_MAX_DAM: // 275
		return m_FireMaxDam.iRatio;
	case STAT_CURR_EARTH_MIN_DAM: // 276
		return m_EarthMinDam.iCurr;
	case STAT_BASE_EARTH_MIN_DAM: // 277
		return m_EarthMinDam.iBase;
	case STAT_ADD_EARTH_MIN_DAM: // 278
		return m_EarthMinDam.iAdd;
	case STAT_RATIO_EARTH_MIN_DAM: // 279
		return m_EarthMinDam.iRatio;
	case STAT_CURR_EARTH_MAX_DAM: // 280
		return m_EarthMaxDam.iCurr;
	case STAT_BASE_EARTH_MAX_DAM: // 281
		return m_EarthMaxDam.iBase;
	case STAT_ADD_EARTH_MAX_DAM: // 282
		return m_EarthMaxDam.iAdd;
	case STAT_RATIO_EARTH_MAX_DAM: // 283
		return m_EarthMaxDam.iRatio;
	case STAT_PHY_IMMUNE: // 284
		return m_iPhyImmune;
	case STAT_ELE_IMMUNE: // 285
		return m_iEleImmune;
	case STAT_WATER_IMMUNE: // 286
		return m_iWaterImmnue;
	case STAT_WIND_IMMUNE: // 287
		return m_iWindImmune;
	case STAT_FIRE_IMMUNE: // 288
		return m_iFireImmune;
	case STAT_EARTH_IMMUNE: // 289
		return m_iEarthImmune;
	case STAT_CURR_ELE_PENETRATION: // 290
		return m_ElePenetration.iCurr;
	case STAT_BASE_ELE_PENETRATION: // 291
		return m_ElePenetration.iBase;
	case STAT_ADD_ELE_PENETRATION: // 292
		return m_ElePenetration.iAdd;
	case STAT_RATIO_ELE_PENETRATION: // 293
		return m_ElePenetration.iRatio;
	case STAT_CURR_DROP_SPEED: // 370
		return m_DropSpeed.iCurr;
	case STAT_BASE_DROP_SPEED: // 371
		return m_DropSpeed.iBase;
	case STAT_ADD_DROP_SPEED: // 372	
		return m_DropSpeed.iAdd;
	case STAT_RATIO_DROP_SPEED: // 373
		return m_DropSpeed.iRatio;
	case STAT_CURR_MAX_DROP_SPEED: // 374
		return m_MaxDropSpeed.iCurr;
	case STAT_BASE_MAX_DROP_SPEED: // 375
		return m_MaxDropSpeed.iBase;
	case STAT_ADD_MAX_DROP_SPEED: // 376
		return m_MaxDropSpeed.iAdd;
	case STAT_RATIO_MAX_DROP_SPEED: // 377
		return m_MaxDropSpeed.iRatio;
	default:
		return 0;
	}
}

bool CreatureStatus::SetStatusValue(CREATURE_STATUS StatusType, int iValue)
{
	switch (StatusType)
	{
	case STAT_LV: // 3
		m_iLevel = iValue;
		return true;
	case STAT_HP: // 6
		m_iHP = iValue;
		return true;
	case STAT_SP: // 7
		m_iSP = iValue;
		return true;
	case STAT_BASE_ATK: // 13
		m_Atk.SetBase(iValue);
		return true;    
	case STAT_ADD_ATK: // 14
		m_Atk.SetAdd(iValue);
		return true;
	case STAT_RATIO_ATK: // 15
		m_Atk.SetRatio(iValue);
		return true;
	case STAT_BASE_LUK: // 17
		m_Luk.SetBase(iValue);
		return true;
	case STAT_ADD_LUK: // 18
		m_Luk.SetAdd(iValue);
		return true;
	case STAT_RATIO_LUK: // 19
		m_Luk.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE: // 21
		m_Ele.SetBase(iValue);
		return true;
	case STAT_ADD_ELE: // 22
		m_Ele.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE: // 23
		m_Ele.SetRatio(iValue);
		return true;
	case STAT_BASE_DEF: // 25
		m_Def.SetBase(iValue);
		return true;
	case STAT_ADD_DEF: // 26
		m_Def.SetAdd(iValue);
		return true;
	case STAT_RATIO_DEF: // 27
		m_Def.SetRatio(iValue);
		return true;
	case STAT_BASE_MHP: // 29
		m_MHP.SetBase(iValue);
		return true;
	case STAT_ADD_MHP: // 30
		m_MHP.SetAdd(iValue);
		return true;
	case STAT_RATIO_MHP: // 31
		m_MHP.SetRatio(iValue);
		return true;
	case STAT_BASE_MSP: // 33
		m_MSP.SetBase(iValue);
		return true;
	case STAT_ADD_MSP: // 34
		m_MSP.SetAdd(iValue);
		return true;
	case STAT_RATIO_MSP: // 35
		m_MSP.SetRatio(iValue);
		return true;
	case STAT_BASE_WATER: // 37
		m_Water.SetBase(iValue);
		return true;
	case STAT_ADD_WATER: // 38
		m_Water.SetAdd(iValue);
		return true;
	case STAT_RATIO_WATER: // 39
		m_Water.SetRatio(iValue);
		return true;
	case STAT_BASE_WIND: // 41
		m_Wind.SetBase(iValue);
		return true;
	case STAT_ADD_WIND: // 42
		m_Wind.SetAdd(iValue);
		return true;
	case STAT_RATIO_WIND: // 43
		m_Wind.SetRatio(iValue);
		return true;
	case STAT_BASE_FIRE: // 45
		m_Fire.SetBase(iValue);
		return true;
	case STAT_ADD_FIRE: // 46
		m_Fire.SetAdd(iValue);
		return true;
	case STAT_RATIO_FIRE: // 47
		m_Fire.SetRatio(iValue);
		return true;
	case STAT_BASE_EARTH: // 49
		m_Earth.SetBase(iValue);
		return true;
	case STAT_ADD_EARTH: // 50
		m_Earth.SetAdd(iValue);
		return true;
	case STAT_RATIO_EARTH: // 51
		m_Earth.SetRatio(iValue);
		return true;
	case STAT_BASE_RECV_HP: // 53
		m_RecvHP.SetBase(iValue);
		return true;
	case STAT_ADD_RECV_HP: // 54
		m_RecvHP.SetAdd(iValue);
		return true;
	case STAT_RATIO_RECV_HP: // 55
		m_RecvHP.SetRatio(iValue);
		return true;
	case STAT_BASE_RECV_SP: // 57
		m_RecvSP.SetBase(iValue);
		return true;
	case STAT_ADD_RECV_SP: // 58
		m_RecvSP.SetAdd(iValue);
		return true;
	case STAT_RATIO_RECV_SP: // 59
		m_RecvSP.SetRatio(iValue);
		return true;
	case STAT_BASE_HEAVY: // 61
		m_Heavy.SetBase(iValue);
		return true;
	case STAT_ADD_HEAVY: // 62
		m_Heavy.SetAdd(iValue);
		return true;
	case STAT_RATIO_HEAVY: // 63
		m_Heavy.SetRatio(iValue);
		return true;
	case STAT_BASE_NORMAL: // 65
		m_Normal.SetBase(iValue);
		return true;
	case STAT_ADD_NORMAL: // 66
		m_Normal.SetAdd(iValue);
		return true;
	case STAT_RATIO_NORMAL: // 67
		m_Normal.SetRatio(iValue);
		return true;
	case STAT_BASE_WATER_RES: // 69
		m_WaterRes.SetBase(iValue);
		return true;
	case STAT_ADD_WATER_RES: // 70
		m_WaterRes.SetAdd(iValue);
		return true;
	case STAT_RATIO_WATER_RES: // 71
		m_WaterRes.SetRatio(iValue);
		return true;
	case STAT_BASE_WIND_RES: // 73
		m_WindRes.SetBase(iValue);
		return true;
	case STAT_ADD_WIND_RES: // 74
		m_WindRes.SetAdd(iValue);
		return true;
	case STAT_RATIO_WIND_RES: // 75
		m_WindRes.SetRatio(iValue);
		return true;
	case STAT_BASE_FIRE_RES: // 77
		m_FireRes.SetBase(iValue);
		return true;
	case STAT_ADD_FIRE_RES: // 78
		m_FireRes.SetAdd(iValue);
		return true;
	case STAT_RATIO_FIRE_RES: // 79
		m_FireRes.SetRatio(iValue);
		return true;
	case STAT_BASE_EARTH_RES: // 81
		m_EarthRes.SetBase(iValue);
		return true;
	case STAT_ADD_EARTH_RES: // 82
		m_EarthRes.SetAdd(iValue);
		return true;
	case STAT_RATIO_EARTH_RES: // 83
		m_EarthRes.SetRatio(iValue);
		return true;
	case STAT_BASE_GUARD: // 85
		m_Guard.SetBase(iValue);
		return true;
	case STAT_ADD_GUARD: // 86
		m_Guard.SetAdd(iValue);
		return true;
	case STAT_RATIO_GUARD: // 87
		m_Guard.SetRatio(iValue);
		return true;
	case STAT_BASE_ATK_MIN_DAM: // 89
		m_AtkMinDam.SetBase(iValue);
		return true;
	case STAT_ADD_ATK_MIN_DAM: // 90
		m_AtkMinDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_ATK_MIN_DAM: // 91
		m_AtkMinDam.SetRatio(iValue);
		return true;
	case STAT_BASE_ATK_MAX_DAM: // 93
		m_AtkMaxDam.SetBase(iValue);
		return true;
	case STAT_ADD_ATK_MAX_DAM: // 94
		m_AtkMaxDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_ATK_MAX_DAM: // 95
		m_AtkMaxDam.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_MIN_DAM: // 97
		m_EleMinDam.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_MIN_DAM: // 98
		m_EleMinDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_MIN_DAM: // 99
		m_EleMinDam.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_MAX_DAM: // 101
		m_EleMaxDam.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_MAX_DAM: // 102
		m_EleMaxDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_MAX_DAM: // 103
		m_EleMaxDam.SetRatio(iValue);
		return true;
	case STAT_BASE_SPEED_X: // 105
		m_SpeedX.SetBase(iValue);
		{
			VELOCITY_STATUS VelocityStatus;
			m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
			VelocityStatus.fSpeedX = m_SpeedX.iCurr;
			m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_ADD_SPEED_X: // 106
		m_SpeedX.SetAdd(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fSpeedX = m_SpeedX.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_RATIO_SPEED_X: // 107
		m_SpeedX.SetRatio(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fSpeedX = m_SpeedX.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_BASE_SPEED_Y: // 109
		m_SpeedY.SetBase(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fSpeedY = m_SpeedY.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_ADD_SPEED_Y: // 110
		m_SpeedY.SetAdd(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fSpeedY = m_SpeedY.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_RATIO_SPEED_Y: // 111
		m_SpeedY.SetRatio(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fSpeedY = m_SpeedY.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_BASE_ROPE_SPEED_X: // 113
		m_RopeSpeedX.SetBase(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fRopeSpeedX = m_RopeSpeedX.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_ADD_ROPE_SPEED_X: // 114
		m_RopeSpeedX.SetAdd(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fRopeSpeedX = m_RopeSpeedX.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_RATIO_ROPE_SPEED_X: // 115
		m_RopeSpeedX.SetRatio(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fRopeSpeedX = m_RopeSpeedX.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_BASE_ROPE_SPEED_Y: // 117
		m_RopeSpeedY.SetBase(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fRopeSpeedY = m_RopeSpeedY.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_ADD_ROPE_SPEED_Y: // 118
		m_RopeSpeedY.SetAdd(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fRopeSpeedY = m_RopeSpeedY.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_RATIO_ROPE_SPEED_Y: // 119
		m_RopeSpeedY.SetRatio(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fRopeSpeedY = m_RopeSpeedY.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_BASE_DAMAGE_ABSOLUTE: // 139
		m_DamageAbsolute.SetBase(iValue);
		return true;
	case STAT_ADD_DAMAGE_ABSOLUTE: // 140
		m_DamageAbsolute.SetAdd(iValue);
		return true;
	case STAT_RATIO_DAMAGE_ABSOLUTE: // 141
		m_DamageAbsolute.SetRatio(iValue);
		return true;
	case STAT_BASE_REDUCE_DAMAGE_ABSOLUTE: // 143
		m_ReduceDamageAbsolute.SetBase(iValue);
		return true;
	case STAT_ADD_REDUCE_DAMAGE_ABSOLUTE: // 144
		m_ReduceDamageAbsolute.SetAdd(iValue);
		return true;
	case STAT_RATIO_REDUCE_DAMAGE_ABSOLUTE: // 145
		m_ReduceDamageAbsolute.SetRatio(iValue);
		return true;
	case STAT_BASE_DEF_PENETRATION: // 147
		m_DefPenetration.SetBase(iValue);
		return true;
	case STAT_ADD_DEF_PENETRATION: // 148
		m_DefPenetration.SetAdd(iValue);
		return true;
	case STAT_RATIO_DEF_PENETRATION: // 149
		m_DefPenetration.SetRatio(iValue);
		return true;
	case STAT_BASE_JUMP_ATK: // 151
		m_JumpAtk.SetBase(iValue);
		return true;
	case STAT_ADD_JUMP_ATK: // 152
		m_JumpAtk.SetAdd(iValue);
		return true;
	case STAT_RATIO_JUMP_ATK: //153
		m_JumpAtk.SetRatio(iValue);
		return true;
	case STAT_BASE_CRITICAL_DAMAGE: // 155
		m_CriticalDamage.SetBase(iValue);
		return true;
	case STAT_ADD_CRITICAL_DAMAGE: // 156
		m_CriticalDamage.SetAdd(iValue);
		return true;
	case STAT_RATIO_CRITICAL_DAMAGE: // 157
		m_CriticalDamage.SetRatio(iValue);
		return true;
	case STAT_ADD_CRITICAL_PROBABILITY: // 158
		m_CriticalProbability.SetAdd(iValue);
		return true;
	case STAT_RATIO_CRITICAL_PROBABILITY: // 159
		m_CriticalProbability.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_CRITICAL_DAMAGE: // 161
		m_EleCriticalDamage.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_CRITICAL_DAMAGE: // 162
		m_EleCriticalDamage.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_CRITICAL_DAMAGE: // 163
		m_EleCriticalDamage.SetRatio(iValue);
		return true;
	case STAT_ADD_ELE_CRITICAL_PROBABILITY: // 164
		m_EleCriticalProbability.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_CRITICAL_PROBABILITY: // 165
		m_EleCriticalProbability.SetRatio(iValue);
		return true;
	case STAT_BASE_HEAL_CRITICAL_AUGMENT: // 167
		m_HealCriticalAugment.SetBase(iValue);
		return true;
	case STAT_ADD_HEAL_CRITICAL_AUGMENT: // 168
		m_HealCriticalAugment.SetAdd(iValue);
		return true;
	case STAT_RATIO_HEAL_CRITICAL_AUGMENT: // 169
		m_HealCriticalAugment.SetRatio(iValue);
		return true;
	case STAT_BASE_HEAL_CRITICAL_PROBABILITY: // 171
		m_HealCriticalProbability.SetBase(iValue);
		return true;
	case STAT_ADD_HEAL_CRITICAL_PROBABILITY: // 172
		m_HealCriticalProbability.SetAdd(iValue);
		return true;
	case STAT_RATIO_HEAL_CRITICAL_PROBABILITY: // 173
		m_HealCriticalProbability.SetRatio(iValue);
		return true;
	case STAT_BASE_PHY_HIT_RATE: // 175
		m_PhyHitRate.SetBase(iValue);
		return true;
	case STAT_ADD_PHY_HIT_RATE: // 176
		m_PhyHitRate.SetAdd(iValue);
		return true;
	case STAT_RATIO_PHY_HIT_RATE: // 177
		m_PhyHitRate.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_HIT_RATE: // 179
		m_EleHitRate.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_HIT_RATE: // 180
		m_EleHitRate.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_HIT_RATE: // 181
		m_EleHitRate.SetRatio(iValue);
		return true;
	case STAT_BASE_PHY_DODGE_RATE: // 183
		m_PhyDodgeRate.SetBase(iValue);
		return true;
	case STAT_ADD_PHY_DODGE_RATE: // 184
		m_PhyDodgeRate.SetAdd(iValue);
		return true;
	case STAT_RATIO_PHY_DODGE_RATE: // 185
		m_PhyDodgeRate.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_DODGE_RATE: // 187
		m_EleDodgeRate.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_DODGE_RATE: // 188
		m_EleDodgeRate.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_DODGE_RATE: // 189
		m_EleDodgeRate.SetRatio(iValue);
		return true;
	case STAT_BASE_BACK_DAM: // 191
		m_BackDam.SetBase(iValue);
		return true;
	case STAT_ADD_BACK_DAM: // 192
		m_BackDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_BACK_DAM: // 193
		m_BackDam.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_BACK_DAM: // 195
		m_EleBackDam.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_BACK_DAM: // 196
		m_EleBackDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_BACK_DAM: // 197
		m_EleBackDam.SetRatio(iValue);
		return true;
	case STAT_BASE_ATK_ADD_FACTOR: // 199
		m_AtkAddFactor.SetBase(iValue);
		return true;
	case STAT_ADD_ATK_ADD_FACTOR: // 200
		m_AtkAddFactor.SetAdd(iValue);
		return true;
	case STAT_RATIO_ATK_ADD_FACTOR: // 201
		m_AtkAddFactor.SetRatio(iValue);
		return true;
	case STAT_BASE_ATK_RATIO_FACTOR: // 203
		m_AtkRatioFactor.SetBase(iValue);
		return true;
	case STAT_ADD_ATK_RATIO_FACTOR: // 204
		m_AtkRatioFactor.SetAdd(iValue);
		return true;
	case STAT_RATIO_ATK_RATIO_FACTOR: // 205
		m_AtkRatioFactor.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_ADD_FACTOR: // 207
		m_EleAddFactor.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_ADD_FACTOR: // 208
		m_EleAddFactor.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_ADD_FACTOR: // 209
		m_EleAddFactor.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_RATIO_FACTOR: // 211
		m_EleRatioFactor.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_RATIO_FACTOR: // 212
		m_EleRatioFactor.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_RATIO_FACTOR: // 213
		m_EleRatioFactor.SetRatio(iValue);
		return true;
	case STAT_BASE_AMOR: // 215
		m_Amor.SetBase(iValue);
		return true;
	case STAT_ADD_AMOR: // 216
		m_Amor.SetAdd(iValue);
		return true;
	case STAT_RATIO_AMOR: // 217
		m_Amor.SetRatio(iValue);
		return true;
	case STAT_WATER_DAM: // 218
		m_iWaterDam = iValue;
		return true;
	case STAT_WIND_DAM: // 219
		m_iWindDam = iValue;
		return true;
	case STAT_FIRE_DAM: // 220
		m_iFireDam = iValue;
		return true;
	case STAT_EARTH_DAM: // 221
		m_iEarthDam = iValue;
		return true;
	case STAT_BASE_HEAL_HP_ADD: // 223
		m_HealHPAdd.SetBase(iValue);
		return true;
	case STAT_ADD_HEAL_HP_ADD: // 224
		m_HealHPAdd.SetAdd(iValue);
		return true;
	case STAT_RATIO_HEAL_HP_ADD: // 225
		m_HealHPAdd.SetRatio(iValue);
		return true;
	case STAT_BASE_HEAL_SP_ADD: // 227
		m_HealSPAdd.SetBase(iValue);
		return true;
	case STAT_ADD_HEAL_SP_ADD: // 228
		m_HealSPAdd.SetAdd(iValue);
		return true;
	case STAT_RATIO_HEAL_SP_ADD: // 229
		m_HealSPAdd.SetRatio(iValue);
		return true;
	case STAT_BASE_HEAL_HP_FACTOR: // 231
		m_HealHPFactor.SetBase(iValue);
		return true;
	case STAT_ADD_HEAL_HP_FACTOR: // 232
		m_HealHPFactor.SetAdd(iValue);
		return true;
	case STAT_RATIO_HEAL_HP_FACTOR: // 233
		m_HealHPFactor.SetRatio(iValue);
		return true;
	case STAT_BASE_HEAL_SP_FACTOR: // 235
		m_HealSPFactor.SetBase(iValue);
		return true;
	case STAT_ADD_HEAL_SP_FACTOR: // 236
		m_HealSPFactor.SetAdd(iValue);
		return true;
	case STAT_RATIO_HEAL_SP_FACTOR: // 237
		m_HealSPFactor.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_DAMAGE_ABSOLUTE: // 239
		m_EleDamageAbsolute.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_DAMAGE_ABSOLUTE: // 240
		m_EleDamageAbsolute.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_DAMAGE_ABSOLUTE: // 241
		m_EleDamageAbsolute.SetRatio(iValue);
		return true;
	case STAT_BASE_ELE_REDUCE_DAMAGE_ABSOLUTE: // 243
		m_EleReduceDamageAbsolute.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_REDUCE_DAMAGE_ABSOLUTE: // 244
		m_EleReduceDamageAbsolute.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_REDUCE_DAMAGE_ABSOLUTE: // 245
		m_EleReduceDamageAbsolute.SetRatio(iValue);
		return true;
	case STAT_BASE_WATER_MIN_DAM: // 253
		m_WaterMinDam.SetBase(iValue);
		return true;
	case STAT_ADD_WATER_MIN_DAM: // 254
		m_WaterMinDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_WATER_MIN_DAM: // 255
		m_WaterMinDam.SetRatio(iValue);
		return true;
	case STAT_BASE_WATER_MAX_DAM: // 257
		m_WaterMaxDam.SetBase(iValue);
		return true;
	case STAT_ADD_WATER_MAX_DAM: // 258
		m_WaterMaxDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_WATER_MAX_DAM: // 259
		m_WaterMaxDam.SetRatio(iValue);
		return true;
	case STAT_BASE_WIND_MIN_DAM: // 261
		m_WindMinDam.SetBase(iValue);
		return true;
	case STAT_ADD_WIND_MIN_DAM: // 262
		m_WindMinDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_WIND_MIN_DAM: // 263
		m_WindMinDam.SetRatio(iValue);
		return true;
	case STAT_BASE_WIND_MAX_DAM: // 265
		m_WindMaxDam.SetBase(iValue);
		return true;
	case STAT_ADD_WIND_MAX_DAM: // 266
		m_WindMaxDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_WIND_MAX_DAM: // 267
		m_WindMaxDam.SetRatio(iValue);
		return true;
	case STAT_BASE_FIRE_MIN_DAM: // 269
		m_FireMinDam.SetBase(iValue);
		return true;
	case STAT_ADD_FIRE_MIN_DAM: // 270
		m_FireMinDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_FIRE_MIN_DAM: // 271
		m_FireMinDam.SetRatio(iValue);
		return true;
	case STAT_BASE_FIRE_MAX_DAM: // 273
		m_FireMaxDam.SetBase(iValue);
		return true;
	case STAT_ADD_FIRE_MAX_DAM: // 274
		m_FireMaxDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_FIRE_MAX_DAM: // 275
		m_FireMaxDam.SetRatio(iValue);
		return true;
	case STAT_BASE_EARTH_MIN_DAM: // 277
		m_EarthMinDam.SetBase(iValue);
		return true;
	case STAT_ADD_EARTH_MIN_DAM: // 278
		m_EarthMinDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_EARTH_MIN_DAM: // 279
		m_EarthMinDam.SetRatio(iValue);
		return true;
	case STAT_BASE_EARTH_MAX_DAM: // 281
		m_EarthMaxDam.SetBase(iValue);
		return true;
	case STAT_ADD_EARTH_MAX_DAM: // 282
		m_EarthMaxDam.SetAdd(iValue);
		return true;
	case STAT_RATIO_EARTH_MAX_DAM: // 283
		m_EarthMaxDam.SetRatio(iValue);
		return true;
	case STAT_PHY_IMMUNE: // 284
		m_iPhyImmune = iValue;
		return true;
	case STAT_ELE_IMMUNE: // 285
		m_iEleImmune = iValue;
		return true;
	case STAT_WATER_IMMUNE: // 286
		m_iWaterImmnue = iValue;
		return true;
	case STAT_WIND_IMMUNE: // 287
		m_iWindImmune = iValue;
		return true;
	case STAT_FIRE_IMMUNE: // 288
		m_iFireImmune = iValue;
		return true;
	case STAT_EARTH_IMMUNE: // 289
		m_iEarthImmune = iValue;
		return true;
	case STAT_BASE_ELE_PENETRATION: // 291
		m_ElePenetration.SetBase(iValue);
		return true;
	case STAT_ADD_ELE_PENETRATION: // 292
		m_ElePenetration.SetAdd(iValue);
		return true;
	case STAT_RATIO_ELE_PENETRATION: // 293
		m_ElePenetration.SetRatio(iValue);
		return true;
	case STAT_BASE_DROP_SPEED: // 371
		m_DropSpeed.SetBase(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fDropSpeed = m_DropSpeed.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_ADD_DROP_SPEED: // 372	
		m_DropSpeed.SetAdd(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fDropSpeed = m_DropSpeed.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_RATIO_DROP_SPEED: // 373
		m_DropSpeed.SetRatio(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fDropSpeed = m_DropSpeed.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_BASE_MAX_DROP_SPEED: // 375
		m_MaxDropSpeed.SetBase(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fMaxDropSpeed = m_MaxDropSpeed.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_ADD_MAX_DROP_SPEED: // 376
		m_MaxDropSpeed.SetAdd(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fMaxDropSpeed = m_MaxDropSpeed.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	case STAT_RATIO_MAX_DROP_SPEED: // 377
		m_MaxDropSpeed.SetRatio(iValue);
		{
		VELOCITY_STATUS VelocityStatus;
		m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
		VelocityStatus.fMaxDropSpeed = m_MaxDropSpeed.iCurr;
		m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
		}
		return true;
	default:
		return false;
	}
}

void CreatureStatus::LoadBaseStatus(BASE_STATUS_INFO* pBaseStatus)
{
	//不能直接赋值 因为要CalStatus
	m_MHP.SetBase(pBaseStatus->iBaseMHP);
	m_MSP.SetBase(pBaseStatus->iBaseMSP);
	//TODO：没有乘factor！这个时机还要讨论
	m_Guard.SetBase(pBaseStatus->iBaseGuard);
	m_AtkMinDam.SetBase(pBaseStatus->iBaseAtkMinDam);
	m_AtkMaxDam.SetBase(pBaseStatus->iBaseAtkMaxDam);
	m_EleMinDam.SetBase(pBaseStatus->iBaseEleMinDam);
	m_EleMaxDam.SetBase(pBaseStatus->iBaseEleMaxDam);
	m_SpeedX.SetBase(pBaseStatus->iBaseSpeedX);
	m_SpeedY.SetBase(pBaseStatus->iBaseSpeedY);
	m_Water.SetBase(pBaseStatus->iBaseWater);
	m_Wind.SetBase(pBaseStatus->iBaseWind);
	m_Fire.SetBase(pBaseStatus->iBaseFire);
	m_Earth.SetBase(pBaseStatus->iBaseEarth);
	m_Dark.SetBase(pBaseStatus->iBaseDark);
	m_DamageAbsolute.SetBase(pBaseStatus->iBaseDamageAbsolute);
	m_ReduceDamageAbsolute.SetBase(pBaseStatus->iBaseReduceDamageAbsolute);
	m_DefPenetration.SetBase(pBaseStatus->iBaseDefPenetration);
	m_ElePenetration.SetBase(pBaseStatus->iBaseElePenetration);
	m_JumpAtk.SetBase(pBaseStatus->iBaseJumpAtk);
	m_Normal.SetBase(pBaseStatus->iBaseNormal);
	m_Heavy.SetBase(pBaseStatus->iBaseHeavy);
	m_CriticalDamage.SetBase(pBaseStatus->iBaseCriticalDamage);
	m_EleCriticalDamage.SetBase(pBaseStatus->iBaseEleCriticalDamage);
	m_HealCriticalAugment.SetBase(pBaseStatus->iBaseHealCriticalAugment);
	m_HealCriticalProbability.SetBase(pBaseStatus->iBaseHealCriticalProbability);
	m_PhyHitRate.SetBase(pBaseStatus->iBasePhyHitRate);
	m_EleHitRate.SetBase(pBaseStatus->iBaseEleHitRate);
	m_PhyDodgeRate.SetBase(pBaseStatus->iBasePhyDodgeRate);
	m_EleDodgeRate.SetBase(pBaseStatus->iBaseEleDodgeRate);
	m_BackDam.SetBase(pBaseStatus->iBaseBackDam);
	m_EleBackDam.SetBase(pBaseStatus->iBaseEleBackDam);
	m_AtkAddFactor.SetBase(pBaseStatus->iBaseAtkAddFactor);
	m_AtkRatioFactor.SetBase(pBaseStatus->iBaseAtkRatioFactor);
	m_EleAddFactor.SetBase(pBaseStatus->iBaseEleAddFactor);
	m_EleRatioFactor.SetBase(pBaseStatus->iBaseEleRatioFactor);
	m_Amor.SetBase(pBaseStatus->iBaseAmor);
	m_HealHPAdd.SetBase(pBaseStatus->iBaseHealHPAdd);
	m_HealSPAdd.SetBase(pBaseStatus->iBaseHealSPAdd);
	m_HealHPFactor.SetBase(pBaseStatus->iBaseHealHPFactor);
	m_HealSPFactor.SetBase(pBaseStatus->iBaseHealSPFactor);
	m_EleDamageAbsolute.SetBase(pBaseStatus->iBaseEleDamageAbsolute);
	m_EleReduceDamageAbsolute.SetBase(pBaseStatus->iBaseEleReduceDamageAbsolute);
	m_WaterMinDam.SetBase(pBaseStatus->iBaseWaterMinDam);
	m_WaterMaxDam.SetBase(pBaseStatus->iBaseWaterMaxDam);
	m_WindMinDam.SetBase(pBaseStatus->iBaseWindMinDam);
	m_WindMaxDam.SetBase(pBaseStatus->iBaseWindMaxDam);
	m_FireMinDam.SetBase(pBaseStatus->iBaseFireMinDam);
	m_FireMaxDam.SetBase(pBaseStatus->iBaseFireMaxDam);
	m_EarthMinDam.SetBase(pBaseStatus->iBaseEarthMinDam);
	m_EarthMaxDam.SetBase(pBaseStatus->iBaseEarthMaxDam);
	m_DarkMinDam.SetBase(pBaseStatus->iBaseDarkMinDam);
	m_DarkMaxDam.SetBase(pBaseStatus->iBaseDarkMaxDam);
	m_WaterRes.SetBase(pBaseStatus->iBaseResWater);
	m_WindRes.SetBase(pBaseStatus->iBaseResWind);
	m_FireRes.SetBase(pBaseStatus->iBaseResFire);
	m_EarthRes.SetBase(pBaseStatus->iBaseResEarth);
	m_DarkRes.SetBase(pBaseStatus->iBaseResDark);
	m_DropSpeed.SetBase(pBaseStatus->iBaseDropSpeed);
	m_MaxDropSpeed.SetBase(pBaseStatus->iBaseMaxDropSpeed);
	VELOCITY_STATUS VelocityStatus;
	m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
	VelocityStatus.fSpeedX = m_SpeedX.iCurr;
	VelocityStatus.fSpeedY = m_SpeedY.iCurr;
	VelocityStatus.fDropSpeed = m_DropSpeed.iCurr;
	VelocityStatus.fMaxDropSpeed = m_MaxDropSpeed.iCurr;
	m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
}

PlayerStatus::PlayerStatus(Player* pkPlayer) : CreatureStatus(pkPlayer)
{
	m_iRequireLv = 200;	//TODO
	m_iProbEnchant = 0;	//TODO
}

void PlayerStatus::LoadBaseStatus(BASE_STATUS_INFO* pBaseStatus)
{
	CreatureStatus::LoadBaseStatus(pBaseStatus);
	PLAYER_BASE_STATUS_INFO* pPlayerBaseStatus = TO<PLAYER_BASE_STATUS_INFO*>(pBaseStatus);
	m_RopeSpeedX.SetBase(pPlayerBaseStatus->iBaseRopeSpeedX);
	m_RopeSpeedY.SetBase(pPlayerBaseStatus->iBaseRopeSpeedY);
	m_iExpMin = pPlayerBaseStatus->iExpMin;
	m_iExpMax = pPlayerBaseStatus->iExpMax;
	VELOCITY_STATUS VelocityStatus;
	m_pkParent->GetCoordPhysics()->GetVelocityStatus(VelocityStatus);
	VelocityStatus.fRopeSpeedX = m_RopeSpeedX.iCurr;
	VelocityStatus.fRopeSpeedY = m_RopeSpeedY.iCurr;
	m_pkParent->GetCoordPhysics()->SetVelocityStatus(VelocityStatus);
}

void PlayerStatus::LoadBaseStatus()
{
	PLAYER_BASE_STATUS_INFO* pBaseStatus = StatusManager::GetInstance()->GetBaseStatus(m_iLevel, m_iClass);
	LoadBaseStatus(pBaseStatus);
}

int PlayerStatus::GetStatusValue(CREATURE_STATUS StatusType)
{
	switch (StatusType)
	{
	case STAT_CLASS:
		return m_iClass;
	case STAT_GENDER:
		return m_Gender;
	case STAT_SKILL_POINT:
		return m_iSkillPoint;
	default:
		return CreatureStatus::GetStatusValue(StatusType);
	}
}

bool PlayerStatus::SetStatusValue(CREATURE_STATUS StatusType, int iValue)
{
	switch (StatusType)
	{
	case STAT_CLASS:
		m_iClass = iValue;
		return true;
	case STAT_GENDER:
		m_Gender = iValue;
		return true;
	case STAT_SKILL_POINT:
		m_iSkillPoint = iValue;
		return true;
		
	default:
		return CreatureStatus::SetStatusValue(StatusType, iValue);
	}
}

int64 PlayerStatus::GetStatusValueEx(CREATURE_STATUS_EX StatusType)
{
	switch (StatusType)
	{
	case STAT_EX_EXP:
		return m_iExp;
	case STAT_EX_EXP_MIN:
		return m_iExpMin;
	case STAT_EX_EXP_MAX:
		return m_iExpMax;
	case STAT_EX_ELY:
		return m_iMoney;
	case STAT_EX_STORAGE_ELY:
		return m_iMoneyStorage;
	default:
		return CreatureStatus::GetStatusValueEx(StatusType);
	}
}

bool PlayerStatus::SetStatusValueEx(CREATURE_STATUS_EX StatusType, int64 iValue)
{
	switch (StatusType)
	{
	case STAT_EX_EXP:
		m_iExp = iValue;
		return true;
	case STAT_EX_EXP_MIN:
		m_iExpMin = iValue;
		return true;
	case STAT_EX_EXP_MAX:
		m_iExpMax = iValue;
		return true;
	case STAT_EX_ELY:
		m_iMoney = iValue;
		return true;
	case STAT_EX_STORAGE_ELY:
		m_iMoneyStorage = iValue;
		return true;
	default:
		return CreatureStatus::SetStatusValueEx(StatusType, iValue);
	}
}