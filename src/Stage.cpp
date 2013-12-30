#include "stdafx.h"

int TerrainAttribute::LoadAttributeFile(std::string fileName)
{
	fileName = (std::string)"DATA/BGFORMAT/" + fileName;
	SPFFileReader fs(fileName);
	this->m_iWidth = fs.ReadInt();
	this->m_iHeight = fs.ReadInt();
	this->xSum = this->m_iWidth / 32;
	this->ySum = this->m_iHeight / 16;
	this->attr = new char[this->xSum * this->ySum * 3];
	memset(this->attr, 0, this->xSum * this->ySum * 3);
	int xBlock, yBlock;
	char attrType;
	for (int iLayer = 0; iLayer < 3; ++iLayer)
	{
		int layerSum = fs.ReadInt();
		for (int i = 0; i < layerSum; ++i)
		{
			xBlock = fs.ReadInt();
			yBlock = fs.ReadInt();
			attrType = fs.ReadByte();
			fs.Skip(3);
			this->attr[iLayer * this->ySum * this->xSum + yBlock * this->xSum + xBlock] = attrType;
		}
	}
	fs.Close();
	return 0;
}


char TerrainAttribute::GetAttr(int iLayer, int iX, int iY)
{
	if (iLayer > 2) return 0;
	if (iX >= this->m_iWidth || iY >= this->m_iHeight) return 0;	//BUG Check：此处的输入检查很重要，否则在地图边界会因为InspectStandLayer的+16造成越界输出未定义的attr
	int xBlock = iX / 32;
	int yBlock = iY / 16;
	return this->attr[iLayer * this->ySum * this->xSum + yBlock * this->xSum + xBlock];
}


void MapGroupInfo::GetMapPos(int iPosX, int iPosY, int& iMapOffsetX, int& iMapOffsetY)
{
	if (this->m_iStructType == 1)
	{
		iMapOffsetX = iPosX % this->m_vMaps[0]->GetWidth();
		iMapOffsetY = iPosY;
	}
	else if (this->m_iStructType == 2)
	{
		iMapOffsetX = iPosX;
		iMapOffsetY = iPosY % this->m_vMaps[0]->GetHeight();
	}
}

void MapGroupInfo::GetPos(int iMapID, int iMapOffsetX, int iMapOffsetY, int &iPosX, int &iPosY)
{
	if (this->m_iStructType == 1)
	{
		iPosX = iMapID * this->m_vMaps[0]->GetWidth() + iMapOffsetX;
		iPosY = iMapOffsetY;
	}
	else if (this->m_iStructType == 2)
	{
		iPosX = iMapOffsetX;
		iPosY = iMapID * this->m_vMaps[0]->GetHeight() + iMapOffsetY;
	}
}

MapInfo* MapGroupInfo::FindMapInfo(float fPosX, float fPosY)
{
	if (this->m_iStructType == 1)
		return this->m_vMaps[(int)(fPosX / this->m_vMaps[0]->GetWidth())];
	else if (this->m_iStructType == 2)
		return this->m_vMaps[(int)(fPosY / this->m_vMaps[0]->GetHeight())];
	return NULL;
}

TerrainAttribute* MapGroupInfo::GetTerrainAttribute(int iPosX, int iPosY, int& iMapOffsetX, int& iMapOffsetY)
{
	MapInfo* pkMapInfo = this->FindMapInfo(iPosX, iPosY);
	this->GetMapPos(iPosX, iPosY, iMapOffsetX, iMapOffsetY);
	return pkMapInfo->GetTerrainAttribute();
}

void StageManager::Init()
{
	LoadCoordModule();
	LoadStageNew();
}

