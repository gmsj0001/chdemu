#pragma once

struct KeySet
{
	//不可用int8 否则Convert::ToString会搞笑
	int iSlot;
	int iType;
	int iValue1;
	int iValue2;
};

class MapCell;
class MapZone;

class Connection;
class GameWorld;
class MapZone;
class MapCell;

class Player : public Creature
{
public:
	Player();
	~Player();

	void SetConn(Connection* pkConn) { m_pkConn = pkConn; }
	Connection* GetConn() { return m_pkConn; }
	void SetName(std::string strName) { m_pkStatus->SetName(strName); }
	std::string GetName() { return m_pkStatus->GetName(); }
	int GetCharID() { return m_iCharID; }
	void Process(float fTimeSpan);
	virtual void Init();
	void InitPlayer();
	void SetWorld(GameWorld* pkWorld) { m_pkWorld = pkWorld; }
	
	void SetMotion(int iMotionID);
	void SetMotionTypeID(int iMotionTypeID) { m_iMotionTypeID = iMotionTypeID; }

	bool NewChar(std::string strCharName, int iUserID, int iClassType, int iGender, std::vector<int> vItemID);
	bool LoadGame(std::string strCharName);
	bool SaveGame();

	virtual void DoPortal(int iStageID, int iMapGroupID, float fPosX, float fPosY);

	GameWorld *m_pkWorld;

	Handler* GetConnHandler() { return m_pkConn->GetHandler(); }
	ItemArchive* GetItemArchive() { return m_pkItemArchive; }
	QuestArchive* GetQuestArchive() { return m_pkQuestArchive; }
	SkillArchive* GetSkillArchive() { return m_pkSkillArchive; }


	//Recv Packet
	void RecvChangeFacing(int iDirection);
	void RecvKeySet(std::vector<KeySet>& vKeySet);
	void RecvAction(uint64 uiAction, int iSeqIndex);
	//void RecvCommandLine(std::string strCmd);
	//Player should not parse commandline, the handler should do it and directly call the interface
	void RecvPortal(int iEventType, int iEventID);
	void RecvPortalFinish();
	void RecvStartNPCTalk();
	void RecvEndNPCTalk();
	void RecvChat(int iChatType, std::string strMessage, std::string strToUser);

	//Send Packet
	void BroadcastAction();
	void BroadcastStatus();
	void SendAddObject(GameObject *pObject);
	void SendDelObject(GameObject *pObject);



protected:
	virtual void OnActionCommand(uint64 uiAction);
	virtual void OnActionCompleteCommand(uint64 uiAction);

	int m_iCharID;
	int m_iMotionID;
	int m_iMotionTypeID;
	ItemArchive* m_pkItemArchive;
	QuestArchive* m_pkQuestArchive;
	SkillArchive* m_pkSkillArchive;
	std::vector<KeySet> m_vKeySet;

	Connection* m_pkConn;
};
