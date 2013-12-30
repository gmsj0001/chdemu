#include "stdafx.h"

SkillManager* Singleton<SkillManager>::m_pkInstance = NULL;

void SkillManager::Init()
{
	this->LoadPlayerMotionInfo();
	this->LoadPlayerMotionStatus();
	this->LoadSkillInfo();
	//this->LoadMobMotionData();
	//this->LoadMobMotionStatus();
}

void SkillManager::LoadPlayerMotionInfo()
{
	stdext::hash_map<std::string, MotionInfo*> mNameMotions;
	LDTFile ldt("MOTIONTYPE");
	void* h = ldt.BeginEnum();
	LDTRecord* row;
	while ((row = ldt.EnumRecord(h)) != NULL)
	{
		for (int i = 0; i < ldt.GetColSize(); ++i)
		{
			std::string sMotionName = row->GetString(i);
			stdext::hash_map<std::string, MotionInfo*>::iterator iter;
			iter = mNameMotions.find(sMotionName);
			MotionInfo* pMotionInfo;
			if (iter == mNameMotions.end())
			{
				pMotionInfo = NULL;
				SPFFileReader fs2;
				if (fs2.Open("DATA/ANITABLE/" + sMotionName + "_R.DRG"))
				{
					if (fs2.ReadInt() != 0)
					{
						__asm int 3;
						continue;
					}
					pMotionInfo = new MotionInfo();
					fs2.Skip(8);
					pMotionInfo->fTime = fs2.ReadFloat();
					mNameMotions.insert(std::pair<std::string, MotionInfo*>(sMotionName, pMotionInfo));
				}
			}
			else
			{
				pMotionInfo = iter->second;
			}
			if (pMotionInfo != NULL)
				this->m_mMotionInfo.insert(std::pair<int, MotionInfo*>(row->GetId() * 100 + (i + 1), pMotionInfo));
		}
		row->Free();
	}
	ldt.EndEnum(h);

}

void SkillManager::LoadPlayerMotionStatus()
{
	LDTFile ldt("MOTION_STATUS");
	void* h = ldt.BeginEnum();
	LDTRecord* row;
	while ((row = ldt.EnumRecord(h)) != NULL)
	{
		MotionStatus* pMotionStatus = new MotionStatus();
		pMotionStatus->m_ApplyEffectInfo.eTargetType = (SKILL_TARGET_TYPE)row->GetInt("_TargetType");
		pMotionStatus->m_ApplyEffectInfo.iTargetNum = row->GetInt("_TargetMaxCount");
		for (int i = 0; i < 4; ++i)
		{
			pMotionStatus->m_ApplyEffectInfo.fDelayTime[i] = row->GetInt("_Effect_Delay" + Convert::ToString(i + 1)) / 10.0f;
			pMotionStatus->m_ApplyEffectInfo.iSelfEffect[i] = row->GetInt("_Effect_Self" + Convert::ToString(i + 1));
			pMotionStatus->m_ApplyEffectInfo.iTargetEffect[i] = row->GetInt("_Effect_Target" + Convert::ToString(i + 1));
		}
		pMotionStatus->m_ActivityVelocity.fDelayTime = row->GetInt("_Speed_Delay") / 100.0f;	//这里是以厘秒为单位，和上面有所不同
		pMotionStatus->m_ActivityVelocity.fTime = row->GetInt("_Speed_Time") / 100.0f;
		pMotionStatus->m_ActivityVelocity.fVelocityX = row->GetInt("_Speed_X");
		pMotionStatus->m_ActivityVelocity.fVelocityY = row->GetInt("_Speed_Y");
		m_mMotionStatus[row->GetId()] = pMotionStatus;
		row->Free();
	}
	ldt.EndEnum(h);
}

