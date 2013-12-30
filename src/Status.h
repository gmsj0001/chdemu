#pragma once

enum CREATURE_STATUS
{
	//所有EffectStatus操作均为+而非替换
    STAT_NULL = 0,
    STAT_CLASS = 1,
    STAT_GENDER = 2,
    STAT_LV = 3,
    STAT_MONEY = 4,
    STAT_EXP = 5,
    STAT_HP = 6,
    STAT_SP = 7,

    STAT_CURR_FAME = 8,
    STAT_BASE_FAME = 9,
    STAT_ADD_FAME = 10,
    STAT_RATIO_FAME = 11,

	STAT_CURR_ATK = 12,
    STAT_BASE_ATK = 13,
    STAT_ADD_ATK = 14,
    STAT_RATIO_ATK = 15,
    STAT_CURR_LUK = 16,
    STAT_BASE_LUK = 17,
    STAT_ADD_LUK = 18,
    STAT_RATIO_LUK = 19,
    STAT_CURR_ELE = 20,
    STAT_BASE_ELE = 21,
    STAT_ADD_ELE = 22,
    STAT_RATIO_ELE = 23,
    STAT_CURR_DEF = 24,
    STAT_BASE_DEF = 25,
    STAT_ADD_DEF = 26,
    STAT_RATIO_DEF = 27,

	STAT_CURR_MHP = 28,
    STAT_BASE_MHP = 29,
    STAT_ADD_MHP = 30,
    STAT_RATIO_MHP = 31,
    STAT_CURR_MSP = 32,
    STAT_BASE_MSP = 33,
    STAT_ADD_MSP = 34,
    STAT_RATIO_MSP = 35,

	STAT_CURR_WATER = 36,
    STAT_BASE_WATER = 37,
    STAT_ADD_WATER = 38,
    STAT_RATIO_WATER = 39,
    STAT_CURR_WIND = 40,
    STAT_BASE_WIND = 41,
    STAT_ADD_WIND = 42,
    STAT_RATIO_WIND = 43,
    STAT_CURR_FIRE = 44,
    STAT_BASE_FIRE = 45,
    STAT_ADD_FIRE = 46,
    STAT_RATIO_FIRE = 47,
    STAT_CURR_EARTH = 48,
    STAT_BASE_EARTH = 49,
    STAT_ADD_EARTH = 50,
    STAT_RATIO_EARTH = 51,

	STAT_CURR_RECV_HP = 52,
    STAT_BASE_RECV_HP = 53,
    STAT_ADD_RECV_HP = 54,
    STAT_RATIO_RECV_HP = 55,
    STAT_CURR_RECV_SP = 56,
    STAT_BASE_RECV_SP = 57,
    STAT_ADD_RECV_SP = 58,
    STAT_RATIO_RECV_SP = 59,

	STAT_CURR_HEAVY = 60,	//武器最大
    STAT_BASE_HEAVY = 61,
    STAT_ADD_HEAVY = 62,
    STAT_RATIO_HEAVY = 63,
    STAT_CURR_NORMAL = 64,	//武器最小
    STAT_BASE_NORMAL = 65,
    STAT_ADD_NORMAL = 66,
    STAT_RATIO_NORMAL = 67,

	STAT_CURR_WATER_RES = 68,
	STAT_BASE_WATER_RES = 69,
	STAT_ADD_WATER_RES = 70,
	STAT_RATIO_WATER_RES = 71,
	STAT_CURR_WIND_RES = 72,
	STAT_BASE_WIND_RES = 73,
	STAT_ADD_WIND_RES = 74,
	STAT_RATIO_WIND_RES = 75,
	STAT_CURR_FIRE_RES = 76,
	STAT_BASE_FIRE_RES = 77,
	STAT_ADD_FIRE_RES = 78,
	STAT_RATIO_FIRE_RES = 79,
	STAT_CURR_EARTH_RES = 80,
	STAT_BASE_EARTH_RES = 81,
	STAT_ADD_EARTH_RES = 82,
	STAT_RATIO_EARTH_RES = 83,

