#include "stdafx.h"

ItemManager* Singleton<ItemManager>::m_pkInstance = NULL;

void ItemManager::Init()
{
	LoadLDTItems();
	InitItemInsertNo();
}

void ItemManager::LoadLDTItems()
{
	for (int iTable = 1; iTable <= 3; ++iTable)
	{
		LDTFile ldt("ITEM_" + Convert::ToString(iTable));
		void* h = ldt.BeginEnum();
		LDTRecord *r;
		while ((r = ldt.EnumRecord(h)) != NULL)
		{
			ItemInfo *pInfo = new ItemInfo();
			pInfo->iID = r->GetId();
			pInfo->iGamePrice = r->GetInt("_GamePrice");
			pInfo->iType = (ITEM_TYPE)r->GetInt("_Type");
			pInfo->iSubType = (ITEM_SUBTYPE)r->GetInt("_SubType");
			pInfo->iPosID1 = (ITEM_EQUIPPOS)r->GetInt("_PosID1");
			pInfo->iSubPosID11 = (ITEM_EQUIPPOS)r->GetInt("_SubPosID11");
			pInfo->iSubPosID12 = (ITEM_EQUIPPOS)r->GetInt("_SubPosID12");
			pInfo->iPosID2 = (ITEM_EQUIPPOS)r->GetInt("_PosID2");
			pInfo->iSubPosID21 = (ITEM_EQUIPPOS)r->GetInt("_SubPosID21");
			pInfo->iSubPosID22 = (ITEM_EQUIPPOS)r->GetInt("_SubPosID22");
			for (int i = 0; i < 4; ++i)
			{
				pInfo->iStatusType[i] = (CREATURE_STATUS)r->GetInt("_StatusType" + Convert::ToString(i + 1));
				pInfo->iStatusValue[i] = r->GetInt("_StatusValue" + Convert::ToString(i + 1));
			}
			pInfo->iStackLimit = r->GetInt("_StackLimit");
			pInfo->iItemLv = r->GetInt("_ItemLv");
			pInfo->iMotionTypeID1 = r->GetInt("_MotionTypeID1");
			pInfo->iMotionTypeID2 = r->GetInt("_MotionTypeID2");
			
			m_hmItemInfo[pInfo->iID] = pInfo;
			r->Free();
		}
		ldt.EndEnum(h);
	}
}

void ItemManager::InitItemInsertNo()
{
	DBResult* r = DataBase::GetInstance()->Query("SELECT MAX(`id`) AS `id` FROM `items`");
	m_iItemInsertNo = r->GetInt("id") + 1;
	r->Free();
}

ItemArchive::ItemArchive(Player* pkPlayer)
{
	m_pkParent = pkPlayer;
	m_cCurWeaponSet = 0;
}

ItemArchive::~ItemArchive()
{
	ClearSlotItems();
}

void ItemArchive::ClearSlotItems()
{
	for (int iContainerType = 0; iContainerType < ContainerMax; ++iContainerType)
	{
		for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(iContainerType); ++iSlotIndex)
		{
			Item* pkItem = GetInvenItem(iContainerType, iSlotIndex);
			if (pkItem != NULL)
			{
				SetInvenItem(iContainerType, iSlotIndex, NULL);
				delete pkItem;
			}
		}
	}
}

void ItemArchive::ResizeSlot()
{
	m_vItems[0].resize(28 + 4 * m_pkParent->GetStatus()->GetStatusValue(STAT_EQUIPMENT_SLOT));
	m_vItems[1].resize(28 + 4 * m_pkParent->GetStatus()->GetStatusValue(STAT_CONSUMPTION_SLOT));
	m_vItems[2].resize(28 + 4 * m_pkParent->GetStatus()->GetStatusValue(STAT_ETC_SLOT));
	m_vItems[3].resize(28 + 4 * m_pkParent->GetStatus()->GetStatusValue(STAT_EVENT_SLOT));
	m_vItems[4].resize(4 + 4 * m_pkParent->GetStatus()->GetStatusValue(STAT_PET_SLOT));
	m_vItems[5].resize(28 + 4 * m_pkParent->GetStatus()->GetStatusValue(STAT_HOUSING_SLOT));
	m_vItems[6].resize(16);
	m_vItems[7].resize(16);
	m_vItems[8].resize(16);
	m_vItems[9].resize(4 + 4 * m_pkParent->GetStatus()->GetStatusValue(STAT_STORAGE_SLOT));
	m_vItems[10].resize(80);
	m_vItems[11].resize(16);
	m_vItems[12].resize(16);
	m_vItems[13].resize(16);
}

