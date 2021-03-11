#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "server.h"
#include "virtualMachine.h"
using namespace std;


// 生成1条服务器信息
ServerInfo GenerateServerInfo(string cpu_cores, string mem_size,
	string buy_cost, string power_cost);

// 生成服务器信息表
void InputServerInfos(unordered_map<string, ServerInfo>& server_infos);

// 遍历输出服务器信息表
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);

// 生成1条虚拟机信息
VMInfo GenerateVMInfo(string cpu_cores, string mem_size, string dual_node);

// 生成虚拟机信息表
void InputVMInfos(unordered_map<string, VMInfo>& vm_infos);

// 遍历输出服务器信息表
void PrintVMInfos(unordered_map<string, VMInfo>& vm_infos);