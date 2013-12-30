#include "stdafx.h"

EventManager* Singleton<EventManager>::m_pkInstance = NULL;

_MAPEVENT_INFO* EventManager::GetEvent(int iItemID)
{
	stdext::hash_map<int, _MAPEVENT_INFO*>::iterator iter = m_hmEventInfo.find(iItemID);
	if (iter == m_hmEventInfo.end())
		return NULL;
	return iter->second;
}

EVENTPORTAL_INFO* EventManager::GetEventPortal(int iEventID)
{
	stdext::hash_map<int, EVENTPORTAL_INFO*>::iterator iter = m_hmEventPortal.find(iEventID);
	if (iter == m_hmEventPortal.end())
		return NULL;
	return iter->second;
}

void EventManager::LoadLDTEvent()
{
	LDTFile ldt("MAPEVENT");
	void* h = ldt.BeginEnum();
	LDTRecord* r;
	while ((r = ldt.EnumRecord(h)) != NULL)
	{
		_MAPEVENT_INFO *pMapEvent = new _MAPEVENT_INFO();
		pMapEvent->Clear();
		pMapEvent->id = r->GetId();
		strncat(pMapEvent->name, r->GetString("_Name").c_str(), 16);
		pMapEvent->mapInfo.uiStageID = r->GetInt("_StageID");
		pMapEvent->mapInfo.uiMapGroupID = r->GetInt("_MapGroupID");
		pMapEvent->mapInfo.uiMapID = r->GetInt("_MapID");
		int iPosX, iPosY;
		StageManager::GetInstance()
			->GetStageInfo(pMapEvent->mapInfo.uiStageID)
			->GetMapGroup(pMapEvent->mapInfo.uiMapGroupID)
			->GetPos(pMapEvent->mapInfo.uiMapID, r->GetInt("_PosX"), r->GetInt("_PosY"), iPosX, iPosY);
		pMapEvent->posX = iPosX;
		pMapEvent->posY = iPosY;
		pMapEvent->rectTop = r->GetInt("_RectTop");
		pMapEvent->rectLeft = r->GetInt("_RectLeft");
		pMapEvent->rectRight = r->GetInt("_RectRight");
		pMapEvent->rectBottom = r->GetInt("_RectBottom");
		pMapEvent->eventType = r->GetInt("_EventType");
		pMapEvent->eventTypeID = r->GetInt("_EventTypeID");
		pMapEvent->eventSubType = r->GetInt("_EventSubType");
		pMapEvent->byDynamicCreate = r->GetInt("_Field_Dynamic_Create");
		m_hmEventInfo[pMapEvent->id] = pMapEvent;
		r->Free();
	}
	ldt.EndEnum(h);
}

void EventManager::LoadLDTEventPortal()
{
	LDTFile ldt("EVENTPORTAL");
	void* h = ldt.BeginEnum();
	LDTRecord* r;
	while ((r = ldt.EnumRecord(h)) != NULL)
	{
		EVENTPORTAL_INFO *pEventPortal = new EVENTPORTAL_INFO();
		pEventPortal->m_iID = r->GetId();
		pEventPortal->m_iStageID = r->GetInt("_StageID");
		pEventPortal->m_iMapGroupID = r->GetInt("_MapGroupID");
		pEventPortal->m_iMapID = r->GetInt("_MapID");
		MapGroupInfo* pMapGroup = StageManager::GetInstance()
			->GetStageInfo(pEventPortal->m_iStageID)
			->GetMapGroup(pEventPortal->m_iMapGroupID);
		if (pMapGroup == NULL) continue;
		int iPosX, iPosY;
		pMapGroup->GetPos(pEventPortal->m_iMapID, r->GetInt("_PosX"), r->GetInt("_PosY"), iPosX, iPosY);
		pEventPortal->m_fPosX = iPosX;
		pEventPortal->m_fPosY = iPosY;
		m_hmEventPortal[pEventPortal->m_iID] = pEventPortal;
		r->Free();
	}
	ldt.EndEnum(h);
}

void EventManager::Init()
{
	LoadLDTEvent();
	LoadLDTEventPortal();
}

void MapEvent::SetEventID(int iEventID)
{
	m_iEventID = iEventID;
	m_pEventInfo = EventManager::GetInstance()->GetEvent(iEventID);
}

float MapEvent::GetPosX()
{
	return m_pEventInfo->posX;
}

float MapEvent::GetPosY()
{
	return m_pEventInfo->posY;
}