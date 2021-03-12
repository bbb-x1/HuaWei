#include <iostream>
#include<fstream>
#include <unordered_map>
#include "../include/input.h"
#include "../include/server.h"
#include "../include/vm.h"
using namespace std;


void Split(const string& s, vector<string>& tokens, const string& delimiters)
{
	string::size_type lastPos = s.find_first_not_of(delimiters, 0);
	string::size_type pos = s.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos)
	{
		tokens.push_back(s.substr(lastPos, pos - lastPos));
		lastPos = s.find_first_not_of(delimiters, pos);
		pos = s.find_first_of(delimiters, lastPos);
	}
}


string Trim(string s) {
	if (s.empty())
		return s;
	s = s.substr(1, s.length() - 2);
	return s;
}

// 遍历输出服务器信息表
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos) {
	cout << server_infos.size() << endl;
	unordered_map<string, ServerInfo>::iterator iter;
	for (iter = server_infos.begin(); iter != server_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.buy_cost << "\t" << iter->second.day_power_cost << endl;
}

// 遍历输出服务器信息表
void PrintVMInfos(unordered_map<string, VMInfo>&vm_infos) {
	cout << vm_infos.size() << endl;
	unordered_map<string, VMInfo>::iterator iter;
	for (iter = vm_infos.begin(); iter != vm_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.dual_node<< endl;
}


void InitializeData(unordered_map<string, ServerInfo>& server_infos, unordered_map<string, VMInfo>& vm_infos, unordered_map<int, vector<Request>>& requests_set)
{
	int server_num = 0;		//服务器数量
	int vm_num = 0;			//虚拟机数量
	int requests_times = 0; //请求天数
	int item_num = 0;		//每天操作次数
	ifstream ifs;
	ifs.open("resource/training-1.txt", ios::in);
	
	string buf;

	//初始化服务器信息
	getline(ifs, buf);
	server_num = atoi(buf.c_str());
	vector<string> tokens;
	for (int i = 0; i < server_num; i++)
	{
		ServerInfo si;

		getline(ifs, buf);
		buf = Trim(buf);
		tokens = vector<string>{};
		Split(buf, tokens, ", ");

		si.cpu = atoi(tokens[1].c_str());
		si.mem = atoi(tokens[2].c_str());
		si.buy_cost = atoi(tokens[3].c_str());
		si.day_power_cost = atoi(tokens[4].c_str());

		server_infos[tokens[0]] = si;
	}

	//初始化虚拟机信息
	getline(ifs, buf);
	vm_num = atoi(buf.c_str());
	for (int i = 0; i < vm_num; i++)
	{
		VMInfo vi;

		getline(ifs, buf);
		buf = Trim(buf);
		tokens = vector<string>{};
		Split(buf, tokens, ", ");

		vi.cpu = atoi(tokens[1].c_str());
		vi.mem = atoi(tokens[2].c_str());
		vi.dual_node = atoi(tokens[3].c_str());

		vm_infos[tokens[0]] = vi;
	}

	//初始化请求序列
	getline(ifs, buf);
	vector<Request> queue_requests;
	requests_times = atoi(buf.c_str());
	for (int i = 0; i < requests_times; i++)
	{
		getline(ifs, buf);
		queue_requests = vector<Request>{};
		item_num = atoi(buf.c_str());
		for (int j = 0; j < item_num; j++)
		{
			Request r;
			getline(ifs, buf);
			buf = Trim(buf);

			tokens = vector<string>{};
			Split(buf, tokens, ", ");
			if (tokens[0] == "add")
			{
				r.op_type = ADD;
				r.vm_type = tokens[1];
				r.vm_id = tokens[2];
			}
			else
			{
				r.op_type = DELETE;
				r.vm_id = tokens[1];
				r.vm_type = "NAN";
			}
			queue_requests.push_back(r);
		}
		cout << "test" << endl;
		requests_set[i] = queue_requests;
	}
}

void StatisticInfo(unordered_map<string, VMInfo>& vm_infos, unordered_map<int, vector<Request>>& requests_set)
{
	unordered_map<int, int> capacitys_cpu_set;
	unordered_map<int, int> capacitys_mem_set;
	for (auto it = requests_set.cbegin(); it != requests_set.cend(); it++)
	{
		vector<Request> vr = it->second;
		for (int i = 0; i < vr.size(); i++)
		{
			capacitys_cpu_set[it->first] += vm_infos[vr[i].vm_type].cpu;
			capacitys_mem_set[it->first] += vm_infos[vr[i].vm_type].mem;
		}
	}
	int cpu_max = -1, mem_max = -1;
	cout << "CPU需求" << endl;
	for (auto it = capacitys_cpu_set.cbegin(); it != capacitys_cpu_set.cend(); it++)
	{
		if (it->second > cpu_max)
			cpu_max = it->second;
		cout << "第" << it->first << "天," << "CPU需求量为" << it->second << endl;
	}
	cout << "内存需求" << endl;
	for (auto it = capacitys_mem_set.cbegin(); it != capacitys_mem_set.cend(); it++)
	{
		if (it->second > mem_max)
			mem_max = it->second;
		cout << "第" << it->first << "天," << "内存需求量为" << it->second << endl;
	}
	cout << "CPU需求量最大为:" << cpu_max << endl;
	cout << "内存需求量最大为:" << mem_max << endl;
}