#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "server.h"
using namespace std;


// ����1����������Ϣ
ServerInfo GenerateServerInfo(string server_type, string cpu_cores, string mem_size,
	string buy_cost, string power_cost);


// ���ɷ�������Ϣ��
void InputServerInfos(string file_path, unordered_map<string, ServerInfo>
	& server_infos);

// ���������������Ϣ��
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);