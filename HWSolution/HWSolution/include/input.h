#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "server.h"
using namespace std;


// 生成1条服务器信息
ServerInfo GenerateServerInfo(string server_type, string cpu_cores, string mem_size,
	string buy_cost, string power_cost);


// 生成服务器信息表
void InputServerInfos(string file_path, unordered_map<string, ServerInfo>
	& server_infos);

// 遍历输出服务器信息表
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);