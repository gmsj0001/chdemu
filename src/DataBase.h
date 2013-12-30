#pragma once

struct sqlite3;
struct sqlite3_stmt;

class DBResult;

class DBConnection
{
public:
	virtual bool Connect(std::string str) = 0;
	virtual void Close() = 0;
	virtual DBResult *Query(std::string sql) = 0;
};

class DBResult
{
public:
	virtual bool HasRow() = 0;
	virtual bool ReadRow() = 0;
	virtual int LastInsertId() = 0;
	virtual std::string GetString(int col) = 0;
	virtual std::string GetString(std::string col) = 0;
	virtual int GetInt(int col) = 0;
	virtual int GetInt(std::string col) = 0;
	virtual __int64 GetInt64(int col) = 0;
	virtual __int64 GetInt64(std::string col) = 0;
	virtual void Free() = 0;
};

class SqliteConnection : public DBConnection
{
public:
	virtual bool Connect(std::string str);
	virtual void Close();
	virtual DBResult *Query(std::string sql);
private:
	sqlite3 *db;
};

class SqliteResult : public DBResult
{
public:
	virtual bool HasRow();
	virtual bool ReadRow();
	virtual int LastInsertId();
	virtual std::string GetString(int col);
	virtual std::string GetString(std::string col);
	virtual int GetInt(int col);
	virtual int GetInt(std::string col);
	virtual __int64 GetInt64(int col);
	virtual __int64 GetInt64(std::string col);
	virtual void Free();
private:
	sqlite3_stmt *stmt;
	std::vector<std::string> colNames;
	bool hasRow;
	bool isFirstRow;
	int lastInsertId;
	friend class SqliteConnection;
};

class Player;

class DataBase
{
public:
	DataBase() { DataBase::instance = this; this->Initialize(); }
	void Initialize();
	int UserLogin(std::string username, std::string password, int *uid);
	void WriteCharacterListData(int uid, Packet *packet);
	DBResult* Query(std::string sql) { return this->db->Query(sql); }
	static DataBase* GetInstance() { return DataBase::instance; }
	static DataBase* instance;
protected:
	void CreateTables();
	DBConnection *db;
};