#include <iostream>
#include<fstream>
#include <unordered_map>
#include "../include/input.h"
#include "../include/server.h"
#include "../include/vm.h"
using namespace std;

pair<int, int> InitializeData(
	unordered_map<string, ServerInfo>& server_infos,
	unordered_map<string, VMInfo>& vm_infos,
	vector<vector<Request>>& requests_set,
	vector<int>& record,
	const string file_path)
{
	FILE* stream1;
	freopen_s(&stream1, file_path.c_str(), "rb", stdin);

	char buf[1024];
	//初始化服务器信息
	cin.getline(buf, 1024);
	int server_num = atoi(buf);

	for (int i = 0; i < server_num; i++)
	{
		cin.getline(buf, 1024);
		int index = 0;
		while (buf[index] != ',') ++index;
		string temp_name = string(buf + 1, buf + index);
		sscanf_s(buf + index + 1, " %d, %d, %d, %d",
			&server_infos[temp_name].cpu,
			&server_infos[temp_name].mem,
			&server_infos[temp_name].buy_cost,
			&server_infos[temp_name].day_power_cost);
	}

	//初始化虚拟机信息
	cin.getline(buf, 1024);
	int vm_num = atoi(buf);
	for (int i = 0; i < vm_num; i++)
	{
		cin.getline(buf, 1024);
		int index = 0;
		while (buf[index] != ',') ++index;
		string temp_name = string(buf + 1, buf + index);
		sscanf_s(buf + index + 1, " %d, %d, %d",
			&vm_infos[temp_name].cpu,
			&vm_infos[temp_name].mem,
			&vm_infos[temp_name].dual_node);
	}


	//初始化请求序列
	vector<Request> queue_requests;
	cin.getline(buf, 1024);

	int requests_times;
	int requests_total;

	sscanf_s(buf, "%d %d", &requests_total, &requests_times);


	for (int i = 0; i < requests_times; i++)
	{
		double record_add = 1.0;
		double record_del = 1.0;
		cin.getline(buf, 1024);
		int item_num = atoi(buf);
		queue_requests = vector<Request>{};
		for (int j = 0; j < item_num; j++)
		{
			Request r;
			cin.getline(buf, 1024);
			string oper = string(buf + 1, buf + 4);

			if (oper == "add")
			{
				r.op_type = ADD;
				int index = 6;
				while (buf[index] != ',') ++index;
				r.vm_type = string(buf + 6, buf + index);
				sscanf_s(buf + index + 1, " %d", &r.vm_id);
				++record_add;
			}
			else
			{
				r.op_type = DELETE;
				sscanf_s(buf + 5, " %d", &r.vm_id);
				r.vm_type = "NAN";
				++record_del;

			}
			queue_requests.push_back(r);
		}
		if (record_add / record_del > 2.0) {
			record.push_back(0);
		}
		else if (record_del / record_add > 2.0) {
			record.push_back(2);
		}
		else {
			record.push_back(1);
		}
		requests_set.push_back(queue_requests);
	}
	return make_pair(requests_total, requests_total - requests_times);
}


void Future(vector<vector<Request> >& requests_set,
	vector<int>& record) {
	//每天操作次数
	int item_num = 0;
	char buf[1024];
	//初始化请求序列
	vector<Request> queue_requests = {};

	cin.getline(buf, 1024);
	item_num = atoi(buf);
	double record_add = 1.0;
	double record_del = 1.0;
	for (int j = 0; j < item_num; j++)
	{
		Request r;
		cin.getline(buf, 1024);
		string oper = string(buf + 1, buf + 4);
		if (oper == "add")
		{
			r.op_type = ADD;
			int index = 6;
			while (buf[index] != ',') ++index;
			r.vm_type = string(buf + 6, buf + index);
			sscanf_s(buf + index + 1, " %d", &r.vm_id);
			++record_add;
		}
		else
		{
			r.op_type = DELETE;
			sscanf_s(buf + 5, " %d", &r.vm_id);
			r.vm_type = "NAN";
			++record_del;
		}
		queue_requests.push_back(r);
	}
	if (record_add / record_del > 2.0) {
		record.push_back(0);
	}
	else if (record_del / record_add > 2.0) {
		record.push_back(2);
	}
	else {
		record.push_back(1);
	}
	requests_set.push_back(queue_requests);
}
