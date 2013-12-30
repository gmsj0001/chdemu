#include "stdafx.h"

bool is_use_spf = true;

char root_path[256] = {0};

int Packet::Read(void *buf, int len)
{
	 memcpy(buf, &this->data[this->pos], len);
	 this->pos += len;
	 return len; 
}

int Packet::Write(const void *buf, int len)
{ 
	memcpy(&this->data[this->pos], buf, len);
	this->pos += len;
	if (this->pos + 4 > this->len)
		this->len = this->pos + 4;
	return len;
}

int Packet::Seek(int len, int origin)
{
	if (origin == SEEK_SET)
	{
		this->pos = len;
		return this->pos;
	}
	else if (origin == SEEK_CUR)
	{
		this->pos += len;
		return this->pos;
	}
	else
	{
		return -1;
	}
}

void Thread::BeginThread()
{
	_beginthread(&Thread::ThreadProc, 0, this);
}

void Thread::ThreadProc(void *lpParam)
{
	static_cast<Thread*>(lpParam)->ThreadProc();
}

std::string BinaryStream::ReadFixString(int len)
{
	char str[256];
	Read(str, len);
	return str;
}

void BinaryStream::WriteFixString(std::string str, int len)
{
	Write(str.c_str(), len);
}

std::string BinaryStream::ReadL1String()
{
	int len = ReadByte();
	std::string str(len, 0);
	Read((char*)str.c_str(), len);
	return str.c_str();
}

void BinaryStream::WriteL1String(std::string str)
{
	int len = str.length() > 255 ? 255 : str.length();
	WriteByte(len);
	Write(str.c_str(), len);
}

std::string BinaryStream::ReadL2String()
{
	int len = ReadUShort();
	std::string str(len, 0);
	Read((char*)str.c_str(), len);
	return str.c_str();
}

void BinaryStream::WriteL2String(std::string str)
{
	int len = str.length() > 65535 ? 65535 : str.length();
	WriteUShort(len);
	Write(str.c_str(), len);
}

void Packet::WriteHexString(std::string str)
{
	const char *p = str.c_str();
	while (*p != '\0')
	{
		if (isalpha(*p) || isdigit(*p))
		{
			int hex;
			sscanf(p, "%02x", &hex);
			WriteByte(hex);
			p += 2;
		}
		else
		{
			++p;
		}
	}
}

static bool spf_sort_comparer(SPFFile m1, SPFFile m2)
{
	return m1.version < m2.version;
}

static int lh_strhash(const char *c)
{
    unsigned long ret=0;
    long n;
    unsigned long v;
    int r;

    if ((c == NULL) || (*c == '\0'))
        return(ret);
    /*
       unsigned char b[16];
       MD5(c,strlen(c),b);
       return(b[0]|(b[1]<<8)|(b[2]<<16)|(b[3]<<24));
       */

    n=0x100;
    while (*c)
    {
        v=n|(*c);
        n+=0x100;
        r= (int)((v>>2)^v)&0x0f;
        ret=(ret<<r)|(ret>>(32-r));
        ret&=0xFFFFFFFFL;
        ret^=v*v;
        c++;
    }
    return((ret>>16)^ret);
}

bool SPFFileReader::Open(std::string fileName)
{
	char path[256];
	strcpy(path, root_path);
	if (is_use_spf)
	{
		std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::toupper);
		stdext::hash_map<int, SPFFileRecord>::iterator iter = SPFFileManager::instance->fileList.find(lh_strhash(fileName.c_str()));
		if (iter == SPFFileManager::instance->fileList.end())
		{
			return false;
		}
		SPFFileRecord fileRecord = (*iter).second;
		strcat(path, SPFFileManager::instance->spfList[fileRecord.spfNo].fileName.c_str());
		this->fp = fopen(path, "rb");
		this->offset = fileRecord.offset;
		this->length = fileRecord.length;
		fseek(this->fp, fileRecord.offset, SEEK_SET);
	}
	else
	{
		strcat(path, fileName.c_str());
		this->fp = fopen(path, "rb");
		if (this->fp == NULL) return false;
		this->offset = 0;
		this->length = 0;
	}
	return true;
}

void SPFFileReader::Close()
{
	if (this->fp)
	{
		fclose(this->fp);
		this->fp = NULL;
	}
}

