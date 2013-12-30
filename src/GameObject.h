#pragma once

enum CLASS_ID
{ 
	CLASS_ID_NULL = 0x0,
	CLASS_ID_INVALID = 0x0,
	CLASS_ID_MOUSECURSOR = 0x1,
	CLASS_ID_ITEM = 0x10020001,
	CLASS_ID_MANAGER = 0x10030001,
	CLASS_ID_WINDOW = 0x20000000,
	CLASS_ID_GOB_FIRST = 0x30000000,
	CLASS_ID_AVATAR = 0x30100001,
	CLASS_ID_MOB_FIRST = 810000000,
	CLASS_ID_MOB_LAST = 811000000,
	CLASS_ID_MAPEVENT_FIRST = 0x30662300,
	CLASS_ID_MAPEVENT_LAST = 0x30756540,
	CLASS_ID_LOOTING_BAG = 0x3084a780,
	CLASS_ID_PET = 0x3093e9c0,
	CLASS_ID_CHAT_BOARD = 0x30a32c00,
	CLASS_ID_MSGBOARD_OBJECT = 0x30c1b080,
	CLASS_ID_PET_SHOP_OBJECT = 0x30d0f2c0,
	CLASS_ID_GOB_LAST = 0x40000000
};

class MapCell;
class MapGroup;
class GameWorld;

class GameObject
{
public:
	GameObject();
	GameObject(uint instanceID, enum CLASS_ID ClassID);
	void SetClassID(enum CLASS_ID ClassID) { this->m_ClassID = ClassID; }
	enum CLASS_ID GetClassID() { return this->m_ClassID; }
	void SetInstanceID(uint InstanceID) { m_InstanceID = InstanceID; }
	uint GetInstanceID() { return m_InstanceID; }
	uint64 GetGUID(void) { return (uint64)m_ClassID << 32 | m_InstanceID; }
	virtual void Process(float) {}
	virtual void Init() {}
	virtual float GetPosX() { return 0.0f; }
	virtual float GetPosY() { return 0.0f; }
	virtual int GetStageID() { return m_iStageID; }
	virtual int GetMapGroupID() { return m_iMapGroupID; }
	virtual void SetStageID(int iStageID) { m_iStageID = iStageID; }
	virtual void SetMapGroupID(int iMapGroupID) { m_iMapGroupID = iMapGroupID; }
	virtual void SetPosX(float fPosX) {}
	virtual void SetPosY(float fPosY) {}
	virtual void SetPos(float fPosX, float fPosY) { SetPosX(fPosX); SetPosY(fPosY); }
	bool IsPlayer() { return m_ClassID == CLASS_ID_AVATAR; }
	bool IsMapEvent() { return m_ClassID >= CLASS_ID_MAPEVENT_FIRST && m_ClassID < CLASS_ID_MAPEVENT_LAST; }
	bool IsMonster() { return m_ClassID >= CLASS_ID_MOB_FIRST && m_ClassID < CLASS_ID_MOB_LAST; }
	void CheckUpdateMapCell();
	void SetCurMapGroup(MapGroup* pkMapGroup) { m_pkCurMapGroup = pkMapGroup; }
	MapGroup* GetCurMapGroup() { return m_pkCurMapGroup; }
	void SetCurMapCell(MapCell* pkMapCell) { m_pkCurMapCell = pkMapCell; }
	MapCell* GetCurMapCell() { return m_pkCurMapCell; }
	void SetCurWorld(GameWorld* pkWorld) { m_pkWorld = pkWorld; }
	GameWorld* GetCurWorld() { return m_pkWorld; }
	
	InRangeIter InRangeBegin() { return m_pkCurMapCell->GetInRangeSetBegin(); }
	InRangeIter InRangeEnd() { return m_pkCurMapCell->GetInRangeSetEnd(); }

	int PerformMessage(int iMsg, int lParam) { return MessageMapFunc(iMsg, lParam); }
protected:
	enum CLASS_ID m_ClassID;
	uint m_InstanceID;
	MapCell* m_pkCurMapCell;
	MapGroup* m_pkCurMapGroup;
	GameWorld* m_pkWorld;
	int m_iStageID;
	int m_iMapGroupID;

	virtual int MessageMapFunc(int iMsg, int lParam) { return -1; }

};

class CreatureStatus;
class EffectArchive;

class Creature : public GameObject, protected ICoordPhysicsSink
{
public:
	virtual std::string GetName() { return m_pkStatus->GetName(); }
	uint64 GetCurAction() { return m_pkCoordPhysics->GetCurAction(); }
	CreatureStatus* GetStatus() { return this->m_pkStatus; }
	ICoordPhysics* GetCoordPhysics() { return this->m_pkCoordPhysics; }
	EffectArchive* GetEffectArchive() { return this->m_pkEffectArchive; }
	virtual float GetPosX() { return m_pkCoordPhysics->GetPosX(); }
	virtual float GetPosY() { return m_pkCoordPhysics->GetPosY(); }
	virtual void SetPosX(float fPosX) { m_pkCoordPhysics->SetPosX(fPosX); }
	virtual void SetPosY(float fPosY) { m_pkCoordPhysics->SetPosY(fPosY); }
	
protected:
	virtual void OnActionCommand(uint64 uiAction) {}
	virtual void OnActionCompleteCommand(uint64 uiAction) {}
	virtual char GetAttribute(int iLayer, int iX, int iY);
	virtual void GetVelocityRatio(VELOCITY_STATUS& VelocityStatus);
	virtual void GetMapGroupSize(int& iMapWidth, int& iMapHeight);

	ICoordPhysics* m_pkCoordPhysics;
	CreatureStatus* m_pkStatus;
	EffectArchive* m_pkEffectArchive;
};

class Monster : public Creature
{
};