	STAT_CURR_GUARD = 84,	//伤害减少
    STAT_BASE_GUARD = 85,
    STAT_ADD_GUARD = 86,
    STAT_RATIO_GUARD = 87,
    STAT_CURR_ATK_MIN_DAM = 88,
    STAT_BASE_ATK_MIN_DAM = 89,
    STAT_ADD_ATK_MIN_DAM = 90,
    STAT_RATIO_ATK_MIN_DAM = 91,
    STAT_CURR_ATK_MAX_DAM = 92,
    STAT_BASE_ATK_MAX_DAM = 93,
    STAT_ADD_ATK_MAX_DAM = 94,
    STAT_RATIO_ATK_MAX_DAM = 95,
    STAT_CURR_ELE_MIN_DAM = 96,
    STAT_BASE_ELE_MIN_DAM = 97,
    STAT_ADD_ELE_MIN_DAM = 98,
    STAT_RATIO_ELE_MIN_DAM = 99,
    STAT_CURR_ELE_MAX_DAM = 100,
    STAT_BASE_ELE_MAX_DAM = 101,
    STAT_ADD_ELE_MAX_DAM = 102,
    STAT_RATIO_ELE_MAX_DAM = 103,

	STAT_CURR_SPEED_X = 104,
    STAT_BASE_SPEED_X = 105,
    STAT_ADD_SPEED_X = 106,
    STAT_RATIO_SPEED_X = 107,
    STAT_CURR_SPEED_Y = 108,
    STAT_BASE_SPEED_Y = 109,
    STAT_ADD_SPEED_Y = 110,
    STAT_RATIO_SPEED_Y = 111,
    STAT_CURR_ROPE_SPEED_X = 112,
    STAT_BASE_ROPE_SPEED_X = 113,
    STAT_ADD_ROPE_SPEED_X = 114,
    STAT_RATIO_ROPE_SPEED_X = 115,
    STAT_CURR_ROPE_SPEED_Y = 116,
    STAT_BASE_ROPE_SPEED_Y = 117,
    STAT_ADD_ROPE_SPEED_Y = 118,
    STAT_RATIO_ROPE_SPEED_Y = 119,