void ItemArchive::InitPlayer()
{
	ResizeSlot();
	m_pkParent->GetConnHandler()->SendItemList(0);
    m_pkParent->GetConnHandler()->SendItemList(1);
	m_pkParent->GetConnHandler()->SendItemList(2);
	m_pkParent->GetConnHandler()->SendItemList(3);
	m_pkParent->GetConnHandler()->SendItemList(4);
	m_pkParent->GetConnHandler()->SendItemList(5);
	m_pkParent->GetConnHandler()->SendItemList(6);
	m_pkParent->GetConnHandler()->SendItemList(7);
	m_pkParent->GetConnHandler()->SendItemList(8);
	m_pkParent->GetConnHandler()->SendItemList(9);

	for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(FIGURE_EQUIP); ++iSlotIndex)
	{
		Item* pkItem = GetInvenItem(FIGURE_EQUIP, iSlotIndex);
		if (pkItem != NULL)
			OnSetEquipStatus(pkItem);
	}
	for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(FIGURE_FASHION); ++iSlotIndex)
	{
		Item* pkItem = GetInvenItem(FIGURE_FASHION, iSlotIndex);
		if (pkItem != NULL)
			OnSetEquipStatus(pkItem);
	}
	for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(FIGURE_BASIC); ++iSlotIndex)
	{
		Item* pkItem = GetInvenItem(FIGURE_BASIC, iSlotIndex);
		if (pkItem != NULL)
			OnSetEquipStatus(pkItem);
	}
	for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(FIGURE_PET); ++iSlotIndex)
	{
		Item* pkItem = GetInvenItem(FIGURE_PET, iSlotIndex);
		if (pkItem != NULL)
			OnSetEquipStatus(pkItem);
	}
	m_pkParent->BroadcastStatus();
	CheckUpdateMotionType();
}

void ItemArchive::RecvChangeSlot(int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo)
{
	//TODO 装备交换时的逻辑判断。否则肯定会卡盾哦
	//iCount在交换中基本是忽略的
	Item* pItemFrom = GetInvenItem(iContainerFrom, iSlotFrom);
	Item* pItemTo = GetInvenItem(iContainerTo, iSlotTo);
	if (pItemFrom == NULL)
		return;
	bool bFigureChange = false;
	if (IsFigureContainer(iContainerTo))	//从包裹到装备栏
	{
		//待装备的物品需要占用的SubPos
		int iFromSubPosID1, iFromSubPosID2;
		if (iSlotTo == pItemFrom->pItemInfo->iPosID1)
		{
			iFromSubPosID1 = pItemFrom->pItemInfo->iSubPosID11;
			iFromSubPosID2 = pItemFrom->pItemInfo->iSubPosID12;
		}
		else if (iSlotTo == pItemFrom->pItemInfo->iPosID2)
		{
			iFromSubPosID1 = pItemFrom->pItemInfo->iSubPosID21;
			iFromSubPosID2 = pItemFrom->pItemInfo->iSubPosID22;
		}		
		for (int iSlot = 1; iSlot <= GetInvenSize(iContainerTo); ++iSlot)
		{
			if (iSlot == iSlotTo)
				continue;
			Item* pkItem = GetInvenItem(iContainerTo, iSlot);
			if (pkItem != NULL)
			{
				//已装备的物品占用的SubPos
				int iToSubPosID1, iToSubPosID2;
				if (iSlot == pkItem->pItemInfo->iPosID1)
				{
					iToSubPosID1 = pkItem->pItemInfo->iSubPosID11;
					iToSubPosID2 = pkItem->pItemInfo->iSubPosID12;
				}
				else if (iSlot == pkItem->pItemInfo->iPosID2)
				{
					iToSubPosID1 = pkItem->pItemInfo->iSubPosID21;
					iToSubPosID2 = pkItem->pItemInfo->iSubPosID22;
				}
				if ((iSlot == iFromSubPosID1 || iSlot == iFromSubPosID2) ||
					(iToSubPosID1 != 0 && (iToSubPosID1 == iSlotTo || iToSubPosID1 == iFromSubPosID1 || iToSubPosID1 == iFromSubPosID2)) ||
					(iToSubPosID2 != 0 && (iToSubPosID2 == iSlotTo || iToSubPosID2 == iFromSubPosID1 || iToSubPosID2 == iFromSubPosID2)))
				{	//拿掉
					int iSlotEmpty = FindEmptySlotIndex(iContainerFrom);
					if (iSlotEmpty == 0)
						return;
					SetInvenItem(iContainerTo, iSlot, NULL);
					SetInvenItem(iContainerFrom, iSlotEmpty, pkItem);
					OnUnsetEquipStatus(pkItem);
					m_pkParent->GetConnHandler()->SendItemChangeSlot(iContainerTo, iSlot, pkItem->iStackCount, iContainerFrom, iSlotEmpty);
				}
			}
		}

	}
	if (pItemTo != NULL && pItemTo->iItemID == pItemFrom->iItemID &&
		pItemTo->pItemInfo->iStackLimit - pItemTo->iStackCount > 0)	//合并情况
	{
		if (iCount > pItemTo->pItemInfo->iStackLimit - pItemTo->iStackCount)
			iCount = pItemTo->pItemInfo->iStackLimit - pItemTo->iStackCount;
		RemoveItem(iContainerFrom, iSlotFrom, iCount);
		pItemTo->iStackCount += iCount;
	}
	else
	{
		if (IsFigureContainer(iContainerFrom))
		{
			OnUnsetEquipStatus(pItemFrom);
			bFigureChange = true;
		}
		else if (IsFigureContainer(iContainerTo))
		{
			if (pItemTo != NULL)
				OnUnsetEquipStatus(pItemTo);
			OnSetEquipStatus(pItemFrom);
			bFigureChange = true;
		}
		SetInvenItem(iContainerFrom, iSlotFrom, pItemTo);
		SetInvenItem(iContainerTo, iSlotTo, pItemFrom);
	}
	m_pkParent->GetConnHandler()->SendItemChangeSlot(iContainerFrom, iSlotFrom, iCount, iContainerTo, iSlotTo);
	if (bFigureChange)
	{
		CheckUpdateMotionType();
		m_pkParent->BroadcastStatus();
	}
}

