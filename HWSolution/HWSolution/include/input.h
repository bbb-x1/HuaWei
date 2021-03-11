#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "server.h"
#include "virtualMachine.h"
using namespace std;


// ����1����������Ϣ
ServerInfo GenerateServerInfo(string cpu_cores, string mem_size,
	string buy_cost, string power_cost);

// ���ɷ�������Ϣ��
void InputServerInfos(unordered_map<string, ServerInfo>& server_infos);

// ���������������Ϣ��
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);

// ����1���������Ϣ
VMInfo GenerateVMInfo(string cpu_cores, string mem_size, string dual_node);

// �����������Ϣ��
void InputVMInfos(unordered_map<string, VMInfo>& vm_infos);

// ���������������Ϣ��
void PrintVMInfos(unordered_map<string, VMInfo>& vm_infos);