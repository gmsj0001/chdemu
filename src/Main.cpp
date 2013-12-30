#include "stdafx.h"

int main(int argc, char** argv)
{
	printf("*****************************************\n");
	printf("*                                       *\n");
	printf("*  CHDEMU LaTale Emulator Build 131230  *\n");
	printf("*  Based on client 280709               *\n");
	printf("*  Powered by gmsj0001   http://lxf.me  *\n");
	printf("*                                       *\n");
	printf("*****************************************\n");

	ConfMgr::LoadConfig("chdemu.ini", "CHDEMU");
	ConfMgr::ParseCommand(argc, argv);
	new DataBase();
	new SPFFileManager();
	new StageManager();
	new StatusManager();
	new EffectManager();
	new ItemManager();
	new QuestManager();
	new SkillManager();
	new EventManager();
	
	Server *server;
	server = new SingleServer();
	server->SetServerID(1);
	std::string strIp = ConfMgr::GetString("ip");
	if (strIp == "")
		strIp = "127.0.0.1";
	int iPort = ConfMgr::GetInt("port");
	if (iPort == 0)
		iPort = 10000;
	server->SetPublicAddr(strIp, iPort);
	server->Listen();
	while (true)
	{
		server->Process();
		Sleep(10);
	}
	return 0;
 }