void SkillManager::LoadSkillInfo()
{
	LDTFile ldt("SKILL_USE");	//现在的版本版本与Status相关的数据改在了SKILL_USE表中
	void* h = ldt.BeginEnum();
	LDTRecord* row;
	while ((row = ldt.EnumRecord(h)) != NULL)
	{
		MotionStatus* pMotionStatus = new MotionStatus();
		pMotionStatus->m_ApplyEffectInfo.eTargetType = (SKILL_TARGET_TYPE)row->GetInt("_TargetType");
		pMotionStatus->m_ApplyEffectInfo.iTargetNum = row->GetInt("_TargetMaxCount");
		for (int i = 0; i < 4; ++i)
		{
			pMotionStatus->m_ApplyEffectInfo.fDelayTime[i] = row->GetInt("_Effect_Delay" + Convert::ToString(i + 1)) / 10.0f;
			pMotionStatus->m_ApplyEffectInfo.iSelfEffect[i] = row->GetInt("_Effect_Self" + Convert::ToString(i + 1));
			pMotionStatus->m_ApplyEffectInfo.iTargetEffect[i] = row->GetInt("_Effect_Target" + Convert::ToString(i + 1));
		}
		pMotionStatus->m_ActivityVelocity.fDelayTime = row->GetInt("_Speed_Delay") / 100.0f;	//这里是以厘秒为单位，和上面有所不同
		pMotionStatus->m_ActivityVelocity.fTime = row->GetInt("_Speed_Time") / 100.0f;
		pMotionStatus->m_ActivityVelocity.fVelocityX = row->GetInt("_Speed_X");
		pMotionStatus->m_ActivityVelocity.fVelocityY = row->GetInt("_Speed_Y");
		m_mSkillMotionStatus[row->GetInt("_Skill_ID")] = pMotionStatus;
		row->Free();
	}
	ldt.EndEnum(h);
}

void SkillManager::LoadMobMotionInfo()
{
	LDTFile ldt("GLOBAL_MOB");
	void* h = ldt.BeginEnum();
	LDTRecord* row;
	while ((row = ldt.EnumRecord(h)) != NULL)
	{
		std::string sMotionName = row->GetString("_Sequence");
		SPFFileReader fs2;
		if (fs2.Open("DATA/ANITABLE/" + sMotionName + "_R.DRG"))
		{
			if (fs2.ReadInt() == 0)
			{
				__asm int 3;
				continue;
			}
			int motion_count = fs2.ReadInt();
			for (int i = 0; i < motion_count; ++i)
			{
				MotionInfo* pMotionInfo = new MotionInfo();
				int motion_type = fs2.ReadInt() + 1;
				fs2.Skip(4);
				pMotionInfo->fTime = fs2.ReadFloat();
				fs2.Skip(4);
				int timer_count = fs2.ReadInt();
				for (int j = 0; j < timer_count; ++j)
				{
					fs2.Skip(4 * 7);
				}
				this->m_mMobMotionInfo[(row->GetId() - 810000000) * 100 + motion_type] = pMotionInfo;
			}
		}
		row->Free();
	}
	ldt.EndEnum(h);

}

void SkillManager::LoadMobMotionStatus()
{
	LDTFile ldt("MOB_MOTIONSTATUS");
	void* h = ldt.BeginEnum();
	LDTRecord* row;
	while ((row = ldt.EnumRecord(h)) != NULL)
	{
		MotionStatus* pMotionStatus = new MotionStatus();
		pMotionStatus->m_ApplyEffectInfo.eTargetType = (SKILL_TARGET_TYPE)row->GetInt("_TargetType");
		for (int i = 0; i < 4; ++i)
		{
			pMotionStatus->m_ApplyEffectInfo.fDelayTime[i] = row->GetInt("_Effect_Delay" + (i + 1)) / 10.0f;
			pMotionStatus->m_ApplyEffectInfo.iSelfEffect[i] = row->GetInt("_Effect_Self" + (i + 1));
			pMotionStatus->m_ApplyEffectInfo.iTargetEffect[i] = row->GetInt("_Effect_Target" + (i + 1));
		}
		m_mMobMotionStatus[row->GetId()] = pMotionStatus;
		row->Free();
	}
	ldt.EndEnum(h);
}

