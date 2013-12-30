#include "stdafx.h"

Player::Player()
{
	this->m_ClassID = CLASS_ID_AVATAR;
	m_pkWorld = NULL;
	m_pkCoordPhysics = StageManager::GetInstance()->GetCoordModule()->CreateCoord(this, 1);
	m_pkStatus = new PlayerStatus(this);
	m_pkEffectArchive = new EffectArchive(this);
	m_pkQuestArchive = new QuestArchive(this);
	m_pkSkillArchive = new SkillArchive(this);
	m_pkItemArchive = new ItemArchive(this);
	m_vKeySet.resize(0x60);
	m_iCharID = 0;
	m_iMotionID = 0;
	m_iMotionTypeID = 1;
}

Player::~Player()
{
	delete this->m_pkSkillArchive;
	delete this->m_pkQuestArchive;
	delete this->m_pkItemArchive;
	delete this->m_pkEffectArchive;
	StageManager::GetInstance()->GetCoordModule()->DeleteCoord(this->m_pkCoordPhysics);
	delete this->m_pkStatus;
}

void Player::Init()
{

}

void Player::InitPlayer()
{
	GetConnHandler()->SendInitChar(0, this);

	m_pkItemArchive->InitPlayer();

	std::vector<KeySet> vKeySet;
	for (int i = 0; i < m_vKeySet.size(); ++i)
	{
		if (m_vKeySet[i].iType != 0)
		{
			vKeySet.push_back(m_vKeySet[i]);
		}
	}
	GetConnHandler()->SendKeySet(vKeySet);

	m_pkQuestArchive->InitPlayer();
	m_pkSkillArchive->InitPlayer();

	GetConnHandler()->SendChannelLoginFinish();
}

void Player::RecvChangeFacing(int iDirection)
{
	m_pkCoordPhysics->SetDirection(iDirection);
}

void Player::RecvKeySet(std::vector<KeySet>& vKeySet)
{
	for (int i = 0; i < vKeySet.size(); ++i)
	{
		m_vKeySet[vKeySet[i].iSlot] = vKeySet[i];
	}
	GetConnHandler()->SendKeySet(vKeySet);
}

void Player::RecvAction(uint64 uiAction, int iSeqIndex)
{
	uint64 uiCurAction = this->GetCoordPhysics()->GetCurAction();
	//HACKFIX: 避免延迟很小的情况下服务端还在下落时客户端已经发来下一次跑造成速度叠加 正确方法为CheckStand检查
	//但是要排除掉跳的情况 游戏是允许在下落过程中起跳的 主要为了避免延迟高的边缘掉落
	if (((uiAction & 0x800) || (uiAction & 0x1000)) && !(uiAction & 0x10))
	{
		if (uiCurAction & 0x20)
		{
			GetConnHandler()->SendAction(-1, NULL);	//必须返回动作失败使客户端重发
			return;
		}
	}
		
	this->GetCoordPhysics()->SetAction(uiAction);

	if ((uiAction & 0x4000) || (uiAction & 0x8000) || (uiAction & 0x800000000ui64) || (uiAction & 0x1000000000ui64))
	{
		GetCoordPhysics()->SetSeqIndex(iSeqIndex);
	}
	SetMotion(TO<PlayerStatus*>(m_pkStatus)->GetClassID() * 100000 + m_iMotionTypeID * 100 + GetCoordPhysics()->GetCurSeqIndex());
	BroadcastAction();
}

void Player::RecvPortal(int iEventType, int iEventID)
{
	for (InRangeIter iter = InRangeBegin(); iter != InRangeEnd(); ++iter)
	{
		if ((*iter)->IsMapEvent())
		{
			MapEvent* pMapEvent = TO<MapEvent*>(*iter);
			if (pMapEvent->GetEventID() == iEventID && iEventType == 3)
			{
				EVENTPORTAL_INFO *pEventPortal = EventManager::GetInstance()->GetEventPortal(pMapEvent->GetEventInfo()->eventTypeID);
				this->DoPortal(pEventPortal->m_iStageID, pEventPortal->m_iMapGroupID, pEventPortal->m_fPosX, pEventPortal->m_fPosY);
				break;
			}
		}
	}
}

