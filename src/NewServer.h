#pragma once

class Server
{
public:
	virtual void Init() {}
	virtual void Listen();
	virtual void SetPublicAddr(std::string strIp, unsigned short uiPort);
	virtual std::string GetPublicIp() { return m_strPublicIp; }
	virtual uint GetPublicPort() { return m_uiListenPort; }
	virtual void SetServerID(uint uiServerID) { this->m_uiServerID = uiServerID; }
	virtual uint GetServerID() { return this->m_uiServerID; }
	virtual void Process() {}
protected:
	virtual void OnConnect(Connection *pConn);
	virtual void OnDisconnect(Connection *pConn) {}

	std::vector<Connection*> m_vpSockets;
	std::string m_strListenIp;
	unsigned short m_uiListenPort;
	std::string m_strPublicIp;
	unsigned short m_uiPublicPort;
	unsigned short m_uiServerID;
	Socket m_listenSocket;
};

class SingleServer : public Server
{
public:
	SingleServer();
	virtual void Process();
	GameWorld* GetWorld() { return m_pkWorld; }
protected:
	virtual void OnDisconnect(Connection *pConn);

	double m_fUpdateTime;
	LARGE_INTEGER m_uiTickFreq;
	GameWorld* m_pkWorld;
};
