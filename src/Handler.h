#pragma once

class Message;
class Connection;

class Handler
{
public:
	Handler(Connection* pkConn) { m_pkParent = pkConn; }

	virtual void ProcessPacket(Packet& p);

	virtual void SendChannelLoginFinish();
	virtual void SendInitChar(int iRetCode, Player* pPlayer);
	virtual void SendKeySet(std::vector<KeySet>& vKeySet);
	virtual void SendChangeFacing(Creature* pkCreature);
	virtual void SendCharStatus(Player* pPlayer);
	virtual void SendAction(int iRetCode, Player* pPlayer);
	virtual void SendAddObject(GameObject* pObject);
	virtual void SendDelObject(GameObject* pObject);
	virtual void SendPortal(int iRetCode, Player* pPlayer);
	virtual void SendChat(int iRetCode, int iChatType, std::string strMessage, Player* pkPlayer);
	virtual void SendItem(int iContainerType, int iSlotIndex);
	virtual void SendItemList(int iContainerType);
	virtual void SendItemChangeSlot(int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo);
	virtual void SendItemTrash(int iContainer, int iSlot, int iCount);
	virtual void SendItemUsing(int iRetCode, int iContainer, int iSlot, int iCount);
	virtual void SendToggleWeaponSet(Player* pkPlayer, int iRetCode, int iWeaponSet);
	virtual void SendItemDivide(int iRetCode, int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo, Item* pkItemTo);
	virtual void SendQuestList();
	virtual void SendSkillList();
	virtual void SendSkillUsing(int iRetCode, int iSkillID, Player* pkPlayer);
	virtual void SendSkillLearn(int iRetCode, SkillActivity* pkSkill);
	virtual void SendSkillLvUp(int iRetCode, SkillActivity* pkSkill);
	
protected:
	virtual void OnPacketCipherInit(Packet& p);	//S101
	virtual void OnPacketEnterServer(Packet& p);	//S300
	virtual void OnPacketLeaveServer(Packet& p);	//S302
	virtual void OnPacketCommandLine(Packet& p);	//S400
	virtual void OnPacketLogin(Packet& p);	//007
	virtual void OnPacketLobbyList(Packet& p);	//100
	virtual void OnPacketChannelList(Packet& p);	//104
	virtual void OnPacketChannelIp(Packet& p);	//107
	virtual void OnPacketChannelLogin(Packet& p);	//109
	virtual void OnPacketCharacterList(Packet& p);	//204
	virtual void OnPacketCreateCharacter(Packet& p);	//206
	virtual void OnPacketDeleteCharacter(Packet& p);	//208
	virtual void OnPacketKeySet(Packet& p);	//220
	virtual void OnPacketChangeFacing(Packet& p);	//224
	virtual void OnPacketAction(Packet& p);	//500
	virtual void OnPacketPortal(Packet& p);	//800
	virtual void OnPacketPortalFinish(Packet& p);	//802
	virtual void OnPacketItemChangeSlot(Packet& p);	//904
	virtual void OnPacketItemTrash(Packet& p);	//906
	virtual void OnPacketItemUsing(Packet& p);	//909
	virtual void OnPacketToggleWeaponSet(Packet& p);	//914
	virtual void OnPacketItemDivide(Packet& p);	//920
	virtual void OnPacketStartNPCTalk(Packet& p);	//1003
	virtual void OnPacketEndNPCTalk(Packet& p);	//1005
	virtual void OnPacketChat(Packet& p);	//1200
	virtual void OnPacketSkillUsing(Packet& p);
	virtual void OnPacketSkillLearn(Packet& p);
	virtual void OnPacketSkillLvUp(Packet& p);


	virtual void WriteBaseStatus(Packet& p, CreatureStatus* pStatus);
	virtual void WriteResStatus(Packet& p, CreatureStatus* pStatus);
	virtual void WriteMovingStatus(Packet& p, CreatureStatus* pStatus);
	virtual void WritePosition(Packet& p, ICoordPhysics* pCoord);
	virtual void WriteCharInfo(Packet& p, Player* pPlayer);
	virtual void WriteCharStatus(Packet& p, PlayerStatus* pStatus);
	virtual void WriteItemNotNull(Packet& p, ItemArchive* pItemArchive, int iContainerType, int iSlotIndex);
	virtual void WriteItem(Packet& p, Item* pItem);
	virtual int WriteItems(Packet& p, ItemArchive* pkItemArchive, int iContainerType);
	virtual int WriteItemsShort(Packet& p, ItemArchive* pkItemArchive, int iContainerType);

	Connection* m_pkParent;
};