void StageManager::LoadStageNew()
{
	SPFFileReader fs("DATA/BGFORMAT/STAGENEW.STG");
	char tmpStr[64];
	int iStageCount = fs.ReadInt();
	this->m_vStages.resize(iStageCount);
	for (int iStage = 0; iStage < iStageCount; ++iStage)
	{
		fs.ReadInt();	//StageID
		printf("\rInitializing Map Terrain Attribute: Stage %d...", iStage);
		StageInfo* pkStageInfo = new StageInfo();
		this->m_vStages[iStage] = pkStageInfo;
		fs.ReadInt();
		fs.ReadInt();
		fs.Skip(64);
		fs.Skip(64);
		fs.ReadInt();	//NEW STAGENEW VERSION!!!
		int iMapGroupCount = fs.ReadInt();
		pkStageInfo->m_vMapGroups.resize(iMapGroupCount);
		for (int iMapGroup = 0; iMapGroup < iMapGroupCount; ++iMapGroup)
		{
			fs.ReadInt();	//MapGroupID
			MapGroupInfo *pkMapGroupInfo = new MapGroupInfo();
			pkStageInfo->m_vMapGroups[iMapGroup] = pkMapGroupInfo;
			pkMapGroupInfo->m_iStructType = fs.ReadInt();
			fs.ReadInt();
			fs.ReadInt();
			fs.Read(tmpStr, 64);
			fs.Skip(64);
			fs.Skip(64);
			fs.Skip(12);
			//pkMapGroupInfo->m_fGravity = fs.ReadInt() / 1000.0f;
			//pkMapGroupInfo->m_fMaxDropSpeed = fs.ReadInt() / 1000.0f;
			//pkMapGroupInfo->m_fVelocityX = fs.ReadInt() / 1000.0f;
			//pkMapGroupInfo->m_fJumpSpeed = fs.ReadInt() / 1000.0f;
			//pkMapGroupInfo->m_fUpDownVelocity = fs.ReadInt() / 1000.0f;
			//pkMapGroupInfo->m_fHangingVelocity = fs.ReadInt() / 1000.0f;
			pkMapGroupInfo->VelocityRatio.fDropSpeed = fs.ReadInt() / 1000.0f;
			pkMapGroupInfo->VelocityRatio.fMaxDropSpeed = fs.ReadInt() / 1000.0f;
			pkMapGroupInfo->VelocityRatio.fSpeedX = fs.ReadInt() / 1000.0f;
			pkMapGroupInfo->VelocityRatio.fSpeedY = fs.ReadInt() / 1000.0f;
			pkMapGroupInfo->VelocityRatio.fRopeSpeedY = fs.ReadInt() / 1000.0f;
			pkMapGroupInfo->VelocityRatio.fRopeSpeedX = fs.ReadInt() / 1000.0f;
			fs.ReadInt();	//NEW STAGENEW VERSION!!!
			int iMapCount = fs.ReadInt();
			pkMapGroupInfo->m_vMaps.resize(iMapCount);
			for (int iMap = 0; iMap < iMapCount; ++iMap)
			{
				fs.ReadInt();
				MapInfo *pkMapInfo = new MapInfo();
				pkMapGroupInfo->m_vMaps[iMap] = pkMapInfo;
				fs.Read(tmpStr, 64);
				fs.Skip(64);
				fs.Read(tmpStr, 64);
				pkMapInfo->m_pkTerrainAttribute = new TerrainAttribute();
				pkMapInfo->m_pkTerrainAttribute->LoadAttributeFile(tmpStr);
				pkMapInfo->m_iWidth = pkMapInfo->m_pkTerrainAttribute->m_iWidth;
				pkMapInfo->m_iHeight = pkMapInfo->m_pkTerrainAttribute->m_iHeight;
				fs.Skip(64);
			}
			if (pkMapGroupInfo->m_iStructType == 1)
			{
				pkMapGroupInfo->m_lGroupWidth = pkMapGroupInfo->m_vMaps[0]->GetWidth() * pkMapGroupInfo->m_vMaps.size();
				pkMapGroupInfo->m_lGroupHeight = pkMapGroupInfo->m_vMaps[0]->GetHeight();
			}
			else if (pkMapGroupInfo->m_iStructType == 2)
			{
				pkMapGroupInfo->m_lGroupWidth = pkMapGroupInfo->m_vMaps[0]->GetWidth();
				pkMapGroupInfo->m_lGroupHeight = pkMapGroupInfo->m_vMaps[0]->GetHeight() * pkMapGroupInfo->m_vMaps.size();
			}
		}
	}
	printf("\rInitializing Map Terrain Attribute: Done.          \n");
}

void StageManager::LoadCoordModule()
{
	HMODULE hModule = LoadLibraryA("MOD_1.DAT");
	void*(WINAPI* pfGetModule)() = (void*(WINAPI*)())GetProcAddress(hModule, "GetModule");
	m_pkCoordModule = (ICoordModule*)pfGetModule();
}

StageManager* Singleton<StageManager>::m_pkInstance = NULL;