void Player::RecvPortalFinish()
{
	for (InRangeIter iter = InRangeBegin(); iter != InRangeEnd(); ++iter)
	{
		if (*iter != this)
		{
			SendAddObject(*iter);
			if ((*iter)->IsPlayer())
				TO<Player*>(*iter)->SendAddObject(this);
		}
	}
	this->GetCoordPhysics()->SetAction(0);
	BroadcastAction();
}

void Player::RecvStartNPCTalk()
{

}

void Player::RecvEndNPCTalk()
{

}

void Player::RecvChat(int iChatType, std::string strMessage, std::string strToChar)
{
	if (iChatType == 1)
	{
		for (InRangeIter iter = InRangeBegin(); iter != InRangeEnd(); ++iter)
		{
			if ((*iter)->IsPlayer())
				TO<Player*>(*iter)->GetConnHandler()->SendChat(0, iChatType, strMessage, this);
		}
	}
}

void Player::Process(float fTimeSpan)
{
	m_pkCoordPhysics->Process(fTimeSpan);
	this->CheckUpdateMapCell();
	m_pkEffectArchive->Update();
}

void Player::OnActionCommand(uint64 uiAction)
{
	uint64 uiCurAction = m_pkCoordPhysics->GetCurAction();
	uiCurAction |= uiAction;
	m_pkCoordPhysics->SetAction(uiCurAction);
	SetMotion(TO<PlayerStatus*>(m_pkStatus)->GetClassID() * 100000 + m_iMotionTypeID * 100 + GetCoordPhysics()->GetCurSeqIndex());
}

void Player::OnActionCompleteCommand(uint64 uiAction)
{
	uint64 uiCurAction = m_pkCoordPhysics->GetCurAction();
	uiCurAction &= ~uiAction;
	m_pkCoordPhysics->SetAction(uiCurAction);
	//如果这里SetMotion则导致跑的速度叠加
	//SetMotion(TO<PlayerStatus*>(m_pkStatus)->GetClassID() * 100000 + m_iMotionTypeID * 100 + GetCoordPhysics()->GetCurSeqIndex());
}

void Player::SendAddObject(GameObject *pObject)
{
	assert(this != pObject);
	GetConnHandler()->SendAddObject(pObject);
}

void Player::SendDelObject(GameObject *pObject)
{
	assert(this != pObject);
	GetConnHandler()->SendDelObject(pObject);
}

void Player::BroadcastAction()
{
	for (InRangeIter iter = InRangeBegin(); iter != InRangeEnd(); ++iter)
	{
		if ((*iter)->IsPlayer())
			TO<Player*>(*iter)->GetConnHandler()->SendAction(0, this);
	}
}

void Player::BroadcastStatus()
{
	for (InRangeIter iter = InRangeBegin(); iter != InRangeEnd(); ++iter)
		if ((*iter)->IsPlayer())
			TO<Player*>(*iter)->GetConnHandler()->SendCharStatus(this);
}

void Player::DoPortal(int iStageID, int iMapGroupID, float fPosX, float fPosY)
{
	//TODO: 当前版本同图不发802，故这里需要判断同图只改坐标，然后由MapCell检测发700
	if (iStageID >= StageManager::GetInstance()->GetStageCount() || iMapGroupID >= StageManager::GetInstance()->GetStageInfo(iStageID)->GetMapGroupCount()) return;
	GetCoordPhysics()->SetPosX(fPosX);
	GetCoordPhysics()->SetPosY(fPosY);
	BroadcastAction();
	if (GetStageID() != iStageID || GetMapGroupID() != iMapGroupID)
	{
		m_pkCurMapGroup->RemoveObject(this);
		SetStageID(iStageID);
		SetMapGroupID(iMapGroupID);
		GetCoordPhysics()->RaiseMapChange();
		this->m_pkWorld->GetMapGroup(iStageID, iMapGroupID)->InsertObject(this);
	}
	GetConnHandler()->SendPortal(0, this);
}

