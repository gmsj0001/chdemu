#pragma once

struct MotionInfo
{
public:
	float fTime;
};

enum MOTION_STATUS_TYPE
{
	MST_NULL = 0,
	MST_ACTIVIE = 1,
	MST_TOGGLE = 2
};

enum SKILL_TARGET_TYPE
{
	STT_NULL = 0,
	STT_ENEMY = 1,
	STT_NEUTRALITY = 2,
	STT_TEAM = 3,
	STT_BOTH = 4
};

struct SKILL_APPLY_EFFECT
{
	enum SKILL_TARGET_TYPE eTargetType;
	int iTargetNum;
	int iTargetSlvNum;
	float fDelayTime[4];
	int iSelfEffect[4];
	int iTargetEffect[4];
};
//TODO 这个结构体不全啊 startdelay和loopcheck没有

struct SKILL_ACTIVITY_VELOCITY
{
	float fVelocityX;
	float fVelocityY;
	float fTime;
	float fDelayTime;
};

struct MOTION_STATUS_STAT
{
	enum EFFECT_STATUS_KIND_TYPE eType;
	int iID;
	int iValue;
	int iValue2;
};



struct SKILL_COMBO
{
	bool bComboCheck;
	int iSelfComboID;
	int iApplyComboID;
	float fComboMinTime;
	float fComboMaxTime;
};

class MotionStatus
{
public:
	SKILL_APPLY_EFFECT m_ApplyEffectInfo;
	SKILL_ACTIVITY_VELOCITY m_ActivityVelocity;
};

struct SkillInfo
{
};

class SkillManager : public Singleton<SkillManager>
{
public:	
	SkillManager() { Init(); }
	void Init();
	MotionInfo* GetPlayerMotionInfo(int iMotionID)
	{
		stdext::hash_map<int, MotionInfo*>::iterator iter = m_mMotionInfo.find(iMotionID);
		return iter != m_mMotionInfo.end() ? iter->second : NULL;
	}
	MotionStatus* GetPlayerMotionStatus(int iMotionID)
	{
		stdext::hash_map<int, MotionStatus*>::iterator iter = m_mMotionStatus.find(iMotionID);
		return iter != m_mMotionStatus.end() ? iter->second : NULL;
	}
	MotionStatus* GetSkillMotionStatus(int iSkillID)
	{
		stdext::hash_map<int, MotionStatus*>::iterator iter = m_mSkillMotionStatus.find(iSkillID);
		return iter != m_mSkillMotionStatus.end() ? iter->second : NULL;
	}
	
private:
	void LoadPlayerMotionInfo();
	void LoadPlayerMotionStatus();
	void LoadSkillInfo();
	void LoadMobMotionInfo();
	void LoadMobMotionStatus();

	stdext::hash_map<int, MotionInfo*> m_mMotionInfo;
	stdext::hash_map<int, MotionStatus*> m_mMotionStatus;
	stdext::hash_map<int, MotionStatus*> m_mSkillMotionStatus;
	stdext::hash_map<int, MotionInfo*> m_mMobMotionInfo;
	stdext::hash_map<int, MotionStatus*> m_mMobMotionStatus;
};

struct SkillActivity
{
	SkillActivity(int iSkillID)
	{
		this->iSkillID = iSkillID;
		this->iLv = 0;
	}
	int iSkillID;
	int iLv;
	//SkillInfo pkSkillInfo;
};

class SkillArchive
{
public:
	SkillArchive(Player* pkParent)
	{
		m_pkParent = pkParent;
	}
	~SkillArchive();
	void Clear();
	void InitPlayer();
	void RecvLearn(int iSkillID);
	void RecvLvUp(int iSkillID);
	void RecvUsing(int iSkillID, int iDirection);
	void LoadDB();
	void SaveDB();
	stdext::hash_map<int, SkillActivity*>& GetSkills()
	{
		return m_hmSkills;
	}
protected:
	stdext::hash_map<int, SkillActivity*> m_hmSkills;
	Player* m_pkParent;
};