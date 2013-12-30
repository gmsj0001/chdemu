#include "stdafx.h"

int Socket::Write(const void *data, int len)
{
	int ret = send(this->fd, (const char*)data, len, 0);
	return ret;
}

int Socket::Read(void *data, int len)
{
	int ret = recv(this->fd, (char*)data, len, 0);
	//__asm int 3;
	if (ret == 0)	//socket closed
	{
		return -1;
	}
	else if (ret == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)
			return 0;
		else if (err == WSAENETDOWN)
			return -1;
	}
	return ret;
}

bool Socket::Init()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return true;
}

bool Socket::Accept(Socket *socket)
{
	sockaddr_in addr;
	int len = sizeof(addr);
	SOCKET s;
	s = accept(this->fd, (sockaddr*)&addr, &len);
	if (s == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		//if (err == WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	socket->fd = s;
	return true;
}

bool Socket::Bind(std::string ip, unsigned short port)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
	bind(this->fd, (sockaddr*)&addr, sizeof(addr));
	return true;
}

bool Socket::Listen()
{
	listen(this->fd, SOMAXCONN);
	return true;
}

bool Socket::SetBlock(bool isBlock)
{
	unsigned long ul = isBlock ? 0 : 1;
	int ret = ioctlsocket(this->fd, FIONBIO, (unsigned long*)&ul);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool Socket::SetDelay(bool isDelay)
{
	BOOL bNoDelay = isDelay ? FALSE : TRUE;
	int ret = setsockopt(this->fd, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
	if (ret == SOCKET_ERROR)
		return false;
	return true;
}

bool Socket::Connect( std::string ip, unsigned short port )
{
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
	connect(this->fd, (sockaddr*)&addr, sizeof(addr));
	return true;
}

bool Socket::Close()
{
	closesocket(this->fd);
	this->fd = 0;	//尽快将失效的fd置0，fd重用会引来很没有逻辑的bug
	return true;
}

void Cipher::Decrypt(char *data, int len)
{
	for (int i = len - 1; i > 0; --i)
	{
		data[i] ^= data[i - 1];
	}
	data[0] ^= 118;
}

void Cipher::Encrypt(char *data, int len)
{
	data[0] ^= 118;
	for (int i = 1; i < len; ++i)
	{
		data[i] ^= data[i - 1];
	}
}

void Cipher2::Encrypt(char *data, int len)
{
	char *key = "qmfaktnpgjs";
	for (int i = 0; i < len; ++i)
	{
		if (data[i] != 0 && data[i] != key[i % 11])
			data[i] ^= key[i % 11];
	}
}

void Cipher2::Decrypt(char *data, int len)
{
	char *key = "qmfaktnpgjs";
	for (int i = 0; i < len; ++i)
	{
		if (data[i] != 0 && data[i] != key[i % 11])
			data[i] ^= key[i % 11];
	}
}

Connection::Connection(Server* pkServer, Socket socket)
{
	 m_iRecvPos = 0;
	 m_iCipherType = 0;
	 m_pkCipher = NULL;
	 m_pkPlayer = NULL;
	 m_pkServer = pkServer;
	 m_Socket = socket;
	 m_iUserID = -1;
	 m_bIsDown = false;
	 m_strRemoteIp = "127.0.0.1";
	 m_pkHandler = new Handler(this);
}

bool Connection::Recv(Packet *packet)
{
	int ret;
	if (this->m_iRecvPos < 4)
	{
		ret = m_Socket.Read(&this->m_RecvBuf[this->m_iRecvPos], 4 - m_iRecvPos);
		if (ret > 0)
		{
			this->m_iRecvPos += ret;
		}
		else if (ret == -1)
		{
			this->m_bIsDown = true; //wo xiang qu shi......................
			m_Socket.Close();
			return false;
		}
	}
	if (this->m_iRecvPos >= 4 && this->m_iRecvPos != *(short*)this->m_RecvBuf)
	{
		ret = m_Socket.Read(&this->m_RecvBuf[this->m_iRecvPos], *(short*)this->m_RecvBuf - this->m_iRecvPos);
		if (ret > 0)
		{
			this->m_iRecvPos += ret;
		}
		else if (ret == -1)
		{
			this->m_bIsDown = true;
			m_Socket.Close();
			return false;
		}
	}
	if (this->m_iRecvPos >= 4 && this->m_iRecvPos == *(short*)this->m_RecvBuf)	//Bug check 不做pos>=4判断则可能会出现pos=len=0造成后面memcpy fffffffc溢出
	{
		short len = *(short*)&this->m_RecvBuf[0];
		short cipherType = *(short*)&this->m_RecvBuf[2];
		char *data = &this->m_RecvBuf[4];
		if (cipherType == 1)
		{
			if (this->m_pkCipher == NULL)
			{
				m_iCipherType = 1;
				if ((data[0] ^ 'v') == 101)				//兼容新外服
					this->m_pkCipher = new Cipher();
				else if ((data[0] ^ 'q') == 101)
					this->m_pkCipher = new Cipher2();
			}
			if (this->m_pkCipher != NULL)
				this->m_pkCipher->Decrypt(data, len - 4);
		}
		packet->len = len;
		packet->pos = 0;
		memset(packet->data, 0, 0x2000);
		//__asm int 3;
		memcpy(packet->data, data, len - 4);
		this->m_iRecvPos = 0;	//!!!!!!!!!!!!!!!!! don't forget!
		return true;
	}
	return false;
}

bool Connection::Send(Packet *packet)
{
	char sendBuf[0x2004];
	*(short*)&sendBuf[0] = packet->len;
	*(short*)&sendBuf[0 + 2] = this->m_iCipherType;
	memcpy(&sendBuf[0 + 4], packet->data, packet->len - 4);
	char *data = &sendBuf[0 + 4];
	if (this->m_iCipherType == 1)
	{
		if (this->m_pkCipher != NULL)
			this->m_pkCipher->Encrypt(data, packet->len - 4);
	}
	int ret = m_Socket.Write(sendBuf, packet->len);
	if (ret > 0)
	{
		return true;
	}
	else if (ret == -1)
	{
		this->m_bIsDown = true;
		m_Socket.Close();
	}
	return false;
}

bool Connection::SendNow(std::string hexString)
{
	Packet pac;
	pac.WriteHexString(hexString);
	return this->Send(&pac);
}

void Connection::SendNow1(std::string hexString)
{
	Packet pac;
	pac.WriteHexString(hexString);
	pac.pos = 4;
	pac.WriteLong(GetPlayer()->GetGUID());
	Send(&pac);
}

void Connection::Process()
{
	Packet packet;
	while (Recv(&packet))
	{
		m_pkHandler->ProcessPacket(packet);
	}
}