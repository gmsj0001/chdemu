

#pragma once

class Socket
{
public:
	Socket() { fd = NULL; }
	Socket(const Socket &socket) { this->fd = socket.fd; }
	int Read(void *data, int len);
	int Write(const void *data, int len);
	bool Init();
	bool Connect(std::string ip, unsigned short port);
	bool Accept(Socket *socket);
	bool Listen();
	bool Bind(std::string ip, unsigned short port);
	bool Close();
	bool SetBlock(bool isBlock);
	bool SetDelay(bool isDelay);
private:
	SOCKET fd;
};

class Cipher
{
public:
	virtual void Encrypt(char *data, int len);
	virtual void Decrypt(char *data, int len);
};

class Cipher2 : public Cipher
{
public:
	virtual void Encrypt(char *data, int len);
	virtual void Decrypt(char *data, int len);
};

class Player;
class Handler;
class Server;

class Connection
{
public:
	Connection(Server* pkServer, Socket socket);
	bool Recv(Packet *packet);
	bool Send(Packet *packet);
	bool SendNow(std::string hexString);
	void SendNow1(std::string hexString);
	
	void SetPlayer(Player* pkPlayer) { m_pkPlayer = pkPlayer; }
	Player* GetPlayer() { return m_pkPlayer; }
	int GetUserID() { return m_iUserID; }
	void SetUserID(int iUserID) { m_iUserID = iUserID; }
	Server* GetServer() { return m_pkServer; } 
	bool IsDown() { return m_bIsDown; }
	void Process();

	Handler* GetHandler() { return m_pkHandler; }

protected:
	

	char m_RecvBuf[0x2004];
	int m_iRecvPos;

	int m_iUserID;
	Player* m_pkPlayer;
	Handler *m_pkHandler;
	Server* m_pkServer;

	Socket m_Socket;
	std::string m_strRemoteIp;
	bool m_bIsDown;
	short m_iCipherType;
	Cipher *m_pkCipher;

};
