#include "stdafx.h"

InRangeIter& InRangeIter::operator ++ ()
{
	_CheckIncMapCell();
	++(*(_MyBase*)this);
	return *this;
}

bool InRangeIter::operator ==(const InRangeIter& _Right)
{
	_CheckIncMapCell();
	if (m_iMapCellIdx != _Right.m_iMapCellIdx)
		return false;
	else
		return *(_MyBase*)this == *(_MyBase*)&_Right;
}
bool InRangeIter::operator !=(const InRangeIter &_Right)
{
	return !(*this == _Right);
}

void InRangeIter::_CheckIncMapCell()
{
	while (*(_MyBase*)this == m_pkMapCell->m_vInRangeMapCells[m_iMapCellIdx]->m_sObjects.end())
	{
		++m_iMapCellIdx;
		if (m_iMapCellIdx == m_pkMapCell->m_vInRangeMapCells.size())
			return;
		*(_MyBase*)this = m_pkMapCell->m_vInRangeMapCells[m_iMapCellIdx]->m_sObjects.begin();
	}
}

void MapCell::InsertObject(GameObject* pkObject)
{
	m_sObjects.insert(pkObject);
	pkObject->SetCurMapCell(this);
}

void MapCell::RemoveObject(GameObject* pkObject)
{
	m_sObjects.erase(pkObject);
	pkObject->SetCurMapCell(NULL);
}

InRangeIter MapCell::GetInRangeSetBegin()
{
	InRangeIter iter;
	*(std::set<GameObject*>::iterator *)&iter = m_vInRangeMapCells.front()->m_sObjects.begin();
	iter.m_pkMapCell = this;
	iter.m_iMapCellIdx = 0;
	return iter;
}

InRangeIter MapCell::GetInRangeSetEnd()
{
	InRangeIter iter;
	*(std::set<GameObject*>::iterator *)&iter = m_vInRangeMapCells.back()->m_sObjects.end();
	iter.m_pkMapCell = this;
	iter.m_iMapCellIdx = m_vInRangeMapCells.size();
	return iter;
}

void MapCell::GetDiffMapCells(MapCell* pkRightHand, MapCellVector& vResult)
{
	vResult.clear();
	std::set_difference(m_vInRangeMapCells.begin(), m_vInRangeMapCells.end(),
		pkRightHand->m_vInRangeMapCells.begin(), pkRightHand->m_vInRangeMapCells.end(),
		std::insert_iterator<MapCellVector>(vResult, vResult.begin()));
}


MapGroup::MapGroup(int iStageID, int iMapGroupID)
{
	m_iStageID = iStageID;
	m_iMapGroupID = iMapGroupID;
	m_pkMapGroupInfo = StageManager::GetInstance()->GetStageInfo(iStageID)->GetMapGroup(iMapGroupID);
	m_iMapWidth = m_pkMapGroupInfo->GetWidth();
	m_iMapHeight = m_pkMapGroupInfo->GetHeight();
	m_iCellCountX = (m_iMapWidth + MAPCELL_SIZE_X - 1) / MAPCELL_SIZE_X;
	m_iCellCountY = (m_iMapHeight + MAPCELL_SIZE_Y - 1) / MAPCELL_SIZE_Y;
	InitMapCells();
}

void MapGroup::InitMapCells()
{
	m_vMapCells.resize(m_iCellCountX * m_iCellCountY);
	for (int iCellY = 0; iCellY < m_iCellCountY; ++iCellY)
	{
		for (int iCellX = 0; iCellX < m_iCellCountX; ++iCellX)
		{
			m_vMapCells[iCellY * m_iCellCountX + iCellX] = new MapCell();
		
		}
	}
	for (int iCellY = 0; iCellY < m_iCellCountY; ++iCellY)
	{
		for (int iCellX = 0; iCellX < m_iCellCountX; ++iCellX)
		{
			MapCell* pkCell = m_vMapCells[iCellY * m_iCellCountX + iCellX];
			const char mask[9][2] = { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 0, -1}, { 0, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 } };
			for (int i = 0; i < 9; ++i)	//MapCell关联九宫格
			{
				int iCellX_ = iCellX + mask[i][0];
				int iCellY_ = iCellY + mask[i][1];
				if (iCellX_ >= 0 && iCellX_ < m_iCellCountX && iCellY_ >= 0 && iCellY_ < m_iCellCountY)
					pkCell->m_vInRangeMapCells.push_back(m_vMapCells[iCellY_ * m_iCellCountX + iCellX_]);
			}
			std::sort(pkCell->m_vInRangeMapCells.begin(), pkCell->m_vInRangeMapCells.end());
		}
	}
}