SkillArchive::~SkillArchive()
{
	Clear();
}

void SkillArchive::Clear()
{
	for (stdext::hash_map<int, SkillActivity*>::iterator iter = m_hmSkills.begin(); iter != m_hmSkills.end(); ++iter)
	{
		SkillActivity* pkSkillActivity = iter->second;
		delete pkSkillActivity;
	}
	m_hmSkills.clear();
}

void SkillArchive::InitPlayer()
{
	m_pkParent->GetConnHandler()->SendSkillList();
}

void SkillArchive::RecvUsing(int iSkillID, int iDirection)
{
	m_pkParent->GetCoordPhysics()->SetDirection(iDirection);
	m_pkParent->GetCoordPhysics()->SetAction(0x10000000);	//skill
	//TODO 回头整SkillMotion的ActionTime
	//TMP: set activity velocity
	MotionStatus* pkMotionStatus = SkillManager::GetInstance()->GetSkillMotionStatus(iSkillID);
	if (pkMotionStatus->m_ActivityVelocity.fTime != 0.0f)
	{
		m_pkParent->GetCoordPhysics()->SetActivityVelocity(
			pkMotionStatus->m_ActivityVelocity.fDelayTime,
			pkMotionStatus->m_ActivityVelocity.fTime,
			m_pkParent->GetCoordPhysics()->GetDirection() == 0 ? -pkMotionStatus->m_ActivityVelocity.fVelocityX : pkMotionStatus->m_ActivityVelocity.fVelocityX,
			pkMotionStatus->m_ActivityVelocity.fVelocityY);
	}
	for (InRangeIter iter = m_pkParent->InRangeBegin(); iter != m_pkParent->InRangeEnd(); ++iter)
	{
		if ((*iter)->IsPlayer())
			TO<Player*>(*iter)->GetConnHandler()->SendSkillUsing(0, iSkillID, m_pkParent);
	}
}

void SkillArchive::RecvLearn(int iSkillID)
{
	SkillActivity* pkSkill = new SkillActivity(iSkillID);
	pkSkill->iLv = 0;	//TODO
	m_hmSkills[iSkillID] = pkSkill;
	m_pkParent->GetConnHandler()->SendSkillLearn(0, pkSkill);
}

void SkillArchive::RecvLvUp(int iSkillID)
{
	SkillActivity* pkSkill = m_hmSkills.find(iSkillID)->second;
	pkSkill->iLv += 1;
	m_pkParent->GetConnHandler()->SendSkillLvUp(0, pkSkill);
}

void SkillArchive::LoadDB()
{
	Clear();
	DBResult* result = DataBase::GetInstance()->Query("SELECT * FROM `skills` WHERE `char_id` = " + Convert::ToString(m_pkParent->GetCharID()));
	while (result->ReadRow())
	{
		SkillActivity* pkSkillActivity = new SkillActivity(result->GetInt("skill_id"));
		pkSkillActivity->iLv = result->GetInt("lv");
		m_hmSkills[pkSkillActivity->iSkillID] = pkSkillActivity;
	}
	result->Free();
}

void SkillArchive::SaveDB()
{
	DBResult* result = DataBase::GetInstance()->Query("DELETE FROM `skills` WHERE `char_id` = " + Convert::ToString(m_pkParent->GetCharID()));
	result->Free();
	for (stdext::hash_map<int, SkillActivity*>::iterator iter = m_hmSkills.begin(); iter != m_hmSkills.end(); ++iter)
	{
		SkillActivity* pkSkillActivity = iter->second;
		std::string sql = 
			"INSERT INTO `skills`("
			"`char_id`, `skill_id`, `lv`"
			") VALUES (" +
			Convert::ToString(m_pkParent->GetCharID()) + ", " +
			Convert::ToString(pkSkillActivity->iSkillID) + ", " +
			Convert::ToString(pkSkillActivity->iLv) +
			")";
		DBResult* r = DataBase::GetInstance()->Query(sql);
		r->Free();
	}
}