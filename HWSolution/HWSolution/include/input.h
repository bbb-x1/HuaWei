#pragma once
#include <iostream>

#include <string>
#include<queue>
#include <unordered_map>
#include "server.h"
#include "vm.h"
#include "request.h"
using namespace std;

/**
 * @brief 根据delimiters分割字符串
 * @param s 源字符串
 * @param tokens 分割后字符串集合
 * @param delimiters 分割符/分割字符串
*/
void Split(const string& s, vector<string>& tokens, const string& delimiters = " ");

/**
 * @brief
 * @param s 源字符串
 * @param brace 多余字符串
 * @return 处理后的字符串
*/
string Trim(string s);

/**
 * @brief 初始化数据
 * @param server_info 服务器信息集合,类型为unordered_map<服务器型号,服务器实体>
 * @param vm_infos 虚拟机信息集合,类型为unordered_map<虚拟机型号,虚拟机实体>
 * @param requests_set 请求序列集合,类型为unordered_map<天数,该天的请求队列>
*/
void InitializeData(unordered_map<string,ServerInfo>& server_infos,unordered_map<string,VMInfo>& vm_infos,
	vector<vector<Request>>& requests_set, const string file_path);

/**
 * @brief 统计请求信息
 * @param vm_infos 虚拟机集合
 * @param requests_set 请求集合
*/
double StatisticInfo(unordered_map<string, VMInfo>& vm_infos, vector<vector<Request>>& requests_set);


// 遍历输出服务器信息表
void PrintVMInfos(unordered_map<string, VMInfo>& vm_infos);

// 遍历输出服务器信息表
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);