MapCell* MapGroup::GetMapCell(int iX, int iY)
{
	int iCellX = iX / MAPCELL_SIZE_X;
	int iCellY = iY / MAPCELL_SIZE_Y;
	if (iCellX < 0 || iCellX >= m_iCellCountX || iCellY < 0 || iCellY >= m_iCellCountY)
		return NULL;
	return m_vMapCells[iCellY * m_iCellCountX + iCellX];
}

void MapGroup::InsertObject(GameObject *pkObject)
{
	//m_dqPendingInsert.push_back(pkObject);
	if (pkObject->IsPlayer())
		m_sPlayers.insert(TO<Player*>(pkObject));
	else if (pkObject->IsMonster())
		m_sMonsters.insert(TO<Monster*>(pkObject));
	else if (pkObject->IsMapEvent())
		m_sEvents.insert(TO<MapEvent*>(pkObject));
	pkObject->SetCurMapGroup(this);
	MapCell* pkMapCell = GetMapCell(pkObject->GetPosX(), pkObject->GetPosY());
	pkMapCell->InsertObject(pkObject);

}

void MapGroup::RemoveObject(GameObject *pkObject)
{
	//m_dqPendingRemove.push_back(pkObject);
		//m_sPlayers.erase(TO<Player*>(pkObject));
	for (InRangeIter iter = pkObject->InRangeBegin(); iter != pkObject->InRangeEnd(); ++iter)
	{
		if (*iter == pkObject)
			continue;
		if (pkObject->IsPlayer())
			TO<Player*>(pkObject)->SendDelObject(*iter);
		if ((*iter)->IsPlayer())
			TO<Player*>(*iter)->SendDelObject(pkObject);
	}
	pkObject->GetCurMapCell()->RemoveObject(pkObject);
	//else if (pkObject->IsMonster())
	//	m_sMonsters.erase(TO<Monster*>(pkObject));
	//else if (pkObject->IsMapEvent())
	//	m_sEvents.erase(TO<MapEvent*>(pkObject));
	pkObject->SetCurMapGroup(NULL);

	m_dqPendingRemove.push_back(pkObject);	//在下一帧时从list中删除

}

void MapGroup::Process(float fTimeSpan)
{
	CheckRemoveObjects();
	//while (m_dqPendingInsert.size() > 0)
	//{
	//	GameObject* pkObject = m_dqPendingInsert.front();
	//	m_dqPendingInsert.pop_front();
	//	if (pkObject->IsPlayer())
	//		m_sPlayers.insert(static_cast<Player*>(pkObject));
	//	else if (pkObject->IsMonster())
	//		m_sMonsters.insert(static_cast<Monster*>(pkObject));
	//	else if (pkObject->IsMapEvent())
	//		m_sEvents.insert(static_cast<MapEvent*>(pkObject));
	//	pkObject->SetCurMapGroup(this);
	//	MapCell* pkMapCell = GetMapCell(pkObject->GetPosX(), pkObject->GetPosY());
	//	pkMapCell->InsertObject(pkObject);
	//	pkObject->SetCurMapCell(pkMapCell);
	//}
	if (m_sPlayers.size() == 0)		//加了这一句 cpu顿时降下来了 哇咔咔
		return;
	for (std::set<Player*>::iterator iter = m_sPlayers.begin(); iter != m_sPlayers.end(); ++iter)
	{
		(*iter)->Process(fTimeSpan);
	}

	for (std::set<Monster*>::iterator iter = m_sMonsters.begin(); iter != m_sMonsters.end(); ++iter)
	{
		(*iter)->Process(fTimeSpan);
	}

	for (std::set<MapEvent*>::iterator iter = m_sEvents.begin(); iter != m_sEvents.end(); ++iter)
	{
		(*iter)->Process(fTimeSpan);
	}

	//while (m_dqPendingRemove.size() > 0)
	//{
	//	GameObject* pkObject = m_dqPendingRemove.front();
	//	m_dqPendingRemove.pop_front();
	//	if (pkObject->IsPlayer())
	//		m_sPlayers.erase(static_cast<Player*>(pkObject));
	//	else if (pkObject->IsMonster())
	//		m_sMonsters.erase(static_cast<Monster*>(pkObject));
	//	else if (pkObject->IsMapEvent())
	//		m_sEvents.erase(static_cast<MapEvent*>(pkObject));
	//	//TODO remove mapcell，但是到这里posx和posy已经更改，mapcell目前还有很多问题，需要进一步考虑
	//	pkObject->GetCurMapCell()->RemoveObject(pkObject);
	//	
	//}

	//异步删除果然一堆麻烦 还是改成同步的吧
}