void ItemArchive::RecvTrash(int iContainer, int iSlot, int iCount)
{
	if (GetInvenItem(iContainer, iSlot) == NULL)
		return;
	RemoveItem(iContainer, iSlot, iCount);
	m_pkParent->GetConnHandler()->SendItemTrash(iContainer, iSlot, iCount);
}

void ItemArchive::RecvUsing(int iContainer, int iSlot)
{
	//TODO
}

void ItemArchive::RecvToggleWeaponSet()
{
	if (m_cCurWeaponSet == 0)
		m_cCurWeaponSet = 1;
	else if (m_cCurWeaponSet == 1)
		m_cCurWeaponSet = 0;
	for (InRangeIter iter = m_pkParent->InRangeBegin(); iter != m_pkParent->InRangeEnd(); ++iter)
	{
		if ((*iter)->IsPlayer())
			TO<Player*>(*iter)->GetConnHandler()->SendToggleWeaponSet(m_pkParent, 0, m_cCurWeaponSet);
	}
	CheckUpdateMotionType();
	m_pkParent->BroadcastStatus();	//客户端只有在status更新时才会将技能栏中不对的技能变灰
}

void ItemArchive::RecvDivide(int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo)
{
	Item* pItemFrom = GetInvenItem(iContainerFrom, iSlotFrom);
	Item* pItemTo = GetInvenItem(iContainerTo, iSlotTo);
	if (pItemFrom == NULL || pItemTo != NULL)
		return;
	Item NewItem = *pItemFrom;
	NewItem.iStackCount = iCount;
	RemoveItem(iContainerFrom, iSlotFrom, iCount);
	CreateSlotItem(iContainerTo, iSlotTo, NewItem);
	m_pkParent->GetConnHandler()->SendItemDivide(0, iContainerFrom, iSlotFrom, iCount, iContainerTo, iSlotTo, m_vItems[iContainerTo][iSlotTo - 1]);
}

void ItemArchive::AddItem(Item& stItem)
{
	int iContainerType = GetContainerTypeFromItemType(stItem.pItemInfo->iType);
	if (iContainerType == -1)
		return;
	int iSlotIndex = FindAddItemSlotIndex(iContainerType, stItem.iItemID, stItem.iStackCount);
	if (iSlotIndex == 0)
	{
		iSlotIndex = FindEmptySlotIndex(iContainerType);
		if (iSlotIndex == 0)
		{
			//包包已满
			return;
		}
		CreateSlotItem(iContainerType, iSlotIndex, stItem);
	}
	else
	{
		GetInvenItem(iContainerType, iSlotIndex)->iStackCount += stItem.iStackCount;
	}
	m_pkParent->GetConnHandler()->SendItem(iContainerType, iSlotIndex);
}

