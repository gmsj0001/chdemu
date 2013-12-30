/*
GameZone

stands for a gamezone, maybe a channel zone or a indun
*/
#pragma once

#define MAPCELL_SIZE_X 800
#define MAPCELL_SIZE_Y 800

class MapCell;

class GameObject;

typedef std::set<GameObject*> ObjectSet;
typedef std::vector<MapCell*> MapCellVector;

class InRangeIter : public std::set<GameObject*>::iterator
{
	friend class MapCell;
public:
	
	typedef std::set<GameObject*>::iterator _MyBase;
	InRangeIter& operator ++();	//++iter
	InRangeIter& operator ++(int);	//iter++
	bool operator == (const InRangeIter& _Right);
	bool operator != (const InRangeIter& _Right);
protected:
	MapCell* m_pkMapCell;
	int m_iMapCellIdx;
	void _CheckIncMapCell();
};

class Player;
class Monster;
class MapEvent;

class MapCell
{
	friend class InRangeIter;
	friend class MapGroup;
public:
	void InsertObject(GameObject* pkObject);
	void RemoveObject(GameObject* pkObject);
	ObjectSet::iterator GetObjectSetBegin() { return m_sObjects.begin(); }
	ObjectSet::iterator GetObjectSetEnd() { return m_sObjects.end(); }
	InRangeIter GetInRangeSetBegin();
	InRangeIter GetInRangeSetEnd();
	void GetDiffMapCells(MapCell* pkRightHand, MapCellVector& vResult);
protected:
	std::set<GameObject*> m_sObjects;
	std::vector<MapCell*> m_vInRangeMapCells;
};


class MapGroup
{
public:
	typedef std::vector<MapCell*> MapCellVector;
	MapGroup(int iStageID, int iMapGroupID);
	void InitMapCells();
	void InsertObject(GameObject *pkObject);
	void RemoveObject(GameObject *pkObject);
	virtual void Process(float fTimeSpan);
	int GetStageID() { return m_pkMapGroupInfo->GetStageID(); }
	int GetMapGroupID() { return m_pkMapGroupInfo->GetMapGroupID(); }
	int GetWidth() { return m_pkMapGroupInfo->GetWidth(); }
	int GetHeight() { return m_pkMapGroupInfo->GetHeight(); }
	MapCell* GetMapCell(int iX, int iY);
protected:
	void CheckRemoveObjects();
	std::set<Player*> m_sPlayers;
	std::set<Monster*> m_sMonsters;
	std::set<MapEvent*> m_sEvents;
	std::deque<GameObject*> m_dqPendingInsert;
	std::deque<GameObject*> m_dqPendingRemove;
	std::vector<MapCell*> m_vMapCells;
	MapGroupInfo* m_pkMapGroupInfo;
	int m_iStageID;
	int m_iMapGroupID;
	int m_iMapWidth;
	int m_iMapHeight;
	int m_iCellCountX;
	int m_iCellCountY;
};


class GameWorld
{
public:
	GameWorld() { Init(); }
	void Init();
	void LoadMapEvents();
	virtual void Process(float fTimeSpan);
	void PlayerEnter(Player *pkPlayer);
	void PlayerLeave(Player *pkPlayer);
	MapGroup* GetMapGroup(int iStageID, int iMapGroupID) { return m_vMapGroups[iStageID][iMapGroupID]; }
private:
	std::vector<std::vector<MapGroup*>> m_vMapGroups;
	int curInstanceId;
};