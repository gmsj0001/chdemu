#include "stdafx.h"

EffectManager* Singleton<EffectManager>::m_pkInstance = NULL;

void EffectManager::Init()
{
	LoadLDTFile();
}

EffectInfo* EffectManager::GetEffectInfo(int iID)
{
	stdext::hash_map<int, EffectInfo*>::iterator iter = this->m_hmEffectList.find(iID);
	if (iter == this->m_hmEffectList.end())
		return NULL;
	else
		return iter->second;
}

void EffectManager::LoadLDTFile()
{
	LDTFile ldt("EFFECT");
	void* h = ldt.BeginEnum();
	LDTRecord* row;
	while ((row = ldt.EnumRecord(h)) != NULL)
	{
		EFFECT_REQUIRE RequireInfo;
		EFFECT_STATUS StatusInfo;
		RequireInfo.iEquipType = row->GetInt("_RequireEquipType");
		RequireInfo.iItemKind = row->GetInt("_RequireItemKind");
		RequireInfo.iMotionType = Convert::Parse<int64>(row->GetString("_RequireAction"));
		RequireInfo.iStandCheck = row->GetInt("_RequireStand");
		StatusInfo.eType = (EFFECT_STATUS_TYPE)row->GetInt("_StatusEffect");
		StatusInfo.eKindType = (EFFECT_STATUS_KIND_TYPE)row->GetInt("_StatusEffectType");
		for (int i = 0; i < 6; ++i)
		{
			StatusInfo.iParameter[i] = row->GetInt("_EffectParameter" + Convert::ToString(i + 1));
		}
		StatusInfo.fDelayTime = row->GetInt("_EffectDelayTime") / 10.0f;
		StatusInfo.fDelayTimeElv = row->GetInt("_EffectDelayTimeElv") / 10.0f;
		EffectInfo* pEffect = new EffectInfo(row->GetId(), (EFFECT_TYPE)row->GetInt("_EffectType"));
		pEffect->SetRequireInfo(RequireInfo);
		pEffect->SetStatusInfo(StatusInfo);
		this->m_hmEffectList[pEffect->GetID()] = pEffect;
		row->Free();
	}
	ldt.EndEnum(h);
}

void Effect::ApplyEffectStatus()
{
	if (m_pkEffectInfo->m_StatusInfo.eType == EST_STAT)
	{
		m_pkParent->GetParent()->GetStatus()->
			ApplyStatusAdd((CREATURE_STATUS)m_pkEffectInfo->m_StatusInfo.eKindType, m_pkEffectInfo->m_StatusInfo.iParameter[0]);
		TO<Player*>(m_pkParent->GetParent())->BroadcastStatus();
	}
}

void Effect::RemoveEffectStatus()
{
	if (m_pkEffectInfo->m_StatusInfo.eType == EST_STAT)
	{
		m_pkParent->GetParent()->GetStatus()->
			ApplyStatusAdd((CREATURE_STATUS)m_pkEffectInfo->m_StatusInfo.eKindType, -m_pkEffectInfo->m_StatusInfo.iParameter[0]);
		TO<Player*>(m_pkParent->GetParent())->BroadcastStatus();
	}
}

void EffectArchive::ApplyEffect(int iID)
{
	if (iID == 0) return;
	EffectInfo* pkEffectInfo = EffectManager::GetInstance()->GetEffectInfo(iID);
	if (pkEffectInfo == NULL) return;
	Effect* pkEffect = new Effect(this, pkEffectInfo);
	if (pkEffectInfo->m_eType == ET_INSTANT)
	{
		pkEffect->ApplyEffectStatus();
	}
	else if (pkEffectInfo->m_eType == ET_KEEP)
	{
		m_sEffectList.insert(pkEffect);
		pkEffect->ApplyEffectStatus();
	}
}

void EffectArchive::CheckEffects()
{
	std::set<Effect*>::iterator iter;
	Creature *pkCreature = this->m_pkParent;
	for (iter = m_sEffectList.begin(); iter != m_sEffectList.end(); )
	{
		//目前仅测试一项动作 用以跑步和梯子速度
		std::set<Effect*>::iterator iter2 = iter;
		++iter;
		Effect *pEffect = *iter2;
		if (!(pkCreature->GetCurAction() & pEffect->GetEffectInfo()->GetRequireInfo()->iMotionType))
		{
			pEffect->RemoveEffectStatus();
			delete pEffect;
			m_sEffectList.erase(iter2);
		}
	}
}

void EffectArchive::Update()
{
	CheckEffects();
}