void Player::SetMotion(int iMotionID)
{
	//if (iMotionID == m_iMotionID) return;
	m_iMotionID = iMotionID;
	MotionInfo* pkMotionInfo = SkillManager::GetInstance()->GetPlayerMotionInfo(iMotionID);
	if (pkMotionInfo != NULL)
	{
		m_pkCoordPhysics->SetActionTime(pkMotionInfo->fTime);
	}
	MotionStatus* pkMotionStatus = SkillManager::GetInstance()->GetPlayerMotionStatus(iMotionID);
	if (pkMotionStatus == NULL) return;
	m_pkEffectArchive->ApplyEffect(pkMotionStatus->m_ApplyEffectInfo.iSelfEffect[0]);
	//TODO effect delay is not implemented currently

	if (pkMotionStatus->m_ActivityVelocity.fTime != 0.0f)
	{
		GetCoordPhysics()->SetActivityVelocity(
			pkMotionStatus->m_ActivityVelocity.fDelayTime,
			pkMotionStatus->m_ActivityVelocity.fTime,
			GetCoordPhysics()->GetDirection() == 0 ? -pkMotionStatus->m_ActivityVelocity.fVelocityX : pkMotionStatus->m_ActivityVelocity.fVelocityX,
			pkMotionStatus->m_ActivityVelocity.fVelocityY);
	}
}

bool Player::NewChar(std::string strCharName, int iUserID, int iClassType, int iGender, std::vector<int> vItemID)
{
	this->SetName(strCharName);
	LDTFile ldt("CREATION_INFO");
	HANDLE hEnum =ldt.BeginEnum();
	LDTRecord *record;
	while((record = ldt.EnumRecord(hEnum)) != NULL)
	{
		if (record->GetInt("_Class") == iClassType && record->GetInt("_Gender") == iGender)
			break;
	}
	ldt.EndEnum(hEnum);
	std::string sql =
		"INSERT INTO `characters` ("
		"`user_id`, `name`, `gender`, `class_type`, `lv`"
		") VALUES (" +
		Convert::ToString(iUserID) + ", " +
		"'" + strCharName + "', " + 
		Convert::ToString(iGender) + ", " +
		Convert::ToString(iClassType) + ", 1)";
	DBResult* result = DataBase::GetInstance()->Query(sql);
	int iCharID = result->LastInsertId();
	result->Free();
	m_iCharID = iCharID;
	PlayerStatus *pStatus = TO<PlayerStatus*>(GetStatus());
	pStatus->SetStatusValue(STAT_LV, 200);	//hahaha
	pStatus->SetStatusValue(STAT_CLASS, iClassType);
	pStatus->LoadBaseStatus();
	pStatus->SetStatusValue(STAT_GENDER, iGender);
	pStatus->SetStatusValue(STAT_BASE_LUK, record->GetInt("_LV_Value"));
	pStatus->SetStatusValue(STAT_BASE_ATK, record->GetInt("_Base_Atk"));
	pStatus->SetStatusValue(STAT_BASE_DEF, record->GetInt("_Base_Def"));
	pStatus->SetStatusValue(STAT_BASE_ELE, record->GetInt("_Base_Ele"));
	pStatus->SetStatusValue(STAT_HP, record->GetInt("_HP_Value"));
	pStatus->SetStatusValue(STAT_SP, record->GetInt("_SP_Value"));
	pStatus->SetStatusValueEx(STAT_EX_EXP, record->GetInt("_Exp_Value"));
	pStatus->SetStatusValueEx(STAT_EX_EXP, pStatus->GetStatusValueEx(STAT_EX_EXP_MIN));
	pStatus->SetStatusValueEx(STAT_EX_ELY, record->GetInt("_Money_Value"));
	pStatus->SetStatusValueEx(STAT_EX_ELY, 100000000);	//hahaha
	
	pStatus->SetStatusValue(STAT_SKILL_POINT, record->GetInt("_SkillPoint"));
	pStatus->SetStatusValue(STAT_SKILL_POINT, 200);	//hahaha
	this->SetStageID(record->GetInt("_Stage_ID"));
	this->SetMapGroupID(record->GetInt("_MapGroup"));
	GetCoordPhysics()->RaiseMapChange();
	SetPosX(record->GetInt("_Pos_X"));
	SetPosY(record->GetInt("_Pos_Y"));
	m_pkItemArchive->ResizeSlot();
	for (int i = 0; i < vItemID.size(); ++i)
	{
		Item item(vItemID[i]);
		item.iStackCount = 1;
		int iContainerType = 8;
		m_pkItemArchive->CreateSlotItem(iContainerType, item.pItemInfo->iPosID1, item);
	}
	for (int i = 1; i <= 4; ++i)
	{
		int iItemID = record->GetInt("_Item_ID" + Convert::ToString(i));
		if (iItemID != 0)
		{
			Item item(iItemID);
			item.iStackCount = record->GetInt("_Item_Stack" + Convert::ToString(i));
			int iContainerType = record->GetInt("_Inventory_Type" + Convert::ToString(i));
			int iPos = record->GetInt("_Inventory_Slot" + Convert::ToString(i));
			m_pkItemArchive->CreateSlotItem(iContainerType, iPos, item);
		}
	}
	for (int i = 1; i <= 10; ++i)
	{
		KeySet key;
		key.iSlot = record->GetInt("_Quick_Slot_No" + Convert::ToString(i));
		key.iType = record->GetInt("_Quick_Slot_Type" + Convert::ToString(i));
		key.iValue1 = record->GetInt("_Quick_Slot_ID" + Convert::ToString(i));
		key.iValue2 = 0;
		if (key.iType != 0)
		{
			m_vKeySet.push_back(key);
		}
	}
	record->Free();

	return true;
}

