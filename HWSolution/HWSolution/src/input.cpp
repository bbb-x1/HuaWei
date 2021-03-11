#include <iostream>
#include <unordered_map>
#include "../include/input.h"
#include "../include/server.h"
using namespace std;

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
void InputServerInfos(string file_path, unordered_map<string, ServerInfo>& server_infos) {
	// �����ض���
	FILE* stream1;
	freopen_s(&stream1, file_path.c_str(), "rb", stdin);
	
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