    STAT_SKILL_POINT = 120,
	STAT_SKILL_POINT_ADD = 121,	//ADD BY LXF
    //STAT_SKILL_POINT_MOD = 122, 这个有可能是经验
	//124 125 经验值获取 124为+经验%的buff
	STAT_CURR_EXP_MOD = 122,
	STAT_BASE_EXP_MOD = 123,
	STAT_ADD_EXP_MOD = 124,
	STAT_RATIO_EXP_MOD = 125,
	//128 129 金钱获取 128为+金钱
	STAT_CURR_ELY_MOD = 126,
	STAT_BASE_ELY_MOD = 127,
	STAT_ADD_ELY_MOD = 128,
	STAT_RATIO_ELY_MOD = 129,
	//132 133 物品掉落概率
	STAT_CUR_ITEM_DROP_MOD = 130,
	STAT_BASE_ITEM_DROP_MOD = 131,
	STAT_ADD_ITEM_DROP_MOD = 132,
	STAT_RATIO_ITEM_DROP_MOD = 133,
	//136 137 多属性发生概率 //这几个都是第一个值是+%的 说明值本身是%
	STAT_CUR_RARE_DROP_MOD = 134,
	STAT_BASE_RARE_DROP_MOD = 135,
	STAT_ADD_RARE_DROP_MOD = 136,
	STAT_RATIO_RARE_DROP_MOD = 137,
	//140 141 物理伤害 phy absolute
	STAT_CURR_DAMAGE_ABSOLUTE = 138,
	STAT_BASE_DAMAGE_ABSOLUTE = 139,
	STAT_ADD_DAMAGE_ABSOLUTE = 140,
	STAT_RATIO_DAMAGE_ABSOLUTE = 141,
	//144 145 物理伤害减少
	STAT_CURR_REDUCE_DAMAGE_ABSOLUTE = 142,
	STAT_BASE_REDUCE_DAMAGE_ABSOLUTE = 143,
	STAT_ADD_REDUCE_DAMAGE_ABSOLUTE = 144,
	STAT_RATIO_REDUCE_DAMAGE_ABSOLUTE = 145,
	//148 149 目标防御力减小 148为% 莫非是DEF_PENETRATION
	STAT_CURR_DEF_PENETRATION = 146,
	STAT_BASE_DEF_PENETRATION = 147,
	STAT_ADD_DEF_PENETRATION = 148,
	STAT_RATIO_DEF_PENETRATION = 149,
	//152 153 跳跃攻击力 
	STAT_CURR_JUMP_ATK = 150,
	STAT_BASE_JUMP_ATK = 151,
	STAT_ADD_JUMP_ATK = 152,
	STAT_RATIO_JUMP_ATK =153,
	//156 157 暴击伤害 %类型
	STAT_CURR_CRITICAL_DAMAGE = 154,
	STAT_BASE_CRITICAL_DAMAGE = 155,
	STAT_ADD_CRITICAL_DAMAGE = 156,
	STAT_RATIO_CRITICAL_DAMAGE = 157,
	//158 159 暴击概率 只有+属性没有base和cur 推测是实时计算
	STAT_ADD_CRITICAL_PROBABILITY = 158,
	STAT_RATIO_CRITICAL_PROBABILITY = 159,
	//162 163 魔法暴击伤害
	STAT_CURR_ELE_CRITICAL_DAMAGE = 160,
	STAT_BASE_ELE_CRITICAL_DAMAGE = 161,
	STAT_ADD_ELE_CRITICAL_DAMAGE = 162,
	STAT_RATIO_ELE_CRITICAL_DAMAGE = 163,
	//164 165 魔法暴击概率
	STAT_ADD_ELE_CRITICAL_PROBABILITY = 164,
	STAT_RATIO_ELE_CRITICAL_PROBABILITY = 165,
	//168 169 暴击痊愈
	STAT_CURR_HEAL_CRITICAL_AUGMENT = 166,
	STAT_BASE_HEAL_CRITICAL_AUGMENT = 167,
	STAT_ADD_HEAL_CRITICAL_AUGMENT = 168,
	STAT_RATIO_HEAL_CRITICAL_AUGMENT = 169,
	//172 173 暴击痊愈概率
	STAT_CURR_HEAL_CRITICAL_PROBABILITY = 170,
	STAT_BASE_HEAL_CRITICAL_PROBABILITY = 171,
	STAT_ADD_HEAL_CRITICAL_PROBABILITY = 172,
	STAT_RATIO_HEAL_CRITICAL_PROBABILITY = 173,
	//176 177 物理命中
	STAT_CURR_PHY_HIT_RATE = 174,
	STAT_BASE_PHY_HIT_RATE = 175,
	STAT_ADD_PHY_HIT_RATE = 176,
	STAT_RATIO_PHY_HIT_RATE = 177,
	//180 181 魔法命中
	STAT_CURR_ELE_HIT_RATE = 178,
	STAT_BASE_ELE_HIT_RATE = 179,
	STAT_ADD_ELE_HIT_RATE = 180,
	STAT_RATIO_ELE_HIT_RATE = 181,
	//184 185 物理躲闪
	STAT_CURR_PHY_DODGE_RATE = 182,
	STAT_BASE_PHY_DODGE_RATE = 183,
	STAT_ADD_PHY_DODGE_RATE = 184,
	STAT_RATIO_PHY_DODGE_RATE = 185,
	//188 189 魔法躲闪
	STAT_CURR_ELE_DODGE_RATE = 186,
	STAT_BASE_ELE_DODGE_RATE = 187,
	STAT_ADD_ELE_DODGE_RATE = 188,
	STAT_RATIO_ELE_DODGE_RATE = 189,
	//192 193 后攻击
	STAT_CURR_BACK_DAM = 190,
	STAT_BASE_BACK_DAM = 191,
	STAT_ADD_BACK_DAM = 192,
	STAT_RATIO_BACK_DAM = 193,
	//196 197 魔法后攻击
	STAT_CURR_ELE_BACK_DAM = 194,
	STAT_BASE_ELE_BACK_DAM = 195,
	STAT_ADD_ELE_BACK_DAM = 196,
	STAT_RATIO_ELE_BACK_DAM = 197,
	//200 201 物理攻击等级
	STAT_CURR_ATK_ADD_FACTOR = 198,
	STAT_BASE_ATK_ADD_FACTOR = 199,
	STAT_ADD_ATK_ADD_FACTOR = 200,
	STAT_RATIO_ATK_ADD_FACTOR = 201,
	//204 205 物理攻击等级%
	STAT_CURR_ATK_RATIO_FACTOR = 202,
	STAT_BASE_ATK_RATIO_FACTOR = 203,
	STAT_ADD_ATK_RATIO_FACTOR = 204,
	STAT_RATIO_ATK_RATIO_FACTOR = 205,
	//208 209 魔法攻击等级
	STAT_CURR_ELE_ADD_FACTOR = 206,
	STAT_BASE_ELE_ADD_FACTOR = 207,
	STAT_ADD_ELE_ADD_FACTOR = 208,
	STAT_RATIO_ELE_ADD_FACTOR = 209,
	//212 213 魔法攻击等级%
	STAT_CURR_ELE_RATIO_FACTOR = 210,
	STAT_BASE_ELE_RATIO_FACTOR = 211,
	STAT_ADD_ELE_RATIO_FACTOR = 212,
	STAT_RATIO_ELE_RATIO_FACTOR = 213,
	//216 217 防御力 amor
	STAT_CURR_AMOR = 214,
	STAT_BASE_AMOR = 215,
	STAT_ADD_AMOR = 216,
	STAT_RATIO_AMOR = 217,
	//218 水属性伤害 物理
	STAT_WATER_DAM = 218,
	//219 风属性伤害 物理
	STAT_WIND_DAM = 219,
	//220 火属性伤害 物理
	STAT_FIRE_DAM = 220,
	//221 地属性伤害 物理
	STAT_EARTH_DAM = 221,
	//224 225 HP痊愈量
	STAT_CURR_HEAL_HP_ADD = 222,
	STAT_BASE_HEAL_HP_ADD = 223,
	STAT_ADD_HEAL_HP_ADD = 224,
	STAT_RATIO_HEAL_HP_ADD = 225,
	//228 229 SP痊愈量
	STAT_CURR_HEAL_SP_ADD = 226,
	STAT_BASE_HEAL_SP_ADD = 227,
	STAT_ADD_HEAL_SP_ADD = 228,
	STAT_RATIO_HEAL_SP_ADD = 229,
	//232 233 HP痊愈效果
	STAT_CURR_HEAL_HP_FACTOR = 230,
	STAT_BASE_HEAL_HP_FACTOR = 231,
	STAT_ADD_HEAL_HP_FACTOR = 232,
	STAT_RATIO_HEAL_HP_FACTOR = 233,
	//236 237 SP痊愈效果
	STAT_CURR_HEAL_SP_FACTOR = 234,
	STAT_BASE_HEAL_SP_FACTOR = 235,
	STAT_ADD_HEAL_SP_FACTOR = 236,
	STAT_RATIO_HEAL_SP_FACTOR = 237,
	//240 241 魔法伤害
	STAT_CURR_ELE_DAMAGE_ABSOLUTE = 238,
	STAT_BASE_ELE_DAMAGE_ABSOLUTE = 239,
	STAT_ADD_ELE_DAMAGE_ABSOLUTE = 240,
	STAT_RATIO_ELE_DAMAGE_ABSOLUTE = 241,
	//244 245 魔法伤害减小
	STAT_CURR_ELE_REDUCE_DAMAGE_ABSOLUTE = 242,
	STAT_BASE_ELE_REDUCE_DAMAGE_ABSOLUTE = 243,
	STAT_ADD_ELE_REDUCE_DAMAGE_ABSOLUTE = 244,
	STAT_RATIO_ELE_REDUCE_DAMAGE_ABSOLUTE = 245,
	//254 255 水属性最小
	STAT_CURR_WATER_MIN_DAM = 252,
	STAT_BASE_WATER_MIN_DAM = 253,
	STAT_ADD_WATER_MIN_DAM = 254,
	STAT_RATIO_WATER_MIN_DAM = 255,
	//258 259 水属性最大
	STAT_CURR_WATER_MAX_DAM = 256,
	STAT_BASE_WATER_MAX_DAM = 257,
	STAT_ADD_WATER_MAX_DAM = 258,
	STAT_RATIO_WATER_MAX_DAM = 259,
	//262 263 风属性最小
	STAT_CURR_WIND_MIN_DAM = 260,
	STAT_BASE_WIND_MIN_DAM = 261,
	STAT_ADD_WIND_MIN_DAM = 262,
	STAT_RATIO_WIND_MIN_DAM = 263,
	//266 267 风属性最大
	STAT_CURR_WIND_MAX_DAM = 264,
	STAT_BASE_WIND_MAX_DAM = 265,
	STAT_ADD_WIND_MAX_DAM = 266,
	STAT_RATIO_WIND_MAX_DAM = 267,
	//270 271 火属性最小
	STAT_CURR_FIRE_MIN_DAM = 268,
	STAT_BASE_FIRE_MIN_DAM = 269,
	STAT_ADD_FIRE_MIN_DAM = 270,
	STAT_RATIO_FIRE_MIN_DAM = 271,
	//274 275 火属性最大
	STAT_CURR_FIRE_MAX_DAM = 272,
	STAT_BASE_FIRE_MAX_DAM = 273,
	STAT_ADD_FIRE_MAX_DAM = 274,
	STAT_RATIO_FIRE_MAX_DAM = 275,
	//278 279 地属性最小
	STAT_CURR_EARTH_MIN_DAM = 276,
	STAT_BASE_EARTH_MIN_DAM = 277,
	STAT_ADD_EARTH_MIN_DAM = 278,
	STAT_RATIO_EARTH_MIN_DAM = 279,
	//282 283 地属性最大
	STAT_CURR_EARTH_MAX_DAM = 280,
	STAT_BASE_EARTH_MAX_DAM = 281,
	STAT_ADD_EARTH_MAX_DAM = 282,
	STAT_RATIO_EARTH_MAX_DAM = 283,
	//284 物理免疫
	STAT_PHY_IMMUNE = 284,
	//285 魔法免疫
	STAT_ELE_IMMUNE = 285,
	//286 水属性免疫
	STAT_WATER_IMMUNE = 286,
	//287 风属性免疫
	STAT_WIND_IMMUNE = 287,
	//288 火属性免疫
	STAT_FIRE_IMMUNE = 288,
	//289 地属性免疫
	STAT_EARTH_IMMUNE = 289,
	//292 293 目标魔法防御力%减小
	STAT_CURR_ELE_PENETRATION = 290,
	STAT_BASE_ELE_PENETRATION = 291,
	STAT_ADD_ELE_PENETRATION = 292,
	STAT_RATIO_ELE_PENETRATION = 293,
	//296 297 HP痊愈量
	//300 301 SP痊愈量
	//302 毒免疫
	//303 出血免疫
	//304 诅咒免疫
	//307 308 毒伤害
	//311 312 出血伤害
	//315 316 诅咒伤害
	//319 320 毒伤害% 应该是对方HP的%吧
	//323 324 出血伤害%
	//327 328 诅咒伤害%
	//331 332 毒抵抗
	//335 336 出血抵抗
	//339 340 诅咒抵抗

