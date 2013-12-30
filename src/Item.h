#pragma once

enum CONTAINER_TYPE
{ 
    CONTAINER_TYPE_INVALID = 0xff,
    INVENTORY_EQUIP = 0x0,
    INVENTORY_CONSUME = 0x1,
    INVENTORY_ETC = 0x2,
    INVENTORY_EVENT = 0x3,
    INVENTORY_PET = 0x4,
    INVENTORY_HOUSING = 0x5,
    FIGURE_EQUIP = 0x6,
    FIGURE_FASHION = 0x7,
    FIGURE_BASIC = 0x8,
    STORAGE = 0x9,
    INVENTORY_CASH = 0xa,
    FIGURE_PET = 0xb,
    INVENTORY_WEAR = 0xc,
    FIGURE_EFFECT = 0xd,
    MOVINGBOX = 0xe,
    MOVINGBOXTWO = 0xf,
    MOVINGBOXTHREE = 0x10,
    MOVINGBOXFOUR = 0x11,
    MOVINGBOXFIVE = 0x12,
    FIGURE_GEM = 0x13,
    CONTAINER_MAX = 0x14,
    CONTAINER_AUCTION = 0x58,
    CONTAINER_MAILBOX = 0x63
};

enum ITEM_TYPE
{ 
	ITEM_TYPE_NULL = 0x0,
	ITEM_TYPE_FIGURE = 0x1,
	ITEM_TYPE_CONSUME = 0x2,
	ITEM_TYPE_ETC = 0x3,
	ITEM_TYPE_EVENT = 0x4,
	ITEM_TYPE_PET = 0x5,
	ITEM_TYPE_HOUSING = 0x6,
	ITEM_TYPE_COMMONE_SEED = 0x7,
	ITEM_TYPE_ADVANCE_SEED = 0x8
};

enum ITEM_SUBTYPE
{
    ITEM_SUBTYPE_NULL = 0,
    ITEM_SUBTYPE_EQUIP_EQUIP = 1,
    ITEM_SUBTYPE_EQUIP_FASHION = 2,
    ITEM_SUBTYPE_EQUIP_EFFECT = 3,
    ITEM_SUBTYPE_EQUIP_FIGURE_GEM = 4,
    ITEM_SUBTYPE_WASTE_NORMAL = 1,
    ITEM_SUBTYPE_WASTE_ENCHANT = 2,
    ITEM_SUBTYPE_WASTE_DEFAULT = 3,
    ITEM_SUBTYPE_WASTE_MSGBOARD = 4,
    ITEM_SUBTYPE_WASTE_SEARCH = 5,
    ITEM_SUBTYPE_WASTE_FOLLOW = 6,
    ITEM_SUBTYPE_WASTE_SNATCH = 7,
    ITEM_SUBTYPE_PET_EQUIP = 1,
    ITEM_SUBTYPE_PET_WASTE = 2,
    ITEM_SUBTYPE_EVENT_QUEST = 1,
    ITEM_SUBTYPE_EVENT_GAMBLE = 2,
    ITEM_SUBTYPE_PLANTBT_OPENED = 0,
    ITEM_SUBTYPE_PLANTBT_UNOPENED = 1
};

enum ITEM_EQUIPPOS
{ 
	ITEM_EQUIPPOS_NULL = 0x0,
	ITEM_EQUIPPOS_HAIRDRESS = 0x1,
	ITEM_EQUIPPOS_GOGGLE = 0x2,
	ITEM_EQUIPPOS_EARING = 0x3,
	ITEM_EQUIPPOS_SHIRTS = 0x4,
	ITEM_EQUIPPOS_PANTS = 0x5,
	ITEM_EQUIPPOS_CLOAK = 0x6,
	ITEM_EQUIPPOS_GLOVE = 0x7,
	ITEM_EQUIPPOS_SHOES = 0x8,
	ITEM_EQUIPPOS_STOCKING = 0x9,
	ITEM_EQUIPPOS_MAKEUP = 0xa,
	ITEM_EQUIPPOS_STONE = 0xb,
	ITEM_EQUIPPOS_RING = 0xc,
	ITEM_EQUIPPOS_ONEHAND_WEAPON = 0xd,
	ITEM_EQUIPPOS_TWOHAND_WEAPON = 0xd,
	ITEM_EQUIPPOS_SHIELD = 0xe,
	ITEM_EQUIPPOS_ONEPIECE = 0x4,
	ITEM_EQUIPPOS_DEFAULT_SHORTS = 0x1,
	ITEM_EQUIPPOS_DEFAULT_PANTS = 0x2,
	ITEM_EQUIPPOS_HAIR = 0x3,
	ITEM_EQUIPPOS_EXPRESSION = 0x4,
	ITEM_EQUIPPOS_SKIN = 0x5,
	ITEM_EQUIPPOS_PET_HEAD = 0x1,
	ITEM_EQUIPPOS_PET_BODY = 0x2,
	ITEM_EQUIPPOS_ONEHAND_WEAPON2 = 0xf,
	ITEM_EQUIPPOS_TWOHAND_WEAPON2 = 0xf,
	ITEM_EQUIPPOS_SHIELD2 = 0x10
};