void ItemArchive::RemoveItem(int iContainerType, int iSlotIndex, int iCount)
{
	Item* pItem = GetInvenItem(iContainerType, iSlotIndex);
	if (pItem == NULL)
		return;
	if (iCount < pItem->iStackCount)
		pItem->iStackCount -= iCount;
	else
		DeleteSlotItem(iContainerType, iSlotIndex);
}

//Item* ItemArchive::GetInvenItem(int iContainerType, int iSlotIndex)
//{
//	Item* pkItem = m_vItems[iContainerType][iSlotIndex - 1];
//	if (pkItem == NULL)
//		return NULL;
//	if (IsFigureContainer(iContainerType))
//	{
//		if (iSlotIndex != pkItem->pItemInfo->iPosID1 && iSlotIndex != pkItem->pItemInfo->iPosID2)
//			return NULL;
//	}
//	return pkItem;
//}

//bool ItemArchive::SetInvenItem(int iContainerType, int iSlotIndex, Item* pkItem, Item** ppkCurItem)
//{
//	Item* pkCurItem = m_vItems[iContainerType][iSlotIndex - 1];
//	if (IsFigureContainer(iContainerType))
//	{
//		//检查装备栏正确性
//		if (pkItem != NULL && iContainerType != GetEquipContainerTypeFromItemType(pkItem->pItemInfo->iType, pkItem->pItemInfo->iSubType))
//			return false;
//		if (pkCurItem != NULL) //把装备拿下来
//		{
//			if (iSlotIndex == pkCurItem->pItemInfo->iPosID1 ||
//				iSlotIndex == pkCurItem->pItemInfo->iSubPosID11 ||
//				iSlotIndex == pkCurItem->pItemInfo->iSubPosID12)
//			{
//				m_vItems[iContainerType][pkCurItem->pItemInfo->iPosID1 - 1] = NULL;
//				if (pkCurItem->pItemInfo->iSubPosID11 != 0)
//					m_vItems[iContainerType][pkCurItem->pItemInfo->iSubPosID11 - 1] = NULL;
//				if (pkCurItem->pItemInfo->iSubPosID12 != 0)
//					m_vItems[iContainerType][pkCurItem->pItemInfo->iSubPosID12 - 1] = NULL;
//			}
//			else if (iSlotIndex == pkCurItem->pItemInfo->iPosID2 ||
//				iSlotIndex == pkCurItem->pItemInfo->iSubPosID21 ||
//				iSlotIndex == pkCurItem->pItemInfo->iSubPosID22)
//			{
//				m_vItems[iContainerType][pkCurItem->pItemInfo->iPosID2 - 1] = NULL;
//				if (pkCurItem->pItemInfo->iSubPosID21 != 0)
//					m_vItems[iContainerType][pkCurItem->pItemInfo->iSubPosID21 - 1] = NULL;
//				if (pkCurItem->pItemInfo->iSubPosID22 != 0)
//					m_vItems[iContainerType][pkCurItem->pItemInfo->iSubPosID22 - 1] = NULL;
//			}
//			else
//			{
//				assert(false);
//				return false;
//			}
//			OnUnsetEquipStatus(pkCurItem);
//		}
//		//把新装备放上去
//		if (pkItem != NULL)
//		{
//			//考虑SubPos
//			if (iSlotIndex == pkItem->pItemInfo->iPosID1 ||
//				iSlotIndex == pkItem->pItemInfo->iSubPosID11 ||
//				iSlotIndex == pkItem->pItemInfo->iSubPosID12)
//			{
//				m_vItems[iContainerType][pkItem->pItemInfo->iPosID1 - 1] = pkItem;
//				if (pkItem->pItemInfo->iSubPosID11 != 0)
//					m_vItems[iContainerType][pkItem->pItemInfo->iSubPosID11 - 1] = pkItem;
//				if (pkItem->pItemInfo->iSubPosID12 != 0)
//					m_vItems[iContainerType][pkItem->pItemInfo->iSubPosID12 - 1] = pkItem;
//			}
//			else if (iSlotIndex == pkItem->pItemInfo->iPosID2 ||
//				iSlotIndex == pkItem->pItemInfo->iSubPosID21 ||
//				iSlotIndex == pkItem->pItemInfo->iSubPosID22)
//			{
//				m_vItems[iContainerType][pkItem->pItemInfo->iPosID2 - 1] = pkItem;
//				if (pkItem->pItemInfo->iSubPosID21 != 0)
//					m_vItems[iContainerType][pkItem->pItemInfo->iSubPosID21 - 1] = pkItem;
//				if (pkItem->pItemInfo->iSubPosID22 != 0)
//					m_vItems[iContainerType][pkItem->pItemInfo->iSubPosID22 - 1] = pkItem;
//			}
//			else
//			{
//				return false;
//				assert(false);
//			}
//			OnSetEquipStatus(pkItem);
//		}
//	}
//	else	//非装备栏
//	{
//		//检查包裹正确性
//		if (pkItem != NULL && iContainerType != GetContainerTypeFromItemType(pkItem->pItemInfo->iType))
//			return false;
//		m_vItems[iContainerType][iSlotIndex - 1] = pkItem;
//	}
//	if (ppkCurItem != NULL)
//		*ppkCurItem = pkCurItem;
//	return true;
//}

