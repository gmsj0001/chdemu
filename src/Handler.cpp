#include "stdafx.h"

void Handler::ProcessPacket(Packet& p)
{
	int iOpCode = p.ReadInt();
	//printf("%d\n", iOpCode);
	switch (iOpCode)
	{
	case 0:
		break;
	case 101:
		OnPacketCipherInit(p);
		break;
	case 300:
		OnPacketEnterServer(p);
		break;
	case 302:
		OnPacketLeaveServer(p);
		break;
	case 401:
		OnPacketCommandLine(p);
		break;
	case 200000007:
		OnPacketLogin(p);
		break;
	case 200000100:
		OnPacketLobbyList(p);
		break;
	case 200000104:
		OnPacketChannelList(p);
		break;
	case 200000107:
		OnPacketChannelIp(p);
		break;
	case 200000109:
		OnPacketChannelLogin(p);
		break;
	case 200000204:
		OnPacketCharacterList(p);
		break;
	case 200000206:
		OnPacketCreateCharacter(p);
		break;
	case 200000208:
		OnPacketDeleteCharacter(p);
		break;
	case 200000220: //key set
		OnPacketKeySet(p);
		break;
	case 200000224: //change direction
		OnPacketChangeFacing(p);
		break;
		//200000402: game option
	case 200000500:
		OnPacketAction(p);
		break;
	case 200000800:
		OnPacketPortal(p);
		break;
	case 200000802:
		OnPacketPortalFinish(p);
		break;
		//200000902: maybe pick bag
	case 200000904:
		OnPacketItemChangeSlot(p);
		break;
	case 200000906:
		OnPacketItemTrash(p);
		break;
	case 200000909:
		OnPacketItemUsing(p);
		break;
	case 200000914: //change weapon
		OnPacketToggleWeaponSet(p);
		break;
		//200000918: view other player package
	case 200000920:
		OnPacketItemDivide(p);
		break;
		//200000922: maybe xiangqian
		//200000938: item mix
		//200001001: may be npc buy
	case 200001003:
		OnPacketStartNPCTalk(p);
		break;
	case 200001005:
		OnPacketEndNPCTalk(p);
		break;
	case 200001200: //chat
		OnPacketChat(p);
		break;
	case 200001802: //using skill
		OnPacketSkillUsing(p);
		break;
	case 200001806:	//skill lvup
		OnPacketSkillLvUp(p);
		break;
	case 200001810:	//skill learn
		OnPacketSkillLearn(p);
		break;
		//200002000: party add
		//200002002: party accept
		//200002006: party quit
		//200002010: maybe party item dispatch type
		//200002300: enter cashshop
		//200002302: leave cashshop
	case 200002324:	//OGP Done loading
		m_pkParent->SendNow("15 CB EB 0B 00 00 00 00 81 C9 01 00 01 00 10 30 00 00 00 00 00 00 00 00");
		break;
	default:
		printf("Unhandled Packet: %d\n", iOpCode);
		break;
	}
}

void Handler::OnPacketCipherInit(Packet& p)
{
	m_pkParent->SendNow("66 00 00 00 00 00 00 00");
}

void Handler::OnPacketEnterServer(Packet& p)
{
	int iUserID = p.ReadInt();
	m_pkParent->SetUserID(iUserID);
	m_pkParent->SendNow("2D 01 00 00 00 00 00 00 00 00 00 00");

}

void Handler::OnPacketLeaveServer(Packet& p)
{
	m_pkParent->SendNow("2F 01 00 00 00 00 00 00");
}

void Handler::OnPacketLogin(Packet& p)
{
	p.Skip(8);
	std::string strUsername = p.ReadL1String();
	p.Skip(2);
	std::string strPassword = p.ReadL1String();
	p.Skip(2);
	int iUserID;
	Packet pac;
	int ret = DataBase::GetInstance()->UserLogin(strUsername, strPassword, &iUserID);
	if (ret == -2)
	{
		pac.WriteInt(200000001);
		pac.WriteByte(3);
		m_pkParent->Send(&pac);
		return;
	}
	if (ret == -1)
	{
		DBResult *result = DataBase::GetInstance()->Query("INSERT INTO `users`(`username`, `password`) VALUES ('" + strUsername + "', '" + strPassword + "')");
		iUserID = result->LastInsertId();
		result->Free();
		//pac.WriteInt(200000001);
		//pac.WriteByte(4);
		//pConn->Send(&pac);
		//return;
	}
	m_pkParent->SetUserID(iUserID);
	pac.WriteInt(200000001);
	pac.WriteByte(0);
	pac.WriteInt(iUserID);	//uid
	pac.WriteHexString("00 00 00 00");
	pac.WriteShort(m_pkParent->GetServer()->GetServerID());	//maybe login server id
	pac.WriteFixString(strUsername, 32);	//username
	m_pkParent->Send(&pac);

	pac.Clear();
	pac.WriteInt(108);
	pac.WriteShort(m_pkParent->GetServer()->GetServerID());
	pac.WriteFixString(m_pkParent->GetServer()->GetPublicIp());
	pac.WriteUShort(m_pkParent->GetServer()->GetPublicPort());
	m_pkParent->Send(&pac);
}

