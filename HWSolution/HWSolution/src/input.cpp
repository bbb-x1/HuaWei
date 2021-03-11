#include <iostream>
#include <unordered_map>
#include "../include/input.h"
#include "../include/server.h"
#include "../include/virtualMachine.h"
using namespace std;

// 生成1条服务器信息
ServerInfo GenerateServerInfo(string cpu_cores, string mem_size,
	string buy_cost, string power_cost) {
	ServerInfo server;

	// 存储cpu核数
	for (int i = 0; i < cpu_cores.size() - 1; i++) {
		server.cpu = 10 * server.cpu + cpu_cores[i] - '0';
	}
	// 存储内存大小
	for (int i = 0; i < mem_size.size() - 1; i++) {
		server.mem = 10 * server.mem + mem_size[i] - '0';
	}
	// 存储购买成本
	for (int i = 0; i < buy_cost.size() - 1; i++) {
		server.buy_cost = 10 * server.buy_cost + buy_cost[i] - '0';
	}
	// 存储能耗成本
	for (int i = 0; i < power_cost.size() - 1; i++) {
		server.day_power_cost = 10 * server.day_power_cost + power_cost[i] - '0';
	}
	return server;
}


// 生成服务器信息表
void InputServerInfos(unordered_map<string, ServerInfo>& server_infos) {
	int server_info_num;
	string server_type, cpu_cores, mem_size, buy_cost, power_cost;
	scanf_s("%d", &server_info_num);

	for (int i = 0; i < server_info_num; i++) {
		cin >> server_type >> cpu_cores >> mem_size >> buy_cost >> power_cost;
		// 存储型号
		string _server_type = "";
		for (int i = 1; i < server_type.size() - 1; i++) {
			_server_type += server_type[i];
		}
		// 存储服务器信息结构体
		ServerInfo server = GenerateServerInfo(cpu_cores, mem_size, buy_cost, power_cost);
		server_infos[_server_type] = server;
	}
}


// 遍历输出服务器信息表
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos) {
	cout << server_infos.size() << endl;
	unordered_map<string, ServerInfo>::iterator iter;
	for (iter = server_infos.begin(); iter != server_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.buy_cost << "\t" << iter->second.day_power_cost << endl;
}


// 生成1条虚拟机信息
VMInfo GenerateVMInfo(string cpu_cores, string mem_size, string dual_node) {
	VMInfo vm;
	// 存储cpu核数
	for (int i = 0; i < cpu_cores.size() - 1; i++) {
		vm.cpu = 10 * vm.cpu + cpu_cores[i] - '0';
	}
	// 存储内存大小
	for (int i = 0; i < mem_size.size() - 1; i++) {
		vm.mem = 10 * vm.mem + mem_size[i] - '0';
	}
	// 是否双节点
	vm.dual_node = dual_node[0] - '0';

	return vm;
}


// 生成虚拟机信息表
void InputVMInfos(unordered_map<string, VMInfo>& vm_infos) {
	int vm_info_num;
	string vm_type, cpu_cores, mem_size, dual_node;
	scanf_s("%d", &vm_info_num);

	for (int i = 0; i < vm_info_num; i++) {
		cin >> vm_type >> cpu_cores >> mem_size >> dual_node;
		// 存储型号
		string _vm_type = "";
		for (int i = 1; i < vm_type.size() - 1; i++) {
			_vm_type += vm_type[i];
		}
		// 存储服务器信息结构体
		VMInfo vm = GenerateVMInfo(cpu_cores, mem_size, dual_node);
		vm_infos[_vm_type] = vm;
	}
}


// 遍历输出服务器信息表
void PrintVMInfos(unordered_map<string, VMInfo>&vm_infos) {
	cout << vm_infos.size() << endl;
	unordered_map<string, VMInfo>::iterator iter;
	for (iter = vm_infos.begin(); iter != vm_infos.end(); iter++)
		cout << iter->first << "\t" << iter->second.cpu << "\t" << iter->second.mem <<
		"\t" << iter->second.dual_node<< endl;
}
