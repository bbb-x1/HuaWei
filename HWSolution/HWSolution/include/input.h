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

// ����1����������Ϣ
ServerInfo GenerateServerInfo(string cpu_cores, string mem_size,
	string buy_cost, string power_cost);

// ���ɷ�������Ϣ��
void InputServerInfos(unordered_map<string, ServerInfo>& server_infos);

// ���������������Ϣ��
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);

void InitialzieData(unordered_map<string,ServerInfo>& server_info,unordered_map<string,VMInfo>& vm_infos,unordered_map<int,vector<Request>>& requests_set);

void StatisticInfo(unordered_map<string, VMInfo>& vm_info, unordered_map<int, vector<Request>>& requests_set);

// ����1���������Ϣ
VMInfo GenerateVMInfo(string cpu_cores, string mem_size, string dual_node);

// �����������Ϣ��
void InputVMInfos(unordered_map<string, VMInfo>& vm_infos);

// ���������������Ϣ��
void PrintVMInfos(unordered_map<string, VMInfo>& vm_infos);