#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "include/server.h"
#include "include/input.h"
#include "include/vm.h"
#include "include/request.h"
using namespace std;


// 测试数据文件
const string kFilePath = "resource/training-1.txt";

// 服务器信息
unordered_map<string, ServerInfo> server_infos;
// 虚拟机信息
unordered_map<string, VMInfo> vm_infos;


// 已购买的服务器
int server_number = 0;
unordered_map<int, Server> server_resources;

// 当前开机服务器
unordered_map<int, Server*> server_runs;
// 当前关机服务器
unordered_map<int, Server*> server_closes;

// 当前开机虚拟机
// int 为vm_id
unordered_map<int, VM> vm_runs;

// 全部请求序列
vector<vector<Request>> requests_set;

// 成本
long long BUYCOST = 0, POWERCOST = 0, TOTALCOST = 0;

//void Purchase(vector<vector<Request>>::const_iterator& it);
void PrintPurchase(unordered_map<string, int>&);
void PrintMigration();
void PrintDeploy(vector<pair<int, int>>);


int main(int argc, char **argv){

	//初始化数据
	InitializeData(server_infos, vm_infos, requests_set, kFilePath);

	// 要购买的服务器类型
	string buy_server_type = SelectPurchaseServer(server_infos);
	
	//每天的工作
	int day = 0;  // 天数
	for (auto it = requests_set.cbegin(); it != requests_set.cend(); ++it) {
		day++;

		// 购买服务器和部署虚拟机
		// 当天购买的服务器，服务器类型->台数
		unordered_map<string, int> one_day_purchase;
		// 当天部署的虚拟机，服务器id->节点
		vector<pair<int, int>> one_day_create_vm;
		for (auto itv = it->cbegin(); itv != it->cend(); ++itv) {
			if (itv->op_type == ADD) {
				pair<int, int>create_vm = CreateVM(itv->vm_id, itv->vm_type, vm_infos, vm_runs,
					server_resources, server_runs, server_closes);
				// 当服务器资源不够创建虚拟机时
				if (create_vm.second == -1) {
					PurchaseServer(buy_server_type, server_number, server_infos, server_resources,
						server_closes, BUYCOST, TOTALCOST);
					// 在当天购买服务器字典里加入刚买的服务器
					if (one_day_purchase.find(buy_server_type) == one_day_purchase.end()) {
						one_day_purchase[buy_server_type] = 1;
					}
					else {
						one_day_purchase[buy_server_type]++;
					}
					// 再次尝试创建虚拟机
					create_vm = CreateVM(itv->vm_id, itv->vm_type, vm_infos, vm_runs,
						server_resources, server_runs, server_closes);
				}
				one_day_create_vm.push_back(create_vm);
			}
			else {
				vm_runs[itv->vm_id].Del(vm_infos, vm_runs, server_resources,
					server_runs, server_closes);
			}
		}

		// 输出
		PrintPurchase(one_day_purchase);
		PrintMigration();
		PrintDeploy(one_day_create_vm);
	}

	return 0;
}

//void Purchase(vector<vector<Request>>::const_iterator& it) {
//	if (it== requests_set.cbegin()) {
//		//统计数据
//		pair<int, int> cpu_mem_needed = StatisticInfo(vm_infos, vm_runs, requests_set);
//
//		int max_buy_cost = 0;
//		string max_server_name;
//		for (auto i = server_infos.cbegin(); i != server_infos.cend(); ++i) {
//			if (i->second.buy_cost > max_buy_cost) {
//				max_buy_cost = i->second.buy_cost;
//				max_server_name = i->first;
//			}
//		}
//		int max_server = max(cpu_mem_needed.first / server_infos[max_server_name].cpu,
//			cpu_mem_needed.second / server_infos[max_server_name].mem);
//
//
//		for (int i = 0; i < max_server + 100; ++i) {
//			PurchaseServer(max_server_name, server_number, server_infos,
//				server_resources, server_closes, BUYCOST, TOTALCOST);
//		}
//
//		cout << "purchase," << max_server << endl;
//		cout << '(' << max_server_name << ',' << max_server << ')' << endl;
//	}
//	else {
//		cout << '(' << "purchase," << 0 << ')' << endl;
//	}
//}

// 输出购买服务器
void PrintPurchase(unordered_map<string, int>& one_day_purchase) {
	int server_kind_num = 0;
	for (auto it = one_day_purchase.cbegin(); it != one_day_purchase.cend(); ++it) {
		++server_kind_num;
	}
	cout << '(' << "purchase," << server_kind_num << ')' << endl;
	for (auto it = one_day_purchase.cbegin(); it != one_day_purchase.cend(); ++it) {
		cout << '(' << it->first << "," << it->second << ')' << endl;
	}
}

// 输出迁移
void PrintMigration() {
	cout << '(' << "migration," << 0 << ')' << endl;
}

// 输出服务器部署
void PrintDeploy(vector<pair<int, int>> one_day_create_vm) {
	for (auto it = one_day_create_vm.cbegin(); it != one_day_create_vm.cend(); ++it) {
		if (it->second == 0) {
			cout << '(' << it->first<< ')' << endl;
		}
		else if (it->second == 1) {
			cout << '(' << it->first << ",A" << ')' << endl;
		}
		else {
			cout << '(' << it->first << ",B" << ')' << endl;
		}
	}
}