struct ItemInfo
{
	int iID;
	int iGamePrice;
	int iType;
	int iSubType;
	int iPosID1;
	int iSubPosID11;
	int iSubPosID12;
	int iPosID2;
	int iSubPosID21;
	int iSubPosID22;
	int iStatusType[4];
	int iStatusValue[4];
	int iStackLimit;
	int iItemLv;
	int iMotionTypeID1;
	int iMotionTypeID2;
};

class ItemManager : public Singleton<ItemManager>
{
public:
	ItemManager() { Init(); }
	void Init();
	ItemInfo* GetItemInfo(int iID)
	{
		stdext::hash_map<int, ItemInfo*>::iterator iter = m_hmItemInfo.find(iID);
		return iter != m_hmItemInfo.end() ? iter->second : NULL;
	}
	int64 GetNextItemNo() { return m_iItemInsertNo++; }
protected:
	void LoadLDTItems();
	void InitItemInsertNo();
	stdext::hash_map<int, ItemInfo*> m_hmItemInfo;
	int64 m_iItemInsertNo;
};

struct Item
{
public:
	Item(int iItemID)
	{
		this->iItemID = iItemID;
		pItemInfo = ItemManager::GetInstance()->GetItemInfo(iItemID);
		//assert(pItemInfo != NULL);
	}
	int iItemID;
	short iStackCount;
	int64 iItemNo;
	ItemInfo* pItemInfo;
};

class Player;

class ItemArchive
{
public:
	ItemArchive(Player* pkPlayer);
	~ItemArchive();
	static const int ContainerMax = 14;
	void UseItem(int iContainerType, int iSlot);
	void ResizeSlot();
	void LoadDB();
	void SaveDB();
	void InitPlayer();
	Player* GetParent() { return m_pkParent; }
	
	void AddItem(Item& Item);
	void RemoveItem(int iItemID, int iCount);
	void RemoveItem(int iContainerType, int iSlotIndex, int iCount);
	int GetItemCount(int iItemID);

	Item* GetInvenItem(int iContainerType, int iSlotIndex)
	{
		return m_vItems[iContainerType][iSlotIndex - 1];
	}
	int GetInvenSize(int iContainerType)
	{
		return m_vItems[iContainerType].size();
	}
	void SetInvenItem(int iContainerType, int iSlotIndex, Item* pkItem)
	{
		m_vItems[iContainerType][iSlotIndex - 1] = pkItem;
	}
	bool IsFigureContainer(int iContainerType)
	{
		switch (iContainerType)
		{
		case FIGURE_EQUIP:
		case FIGURE_FASHION:
		case FIGURE_BASIC:
		case FIGURE_PET:
			return true;
		default:
			return false;
		}
	}
	void ClearSlotItems();
	void CreateSlotItem(int iContainerType, int iSlotIndex, Item& Item);
	void DeleteSlotItem(int iContainerType, int iSlotIndex);
	int FindEmptySlotIndex(int iContainerType);
	int FindFirstSlotIndex(int iContainerType, int iItemID);
	int FindAddItemSlotIndex(int iContainerType, int iItemID, int iCount);
	int GetContainerTypeFromItemType(int iType);
	int GetEquipContainerTypeFromItemType(int iType, int iSubType);
	//
	void GetCurEquipSubPos(Item* pkItem, int iPosID, int& iSubPosID1, int& iSubPosID2)
	{
		if (iPosID == pkItem->pItemInfo->iPosID1)
		{
			iSubPosID1 = pkItem->pItemInfo->iSubPosID11;
			iSubPosID2 = pkItem->pItemInfo->iSubPosID12;
		}
		else if (iPosID == pkItem->pItemInfo->iPosID2)
		{
			iSubPosID1 = pkItem->pItemInfo->iSubPosID21;
			iSubPosID2 = pkItem->pItemInfo->iSubPosID22;
		}
	}
	void RecvChangeSlot(int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo);
	void RecvTrash(int iContainer, int iSlot, int iCount);
	void RecvUsing(int iContainer, int iSlot);
	void RecvToggleWeaponSet();
	void RecvDivide(int iContainerFrom, int iSlotFrom, int iCount, int iContainerTo, int iSlotTo);
protected:
	void OnSetEquipStatus(Item* pkItem);
	void OnUnsetEquipStatus(Item* pkItem);
	void CheckUpdateMotionType();
	Player* m_pkParent;
	char m_cCurWeaponSet;
	std::vector<Item*> m_vItems[ContainerMax];
	std::set<int64> m_sDbInsertPool;
	std::set<int64> m_sDbDeletePool;
};