    STAT_EQUIPMENT_SLOT = 341,	//以4格为一个1
    STAT_CONSUMPTION_SLOT = 342,
    STAT_ETC_SLOT = 343,
    STAT_EVENT_SLOT = 344,
    STAT_HOUSING_SLOT = 345,
    STAT_PET_SLOT = 346,
    STAT_STORAGE_SLOT = 347,
	//SD搞毛线的随机包裹

	//351 力量概率
	//355 体力概率
	//359 魔法力概率
	//363 幸运概率

    STAT_REQUIRE_LV = 365,

	//367 提升技能等级
	//369 镶嵌成功率%
	//371 冰魔冰枪高度值修改？！
	STAT_CURR_DROP_SPEED = 370,
	STAT_BASE_DROP_SPEED = 371,
	STAT_ADD_DROP_SPEED = 372,	//目前的数据库里372比较多 暂且假设它是ADD
	STAT_RATIO_DROP_SPEED = 373,
	STAT_CURR_MAX_DROP_SPEED = 374,
	STAT_BASE_MAX_DROP_SPEED = 375,
	STAT_ADD_MAX_DROP_SPEED = 376,
	STAT_RATIO_MAX_DROP_SPEED = 377,

    STAT_GUILD_LV = 379,
	//380 家族经验
	STAT_GUILD_EXP = 380,
    STAT_ITEMMIX_EXP = 381,
    STAT_ITEMMIX_LV = 382,
    STAT_ITEMMIX_RARE = 383,	//属性添加
    STAT_ITEMMIX_SUCCESS = 384,
    STAT_ITEMMIX_OPT = 385,	//耐久强化
    STAT_PVP_POINT = 388,
	//389 pvp point %
    STAT_PVP_LV = 392,
	//393 无敌？
    STAT_LOVE_POINT = 395,
    STAT_LOVE_POINT_MAX = 396,
    STAT_LOVE_DAY = 401,
    STAT_LOVE_STATE = 402,
    STAT_EXPAND_SLOT = 403,
    STAT_LOVE_LEVEL = 404,
    STAT_MOB_RECALL_CNT = 405,
    STAT_RANKING_LEVEL = 397,
    STAT_RANKING_PVP = 398,
    STAT_RANKING_LOVE = 399,
    STAT_RANKING_GUILD = 400,
    STAT_EXPAND_STAT_AP = 412,
    STAT_EXPAND_STAT_CP = 413,
    STAT_EXPAND_STAT_DP = 414,
    STAT_EXPAND_STAT_CAP = 415,
    STAT_EXPAND_STAT_EP = 416,
    STAT_EXPAND_STAT_BP = 417,
    STAT_PER_HP = 442,
    STAT_PER_SP = 443,
    STAT_CLASS_SWITCH_REMAIN = 447,
    STAT_CP_POINT = 450,
    STAT_GUILD_CONTRIBUTION = 451,
    STAT_CURR_DARK = 468,
    STAT_BASE_DARK = 469,
    STAT_ADD_DARK = 470,
    STAT_RATIO_DARK = 471,
	MAX_CREATURE_STATUS = 512
};

