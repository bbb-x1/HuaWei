#pragma once
#include <iostream>
#include <string>
#include<queue>
#include <unordered_map>
#include "server.h"
#include "vm.h"
#include "request.h"
using namespace std;

void split(const string& s, vector<string>& tokens, const string& delimiters = " ");
string trim(string s, string brace);

// 生成1条服务器信息
ServerInfo GenerateServerInfo(string cpu_cores, string mem_size,
	string buy_cost, string power_cost);

// 生成服务器信息表
void InputServerInfos(unordered_map<string, ServerInfo>& server_infos);

// 遍历输出服务器信息表
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);

void InitialzieData(unordered_map<string,ServerInfo>& server_info,unordered_map<string,VMInfo>& vm_infos,unordered_map<int,vector<Request>>& requests_set);

void StatisticInfo(unordered_map<string, VMInfo>& vm_info, unordered_map<int, vector<Request>>& requests_set);

// 生成1条虚拟机信息
VMInfo GenerateVMInfo(string cpu_cores, string mem_size, string dual_node);

// 生成虚拟机信息表
void InputVMInfos(unordered_map<string, VMInfo>& vm_infos);

// 遍历输出服务器信息表
void PrintVMInfos(unordered_map<string, VMInfo>& vm_infos);