void Handler::OnPacketLobbyList(Packet& p)
{
	Packet pac;
	pac.WriteInt(200000101);
	pac.WriteByte(1);	//server count
	pac.WriteShort(1);
	pac.WriteFixString("CHDEMU World");
	pac.WriteShort(1);	//server id
	pac.WriteFixString(m_pkParent->GetServer()->GetPublicIp());
	pac.WriteUShort(m_pkParent->GetServer()->GetPublicPort());
	DBResult *result = DataBase::GetInstance()->Query("SELECT COUNT(*) AS `char_count` FROM `characters` WHERE `user_id` = " + Convert::ToString(m_pkParent->GetUserID()));
	pac.WriteByte(result->GetInt("char_count"));	//character count
	result->Free();
	pac.WriteByte(0);	//server payload
	m_pkParent->Send(&pac);
}

void Handler::OnPacketChannelList(Packet& p)
{
	Packet pac;
	pac.WriteInt(200000105);
	pac.WriteByte(1);	//channel count
	pac.WriteShort(1);	//server id
	pac.WriteFixString("CH_01");
	pac.WriteByte(0);	//server payload
	m_pkParent->Send(&pac);

}

void Handler::OnPacketChannelIp(Packet& p)
{
	Packet pac;
	pac.WriteInt(200000108);
	pac.WriteByte(0);
	pac.WriteFixString(m_pkParent->GetServer()->GetPublicIp());
	pac.WriteUShort(m_pkParent->GetServer()->GetPublicPort());
	m_pkParent->Send(&pac);
}

void Handler::OnPacketCharacterList(Packet& p)
{
	Packet pac;
	pac.WriteInt(200000205);
	pac.WriteInt(0);
	pac.WriteByte(0);

	DBResult *result = DataBase::GetInstance()->Query("SELECT COUNT(`id`) FROM `characters` WHERE `user_id` = " + Convert::ToString(m_pkParent->GetUserID()));
	result->ReadRow();
	int charCount = result->GetInt(0);
	result->Free();
	pac.WriteByte(charCount);
	if (charCount > 0)
	{
		result = DataBase::GetInstance()->Query("SELECT `id`, `name`, `lv`, `class_type`, `gender` FROM `characters` WHERE `user_id` = " + Convert::ToString(m_pkParent->GetUserID()));
		while (result->ReadRow())
		{
			Player* pPlayer = new Player();
			pPlayer->LoadGame(result->GetString("name"));

			WriteCharInfo(pac, pPlayer);

			int iCountPos = pac.pos;
			int iCount = 0;
			pac.WriteShort(iCount);
			iCount += WriteItems(pac, pPlayer->GetItemArchive(), 6);
			iCount += WriteItems(pac, pPlayer->GetItemArchive(), 7);
			iCount += WriteItems(pac, pPlayer->GetItemArchive(), 8);
			*(short*)&pac.data[iCountPos] = iCount;

			pac.Pad(0, 0x0d);

			delete pPlayer;
			pac.WriteByte(0);

		}
		result->Free();
	}
	m_pkParent->Send(&pac);
}

void Handler::OnPacketChannelLogin(Packet& p)
{
	int iUid = p.ReadInt();
	p.Skip(6);
	std::string strCharName = p.ReadFixString();

	//暂不检查 按Mode1处理

	Player *pkPlayer = new Player();
	pkPlayer->SetWorld(TO<SingleServer*>(m_pkParent->GetServer())->GetWorld());
	pkPlayer->Init();
	pkPlayer->SetConn(m_pkParent);
	m_pkParent->SetPlayer(pkPlayer);
	pkPlayer->LoadGame(strCharName);
	//DataBase::GetInstance()->LoadPlayer(charName, player);
	TO<SingleServer*>(m_pkParent->GetServer())->GetWorld()->PlayerEnter(pkPlayer);

}

