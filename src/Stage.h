#pragma once

class TerrainAttribute
{
	friend class StageManager;
public:
	TerrainAttribute() 
	{ 
		this->m_iWidth = 0;
		this->m_iHeight = 0;
		this->xSum = 0;
		this->ySum = 0;
		this->attr = NULL;
	}
	~TerrainAttribute() { if (this->attr) delete attr; }
	char GetAttribute(float x, float y);
	int LoadAttributeFile(std::string fileName);
	int m_iWidth;
	int m_iHeight;

	char GetAttr(int iLayer, int iX, int iY);
private:
	int xSum;
	int ySum;
	char *attr;
};

class MapInfo
{
	friend class StageManager;
public:
	int GetWidth() { return m_iWidth; }
	int GetHeight() { return m_iHeight; }
	TerrainAttribute* GetTerrainAttribute() { return m_pkTerrainAttribute; }
protected:
	TerrainAttribute* m_pkTerrainAttribute;
	int m_iWidth;
	int m_iHeight;
};

class MapGroupInfo;

class StageInfo
{
	friend StageManager;
public:
	MapGroupInfo* GetMapGroup(int iMapGroupID)
	{ 
		if (iMapGroupID >= m_vMapGroups.size()) 
			return NULL; 
		else 
			return m_vMapGroups[iMapGroupID]; 
	}
	int GetMapGroupCount() 
	{
		return m_vMapGroups.size(); 
	}
	int GetStageID() { return m_iStageID; }
protected:
	int m_iStageID;
	std::vector<MapGroupInfo*> m_vMapGroups;
};

class MapGroupInfo
{
	friend class StageManager;
public:
	MapGroupInfo() 
	{ 
		VelocityRatio.fSpeedX = 1.0f;
		VelocityRatio.fSpeedY = 1.0f;
		VelocityRatio.fRopeSpeedY = 1.0f;
		VelocityRatio.fRopeSpeedX = 1.0f;
		VelocityRatio.fDropSpeed = 1.0f;
		VelocityRatio.fMaxDropSpeed = 1.0f;
	}
	int GetMapGroupID() { return m_iMapGroupID; }
	int GetStageID() { return m_pkStageInfo->GetStageID(); }
	int GetWidth() { return m_lGroupWidth; }
	int GetHeight() { return m_lGroupHeight; }
	void GetVelocityRatio(VELOCITY_STATUS& VelocityRatio) { VelocityRatio = this->VelocityRatio; }
	void GetMapPos(int iPosX, int iPosY, int& iMapOffsetX, int &iMapOffsetY);
	void GetPos(int iMapID, int iMapOffsetX, int iMapOffsetY, int &iPosX, int &iPosY);
	TerrainAttribute * GetTerrainAttribute(int iPosX, int iPosY, int &iMapOffsetX, int &iMapOffsetY);
	MapInfo* FindMapInfo(float fPosX, float fPosY);
protected:
	int m_iMapGroupID;
	StageInfo* m_pkStageInfo;
	std::vector<MapInfo*> m_vMaps;
	int m_iStructType;
	int m_lGroupWidth;
	int m_lGroupHeight;

	VELOCITY_STATUS VelocityRatio;
};


class StageManager : public Singleton<StageManager>
{
public:
	StageManager() { Init(); }
	void Init();
	void LoadStageNew();
	void LoadCoordModule();
	StageInfo* GetStageInfo(int iStageID) 
	{
		return m_vStages[iStageID];
	}
	int GetStageCount() 
	{
		return m_vStages.size(); 
	}
	ICoordModule* GetCoordModule() { return m_pkCoordModule; }
protected:
	std::vector<StageInfo*> m_vStages;
	ICoordModule* m_pkCoordModule;
};
