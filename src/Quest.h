
struct RequireInfo
{
	int iRequireType;
	int iRequireID;
	int iRequireValue1;
	int iRequireValue2;
};

struct QuestInfo
{
	int iQuestID;
	int iQuestType;
	RequireInfo RequireInfo[5];
};

class QuestManager : public Singleton<QuestManager>
{
public:
	QuestManager()
	{
		Init();
	}
	void Init();
	QuestInfo* GetQuestInfo(int iQuestID)
	{
		stdext::hash_map<int, QuestInfo*>::iterator iter = m_hmQuestInfo.find(iQuestID);
		return iter != m_hmQuestInfo.end() ? iter->second : NULL;
	}
	stdext::hash_map<int, QuestInfo*>& GetQuestInfos()
	{
		return m_hmQuestInfo;
	}
protected:
	void LoadLDTQuestInfo();
	stdext::hash_map<int, QuestInfo*> m_hmQuestInfo;
};

class Player;

class QuestArchive
{
public:
	QuestArchive(Player* pkParent)
	{
		m_pkParent = pkParent;
	}
	void InitPlayer();
	std::set<int>& GetQuestList()
	{
		return m_sQuestList;
	}

protected:
	std::set<int> m_sQuestList;
	Player* m_pkParent;
};