void Handler::OnPacketCommandLine(Packet &p)
{
	std::string command = p.ReadL1String();
	if (command.length() == 0) return;
	std::vector<std::string> params;
	std::string::size_type start = 0, end = command.find(' ');
	while (end != std::string::npos)
	{
		params.push_back(command.substr(start, end - start));
		start = end + 1;
		end = command.find(' ', start);
	}
	params.push_back(command.substr(start));

	if (params[0] == "portal")
	{
		if (params.size() == 3 || params.size() == 5)
		{
			float fPosX = atof(params[1].c_str());
			float fPosY = atof(params[2].c_str());
			int iStage = m_pkParent->GetPlayer()->GetStageID();
			int iMapGroup = m_pkParent->GetPlayer()->GetMapGroupID();
			if (params.size() == 5)
			{
				iStage = atoi(params[3].c_str());
				iMapGroup = atoi(params[4].c_str());
			}
			m_pkParent->GetPlayer()->DoPortal(iStage, iMapGroup, fPosX, fPosY);
		}
	}
	else if (params[0] == "echo")
	{
		if (params.size() == 2)
		{
			m_pkParent->SendNow(params[1]);
		}
	}
	else if (params[0] == "giveitem")
	{
		if (params.size() == 2 || params.size() == 3)
		{
			int iItemID = atoi(params[1].c_str());
			int iCount = 1;
			if (params.size() == 3)
				iCount = atoi(params[2].c_str());
			Item item(iItemID);
			item.iItemID = iItemID;
			item.iStackCount = iCount;
			if (item.pItemInfo == NULL)
				return;
			m_pkParent->GetPlayer()->GetItemArchive()->AddItem(item);
		}
	}


}

void Handler::OnPacketCreateCharacter(Packet& p)
{
	Packet pac;
	DBResult *result;
	std::string sql;
	int iLobbyID = p.ReadShort();	//may be lobbyid

	p.ReadULong();
	std::string sCharName = p.ReadFixString();
	p.Skip(0x27);
	int iUserID = p.ReadInt();
	p.Skip(0x0c);
	int iClass = p.ReadByte();
	int iGender = p.ReadByte();
	p.Skip(0x35);
	std::vector<int> vItemID;
	for (int i = 0; i < 5; ++i)
	{
		int iItemID = p.ReadInt();
		if (iItemID != 0)
			vItemID.push_back(iItemID);
	}

	sql = "SELECT COUNT(`id`) AS `char_count` FROM `characters` WHERE `name` = '" + sCharName + "'";
	result = DataBase::GetInstance()->Query(sql);
	if (result->GetInt("char_count") > 0)	//name in use
	{
		result->Free();
		pac.WriteInt(200000207);
		pac.WriteInt(17);
		m_pkParent->Send(&pac);
		return;
	}
	result->Free();
	Player* pPlayer = new Player();
	pPlayer->NewChar(sCharName, iUserID, iClass, iGender, vItemID);
	pPlayer->SaveGame();
	pac.WriteInt(200000207);
	pac.WriteInt(0);
	pac.WriteInt(pPlayer->GetCharID());
	m_pkParent->Send(&pac);
	delete pPlayer;

}

void Handler::OnPacketDeleteCharacter(Packet& p)
{
	p.ReadShort();
	p.ReadInt();
	int iCharID = p.ReadInt();
	std::string sCharName = p.ReadFixString();
	DBResult *result;
	result = DataBase::GetInstance()->Query("DELETE FROM `items` WHERE `char_id` = " + Convert::ToString(iCharID));
	result->Free();
	result = DataBase::GetInstance()->Query("DELETE FROM `skills` WHERE `char_id` = " + Convert::ToString(iCharID));
	result->Free();
	result = DataBase::GetInstance()->Query("DELETE FROM `keys` WHERE `char_id` = " + Convert::ToString(iCharID));
	result->Free();
	result = DataBase::GetInstance()->Query("DELETE FROM `characters` WHERE `id` = " + Convert::ToString(iCharID));
	result->Free();
	Packet pac;
	pac.WriteInt(200000209);
	pac.WriteInt(0);
	pac.WriteInt(iCharID);
	pac.WriteInt(1);
	pac.Pad(0, 9);
	m_pkParent->Send(&pac);
}

void Handler::OnPacketKeySet(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iCount = p.ReadByte();
	std::vector<KeySet> vKeySet(iCount);
	for (int i = 0; i < iCount; ++i)
	{
		vKeySet[i].iType = p.ReadByte();
		vKeySet[i].iValue1 = p.ReadInt();
		vKeySet[i].iValue2 = p.ReadInt();
		vKeySet[i].iSlot = p.ReadByte();
	}
	m_pkParent->GetPlayer()->RecvKeySet(vKeySet);
}

void Handler::OnPacketChangeFacing(Packet& p)
{
	int iDirection = p.ReadByte() - 1;	//in coord, direction enum are 0/1, here are 1/2
	m_pkParent->GetPlayer()->GetCoordPhysics()->SetDirection(iDirection);
	this->SendChangeFacing(m_pkParent->GetPlayer());
}

void Handler::OnPacketAction(Packet &p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	uint64 uiAction = p.ReadULong();
	int iSeqIndex = p.ReadInt();
	m_pkParent->GetPlayer()->RecvAction(uiAction, iSeqIndex);
}