int SPFFileReader::Seek(int len, int origin)
{
	if (is_use_spf)
	{
		if (origin == SEEK_SET)
		{
			fseek(this->fp, this->offset + len, SEEK_SET);
			return ftell(this->fp) - this->offset;
		}
		else if (origin == SEEK_CUR)
		{
			fseek(this->fp, len, SEEK_CUR);
			return ftell(this->fp) - this->offset;
		}
		else if (origin == SEEK_END)
		{
			fseek(this->fp, this->offset + this->length + len, SEEK_SET);
			return ftell(this->fp) - this->offset;
		}
		else
			return -1;
	}
	else
	{
		fseek(this->fp, len, origin);
		return ftell(this->fp);
	}
}

int SPFFileReader::Read(void *buf, int len)
{
	return fread(buf, 1, len, this->fp);
}

const char* spffile_list[] = {
	"JINSSAGA.SPF",
	"ROWID.SPF",
	"AJJIYA.SPF",
	"FREE.SPF",		//I'll use this file to do the patch
	NULL
};

void SPFFileManager::Initialize()
{
	if (ConfMgr::GetInt("Pack") == 0)
		is_use_spf = false;
	if (ConfMgr::GetString("ResourceDir") != "")
		strcpy(root_path, ConfMgr::GetString("ResourceDir").c_str());
	if (!is_use_spf)
		return;
	
	//Step 1: Enum SPF Files Versions and sort
	//std::vector<SPFFile> spfSortList;
	//SPFFile spfFile;
	char path[256];
	//strcpy(path, root_path);
	//strcat(path, "*.SPF");
	//_finddata_t findData;
	//long hFind = _findfirst(path, &findData);
	//do
	//{
	//	spfFile.fileName = findData.name;
	//	strcpy(path, root_path);
	//	//strcat(path, "\\");
	//	strcat(path, findData.name);
	//	FILE *fp = fopen(path, "rb");
	//	fseek(fp, -4, SEEK_END);
	//	fread(&spfFile.version, 4, 1, fp);
	//	spfSortList.push_back(spfFile);
	//	fclose(fp);
	//}
	//while (_findnext(hFind, &findData) == 0);
	//_findclose(hFind);
	//std::sort(spfSortList.begin(), spfSortList.end(), &spf_sort_comparer);

	//step2: load spf files
	//std::vector<SPFFile>::iterator iter;
	//for (iter = spfSortList.begin(); iter != spfSortList.end(); ++iter)
	//{
	for (const char** pfile = spffile_list; *pfile != NULL; ++pfile)
	{
		strcpy(path, root_path);
		//strcat(path, "\\");
		strcat(path, *pfile);
		FILE *fp = fopen(path, "rb");
		if (fp == NULL)
			continue;
		SPFFile spfFile;
		spfFile.fileName = *pfile;
		printf("\rInitializing SPF File: %s...\n", spfFile.fileName.c_str());

		fseek(fp, -0x8c, SEEK_END);
		int fileTableOffset;
		char spfNo;
		fread(&fileTableOffset, 4, 1, fp);
		fread(&spfNo, 1, 1, fp);
		this->spfList.insert(std::pair<char, SPFFile>(spfNo, spfFile));
		int fileCount = fileTableOffset / 0x8c;
		fseek(fp, -0x8c - fileTableOffset, SEEK_END);
		for (int i = 0; i < fileCount; ++i)
		{
			char fileNameCStr[128];
			SPFFileRecord fileRecord;
			fread(fileNameCStr, 1, 128, fp);
			fread(&fileRecord.offset, 4, 1, fp);
			fread(&fileRecord.length, 4, 1, fp);
			fseek(fp, 4, SEEK_CUR);
			fileRecord.spfNo = spfNo;
			std::string fileName = fileNameCStr;
			std::transform(fileName.begin(), fileName.end(), fileName.begin(), &toupper);
			this->fileList[lh_strhash(fileName.c_str())] = fileRecord;
		}
		fclose(fp);
	}
	SPFFileManager::instance = this;
}

SPFFileManager* SPFFileManager::instance = NULL;