bool Player::LoadGame(std::string strCharName)
{
	this->SetName(strCharName);
	DBResult *result = DataBase::GetInstance()->Query("SELECT * FROM `characters` WHERE `name` = '" + strCharName + "'");
	if (!result->HasRow()) return false;
	result->ReadRow();
	m_iCharID = result->GetInt("id");
	SetInstanceID(m_iCharID);

	PlayerStatus *pStatus = TO<PlayerStatus*>(GetStatus());
	pStatus->SetStatusValue(STAT_LV, result->GetInt("lv"));
	pStatus->SetStatusValue(STAT_REQUIRE_LV, result->GetInt("lv"));
	pStatus->SetStatusValue(STAT_CLASS, result->GetInt("class_type"));
	pStatus->LoadBaseStatus();
	pStatus->SetStatusValue(STAT_GENDER, result->GetInt("gender"));
	pStatus->SetStatusValue(STAT_BASE_LUK, result->GetInt("luk_base"));
	pStatus->SetStatusValue(STAT_BASE_ATK, result->GetInt("atk_base"));
	pStatus->SetStatusValue(STAT_BASE_DEF, result->GetInt("def_base"));
	pStatus->SetStatusValue(STAT_BASE_ELE, result->GetInt("ele_base"));
	pStatus->SetStatusValue(STAT_HP, result->GetInt("hp"));
	pStatus->SetStatusValue(STAT_SP, result->GetInt("sp"));
	pStatus->SetStatusValue(STAT_SKILL_POINT, result->GetInt("skill_point"));
	pStatus->SetStatusValue(STAT_PVP_LV, result->GetInt("pvp_lv"));
	pStatus->SetStatusValue(STAT_PVP_POINT, result->GetInt("pvp_point"));

	pStatus->SetStatusValueEx(STAT_EX_EXP, result->GetInt64("exp"));
	pStatus->SetStatusValueEx(STAT_EX_ELY, result->GetInt64("ely"));
	pStatus->SetStatusValueEx(STAT_EX_STORAGE_ELY, result->GetInt64("storage_ely"));

	this->SetStageID(result->GetInt("stage_id"));
	this->SetMapGroupID(result->GetInt("mapgroup_id"));
	GetCoordPhysics()->RaiseMapChange();
	SetPosX(result->GetInt("pos_x"));
	SetPosY(result->GetInt("pos_y"));
	m_pkItemArchive->ResizeSlot();
	m_pkItemArchive->LoadDB();
	m_pkSkillArchive->LoadDB();

	result->Free();

	result = DataBase::GetInstance()->Query("SELECT * FROM `keys` WHERE `char_id` = " + Convert::ToString(this->GetCharID()));
	while (result->ReadRow())
	{
		int iSlot = result->GetInt("slot");
		m_vKeySet[iSlot].iSlot = iSlot;
		m_vKeySet[iSlot].iType = result->GetInt("type");
		m_vKeySet[iSlot].iValue1 = result->GetInt("value1");
		m_vKeySet[iSlot].iValue2 = result->GetInt("value2");
	}
	result->Free();
	return true;
}