void Handler::OnPacketPortal(Packet &p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iEventType = p.ReadByte();
	int iEventID = p.ReadInt();
	m_pkParent->GetPlayer()->RecvPortal(iEventType, iEventID);
}

void Handler::OnPacketPortalFinish(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	m_pkParent->GetPlayer()->RecvPortalFinish();
}

void Handler::OnPacketItemChangeSlot(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iContainerFrom = p.ReadInt();
	int iSlotFrom = p.ReadByte();
	int iCount = p.ReadByte();
	int iContainerTo = p.ReadInt();
	int iSlotTo = p.ReadByte();
	m_pkParent->GetPlayer()->GetItemArchive()->RecvChangeSlot(iContainerFrom, iSlotFrom, iCount, iContainerTo, iSlotTo);
}

void Handler::OnPacketItemTrash(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iContainer = p.ReadInt();
	int iSlot = p.ReadByte();
	int iCount = p.ReadByte();
	m_pkParent->GetPlayer()->GetItemArchive()->RecvTrash(iContainer, iSlot, iCount);
}

void Handler::OnPacketItemUsing(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	p.Skip(8);
	int iContainer = p.ReadInt();
	int iSlot = p.ReadByte();
	m_pkParent->GetPlayer()->GetItemArchive()->RecvUsing(iContainer, iSlot);
}

void Handler::OnPacketToggleWeaponSet(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	m_pkParent->GetPlayer()->GetItemArchive()->RecvToggleWeaponSet();
}

void Handler::OnPacketItemDivide(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iContainerFrom = p.ReadInt();
	int iSlotFrom = p.ReadByte();
	int iCount = p.ReadByte();
	int iContainerTo = p.ReadInt();
	int iSlotTo = p.ReadByte();
	m_pkParent->GetPlayer()->GetItemArchive()->RecvDivide(iContainerFrom, iSlotFrom, iCount, iContainerTo, iSlotTo);
}

void Handler::OnPacketStartNPCTalk(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	Packet pac(200001004);
	pac.WriteULong(m_pkParent->GetPlayer()->GetGUID());
	m_pkParent->Send(&pac);
}

void Handler::OnPacketEndNPCTalk(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	Packet pac(200001006);
	pac.WriteULong(m_pkParent->GetPlayer()->GetGUID());
	m_pkParent->Send(&pac);
}

void Handler::OnPacketChat(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iChatType = p.ReadInt();
	if (iChatType == 1)
	{
		p.Skip(4);
		std::string strChat = p.ReadL1String();
		m_pkParent->GetPlayer()->RecvChat(iChatType, strChat, "");
	}
}

void Handler::OnPacketSkillUsing(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iSkillID = p.ReadInt();
	int iDirection = p.ReadByte();
	p.ReadShort();	//指挥会用到这个数
	m_pkParent->GetPlayer()->GetSkillArchive()->RecvUsing(iSkillID, iDirection);
}

void Handler::OnPacketSkillLvUp(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iSkillID = p.ReadInt();
	m_pkParent->GetPlayer()->GetSkillArchive()->RecvLvUp(iSkillID);
}

void Handler::OnPacketSkillLearn(Packet& p)
{
	if (m_pkParent->GetPlayer() == NULL)
		return;
	int iSkillID = p.ReadInt();
	p.ReadInt();
	p.ReadInt();
	p.Skip(3);
	m_pkParent->GetPlayer()->GetSkillArchive()->RecvLearn(iSkillID);
}

void Handler::WriteBaseStatus(Packet& p, CreatureStatus* pStatus)
{
	p.WriteShort(pStatus->m_iLevel);
	p.WriteInt(pStatus->m_MHP.iBase);
	p.WriteInt(pStatus->m_MHP.iCurr);
	p.WriteInt(pStatus->m_MSP.iBase);
	p.WriteInt(pStatus->m_MSP.iCurr);
	p.WriteInt(pStatus->m_iHP);
	p.WriteInt(pStatus->m_iSP);
	p.WriteInt(pStatus->m_RecvHP.iCurr);
	p.WriteInt(pStatus->m_RecvSP.iCurr);
}

void Handler::WriteResStatus(Packet& p, CreatureStatus* pStatus)
{
	//core274: 0x1e bytes
	p.WriteShort(pStatus->m_Luk.iBase);
	p.WriteShort(pStatus->m_Luk.iCurr);
	p.WriteShort(pStatus->m_Atk.iBase);
	p.WriteShort(pStatus->m_Atk.iCurr);
	p.WriteShort(pStatus->m_Def.iBase);
	p.WriteShort(pStatus->m_Def.iCurr);
	p.WriteShort(pStatus->m_Ele.iBase);
	p.WriteShort(pStatus->m_Ele.iCurr);
	p.WriteShort(pStatus->m_Water.iCurr);
	p.WriteShort(pStatus->m_Earth.iCurr);
	p.WriteShort(pStatus->m_Fire.iCurr);
	p.WriteShort(pStatus->m_Wind.iCurr);
	//p.WriteShort(pStatus->m_Dark.iCurr);
	p.WriteShort(pStatus->m_Normal.iCurr);
	p.WriteShort(pStatus->m_Heavy.iCurr);
	p.WriteShort(0);	//第三个ATK是嘛。。。STATUS枚举里没有找到JUMP ATK 还要去逆一下
}