int ItemArchive::FindEmptySlotIndex(int iContainerType)
{
	for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(iContainerType); ++iSlotIndex)
	{
		if (GetInvenItem(iContainerType, iSlotIndex) == NULL)
			return iSlotIndex;
	}
	return 0;
}

int ItemArchive::FindFirstSlotIndex(int iContainerType, int iItemID)
{
	for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(iContainerType); ++iSlotIndex)
	{
		Item* pItem = GetInvenItem(iContainerType, iSlotIndex);
		if (pItem != NULL && pItem->iItemID == iItemID)
			return iSlotIndex;
	}
	return 0;
}

int ItemArchive::FindAddItemSlotIndex(int iContainerType, int iItemID, int iCount)
{
	for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(iContainerType); ++iSlotIndex)
	{
		Item* pItem = GetInvenItem(iContainerType, iSlotIndex);
		if (pItem != NULL && pItem->iItemID == iItemID &&
			pItem->iStackCount + iCount <= pItem->pItemInfo->iStackLimit)
			return iSlotIndex;
	}
	return 0;
}

int ItemArchive::GetContainerTypeFromItemType(int iType)
{
	switch (iType)
	{
	case ITEM_TYPE_FIGURE:
		return 0;
	case ITEM_TYPE_CONSUME:
		return 1;
	case ITEM_TYPE_ETC:
		return 2;
	case ITEM_TYPE_EVENT:
		return 3;
	case ITEM_TYPE_PET:
		return 4;
	default:
		return -1;
	}
}

int ItemArchive::GetEquipContainerTypeFromItemType(int iType, int iSubType)
{
	switch (iType)
	{
	case ITEM_TYPE_FIGURE:
		switch (iSubType)
		{
		case ITEM_SUBTYPE_EQUIP_EQUIP:
			return FIGURE_EQUIP;
		case ITEM_SUBTYPE_EQUIP_FASHION:
			return FIGURE_FASHION;
		default:
			return CONTAINER_TYPE_INVALID;
		}
	case 2:
		switch (iSubType)
		{
		case 3:
			return FIGURE_BASIC;
		default:
			return CONTAINER_TYPE_INVALID;
		}
	default:
		return CONTAINER_TYPE_INVALID;
	}
}

void ItemArchive::OnSetEquipStatus(Item* pkItem)
{
	if (pkItem == NULL) return;
	for (int i = 0; i < 4; ++i)
	{
		if (pkItem->pItemInfo->iStatusType[i] != STAT_NULL)
		{
			m_pkParent->GetStatus()->ApplyStatusAdd((CREATURE_STATUS)pkItem->pItemInfo->iStatusType[i], pkItem->pItemInfo->iStatusValue[i]);
		}
	}
}

void ItemArchive::OnUnsetEquipStatus(Item* pkItem)
{
	if (pkItem == NULL) return;
	for (int i = 0; i < 4; ++i)
	{
		if (pkItem->pItemInfo->iStatusType[i] != STAT_NULL)
		{
			m_pkParent->GetStatus()->ApplyStatusAdd((CREATURE_STATUS)pkItem->pItemInfo->iStatusType[i], - pkItem->pItemInfo->iStatusValue[i]);
		}
	}
}

