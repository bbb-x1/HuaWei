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

// ���������������Ϣ��
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos) {
	cout << server_infos.size() << endl;
	unordered_map<string, ServerInfo>::iterator iter;
	for (iter = server_infos.begin(); iter != server_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.buy_cost << "\t" << iter->second.day_power_cost << endl;
}

// ���������������Ϣ��
void PrintVMInfos(unordered_map<string, VMInfo>&vm_infos) {
	cout << vm_infos.size() << endl;
	unordered_map<string, VMInfo>::iterator iter;
	for (iter = vm_infos.begin(); iter != vm_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.dual_node<< endl;
}

pair<int, int> InitializeData(unordered_map<string, ServerInfo>& server_infos, unordered_map<string, VMInfo>& vm_infos,
	vector<vector<Request>>& requests_set, const string file_path)
{
	int server_num = 0;		//����������
	int vm_num = 0;			//���������
	int requests_times = 0; //��������
	int item_num = 0;		//ÿ���������
	//FILE* stream1;
	//freopen_s(&stream1, file_path.c_str(), "rb", stdin);
	char buf[1024];
	//��ʼ����������Ϣ
	cin.getline(buf, 1024);
	server_num = atoi(buf);
	vector<string> tokens;
	string sbuf;
	for (int i = 0; i < server_num; i++)
	{
		ServerInfo si;

		cin.getline(buf,1024);
		sbuf = string(buf);
		sbuf = Trim(sbuf);
		tokens = vector<string>{};
		Split(sbuf, tokens, ", ");

		si.cpu = atoi(tokens[1].c_str());
		si.mem = atoi(tokens[2].c_str());
		si.buy_cost = atoi(tokens[3].c_str());
		si.day_power_cost = atoi(tokens[4].c_str());

		server_infos[tokens[0]] = si;
	}

	//��ʼ���������Ϣ
	cin.getline(buf,1024);
	vm_num = atoi(buf);
	for (int i = 0; i < vm_num; i++)
	{
		VMInfo vi;

		cin.getline(buf,1024);
		sbuf = string(buf);
		sbuf = Trim(sbuf);
		tokens = vector<string>{};
		Split(sbuf, tokens, ", ");

		vi.cpu = atoi(tokens[1].c_str());
		vi.mem = atoi(tokens[2].c_str());
		vi.dual_node = atoi(tokens[3].c_str());

		vm_infos[tokens[0]] = vi;
	}

	//��ʼ����������
	cin.getline(buf, 1024);
	sbuf = string(buf);
	tokens = vector<string>{};
	Split(sbuf, tokens, ", ");
	vector<Request> queue_requests;
	requests_times = atoi(tokens[1].c_str());
	int requests_total = atoi(tokens[0].c_str());
	for (int i = 0; i < requests_times; i++)
	{
		cin.getline(buf, 1024);
		queue_requests = vector<Request>{};
		item_num = atoi(buf);
		for (int j = 0; j < item_num; j++)
		{
			Request r;
			cin.getline(buf,1024);
			sbuf = string(buf);
			sbuf = Trim(sbuf);

			tokens = vector<string>{};
			Split(sbuf, tokens, ", ");
			if (tokens[0] == "add")
			{
				r.op_type = ADD;
				r.vm_type = tokens[1];
				r.vm_id = atoi(tokens[2].c_str());
			}
			else
			{
				r.op_type = DELETE;
				r.vm_id = atoi(tokens[1].c_str());
				r.vm_type = "NAN";
			}
			queue_requests.push_back(r);
		}
		requests_set.push_back(queue_requests);
	}

	return make_pair(requests_total,requests_total - requests_times);
}


double StatisticInfo(unordered_map<string, VMInfo>& vm_infos, vector<vector<Request>>& requests_set)
{
	unordered_map<int, VM> vm_runs;
	int cpu_set = 0;
	int mem_set = 0;
	int cpu_max = 0;
	int mem_max = 0;
	for (auto it = requests_set.cbegin(); it != requests_set.cend(); it++)
	{
		vector<Request> vr = *it;

		for (int i = 0; i < vr.size(); i++)
		{
			//���������
			if (vr[i].op_type == ADD)
			{
				VM vm(vr[i].vm_id, vr[i].vm_type);
				vm_runs[vm.vm_id_] = vm;
				cpu_set += vm_infos[vr[i].vm_type].cpu;
				mem_set += vm_infos[vr[i].vm_type].mem;

			}
			else  //ɾ�������
			{
				cpu_set -= vm_infos[vm_runs[vr[i].vm_id].vm_str_].cpu;
				mem_set -= vm_infos[vm_runs[vr[i].vm_id].vm_str_].mem;
			}
			if (cpu_set > cpu_max) {
				cpu_max = cpu_set;
			}
			if (mem_set > mem_max) {
				mem_max = mem_set;
			}
		}
	}

	return double(mem_max)/cpu_max;
}


void Future(vector<vector<Request> >& requests_set) {
	//ÿ���������
	int item_num = 0;
	vector<string> tokens;
	string sbuf;
	char buf[1024];
	//��ʼ����������
	vector<Request> queue_requests;
	tokens = vector<string>{};
	cin.getline(buf, 1024);
	queue_requests = vector<Request>{};
	item_num = atoi(buf);
	for (int j = 0; j < item_num; j++)
	{
		Request r;
		cin.getline(buf, 1024);
		sbuf = string(buf);
		sbuf = Trim(sbuf);
		tokens = vector<string>{};
		Split(sbuf, tokens, ", ");
		if (tokens[0] == "add")
		{
			r.op_type = ADD;
			r.vm_type = tokens[1];
			r.vm_id = atoi(tokens[2].c_str());
		}
		else
		{
			r.op_type = DELETE;
			r.vm_id = atoi(tokens[1].c_str());
			r.vm_type = "NAN";
		}
		queue_requests.push_back(r);
	}
	if (!queue_requests.empty()) {
		requests_set.push_back(queue_requests);
	}

}