void Handler::WriteMovingStatus(Packet& p, CreatureStatus* pStatus)
{
	p.WriteFloat(pStatus->m_SpeedX.iCurr);
	p.WriteFloat(pStatus->m_SpeedY.iCurr);
	p.WriteFloat(pStatus->m_RopeSpeedY.iCurr);
	p.WriteFloat(pStatus->m_RopeSpeedX.iCurr);
	p.WriteFloat(pStatus->m_DropSpeed.iCurr);
	p.WriteFloat(pStatus->m_MaxDropSpeed.iCurr);
}

void Handler::WritePosition(Packet& p, ICoordPhysics* pCoord)
{
	uint8 Buffer[0x32];
	pCoord->GetLTPosition(Buffer);
	p.Write(Buffer, sizeof(Buffer));
}

void Handler::WriteCharInfo(Packet& p, Player* pPlayer)
{
	//core274: 0x82 bytes
	PlayerStatus* pStatus = TO<PlayerStatus*>(pPlayer->GetStatus());
	p.WriteLong(pPlayer->GetGUID());
	p.WriteFixString(pPlayer->GetName());
	WriteBaseStatus(p, pStatus);
	p.WriteByte(0);	//iUP_Type
	p.WriteInt(0);	//uiMaxUP
	
	p.WriteInt(pStatus->m_iUserID);
	p.WriteInt(0);	//uiTitleID;
	p.WriteLong(pStatus->m_iMoney);	//uiMoney;

	p.WriteByte(pStatus->m_iClass);
	p.WriteByte(pStatus->m_Gender);
	p.WriteByte(0);	//uiWeaponSet;
	p.WriteLong(pStatus->m_iMoneyStorage);	//uiStorageMoney
	WriteMovingStatus(p, pStatus);
	p.WriteLong(0);	//fame
	p.WriteLong(0x7f7fffffffi64);	//game option
}

void Handler::WriteCharStatus(Packet& p, PlayerStatus* pStatus)
{
	//core274: 0x52 bytes
	p.WriteLong(pStatus->m_iExp);
	p.WriteLong(pStatus->m_iExpMin);	
	p.WriteLong(pStatus->m_iExpMax);	

	WriteResStatus(p, pStatus);
	
	p.WriteShort(pStatus->m_iSkillPoint);
	//p.WriteInt(pStatus->m_Amor.iCurr);	//S2 amor 4byte
	p.WriteShort(pStatus->m_Amor.iCurr);
	p.WriteShort(pStatus->m_EarthRes.iCurr);
	p.WriteShort(pStatus->m_WindRes.iCurr);
	p.WriteShort(pStatus->m_WaterRes.iCurr);
	p.WriteShort(pStatus->m_FireRes.iCurr);
	//p.WriteShort(pStatus->m_DarkRes.iCurr);
	p.WriteShort(pStatus->m_iRequireLv);
	p.WriteShort(pStatus->m_iProbEnchant);
	p.WriteByte(pStatus->m_CriticalProbability.iAdd);
	p.WriteByte(pStatus->m_EleCriticalProbability.iAdd);
	p.WriteShort(pStatus->m_PhyDodgeRate.iCurr);
	p.WriteShort(pStatus->m_EleDodgeRate.iCurr);
	p.WriteShort(pStatus->m_PhyHitRate.iCurr);
	p.WriteShort(pStatus->m_EleHitRate.iCurr);
	p.WriteShort(0);	//iProbOptionReset
}

void Handler::WriteItem(Packet& p, Item* pItem)
{
	//core274: Item without container is 0x3c bytes
	if (pItem != NULL)
	{
		p.WriteInt(pItem->iItemID);
		p.WriteByte(pItem->iStackCount);
		p.WriteInt(0);	//iLifeTime
		p.WriteByte(1);	//iRare
		for (int i = 0; i < 5; ++i)
		{
			p.WriteShort(0);	//iID
			p.WriteShort(0);	//iValue
		}
		p.WriteLong(pItem->iItemNo);
		p.WriteInt(0);	//iBackPrice
		p.WriteShort(0);	//iOptPoint
		p.WriteLong(0);	//iItemExp
		p.WriteShort(0);	//iItemSP
		p.WriteByte(0);	//iEnchantedLevel
		p.WriteByte(0);	//iLock
		p.WriteInt(0);	//iLockRemainTime;
	}
	else
	{
		p.Pad(0, 0x3C);
	}
}

