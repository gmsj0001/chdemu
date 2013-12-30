#include "stdafx.h"

void Server::Listen()
{
	this->m_strListenIp = this->m_strPublicIp == "127.0.0.1" ? "127.0.0.1" : "0.0.0.0";
	this->m_uiListenPort = this->m_uiPublicPort;
	this->m_listenSocket.Init();
	this->m_listenSocket.SetBlock(false);
	this->m_listenSocket.Bind(this->m_strListenIp, this->m_uiListenPort);
	this->m_listenSocket.Listen();
	printf("\rEmulator server listening on port %d.\n", this->m_uiListenPort);
}

void Server::SetPublicAddr(std::string strIp, unsigned short uiPort)
{
	this->m_strPublicIp = strIp;
	this->m_uiPublicPort = uiPort;
}

void Server::OnConnect(Connection *pConn)
{
	pConn->SendNow("64 00 00 00 00 01");
}

SingleServer::SingleServer()
{
	this->m_pkWorld = new GameWorld();
	this->m_pkWorld->Init();
}

void SingleServer::Process()
{

	//GameZone MainLoop
	double fCurTime = GetTickCount() / 1000.0f;
	double fTimeSpan = fCurTime - this->m_fUpdateTime;
	m_pkWorld->Process(fTimeSpan);
	this->m_fUpdateTime = fCurTime;

	//试试先循环世界再处理封包
	//侦听连接
	Socket sock;
	while (this->m_listenSocket.Accept(&sock))
	{
		//printf("\rClient connected.\n");
		sock.SetBlock(false);
		sock.SetDelay(false);	//VERY IMPORTANT!
		Connection *client = new Connection(this, sock);
		this->m_vpSockets.push_back(client);
		this->OnConnect(client);
	}
	//接收封包
	Packet packet;
	for (std::vector<Connection*>::iterator iter = this->m_vpSockets.begin(); iter != this->m_vpSockets.end(); )
	{
		(*iter)->Process();
		if ((*iter)->IsDown())	//断线
		{
			//printf("\rClient disconnected.\n");
			this->OnDisconnect(*iter);
			delete *iter;
			iter = this->m_vpSockets.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void SingleServer::OnDisconnect(Connection *pConn)
{
	if (pConn->GetUserID() != NULL)	//connection lost
	{
		if (pConn->GetPlayer() != NULL)
		{
			m_pkWorld->PlayerLeave(pConn->GetPlayer());
			pConn->GetPlayer()->SaveGame();	//跨线程的delete player还未实现
			//delete pConn->GetPlayer();	
			//又不能删了 MapGroup CheckRemove时有判断对象类型
			pConn->SetPlayer(NULL);
		}
	}
}