void LDTFile::InitLDT(std::string table)
{
	printf("\rLoading LDT Table: %s\n", table.c_str());
	this->tableName = table;
	SPFFileReader fs("DATA/LDT/" + table + ".LDT");
	fs.Seek(4, SEEK_SET);
	int colCount = fs.ReadInt();
	this->colTypes.resize(colCount);
	this->rowCount = fs.ReadInt();
	int iCol;
	for (iCol = 0; iCol < colCount; ++iCol)
	{
		std::string colName = fs.ReadFixString(64);
		this->colNames[colName] = iCol;
	}
	fs.Seek(0x200c, SEEK_SET);
	for (iCol = 0; iCol < colCount; ++iCol)
	{
		this->colTypes[iCol] = fs.ReadInt();
	}
	fs.Seek(0x220c, SEEK_SET);
	for (int iRow = 0; iRow < rowCount; ++iRow)
	{
		int id = fs.ReadInt();
		this->rowOffsets[id] = fs.Seek(0, SEEK_CUR);
		for (int iCol = 0; iCol < colCount; ++iCol)
		{
			switch (this->colTypes[iCol])
			{
			case 0:
			case 3:
			case 2:
			case 4:
				//fs.Skip(4);
				fs.ReadInt();
				break;
			case 1:
				//fs.Skip(fs.ReadShort());
				fs.ReadL2String();
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

LDTRecord* LDTFile::GetRecord(int id)
{
	int offset = this->rowOffsets[id];
	SPFFileReader fs("DATA/LDT/" + this->tableName + ".LDT");
	fs.Seek(offset, SEEK_SET);
	LDTRecord *record = new LDTRecord();
	record->ldtFile = this;
	record->id = id;
	record->data.resize(this->colTypes.size());
	for (int iCol = 0; iCol < this->colTypes.size(); ++iCol)
	{
		switch (this->colTypes[iCol])
		{
		case 0:
		case 3:
		case 2:
		case 4:
			record->data[iCol] = (void*)fs.ReadInt();
			break;
		case 1:
			record->data[iCol] = new std::string(fs.ReadL2String());
			break;
		}
	}
	return record;
}

void* LDTFile::BeginEnum()
{
	//return new stdext::hash_map<int, int>::iterator(this->rowOffsets.begin());
	LDTEnumHandle* h = new LDTEnumHandle();
	h->fs = new SPFFileReader();
	h->fs->Open("DATA/LDT/" + tableName + ".LDT");
	h->fs->Seek(0x220c, SEEK_SET);
	h->row = 0;
	return h;
}

LDTRecord* LDTFile::EnumRecord(void* handle)
{
	//stdext::hash_map<int, int>::iterator *iter = (stdext::hash_map<int, int>::iterator*)handle;
	//if (*iter == this->rowOffsets.end())
	//	return NULL;
	//else
	//	return this->GetRecord(((*iter)++)->first);
label_start:

	LDTEnumHandle* ph = (LDTEnumHandle*)handle;
	if (ph->row++ == this->rowCount)
		return NULL;
	SPFFileReader &fs = *ph->fs;
	
	LDTRecord *record = new LDTRecord();
	record->ldtFile = this;
	record->id = fs.ReadInt();
	record->data.resize(this->colTypes.size());
	for (int iCol = 0; iCol < this->colTypes.size(); ++iCol)
	{
		switch (this->colTypes[iCol])
		{
		case 0:
		case 3:
		case 2:
		case 4:
			record->data[iCol] = (void*)fs.ReadInt();
			break;
		case 1:
			record->data[iCol] = new std::string(fs.ReadL2String());
			break;
		}
	}
	if (record->id == 0)
	{
		record->Free();
		goto label_start;
	}
	return record;
}

void LDTFile::EndEnum(void* handle)
{
	//stdext::hash_map<int, int>::iterator *iter = (stdext::hash_map<int, int>::iterator*)handle;
	//delete iter;
	LDTEnumHandle* ph = (LDTEnumHandle*)handle;
	delete ph->fs;
	delete ph;
}

LDTRecord* LDTFile::GetRecord(int id, std::string table)
{
	return LDTFile::GetLDTFile(table)->GetRecord(id);
}

LDTFile* LDTFile::GetLDTFile(std::string table)
{
	LDTFile *ldtFile = LDTFile::tables[table];
	if (ldtFile == NULL)
	{
		ldtFile = new LDTFile(table);
		LDTFile::tables[table] = ldtFile;
	}
	return ldtFile;
}

stdext::hash_map<std::string, LDTFile*> LDTFile::tables;

std::string LDTRecord::GetString(const std::string& col)
{
	//for (int iCol = 0; iCol < this->ldtFile->colNames.size(); ++iCol)
	//{
	//	if (col == this->ldtFile->colNames[iCol])
	//	{
	//		return *(std::string*)this->data[iCol];
	//	}
	//}
	//return "";
	stdext::hash_map<std::string, int>::iterator iter = this->ldtFile->colNames.find(col);
	if (iter == this->ldtFile->colNames.end())
	{
		assert(0);
		return "";
	}
	return GetString(iter->second);
}

std::string LDTRecord::GetString(int col)
{
	ASSERT(this->ldtFile->colTypes[col] == 1);
	return *(std::string*)this->data[col];
}

int LDTRecord::GetInt(const std::string& col)
{
	//for (int iCol = 0; iCol < this->ldtFile->colNames.size(); ++iCol)
	//{
	//	if (col == this->ldtFile->colNames[iCol])
	//		return (int)this->data[iCol];
	//}
	//return 0;
	stdext::hash_map<std::string, int>::iterator iter = this->ldtFile->colNames.find(col);
	if (iter == this->ldtFile->colNames.end()) 
	{
		assert(0);
		return 0;
	}
	return GetInt(iter->second);
}

int LDTRecord::GetInt(int col)
{
	ASSERT(this->ldtFile->colTypes[col] != 1);
	return (int)this->data[col];
}

void LDTRecord::Free()
{
	for (int iCol = 0; iCol < this->ldtFile->colTypes.size(); ++iCol)
	{
		if (this->ldtFile->colTypes[iCol] == 1)
			delete this->data[iCol];
	}
	delete this;
}

stdext::hash_map<std::string, std::string> ConfMgr::m_hmConf;

void ConfMgr::LoadConfig(std::string file, std::string section)
{
	file = ".\\" + file;
	char keys[1024];
	GetPrivateProfileStringA(section.c_str(), NULL, NULL, keys, sizeof(keys), file.c_str());
	char* p = keys;
	while (*p != 0)
	{
		char value[256] = {0};
		GetPrivateProfileStringA(section.c_str(), p, NULL, value, sizeof(value), file.c_str());
		std::string key = p;
		std::transform(key.begin(), key.end(), key.begin(), &tolower);
		m_hmConf[key] = value;
		p += strlen(p) + 1;
	}
}

void ConfMgr::ParseCommand(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		if (strlen(argv[i]) > 1 && argv[i][0] == '-')
		{
			std::string key = argv[i] + 1;
			std::transform(key.begin(), key.end(), key.begin(), &tolower);
			if (i + 1 < argc && argv[i + 1][0] != '-')
			{
				
				m_hmConf[key] = argv[i + 1];
				++i;
			}
			else
			{
				m_hmConf[key]= "";
			}
		}
	}
}

std::string ConfMgr::GetString(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(), &tolower);
	if (m_hmConf.find(key) == m_hmConf.end())
		return "";
	return m_hmConf[key];
}

int ConfMgr::GetInt(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(), &tolower);
	if (m_hmConf.find(key) == m_hmConf.end())
		return 0;
	return Convert::Parse<int>(m_hmConf[key]);
}

int FileStream::Read(void *buf, int len)
{
	int nRead = fread(buf, 1, len, this->m_fp);
	if (nRead < len)
	{
		int nErr = ferror(this->m_fp);
		Sleep(100);
		this->Seek(this->m_pos, SEEK_SET);
		nRead = this->Read(buf, len);
	}
	this->m_pos += nRead;
	return nRead;
}

int FileStream::Write(const void *buf, int len)
{
	int nWrite = fwrite(buf, 1, len, this->m_fp);
	if (nWrite < len)
	{
		int nErr = ferror(this->m_fp);
		Sleep(100);
		this->Seek(this->m_pos, SEEK_SET);
		nWrite = this->Write(buf, len);
	}
	this->m_pos += nWrite;
	return nWrite;
}

int FileStream::Seek(int len, int origin)
{
	if (fseek(this->m_fp, len, origin) != 0)
	{
		int nErr = ferror(this->m_fp);
		return -1;
	}
	this->m_pos = ftell(this->m_fp);
	return this->m_pos;
}

bool FileStream::Open(const char *name, const char *mode)
{
	this->m_fp = fopen(name, mode);
	this->m_pos = 0;
	if (this->m_fp != NULL)
		return true;
	else
		return false;
}

void FileStream::Close()
{
	if (this->m_fp != NULL)
	{
		fclose(this->m_fp);
		this->m_fp = NULL;
	}
}