void Handler::WriteItemNotNull(Packet& p, ItemArchive* pItemArchive, int iContainerType, int iSlotIndex)
{
	Item* pItem = pItemArchive->GetInvenItem(iContainerType, iSlotIndex);
	p.WriteInt(iContainerType);
	p.WriteByte(iSlotIndex);
	WriteItem(p, pItem);
}

int Handler::WriteItems(Packet& p, ItemArchive* pItemArchive, int iContainerType)
{
	int iCount = 0;
	for (int iSlotIndex = 1; iSlotIndex <= pItemArchive->GetInvenSize(iContainerType); ++iSlotIndex)
	{
		Item* pItem = pItemArchive->GetInvenItem(iContainerType, iSlotIndex);
		if (pItem != NULL)
		{
			p.WriteInt(iContainerType);
			p.WriteByte(iSlotIndex);
			WriteItem(p, pItem);
			++iCount;
		}
	}
	return iCount;
}

int Handler::WriteItemsShort(Packet& p, ItemArchive* pkItemArchive, int iContainerType)
{
	int iCount = 0;
	for (int iSlotIndex = 1; iSlotIndex <= pkItemArchive->GetInvenSize(iContainerType); ++iSlotIndex)
	{
		Item* pItem = pkItemArchive->GetInvenItem(iContainerType, iSlotIndex);
		if (pItem != NULL)
		{
			p.WriteInt(iContainerType);
			p.WriteByte(iSlotIndex);
			p.WriteInt(pItem->iItemID);
			++iCount;
		}
	}
	return iCount;
}

void Handler::SendChannelLoginFinish()
{
	Packet p(200000110);
	p.WriteByte(0);
	m_pkParent->Send(&p);
}

void Handler::SendInitChar(int iRetCode, Player* pPlayer)
{
	Packet p(200000211);
	PlayerStatus *pStatus = TO<PlayerStatus*>(pPlayer->GetStatus());
	p.WriteByte(iRetCode);
	if (iRetCode == 0)
	{
		WriteCharInfo(p, pPlayer);//0x82 bytes

		WritePosition(p, pPlayer->GetCoordPhysics()); //0x32 bytes
		WriteCharStatus(p, pStatus);
		
		p.WriteShort(pPlayer->GetStageID());
		p.WriteShort(pPlayer->GetMapGroupID());
		p.WriteShort(0);	//mapid ignored

		p.WriteByte(0);	//pvp lv
		p.WriteLong(0);	//iGlory
		p.WriteInt(0);	//iPoint

		p.WriteHexString("0000000100000000000000000000000000000000000000000100000000");
	}
	m_pkParent->Send(&p);
}

void Handler::SendKeySet(std::vector<KeySet>& vKeySet)
{
	Packet p(200000221);
	p.WriteULong(m_pkParent->GetPlayer()->GetGUID());
	p.WriteByte(vKeySet.size());
	for (int i = 0; i < vKeySet.size(); ++i)
	{
		p.WriteByte(vKeySet[i].iType);
		p.WriteInt(vKeySet[i].iValue1);
		p.WriteInt(vKeySet[i].iValue2);
		p.WriteByte(vKeySet[i].iSlot);
	}
	m_pkParent->Send(&p);
}


void Handler::SendChangeFacing(Creature* pkCreature)
{
	Packet p(200000225);
	p.WriteULong(pkCreature->GetGUID());
	//in coord, direction enum are 0/1, here are 1/2
	p.WriteByte(pkCreature->GetCoordPhysics()->GetDirection() + 1);
	m_pkParent->Send(&p);
}

void Handler::SendCharStatus(Player* pPlayer)
{
	Packet p(200000228);
	PlayerStatus *pStatus = TO<PlayerStatus*>(pPlayer->GetStatus());

	p.WriteULong(pPlayer->GetGUID());
	
	WriteBaseStatus(p, pStatus);	//from Lv to RSP
	
	p.Skip(8);
	//EXP
	WriteCharStatus(p, pStatus);

	WriteMovingStatus(p, pStatus);

	m_pkParent->Send(&p);
}

void Handler::SendAction(int iRetCode, Player* pPlayer)
{
	Packet p(200000501);
	p.WriteByte(iRetCode);
	if (iRetCode == 0)
	{
		p.WriteULong(pPlayer->GetGUID());
		p.WriteULong(pPlayer->GetCurAction());
		p.WriteInt(pPlayer->GetCoordPhysics()->GetCurSeqIndex());	//SeqIndex
		WritePosition(p, pPlayer->GetCoordPhysics());
	}
	m_pkParent->Send(&p);
}