enum CREATURE_STATUS_EX
{
    STAT_EX_NONE = 0x0,
    STAT_EX_EXP = 0x1,
    STAT_EX_EXP_MAX = 0x2,
    STAT_EX_EXP_MIN = 0x3,
    STAT_EX_ELY = 0x4,
    STAT_EX_STORAGE_ELY = 0x5,
    STAT_EX_PVP_GLORY = 0x6,
    MAX_STAT_EX = 0x7
};

struct STATUS_VALUE
{
	int iCurr;
	int iBase;
	int iAdd;
	int iRatio;
	STATUS_VALUE() { memset(this, 0, sizeof(*this)); }
	void CalValue() { iCurr = iBase * (100 + iRatio) / 100 + iAdd; }
	void SetBase(int iValue) { iBase = iValue; CalValue(); }
	void SetRatio(int iValue) { iRatio = iValue; CalValue(); }
	void SetAdd(int iValue) { iAdd = iValue; CalValue(); }
	int GetCurr() { return iCurr; }
	int GetBase() { return iBase; }
	int GetAdd() { return iAdd; }
	int GetRatio() { return iRatio; }
};

struct BASE_STATUS_INFO
{
	int iBaseMHP, iBaseMSP;
	int16 iBaseGuard;
	int16 iBaseAtkMinDam, iBaseAtkMaxDam, iBaseEleMinDam, iBaseEleMaxDam;
	int16 iBaseSpeedX, iBaseSpeedY;
	int16 iBaseWater, iBaseWind, iBaseFire, iBaseEarth, iBaseDark;
	int16 iBaseDamageAbsolute, iBaseReduceDamageAbsolute;
	int16 iBaseDefPenetration, iBaseElePenetration;
	int16 iBaseJumpAtk;
	int16 iBaseNormal, iBaseHeavy;
	int16 iBaseCriticalDamage, iBaseEleCriticalDamage;
	int16 iBaseHealCriticalAugment, iBaseHealCriticalProbability;
	int16 iClassMHPFactor, iClassMSPFactor, iClassRecvHPFactor, iClassRecvSPFactor;
	int16 iBasePhyHitRate, iBaseEleHitRate, iBasePhyDodgeRate, iBaseEleDodgeRate;
	int16 iBaseBackDam, iBaseEleBackDam;
	int16 iBaseAtkAddFactor, iBaseAtkRatioFactor, iBaseEleAddFactor, iBaseEleRatioFactor;
	int16 iBaseAmor;
	int16 iBaseHealHPAdd, iBaseHealSPAdd, iBaseHealHPFactor, iBaseHealSPFactor;
	int16 iBaseEleDamageAbsolute, iBaseEleReduceDamageAbsolute;
	int16 iBaseWaterMinDam, iBaseWaterMaxDam, iBaseWindMinDam, iBaseWindMaxDam, iBaseFireMinDam, iBaseFireMaxDam, iBaseEarthMinDam, iBaseEarthMaxDam, iBaseDarkMinDam, iBaseDarkMaxDam;
	int16 iBaseResWater, iBaseResWind, iBaseResFire, iBaseResEarth, iBaseResDark;
	int16 iBaseDropSpeed, iBaseMaxDropSpeed;
};

