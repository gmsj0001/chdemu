#include "stdafx.h"
#include "sqlite3.h"

bool SqliteConnection::Connect(std::string str)
{
	if (sqlite3_open(str.c_str(), &this->db) == SQLITE_OK)
		return true;
	else
		sqlite3_close(this->db);
	return false;
}

void SqliteConnection::Close()
{
	sqlite3_close(this->db);
}

DBResult* SqliteConnection::Query(std::string sql)
{
	//__asm int 3;
	sqlite3_stmt *stmt;
	if (sqlite3_prepare(this->db, sql.c_str(), sql.length() + 1, &stmt, NULL) != SQLITE_OK)
		return NULL;
	int ret = sqlite3_step(stmt);
	SqliteResult *result = NULL;
	if (ret == SQLITE_ROW || ret == SQLITE_DONE)
	{
		result = new SqliteResult();
	}
	result->stmt = stmt;
	if (ret == SQLITE_ROW)
	{
		int colCount = sqlite3_column_count(stmt);
		result->colNames.resize(colCount);
		for (int iCol = 0; iCol < colCount; ++iCol)
		{
			result->colNames[iCol] = sqlite3_column_name(stmt, iCol);
		}
		result->hasRow = true;
		result->isFirstRow = true;
	}
	else if (ret == SQLITE_DONE)
	{
		result->hasRow = false;
		result->lastInsertId = sqlite3_last_insert_rowid(this->db);
	}
	ASSERT(result != NULL);
	return result;
}

bool SqliteResult::HasRow()
{
	return this->hasRow;
}

bool SqliteResult::ReadRow()
{
	if (!this->hasRow) return false;
	if (this->isFirstRow)
	{
		this->isFirstRow = false;
		return true;
	}
	else
	{
		int ret = sqlite3_step(this->stmt);
		if (ret == SQLITE_ROW)
			return true;
		else if (ret == SQLITE_DONE)
			return false;
		else
			return false;
	}
}

int SqliteResult::LastInsertId()
{
	return this->lastInsertId;
}

std::string SqliteResult::GetString(std::string col)
{
	for (int iCol = 0; iCol < this->colNames.size(); ++iCol)
	{
		if (colNames[iCol] == col)
			return this->GetString(iCol);
	}
	return "";
}

std::string SqliteResult::GetString(int col)
{
	return (const char*)sqlite3_column_text(this->stmt, col);
}

int SqliteResult::GetInt(std::string col)
{
	for (int iCol = 0; iCol < this->colNames.size(); ++iCol)
	{
		if (colNames[iCol] == col)
			return this->GetInt(iCol);
	}
	return 0;
}

int SqliteResult::GetInt(int col)
{
	return sqlite3_column_int(this->stmt, col);
}

__int64 SqliteResult::GetInt64(std::string col)
{
	for (int iCol = 0; iCol < this->colNames.size(); ++iCol)
	{
		if (colNames[iCol] == col)
			return this->GetInt64(iCol);
	}
	return 0;
}

__int64 SqliteResult::GetInt64(int col)
{
	return sqlite3_column_int64(this->stmt, col);
}

void SqliteResult::Free()
{
	sqlite3_finalize(this->stmt);
	delete this;
}

DataBase* DataBase::instance = NULL;

void DataBase::Initialize()
{
	printf("\rLoading Sqlite Database...\n");
	this->db = new SqliteConnection();
	this->db->Connect("chdemu.db");
	this->CreateTables();
}

void DataBase::CreateTables()
{
	DBResult *result;
	std::string sql;
	result = this->Query("PRAGMA foreign_keys = ON");
	result->Free();
	sql = 
		"CREATE TABLE IF NOT EXISTS `users`("
		"`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
		"`username` TEXT UNIQUE, "
		"`password` TEXT "
		")";
	result = this->Query(sql);
	result->Free();
	sql = 
		"CREATE TABLE IF NOT EXISTS `characters`("
		"`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
		"`user_id` INTEGER REFERENCES `users`(`id`), "
		"`name` TEXT UNIQUE, "
		"`gender` INTEGER, "
		"`lv` INTEGER, "
		"`luk_base` INTEGER, "
		"`atk_base` INTEGER, "
		"`def_base` INTEGER, "
		"`ele_base` INTEGER, "
		"`hp` INTEGER, "
		"`sp` INTEGER, "
		"`title_id` INTEGER, "
		"`class_type` INTEGER, "
		"`exp` INTEGER, "
		"`ely` INTEGER, "
		"`storage_ely` INTEGER, "
		"`skill_point` INTEGER, "
		"`pvp_lv` INTEGER, "
		"`pvp_point` INTEGER, "
		"`pvp_glory` INTEGER, "
		"`stage_id` INTEGER, "
		"`mapgroup_id` INTEGER, "
		"`pos_x` INTEGER, "
		"`pos_y` INTEGER"
		")";
	result = this->Query(sql);
	result->Free();
	sql = 
		"CREATE TABLE IF NOT EXISTS `items`("
		"`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
		"`char_id` INTEGER REFERENCES `characters`(`id`), "
		"`container_type` INTEGER, "
		"`slot_index` INTEGER, "
		"`item_id` INTEGER, "
		"`stack_count` INTEGER"
		")";
	result = this->Query(sql);
	result->Free();
	sql = 
		"CREATE TABLE IF NOT EXISTS `skills`("
		"`char_id` INTEGER REFERENCES `characters`(`id`), "
		"`skill_id` INTEGER, "
		"`lv` INTEGER"
		")";
	result = this->Query(sql);
	result->Free();
	sql = 
		"CREATE TABLE IF NOT EXISTS `keys`("
		"`char_id` INTEGER REFERENCES `characters`(`id`), "
		"`slot` INTEGER, "
		"`type` INTEGER, "
		"`value1` INTEGER, "
		"`value2` INTEGER"
		")";
	result = this->Query(sql);
	result->Free();
}

int DataBase::UserLogin(std::string username, std::string password, int *uid)
{
	std::string sql;
	sql = 
		"SELECT `id`, `username`, `password` "
		"FROM `users` "
		"WHERE `username` = '" + username + "'";
	DBResult *result = this->db->Query(sql);
	if (!result->HasRow())
	{
		result->Free();
		return -1;
	}
	result->ReadRow();
	if (result->GetString("password") != password)
	{
		result->Free();
		return -2;
	}
	*uid = result->GetInt("id");
	result->Free();
	return 0;
}