void Handler::SendAddObject(GameObject* pObject)
{
	Packet p(200000700);
	p.WriteULong(pObject->GetGUID());
	if (pObject->IsPlayer())
	{
		Player* pPlayer = TO<Player*>(pObject);
		WriteCharInfo(p, pPlayer);
		p.WriteULong(pPlayer->GetCurAction());
		p.WriteInt(pPlayer->GetCoordPhysics()->GetCurSeqIndex());	//SeqIndex
		WritePosition(p, pPlayer->GetCoordPhysics());
		int iCountPos = p.pos;
		int iCount = 0;
		p.WriteShort(iCount);
		iCount += WriteItemsShort(p, pPlayer->GetItemArchive(), 6);
		iCount += WriteItemsShort(p, pPlayer->GetItemArchive(), 7);
		iCount += WriteItemsShort(p, pPlayer->GetItemArchive(), 8);
		*(short*)&p.data[iCountPos] = iCount;
		p.WriteShort(0);	//buff count，每个0x15字节

		//_GUILD_BROADCAST guildInfo = {0};
		//p.Write(&guildInfo, sizeof(_GUILD_BROADCAST));
		p.Pad(0, 0x1a);
		p.WriteByte(0);	//pet count

	}
	else if (pObject->IsMapEvent())
	{
		MapEvent* pMapEvent = TO<MapEvent*>(pObject);
		_MAPEVENT_INFO* pEventInfo = pMapEvent->GetEventInfo();
		p.WriteInt(pEventInfo->id);
		p.WriteShort(pEventInfo->mapInfo.uiStageID);
		p.WriteShort(pEventInfo->mapInfo.uiMapGroupID);
		p.WriteShort(pEventInfo->mapInfo.uiMapID);
		p.WriteFloat(pEventInfo->posX);
		p.WriteFloat(pEventInfo->posY);
	}
	else
	{
		return;
	}
	m_pkParent->Send(&p);
}

void Handler::SendDelObject(GameObject* pObject)
{
	Packet p(200000701);
	p.WriteULong(pObject->GetGUID());
	m_pkParent->Send(&p);
}

void Handler::SendItem(int iContainerType, int iSlotIndex)
{
	ItemArchive* pItemArchive = m_pkParent->GetPlayer()->GetItemArchive();
	Packet p(200000900);
	p.WriteLong(m_pkParent->GetPlayer()->GetGUID());
	WriteItemNotNull(p, pItemArchive, iContainerType, iSlotIndex);
	p.WriteByte(0);
	m_pkParent->Send(&p);
}

void Handler::SendItemList(int iContainerType)
{
	ItemArchive* pItemArchive = m_pkParent->GetPlayer()->GetItemArchive();
	Packet p(200000901);
	p.WriteLong(m_pkParent->GetPlayer()->GetGUID());
	int iCountPos = p.pos;
	p.WriteShort(0);
	int iCount = WriteItems(p, pItemArchive, iContainerType);
	p.Seek(iCountPos, SEEK_SET);
	p.WriteShort(iCount);
	m_pkParent->Send(&p);
}

void Handler::SendItemChangeSlot(int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo)
{
	Packet p(200000905);
	p.WriteLong(m_pkParent->GetPlayer()->GetGUID());
	p.WriteInt(iContainerFrom);
	p.WriteByte(iSlotFrom);
	p.WriteByte(iCount);
	p.WriteInt(iContainerTo);
	p.WriteByte(iSlotTo);
	m_pkParent->Send(&p);
}

void Handler::SendItemTrash(int iContainer, int iSlot, int iCount)
{
	Packet p(200000907);
	p.WriteLong(m_pkParent->GetPlayer()->GetGUID());
	p.WriteByte(1);
	p.WriteInt(iContainer);
	p.WriteByte(iSlot);
	p.WriteByte(iCount);
	m_pkParent->Send(&p);
}

void Handler::SendItemUsing(int iRetCode, int iContainer, int iSlot, int iCount)
{
	Packet p(200000910);
	p.WriteLong(m_pkParent->GetPlayer()->GetGUID());
	p.WriteInt(iRetCode);
	if (iRetCode == 0)
	{
		p.WriteInt(iContainer);
		p.WriteByte(iSlot);
		p.WriteByte(iCount);
	}
	m_pkParent->Send(&p);
}

void Handler::SendToggleWeaponSet(Player* pkPlayer, int iRetCode, int iWeaponSet)
{
	Packet p(200000915);
	p.WriteULong(pkPlayer->GetGUID());
	p.WriteByte(iRetCode);	//success
	p.WriteByte(iWeaponSet);	//weapon set
	m_pkParent->Send(&p);
}

