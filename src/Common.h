/*
LaTaleCommon Module

defines most useful classes in latale
Stream, BinaryStream
SPFFileReader
Packet
LDTReader
*/

template<class T, class U> T TO(U u) { return static_cast<T>(u); }

class Convert
{
public:
	template<typename T>
	static std::string ToString(T v)
	{
		std::stringstream stream;
		stream << v;
		return stream.str();
	}
	template<typename T>
	static T Parse(std::string str)
	{
		std::stringstream stream;
		stream.str(str);
		T v;
		stream >> v;
		return v;
	}
};

class Mutex
{
public:
	Mutex() { InitializeCriticalSection(&m_cs); }
	~Mutex() { DeleteCriticalSection(&m_cs); }
	void Acquire() { EnterCriticalSection(&m_cs); }
	void Release() { LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs;
};

class Thread
{
public:
	void BeginThread();
protected:
	virtual void ThreadProc() {}
private:
	static void ThreadProc(void *lpParam);
};

template<class T> class Singleton
{
public:
	Singleton()
	{
		m_pkInstance = (T*)this;
	}

	static T* GetInstance()
	{
		return m_pkInstance;
	}
protected:
	static T* m_pkInstance;
};

class Stream
{
public:
	virtual int Read(void *buf, int len) = 0;
	virtual int Write(const void *buf, int len) = 0;
	virtual int Seek(int len, int origin) = 0;
};

class BinaryStream : public Stream
{
public:
	signed char ReadSByte() { signed char v; Read(&v, 1); return v; }
	unsigned char ReadByte() { unsigned char v; Read(&v, 1); return v; }
	signed short ReadShort() { signed short v; Read(&v, 2); return v; }
	unsigned short ReadUShort() { unsigned short v; Read(&v, 2); return v; }
	signed int ReadInt() { signed int v; Read(&v, 4); return v; }
	unsigned int ReadUInt() { unsigned int v; Read(&v, 4); return v; }
	signed __int64 ReadLong() { signed __int64 v; Read(&v, 8); return v; }
	unsigned __int64 ReadULong() { unsigned __int64 v; Read(&v, 8); return v; }
	float ReadFloat() { float v; Read(&v, 4); return v; }

	void WriteSByte(signed char v) { Write(&v, 1); }
	void WriteByte(unsigned char v) { Write(&v, 1); }
	void WriteShort(signed short v) { Write(&v, 2); }
	void WriteUShort(unsigned short v) { Write(&v, 2); }
	void WriteInt(signed int v) { Write(&v, 4); }
	void WriteUInt(unsigned int v) { Write(&v, 4); }
	void WriteLong(signed __int64 v) { Write(&v, 8); }
	void WriteULong(unsigned __int64 v) { Write(&v, 8); }
	void WriteFloat(float v) { Write(&v, 4); }
	
	std::string ReadFixString(int len = 16);
	void WriteFixString(std::string str, int len = 16);
	std::string ReadL1String();
	void WriteL1String(std::string str);
	std::string ReadL2String();
	void WriteL2String(std::string str);

	void Skip(int len) { this->Seek(len, SEEK_CUR); }
	void Pad(char v, int len) { while (len--) WriteByte(v); }
};

class FileStream : public BinaryStream
{
public:
	FileStream() { this->m_fp = NULL; }
	FileStream(const char *name, const char *mode) { this->Open(name, mode); }
	virtual int Read(void *buf, int len);
	virtual int Write(const void *buf, int len);
	virtual int Seek(int len, int origin);
	virtual bool Open(const char *name, const char *mode);
	virtual void Close();
private:
	FILE *m_fp;
	unsigned int m_pos;
};

class Packet : public BinaryStream
{
public:
	Packet() { this->Clear(); }
	Packet(int iOpCode) { this->Clear(); WriteInt(iOpCode); }
	void Clear() { pos = 0; len = 4; memset(data, 0, 0x2000); }
	Packet(const Packet &packet) { this->Clear(); this->len = packet.len; memcpy(this->data, packet.data, packet.len - 4); }
	int pos;
	int len;
	unsigned char data[0x2000];
	virtual int Read(void *buf, int len);
	virtual int Write(const void *buf, int len);
	virtual int Seek(int len, int origin);
	void WriteHexString(std::string str);
	int OpCode() { return *(int*)data; }
	void Reset() { pos = 0; }
};

class Message
{
public:
	int code;
	char data[0x1000];
	Message(int code)
	{
		this->code = code;
		memset(data, 0, sizeof(data));
		_rpos = _wpos = 0;
	}
	template<typename T> T read()
	{
		T r = *(T*)&data[_rpos];
		_rpos += sizeof(T);
		return r;
	}
	template<typename T> void write(T v)
	{
		*(T*)&data[_wpos] = v;
		_wpos += sizeof(T);
	}
	template<typename T> Message& operator << (T v)
	{
		write(v);
		return *this;
	}
	template<typename T> Message& operator >> (T& v)
	{
		v = read<T>();
		return *this;
	}
protected:
	int _rpos;
	int _wpos;
};

class SPFFileReader : public BinaryStream
{
public:
	SPFFileReader() { this->fp = NULL; }
	~SPFFileReader() { if (this->fp) Close(); }
	SPFFileReader(std::string fileName) { new(this) SPFFileReader(); Open(fileName); }
	bool Open(std::string fileName);
	void Close();
	virtual int Read(void *buf, int len);
	virtual int Write(const void *buf, int len) {return 0;}
	virtual int Seek(int len, int origin);
private:
	FILE *fp;
	int offset;
	int length;
};

struct SPFFileRecord
{
	char spfNo;
	int offset;
	int length;
};

struct SPFFile
{
	int version;
	std::string fileName;
};

class SPFFileManager
{
public:
	SPFFileManager() { this->Initialize(); }
	void Initialize();
	stdext::hash_map<char, SPFFile> spfList;
	stdext::hash_map<int, SPFFileRecord> fileList;

	static SPFFileManager* instance;

};

struct LDTEnumHandle
{
	SPFFileReader* fs;
	int row;
};

class LDTRecord;

class LDTFile
{
public:
	LDTFile(std::string table) { this->InitLDT(table); }
	void InitLDT(std::string table);
	LDTRecord* GetRecord(int id);
	int GetColSize() { return this->colTypes.size(); }
	void* BeginEnum();
	LDTRecord* EnumRecord(void* handle);
	void EndEnum(void* handle);
	static class LDTRecord* GetRecord(int id, std::string table);
	static class LDTFile* GetLDTFile(std::string table);
private:
	std::string tableName;
	stdext::hash_map<std::string, int> colNames;
	std::vector<char> colTypes;
	stdext::hash_map<int, int> rowOffsets;
	int rowCount;
	static stdext::hash_map<std::string, LDTFile*> tables;
	friend class LDTRecord;
};

class LDTRecord
{
public:
	int GetId() { return this->id; }
	int GetInt(const std::string& col);
	//int64 GetInt64(std::string col);
	//float GetFloat(std::string col);
	int GetInt(int col);
	std::string GetString(const std::string& col);
	std::string GetString(int col);
	void Free();
private:
	int id;
	std::vector<void*> data;
	LDTFile *ldtFile;
	friend class LDTFile;
};

class ConfMgr
{
public:
	static void LoadConfig(std::string file, std::string section);
	static void ParseCommand(int argc, char** argv);
	static int GetInt(std::string key);
	static std::string GetString(std::string key);
protected:
	static stdext::hash_map<std::string, std::string> m_hmConf;
};