struct PLAYER_BASE_STATUS_INFO : BASE_STATUS_INFO
{
	int16 iStatPoint, iSkillPoint;
	int16 iBaseAtkProb, iBaseDefProb, iBaseEleProb, iBaseLukProb;
	int16 iBaseRopeSpeedX, iBaseRopeSpeedY;
	int16 iBaseExpMod, iBaseItemDropMod;
	int64 iExpMin, iExpMax;
};

struct MOB_BASE_STATUS_INFO : BASE_STATUS_INFO
{
	int16 iBaseAtk, iBaseDef, iBaseEle, iBaseLuk;
	int16 iWaterDam, iWindDam, iFireDam, iEarthDam, iDarkDam;
	int8 iPhyImmune, iEleImmune, iWaterImmune, iWindImmune, iFireImmune, iEarthImmune, iDarkImmune;
};

class StatusManager : public Singleton<StatusManager>
{
public:
	StatusManager() { Init(); }
	void Init();
	PLAYER_BASE_STATUS_INFO* GetBaseStatus(uint16 uiLv, uint8 uiClass);
	MOB_BASE_STATUS_INFO* GetMobBaseStatus(int iID);
protected:
	void LoadLDTBaseStatus();
	void LoadLDTMobBaseStatus();
	stdext::hash_map<int, PLAYER_BASE_STATUS_INFO*> m_hmBaseStatus;
	stdext::hash_map<int, MOB_BASE_STATUS_INFO*> m_hmMobBaseStatus;
};