void Handler::SendItemDivide(int iRetCode, int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo, Item* pkItemTo)
{
	Packet p(200000921);
	p.WriteLong(m_pkParent->GetPlayer()->GetGUID());
	p.WriteInt(iRetCode);
	if (iRetCode == 0)
	{
		p.WriteInt(iContainerFrom);
		p.WriteByte(iSlotFrom);
		p.WriteByte(iCount);
		p.WriteInt(iContainerTo);
		p.WriteByte(iSlotTo);
		WriteItem(p, pkItemTo);
	}
	m_pkParent->Send(&p);
}

void Handler::SendPortal(int iRetCode, Player* pPlayer)
{
	Packet p(200000801);
	p.WriteByte(iRetCode);
	if (iRetCode == 0)
	{
		p.WriteByte(3);
		p.WriteInt(0);
		p.WriteShort(pPlayer->GetStageID());
		p.WriteShort(pPlayer->GetMapGroupID());
		p.WriteShort(0);
		p.WriteFloat(pPlayer->GetPosX());
		p.WriteFloat(pPlayer->GetPosY());
	}
	m_pkParent->Send(&p);
}

void Handler::SendChat(int iRetCode, int iChatType, std::string strMessage, Player* pkPlayer)
{
	Packet p(200001201);
	p.WriteInt(iRetCode);
	if (iChatType == 1)
	{
		p.WriteInt(iChatType);
		p.WriteShort(0);
		p.WriteL1String(strMessage);
		p.WriteULong(pkPlayer->GetGUID());
		p.WriteL1String(pkPlayer->GetName());
	}
	else
	{
		return;
	}
	m_pkParent->Send(&p);
}

void Handler::SendQuestList()
{
	Packet p(200001600);
	p.WriteLong(m_pkParent->GetPlayer()->GetGUID());
	std::set<int>& sQuestList = m_pkParent->GetPlayer()->GetQuestArchive()->GetQuestList();
	p.WriteInt(sQuestList.size());
	for (std::set<int>::iterator iter = sQuestList.begin(); iter != sQuestList.end(); ++iter)
	{
		QuestInfo* pkInfo = QuestManager::GetInstance()->GetQuestInfo(*iter);
		p.WriteInt(pkInfo->iQuestID);
		p.WriteInt(0);
		p.WriteByte(0);
		p.WriteByte(pkInfo->iQuestType);
	}
	m_pkParent->Send(&p);
}

void Handler::SendSkillList()
{
	Packet p(200001800);
	p.WriteULong(m_pkParent->GetPlayer()->GetGUID());
	stdext::hash_map<int, SkillActivity*>& hmSkills = m_pkParent->GetPlayer()->GetSkillArchive()->GetSkills();
	p.WriteShort(hmSkills.size());
	for (stdext::hash_map<int, SkillActivity*>::iterator iter = hmSkills.begin(); iter != hmSkills.end(); ++iter)
	{
		SkillActivity* pkSkill = iter->second;
		p.WriteInt(pkSkill->iSkillID);
		p.WriteShort(pkSkill->iLv);
		p.WriteShort(pkSkill->iLv);
		p.WriteShort(0);
		p.WriteByte(0);
	}
	m_pkParent->Send(&p);
}

void Handler::SendSkillUsing(int iRetCode, int iSkillID, Player* pkPlayer)
{
	Packet p(200001803);
	p.WriteByte(iRetCode);
	p.WriteULong(pkPlayer->GetGUID());
	p.WriteInt(iSkillID);
	WritePosition(p, pkPlayer->GetCoordPhysics());
	m_pkParent->Send(&p);
}

void Handler::SendSkillLearn(int iRetCode, SkillActivity* pkSkill)
{
	Packet p(200001811);
	p.WriteInt(iRetCode);
	p.WriteULong(m_pkParent->GetPlayer()->GetGUID());
	p.WriteInt(pkSkill->iSkillID);
	p.WriteShort(pkSkill->iLv);
	p.WriteShort(pkSkill->iLv);
	p.WriteShort(0);
	p.WriteByte(0);
	p.WriteLong(m_pkParent->GetPlayer()->GetStatus()->GetStatusValueEx(STAT_EX_ELY));
	p.WriteHexString("FFFFFFFF000000");
	m_pkParent->Send(&p);
}

void Handler::SendSkillLvUp(int iRetCode, SkillActivity* pkSkill)
{
	Packet p(200001807);
	p.WriteByte(iRetCode);
	p.WriteULong(m_pkParent->GetPlayer()->GetGUID());
	p.WriteShort(m_pkParent->GetPlayer()->GetStatus()->GetStatusValue(STAT_SKILL_POINT));
	p.WriteInt(pkSkill->iSkillID);
	p.WriteShort(pkSkill->iLv);
	p.WriteShort(pkSkill->iLv);
	p.WriteShort(0);
	p.WriteByte(0);
	m_pkParent->Send(&p);
}