void MapGroup::CheckRemoveObjects()
{
	while (m_dqPendingRemove.size() > 0)
	{
		GameObject* pkObject = m_dqPendingRemove.front();
		m_dqPendingRemove.pop_front();
		if (pkObject->IsPlayer())
			m_sPlayers.erase(static_cast<Player*>(pkObject));
		else if (pkObject->IsMonster())
			m_sMonsters.erase(static_cast<Monster*>(pkObject));
		else if (pkObject->IsMapEvent())
			m_sEvents.erase(static_cast<MapEvent*>(pkObject));		
	}
}

void GameWorld::Init()
{
	this->curInstanceId = 10000;
	int iStage, iMapGroup;
	int iStageCount = StageManager::GetInstance()->GetStageCount();
	this->m_vMapGroups.resize(iStageCount);
	for (iStage = 0; iStage < iStageCount; ++iStage)
	{
		int iMapGroupCount = StageManager::GetInstance()->GetStageInfo(iStage)->GetMapGroupCount();
		this->m_vMapGroups[iStage].resize(iMapGroupCount);
		for (iMapGroup = 0; iMapGroup < iMapGroupCount; ++iMapGroup)
		{
			this->m_vMapGroups[iStage][iMapGroup] = new MapGroup(iStage, iMapGroup);
		}
	}
	this->LoadMapEvents();
}

void GameWorld::LoadMapEvents()
{
	int instanceId = 1000;
	for (EventManager::EventHashMap::iterator iter = EventManager::GetInstance()->EventEnumIterBegin(); iter != EventManager::GetInstance()->EventEnumIterEnd(); ++iter)
	{
		int eventType = iter->second->eventType;
		int dynamicCreate = iter->second->byDynamicCreate;
		if ((eventType == 1 || eventType == 2 || eventType == 3) && dynamicCreate == 0)
		{
			MapEvent *pkMapEvent = new MapEvent(instanceId++, CLASS_ID_MAPEVENT_FIRST);
			pkMapEvent->SetCurWorld(this);
			pkMapEvent->SetEventID(iter->second->id);
			MapGroup *pkMapGroup = this->GetMapGroup(iter->second->mapInfo.uiStageID, iter->second->mapInfo.uiMapGroupID);
			pkMapGroup->InsertObject(pkMapEvent);
		}
	}
}

void GameWorld::Process(float fTimeSpan)
{
	for (int iStage = 0; iStage < m_vMapGroups.size(); ++iStage)
	{
		for (int iMapGroup = 0; iMapGroup < m_vMapGroups[iStage].size(); ++iMapGroup)
		{
			m_vMapGroups[iStage][iMapGroup]->Process(fTimeSpan);
		}
	}
}

void GameWorld::PlayerEnter(Player* pkPlayer)
{
	
	pkPlayer->SetInstanceID(this->curInstanceId++);
	this->GetMapGroup(pkPlayer->GetStageID(), pkPlayer->GetMapGroupID())->InsertObject(pkPlayer);
	pkPlayer->InitPlayer();
}

void GameWorld::PlayerLeave(Player* pkPlayer)
{
	this->GetMapGroup(pkPlayer->GetStageID(), pkPlayer->GetMapGroupID())->RemoveObject(pkPlayer);
}

