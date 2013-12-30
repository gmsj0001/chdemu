#include "stdafx.h"

GameObject::GameObject()
{
	m_ClassID = CLASS_ID_NULL;
	m_InstanceID = 0;
	m_pkCurMapCell = NULL;
	m_pkCurMapGroup = NULL;
	m_pkWorld = NULL;
	m_iStageID = 0;
	m_iMapGroupID = 0;
}

void GameObject::CheckUpdateMapCell()
{
	MapCell* pkMapCell = m_pkCurMapGroup->GetMapCell(GetPosX(), GetPosY());
	if (pkMapCell != m_pkCurMapCell)
	{
		MapCellVector vCellIn, vCellOut;
		m_pkCurMapCell->GetDiffMapCells(pkMapCell, vCellOut);
		pkMapCell->GetDiffMapCells(m_pkCurMapCell, vCellIn);

		m_pkCurMapCell->RemoveObject(this);

		for (int i = 0; i < vCellOut.size(); ++i)
		{
			for (ObjectSet::iterator iter = vCellOut[i]->GetObjectSetBegin(); iter != vCellOut[i]->GetObjectSetEnd(); ++iter)
			{
				if (IsPlayer())
					TO<Player*>(this)->SendDelObject(*iter);
				if ((*iter)->IsPlayer())	//notify other player to del object
					TO<Player*>(*iter)->SendDelObject(this);
			}
		}

		for (int i = 0; i < vCellIn.size(); ++i)
		{
			for (ObjectSet::iterator iter = vCellIn[i]->GetObjectSetBegin(); iter != vCellIn[i]->GetObjectSetEnd(); ++iter)
			{
				if (IsPlayer())
					TO<Player*>(this)->SendAddObject(*iter);
				if ((*iter)->IsPlayer())
					TO<Player*>(*iter)->SendAddObject(this);
			}
		}

		pkMapCell->InsertObject(this);
	}
}

char Creature::GetAttribute(int iLayer, int iX, int iY)
{
	MapGroupInfo *pkMapGroup = StageManager::GetInstance()->GetStageInfo(GetStageID())->GetMapGroup(GetMapGroupID());
	MapInfo* pkMapInfo = pkMapGroup->FindMapInfo(iX, iY);
	pkMapGroup->GetMapPos(iX, iY, iX, iY);
	TerrainAttribute* pkTerrainAttribute = pkMapInfo->GetTerrainAttribute();
	return pkTerrainAttribute->GetAttr(iLayer, iX, iY);
}

void Creature::GetVelocityRatio(VELOCITY_STATUS& VelocityRatio)
{
	StageManager::GetInstance()->GetStageInfo(GetStageID())->GetMapGroup(GetMapGroupID())->GetVelocityRatio(VelocityRatio);
}

void Creature::GetMapGroupSize(int& iMapWidth, int& iMapHeight)
{
	MapGroupInfo* pkMapGroup = StageManager::GetInstance()->GetStageInfo(GetStageID())->GetMapGroup(GetMapGroupID());
	iMapWidth = pkMapGroup->GetWidth();
	iMapHeight = pkMapGroup->GetHeight();
}