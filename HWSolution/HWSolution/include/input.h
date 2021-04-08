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
 * @brief 初始化数据
 * @param server_info 服务器信息集合,类型为unordered_map<服务器型号,服务器实体>
 * @param vm_infos 虚拟机信息集合,类型为unordered_map<虚拟机型号,虚拟机实体>
 * @param requests_set 请求序列集合,类型为unordered_map<天数,该天的请求队列>
*/
pair<int, int> InitializeData(
	unordered_map<string, ServerInfo>& server_infos,
	unordered_map<string, VMInfo>& vm_infos,
	vector<vector<Request>>& requests_set,
	vector<int>& record,
	const string file_path);

void Future(vector<vector<Request> >& requests_set,
	vector<int>& record);