class Creature;

class CreatureStatus
{
	friend class Handler;
public:
	void Init();
	CreatureStatus(Creature* pkCreature);
	virtual void SetNull(void);
	virtual void SetName(std::string strName) { this->m_strName = strName; }
	virtual std::string GetName() { return m_strName; }
	virtual int64 GetStatusValueEx(enum CREATURE_STATUS_EX) { return 0; }
	virtual bool SetStatusValueEx(enum CREATURE_STATUS_EX StatusType, int64 uiValue) { return false; }
	virtual int GetStatusValue(enum CREATURE_STATUS);
	virtual bool SetStatusValue(enum CREATURE_STATUS StatusType, int iValue);
	virtual bool ApplyStatusAdd(enum CREATURE_STATUS StatusType, int iValueAdd)
	{
		return SetStatusValue(StatusType, GetStatusValue(StatusType) + iValueAdd);
	}
	virtual void LoadBaseStatus(BASE_STATUS_INFO* pBaseStatus);

protected:
	Creature *m_pkParent;
	std::string m_strName;
	uint16 m_iLevel;
	int m_iHP;
	int m_iSP;
	STATUS_VALUE m_Atk, m_Luk, m_Ele, m_Def;
	STATUS_VALUE m_MHP, m_MSP;
	STATUS_VALUE m_Water, m_Wind, m_Fire, m_Earth, m_Dark;
	STATUS_VALUE m_RecvHP, m_RecvSP;
	STATUS_VALUE m_Heavy, m_Normal;
	STATUS_VALUE m_Guard;
	STATUS_VALUE m_AtkMinDam, m_AtkMaxDam, m_EleMinDam, m_EleMaxDam;
	STATUS_VALUE m_SpeedX, m_SpeedY, m_RopeSpeedX, m_RopeSpeedY;
	STATUS_VALUE m_WaterRes, m_WindRes, m_FireRes, m_EarthRes, m_DarkRes;
	STATUS_VALUE m_DamageAbsolute, m_ReduceDamageAbsolute;
	STATUS_VALUE m_DefPenetration;
	STATUS_VALUE m_JumpAtk;
	STATUS_VALUE m_CriticalDamage, m_CriticalProbability, m_EleCriticalDamage, m_EleCriticalProbability;
	STATUS_VALUE m_HealCriticalAugment, m_HealCriticalProbability;
	STATUS_VALUE m_PhyHitRate, m_EleHitRate, m_PhyDodgeRate, m_EleDodgeRate;
	STATUS_VALUE m_BackDam, m_EleBackDam;
	STATUS_VALUE m_AtkAddFactor, m_AtkRatioFactor, m_EleAddFactor, m_EleRatioFactor;
	STATUS_VALUE m_Amor;
	int16 m_iWaterDam;
	int16 m_iWindDam;
	int16 m_iFireDam;
	int16 m_iEarthDam;
	STATUS_VALUE m_HealHPAdd, m_HealSPAdd, m_HealHPFactor, m_HealSPFactor;
	STATUS_VALUE m_EleDamageAbsolute, m_EleReduceDamageAbsolute;
	STATUS_VALUE m_WaterMinDam, m_WaterMaxDam, m_WindMinDam, m_WindMaxDam, m_FireMinDam, m_FireMaxDam, m_EarthMinDam, m_EarthMaxDam, m_DarkMinDam, m_DarkMaxDam;
	int8 m_iPhyImmune;
	int8 m_iEleImmune;
	int8 m_iWaterImmnue;
	int8 m_iWindImmune;
	int8 m_iFireImmune;
	int8 m_iEarthImmune;
	STATUS_VALUE m_ElePenetration;
	STATUS_VALUE m_DropSpeed, m_MaxDropSpeed;
};

