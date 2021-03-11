#include <iostream>
#include<fstream>
#include <unordered_map>
#include "../include/input.h"
#include "../include/server.h"
#include "../include/virtualMachine.h"
using namespace std;

/**
 * @brief ����delimiters�ָ��ַ���
 * @param s Դ�ַ���
 * @param tokens �ָ���ַ�������
 * @param delimiters �ָ��/�ָ��ַ���
*/
void split(const string& s, vector<string>& tokens, const string& delimiters)
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

/**
 * @brief
 * @param s Դ�ַ���
 * @param brace �����ַ���
 * @return �������ַ���
*/
string trim(string s, string brace) {
	if (s.empty())
		return s;
	s.erase(0, s.find_first_not_of(brace));
	s.erase(s.find_last_not_of(brace) + 1);
	return s;
}

// ����1����������Ϣ
ServerInfo GenerateServerInfo(string cpu_cores, string mem_size,
	string buy_cost, string power_cost) {
	ServerInfo server;

	// �洢cpu����
	for (int i = 0; i < cpu_cores.size() - 1; i++) {
		server.cpu = 10 * server.cpu + cpu_cores[i] - '0';
	}
	// �洢�ڴ��С
	for (int i = 0; i < mem_size.size() - 1; i++) {
		server.mem = 10 * server.mem + mem_size[i] - '0';
	}
	// �洢����ɱ�
	for (int i = 0; i < buy_cost.size() - 1; i++) {
		server.buy_cost = 10 * server.buy_cost + buy_cost[i] - '0';
	}
	// �洢�ܺĳɱ�
	for (int i = 0; i < power_cost.size() - 1; i++) {
		server.day_power_cost = 10 * server.day_power_cost + power_cost[i] - '0';
	}
	return server;
}


// ���ɷ�������Ϣ��
void InputServerInfos(unordered_map<string, ServerInfo>& server_infos) {
	int server_info_num;
	string server_type, cpu_cores, mem_size, buy_cost, power_cost;
	scanf_s("%d", &server_info_num);

	for (int i = 0; i < server_info_num; i++) {
		cin >> server_type >> cpu_cores >> mem_size >> buy_cost >> power_cost;
		// �洢�ͺ�
		string _server_type = "";
		for (int i = 1; i < server_type.size() - 1; i++) {
			_server_type += server_type[i];
		}
		// �洢��������Ϣ�ṹ��
		ServerInfo server = GenerateServerInfo(cpu_cores, mem_size, buy_cost, power_cost);
		server_infos[_server_type] = server;
	}
}


// ���������������Ϣ��
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos) {
	cout << server_infos.size() << endl;
	unordered_map<string, ServerInfo>::iterator iter;
	for (iter = server_infos.begin(); iter != server_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.buy_cost << "\t" << iter->second.day_power_cost << endl;
}


// ����1���������Ϣ
VMInfo GenerateVMInfo(string cpu_cores, string mem_size, string dual_node) {
	VMInfo vm;
	// �洢cpu����
	for (int i = 0; i < cpu_cores.size() - 1; i++) {
		vm.cpu = 10 * vm.cpu + cpu_cores[i] - '0';
	}
	// �洢�ڴ��С
	for (int i = 0; i < mem_size.size() - 1; i++) {
		vm.mem = 10 * vm.mem + mem_size[i] - '0';
	}
	// �Ƿ�˫�ڵ�
	vm.dual_node = dual_node[0] - '0';

	return vm;
}


// �����������Ϣ��
void InputVMInfos(unordered_map<string, VMInfo>& vm_infos) {
	int vm_info_num;
	string vm_type, cpu_cores, mem_size, dual_node;
	scanf_s("%d", &vm_info_num);

	for (int i = 0; i < vm_info_num; i++) {
		cin >> vm_type >> cpu_cores >> mem_size >> dual_node;
		// �洢�ͺ�
		string _vm_type = "";
		for (int i = 1; i < vm_type.size() - 1; i++) {
			_vm_type += vm_type[i];
		}
		// �洢��������Ϣ�ṹ��
		VMInfo vm = GenerateVMInfo(cpu_cores, mem_size, dual_node);
		vm_infos[_vm_type] = vm;
	}
}


// ���������������Ϣ��
void PrintVMInfos(unordered_map<string, VMInfo>&vm_infos) {
	cout << vm_infos.size() << endl;
	unordered_map<string, VMInfo>::iterator iter;
	for (iter = vm_infos.begin(); iter != vm_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.dual_node<< endl;
}

/**
 * @brief ��ʼ������
 * @param server_info ��������Ϣ����,����Ϊunordered_map<�������ͺ�,������ʵ��>
 * @param vm_infos �������Ϣ����,����Ϊunordered_map<������ͺ�,�����ʵ��>
 * @param requests_set �������м���,����Ϊunordered_map<����,������������>
*/
void InitialzieData(unordered_map<string, ServerInfo>& server_info, unordered_map<string, VMInfo>& vm_infos, unordered_map<int, vector<Request>>& requests_set)
{
	int server_num = 0;		//����������
	int vm_num = 0;			//���������
	int requests_times = 0; //��������
	int item_num = 0;		//ÿ���������
	ifstream ifs;
	ifs.open("resource/training-1.txt", ios::in);
	
	string buf;

	//��ʼ����������Ϣ
	getline(ifs, buf);
	server_num = atoi(buf.c_str());
	vector<string> tokens;
	for (int i = 0; i < server_num; i++)
	{
		ServerInfo si;

		getline(ifs, buf);
		buf = trim(buf, "(");
		buf = trim(buf, ")");
		tokens = vector<string>{};
		split(buf, tokens, ", ");

		si.cpu = atoi(tokens[1].c_str());
		si.mem = atoi(tokens[2].c_str());
		si.buy_cost = atoi(tokens[3].c_str());
		si.day_power_cost = atoi(tokens[4].c_str());

		server_info[tokens[0]] = si;
	}

	//��ʼ���������Ϣ
	getline(ifs, buf);
	vm_num = atoi(buf.c_str());
	for (int i = 0; i < vm_num; i++)
	{
		VMInfo vi;

		getline(ifs, buf);
		buf = trim(buf, "(");
		buf = trim(buf, ")");
		tokens = vector<string>{};
		split(buf, tokens, ", ");

		vi.cpu = atoi(tokens[1].c_str());
		vi.mem = atoi(tokens[2].c_str());
		vi.dual_node = atoi(tokens[3].c_str());

		vm_infos[tokens[0]] = vi;
	}

	//��ʼ����������
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
			buf = trim(buf, "(");
			buf = trim(buf, ")");

			tokens = vector<string>{};
			split(buf, tokens, ", ");
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

/**
 * @brief ͳ��������Ϣ
 * @param vm_infos ���������
 * @param requests_set ���󼯺�
*/
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
	cout << "CPU����" << endl;
	for (auto it = capacitys_cpu_set.cbegin(); it != capacitys_cpu_set.cend(); it++)
	{
		if (it->second > cpu_max)
			cpu_max = it->second;
		cout << "��" << it->first << "��," << "CPU������Ϊ" << it->second << endl;
	}
	cout << "�ڴ�����" << endl;
	for (auto it = capacitys_mem_set.cbegin(); it != capacitys_mem_set.cend(); it++)
	{
		if (it->second > mem_max)
			mem_max = it->second;
		cout << "��" << it->first << "��," << "�ڴ�������Ϊ" << it->second << endl;
	}
	cout << "CPU���������Ϊ:" << cpu_max << endl;
	cout << "�ڴ����������Ϊ:" << mem_max << endl;
}