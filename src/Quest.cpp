#include "stdafx.h"

QuestManager* Singleton<QuestManager>::m_pkInstance = NULL;

void QuestManager::Init()
{
	LoadLDTQuestInfo();
}

void QuestManager::LoadLDTQuestInfo()
{
	LDTFile ldt("QUEST");
	void* h = ldt.BeginEnum();
	LDTRecord* r;
	while ((r = ldt.EnumRecord(h)) != NULL)
	{
		QuestInfo *pkInfo = new QuestInfo();
		pkInfo->iQuestID = r->GetId();
		pkInfo->iQuestType = r->GetInt("_Quest_Type");
		for (int i = 0; i < 5; ++i)
		{
			pkInfo->RequireInfo[i].iRequireType = r->GetInt("_Require" + Convert::ToString(i + 1) + "_Type");
			pkInfo->RequireInfo[i].iRequireID = r->GetInt("_Require" + Convert::ToString(i + 1) + "_ID");
			pkInfo->RequireInfo[i].iRequireValue1 = Convert::Parse<int>(r->GetString("_Require" + Convert::ToString(i + 1) + "_Value1"));
			pkInfo->RequireInfo[i].iRequireValue2 = Convert::Parse<int>(r->GetString("_Require" + Convert::ToString(i + 1) + "_Value2"));
		}
		m_hmQuestInfo[pkInfo->iQuestID] = pkInfo;
		r->Free();
	}
	ldt.EndEnum(h);
}

//i dont wanna implement quest yet but... if quest are not inited correctly the item system will be locked...fxxk
void QuestArchive::InitPlayer()
{
	//HACKFIX: try generate the quest list
	stdext::hash_map<int, QuestInfo*>& hmQuestInfo = QuestManager::GetInstance()->GetQuestInfos();
	for (stdext::hash_map<int, QuestInfo*>::iterator iter = hmQuestInfo.begin(); iter != hmQuestInfo.end(); ++iter)
	{
		QuestInfo* pkInfo = iter->second;
		if (pkInfo->iQuestType != 3)
			goto label_next;
		for (int i = 0; i < 5; ++i)
		{
			if (pkInfo->RequireInfo[i].iRequireType == 1 && pkInfo->RequireInfo[i].iRequireID == 3)
			{
				if (m_pkParent->GetStatus()->GetStatusValue((CREATURE_STATUS)3) < pkInfo->RequireInfo[i].iRequireValue1)	//
					goto label_next;	//等级不够
			}
			else if (pkInfo->RequireInfo[i].iRequireType == 4 && pkInfo->RequireInfo[i].iRequireID == 1)
				goto label_next;	//前置任务
		}
		m_sQuestList.insert(pkInfo->iQuestID);
label_next:
		;
	}
	m_pkParent->GetConnHandler()->SendQuestList();
}