class PlayerStatus : public CreatureStatus
{
	friend class Handler;
public:
	PlayerStatus(Player* pkPlayer);
	int GetClassID() { return m_iClass; }
	void SetClassID(int iClass) { m_iClass = iClass; }
	virtual int64 GetStatusValueEx(enum CREATURE_STATUS_EX);
	virtual bool SetStatusValueEx(enum CREATURE_STATUS_EX StatusType, int64 uiValue);
	virtual int GetStatusValue(enum CREATURE_STATUS);
	virtual bool SetStatusValue(enum CREATURE_STATUS StatusType, int iValue);
	virtual void LoadBaseStatus(BASE_STATUS_INFO* pBaseStatus);
	void LoadBaseStatus();
protected:
	int8 m_iClass;
	int8 m_Gender;
	int64 m_iMoney;
	int64 m_iMoneyStorage;
	int64 m_iExp;
	int64 m_iExpMin;
	int64 m_iExpMax;
	int m_iUserID;
	STATUS_VALUE m_Fame;
	int16 m_iRequireLv;
	int8 m_iGuildLv;
	int m_iItemMixExp;
	int8 m_iItemMixLv;
	int64 m_iPvpPoint;
	int8 m_iPvpLv;
	int m_iLovePoint;
	int m_iLovePointMax;
	int m_iAp, m_iCp, m_iDp, m_iCap, m_iEp, m_iBp;
	int16 m_iSkillPoint;
	int16 m_iProbEnchant;

	int8 m_iEquipmentSlot, m_iConsumptionSlot, m_iEtcSlot, m_iEventSlot, m_iHousingSlot, m_iPetSlot, m_iStorageSlot;
};