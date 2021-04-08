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
 * @brief ��ʼ������
 * @param server_info ��������Ϣ����,����Ϊunordered_map<�������ͺ�,������ʵ��>
 * @param vm_infos �������Ϣ����,����Ϊunordered_map<������ͺ�,�����ʵ��>
 * @param requests_set �������м���,����Ϊunordered_map<����,������������>
*/
pair<int, int> InitializeData(
	unordered_map<string, ServerInfo>& server_infos,
	unordered_map<string, VMInfo>& vm_infos,
	vector<vector<Request>>& requests_set,
	vector<int>& record,
	const string file_path);

void Future(vector<vector<Request> >& requests_set,
	vector<int>& record);