bool Player::SaveGame()
{
	PlayerStatus* pStatus = TO<PlayerStatus*>(GetStatus());
	DBResult *result = DataBase::GetInstance()->Query(
		"UPDATE `characters` SET "
		"`lv` = " + Convert::ToString(pStatus->GetStatusValue(STAT_LV)) + ", " +
		"`ely` = " + Convert::ToString(pStatus->GetStatusValueEx(STAT_EX_ELY)) + ", " +
		"`exp` = " + Convert::ToString(pStatus->GetStatusValueEx(STAT_EX_EXP)) + ", " +
		"`class_type` = " + Convert::ToString(pStatus->GetStatusValue(STAT_CLASS)) + ", " +
		"`gender` = " + Convert::ToString(pStatus->GetStatusValue(STAT_GENDER)) + ", " +
		"`luk_base` = " + Convert::ToString(pStatus->GetStatusValue(STAT_BASE_LUK)) + ", " +
		"`atk_base` = " + Convert::ToString(pStatus->GetStatusValue(STAT_BASE_ATK)) + ", " +
		"`def_base` = " + Convert::ToString(pStatus->GetStatusValue(STAT_BASE_DEF)) + ", " +
		"`ele_base` = " + Convert::ToString(pStatus->GetStatusValue(STAT_BASE_ELE)) + ", " +
		"`hp` = " + Convert::ToString(pStatus->GetStatusValue(STAT_HP)) + ", " +
		"`sp` = " + Convert::ToString(pStatus->GetStatusValue(STAT_SP)) + ", " +
		"`skill_point` = " + Convert::ToString(pStatus->GetStatusValue(STAT_SKILL_POINT)) + ", " +
		"`stage_id` = " + Convert::ToString(GetStageID()) + ", " +
		"`mapgroup_id` = " + Convert::ToString(GetMapGroupID()) + ", " +
		"`pos_x` = " + Convert::ToString((int)this->GetPosX()) + ", " +
		"`pos_y` = " + Convert::ToString((int)this->GetPosY()) + " " +
		"WHERE `name` = '" + this->GetName() + "'");
	result->Free();
	m_pkItemArchive->SaveDB();
	m_pkSkillArchive->SaveDB();

	//KeySet
	result = DataBase::GetInstance()->Query(
		"DELETE FROM `keys` WHERE `char_id` = " + Convert::ToString(GetCharID())
		);
	result->Free();
	for (int i = 0; i < m_vKeySet.size(); ++i)
	{
		if (m_vKeySet[i].iType != 0)
		{
			result = DataBase::GetInstance()->Query(
				"INSERT INTO `keys`("
				"`char_id`, `slot`, `type`, `value1`, `value2`"
				") VALUES (" +
				Convert::ToString(GetCharID()) + ", " +
				Convert::ToString(m_vKeySet[i].iSlot) + ", " +
				Convert::ToString(m_vKeySet[i].iType) + ", " +
				Convert::ToString(m_vKeySet[i].iValue1) + ", " +
				Convert::ToString(m_vKeySet[i].iValue2) +
				")"
				);
			result->Free();
		}
	}
	return true;
}