void ItemArchive::CheckUpdateMotionType()
{
	Item* pkWeapon;
	Item* pkShield;
	if (m_cCurWeaponSet == 0)
	{
		pkWeapon = GetInvenItem(FIGURE_EQUIP, 13);
		pkShield = GetInvenItem(FIGURE_EQUIP, 14);
	}
	else
	{
		pkWeapon = GetInvenItem(FIGURE_EQUIP, 15);
		pkShield = GetInvenItem(FIGURE_EQUIP, 16);
	}
	if (pkWeapon != NULL)
	{
		if (pkShield == NULL)
			m_pkParent->SetMotionTypeID(pkWeapon->pItemInfo->iMotionTypeID1);
		else
			m_pkParent->SetMotionTypeID(pkWeapon->pItemInfo->iMotionTypeID2);
	}
	else
	{
		if (pkShield == NULL)
			m_pkParent->SetMotionTypeID(1);
		else
			m_pkParent->SetMotionTypeID(2);
	}
}

void ItemArchive::CreateSlotItem(int iContainerType, int iSlotIndex, Item& NewItem)
{
	if (GetInvenItem(iContainerType, iSlotIndex) != NULL)
		return;
	Item* item = new Item(NewItem);
	item->iItemNo = ItemManager::GetInstance()->GetNextItemNo();
	SetInvenItem(iContainerType, iSlotIndex, item);
	m_sDbInsertPool.insert(item->iItemNo);
}

void ItemArchive::DeleteSlotItem(int iContainerType, int iSlotIndex)
{
	Item* pkItem = GetInvenItem(iContainerType, iSlotIndex);
	if (pkItem == NULL)
		return;
	m_sDbDeletePool.insert(pkItem->iItemNo);
	delete pkItem;
	SetInvenItem(iContainerType, iSlotIndex, NULL);
}

void ItemArchive::LoadDB()
{
	ClearSlotItems();
	DBResult *result;
	result = DataBase::GetInstance()->Query("SELECT * FROM `items` WHERE `char_id` = " + Convert::ToString(m_pkParent->GetCharID()));
	while (result->ReadRow())
	{
		int iContainerType = result->GetInt("container_type");
		int iSlotIndex = result->GetInt("slot_index");
		Item* item = new Item(result->GetInt("item_id"));
		item->iStackCount = result->GetInt("stack_count");
		item->iItemNo = result->GetInt("id");
		SetInvenItem(iContainerType, iSlotIndex, item);
	}
	result->Free();
}

void ItemArchive::SaveDB()
{
	for (std::set<int64>::iterator iter = m_sDbDeletePool.begin(); iter != m_sDbDeletePool.end(); ++iter)
	{
		int64 iItemNo = *iter;
		if (m_sDbInsertPool.find(iItemNo) == m_sDbInsertPool.end())	//item inserted and then deleted will not be saved
		{
			DBResult *r = DataBase::GetInstance()->Query("DELETE FROM `items` WHERE `id` = " + Convert::ToString(iItemNo));
			r->Free();
		}
	}
	for (int iContainerType = 0; iContainerType < ContainerMax; ++iContainerType)
	{
		for (int iSlotIndex = 1; iSlotIndex <= GetInvenSize(iContainerType); ++iSlotIndex)
		{
			Item* item = GetInvenItem(iContainerType, iSlotIndex);
			if (item != NULL)
			{
				if (m_sDbInsertPool.find(item->iItemNo) != m_sDbInsertPool.end())
				{
					std::string sql = 
						"INSERT INTO `items`("
						"`id`, `char_id`, `container_type`, `slot_index`, `item_id`, `stack_count`"
						") VALUES (" +
						Convert::ToString(item->iItemNo) + ", " +
						Convert::ToString(m_pkParent->GetCharID()) + ", " +
						Convert::ToString(iContainerType) + ", " +
						Convert::ToString(iSlotIndex) + ", " +
						Convert::ToString(item->iItemID) + ", " +
						Convert::ToString(item->iStackCount) + 
						")";
					DBResult *r = DataBase::GetInstance()->Query(sql);
					r->Free();
				}
				else
				{
					std::string sql = 
						"UPDATE `items` SET "
						"`container_type` = " + Convert::ToString(iContainerType) + ", " +
						"`slot_index` = " + Convert::ToString(iSlotIndex) + ", " +
						"`stack_count` = " + Convert::ToString(item->iStackCount) + " " +
						"WHERE `id` = " + Convert::ToString(item->iItemNo);
					DBResult *r = DataBase::GetInstance()->Query(sql);
					r->Free();
				}
			}
		}
	}
	m_sDbInsertPool.clear();
	m_sDbDeletePool.clear();
}
