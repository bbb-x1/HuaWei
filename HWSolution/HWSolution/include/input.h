#pragma once
#include <iostream>
#include <string>
#include<queue>
#include <unordered_map>
#include "server.h"
#include "vm.h"
#include "request.h"
using namespace std;

void Split(const string& s, vector<string>& tokens, const string& delimiters = " ");
string Trim(string s);

void InitialzieData(unordered_map<string,ServerInfo>& server_info,unordered_map<string,VMInfo>& vm_infos,unordered_map<int,vector<Request>>& requests_set);

void StatisticInfo(unordered_map<string, VMInfo>& vm_info, unordered_map<int, vector<Request>>& requests_set);


// 遍历输出服务器信息表
void PrintVMInfos(unordered_map<string, VMInfo>& vm_infos);

// 遍历输出服务器信息表
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);