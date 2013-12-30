#include <windows.h>
#include <winnt.h>

#pragma comment(lib, "ws2_32.lib")

#define APPNAME "GhostCHD_PacketSniffer"
#define BUFFERSIZE 512

#pragma pack(push, 1)
struct Packet
{
	unsigned short len;
	unsigned short cipher;
	unsigned char data[0x2000];
};
#pragma pack(pop)

struct MapData
{
	int setindex;
	int getindex;
	Packet packets[BUFFERSIZE];
};

HANDLE hMapFile;
MapData* pMapData;

void Decrypt(unsigned char *data, unsigned int len)
{
	const char *key = "qmfaktnpgjs";
	for (unsigned int i = 0; i < len; ++i)
	{
		if (data[i] != 0 && data[i] != key[i % 11])
		{
			data[i] ^= key[i % 11];
		}
	}
}

int PASCAL Hook_send(SOCKET s, const char *buf, int len, int flags)
{
	int pos = 0;
	while (pos < len)
	{
		Packet pac = {0};
		memcpy(&pac, &buf[pos], *(short*)&buf[pos]);
		if (pac.cipher != 0)
			Decrypt(pac.data, pac.len - 4);
		pac.cipher = 100;	//signature for send
		memcpy(&pMapData->packets[pMapData->setindex], &pac, sizeof(pac));
		pMapData->setindex = (pMapData->setindex + 1) % BUFFERSIZE;
		pos += pac.len;
	}
	return send(s, buf, len, flags);
}

static unsigned char recvBuf[0x2004];
static unsigned short recvPos = 0;

int PASCAL Hook_recv(SOCKET s, char FAR * buf, int len, int flags)
{
	int ret = recv(s, buf, len, flags);
	if (ret > 0)
	{
		int pos = 0;
		while (pos < ret)
		{
			if (recvPos < 4)	//phase of recv 4-byte header
			{
				unsigned int n = ret - pos < 4 ? ret - pos : 4;
				memcpy(&recvBuf[recvPos], &buf[pos], n);
				recvPos += n;
				pos += n;
			}
			else if (recvPos >= 4 && recvPos < *(unsigned short*)recvBuf)	//phase of recv body
			{
				unsigned int n = ret - pos < *(unsigned short*)recvBuf - recvPos ? ret - pos : *(unsigned short*)recvBuf - recvPos;
				memcpy(&recvBuf[recvPos], &buf[pos], n);
				recvPos += n;
				pos += n;
				if (recvPos >= 4 && recvPos == *(unsigned short*)recvBuf)
				{
					Packet pac = {0};
					memcpy(&pac, recvBuf, recvPos);
					if (pac.cipher != 0)
						Decrypt(pac.data, pac.len - 4);
					pac.cipher = 101;	//signature for recv
					memcpy(&pMapData->packets[pMapData->setindex], &pac, sizeof(pac));
					pMapData->setindex = (pMapData->setindex + 1) % BUFFERSIZE;
					recvPos = 0;
				}
			}
		}
	}
	return ret;
}

void hookiat(DWORD pfnold, DWORD pfnnew)
{
	DWORD codebase = 0x400000;
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)codebase;
	PIMAGE_NT_HEADERS nt_header = (PIMAGE_NT_HEADERS)(codebase + dos_header->e_lfanew);
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_header);
	++section;	//rdata is the second section
	DWORD* cmpdata = (DWORD*)(codebase + section->VirtualAddress);
	DWORD oldprotect;
	VirtualProtect(cmpdata, 4096, PAGE_READWRITE, &oldprotect);
	while (true)
	{
		for (int i = 0; i < 1024; ++i)	//iat is at the beginning of rdata so we only compare 4096 bytes
		{
			if (cmpdata[i] == pfnold)
			{
				cmpdata[i] = pfnnew;
				VirtualProtect(cmpdata, 4096, oldprotect, &oldprotect);
				return;
			}
		}
		Sleep(10);
	}
}

extern "C" __declspec(dllexport) int main()
{
	hMapFile = CreateFileMappingA((HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(MapData), APPNAME);
	pMapData = (MapData*)MapViewOfFile(hMapFile, 2, 0, 0, 0);
	DWORD pfn_send = (DWORD)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "send");
	DWORD pfn_recv = (DWORD)GetProcAddress(GetModuleHandleA("ws2_32.dll"), "recv");

	hookiat(pfn_send, (DWORD)&Hook_send);
	hookiat(pfn_recv, (DWORD)&Hook_recv);
	
	return (DWORD)pMapData;
}