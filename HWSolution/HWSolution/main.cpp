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
unordered_map<int, Server> server_runs;
// 当前关机服务器
unordered_map<int, Server> server_closes;

// 当前开机虚拟机
// int 为vm_id
unordered_map<int, VM> vm_runs;

// 全部请求序列
vector<vector<Request>> requests_set;

// 成本
long long BUYCOST = 0, POWERCOST = 0, TOTALCOST = 0;

void Purchase(vector<vector<Request>>::const_iterator& it);
void Migration();
int main(int argc, char **argv){

	//初始化数据
	//InitializeData(server_infos, vm_infos, requests_set);*/
	//// 打印服务器信息
	//PrintServerInfos(server_infos);
	//// 打印虚拟机信息
	//PrintVMInfos(vm_infos);

	//每天的工作
	for (auto it = requests_set.cbegin(); it != requests_set.cend(); ++it) {
		Purchase(it);
		Migration();
		for (auto itv = it->cbegin(); itv != it->cend(); ++itv) {
			if (itv->op_type == ADD) {
				CreateVM(itv->vm_id, itv->vm_type, vm_infos, vm_runs,
					server_resources, server_runs, server_closes);
			}
			else {
				vm_runs[itv->vm_id].Del(vm_infos, vm_runs, server_resources,
					server_runs, server_closes);
			}

		}
	}

	system("pause");



	return 0;
}

void Purchase(vector<vector<Request>>::const_iterator& it) {
	if (it== requests_set.cbegin()) {
		//统计数据
		pair<int, int> cpu_mem_needed = StatisticInfo(vm_infos, vm_runs, requests_set);

		int max_buy_cost = 0;
		string max_server_name;
		for (auto i = server_infos.cbegin(); i != server_infos.cend(); ++i) {
			if (i->second.buy_cost > max_buy_cost) {
				max_buy_cost = i->second.buy_cost;
				max_server_name = i->first;
			}
		}
		int max_server = max(cpu_mem_needed.first / server_infos[max_server_name].cpu,
			cpu_mem_needed.second / server_infos[max_server_name].mem);


		for (int i = 0; i < max_server + 10; ++i) {
			PurchaseServer(max_server_name, server_number, server_infos,
				server_resources, server_closes, BUYCOST, TOTALCOST);
		}

		cout << "purchase," << max_server << endl;
		cout << '(' << max_server_name << ',' << max_server << ')' << endl;
	}
	else {
		cout << '(' << "purchase," << 0 << ')' << endl;
	}
}

void Migration() {
	cout << '(' << "migration," << 0 << ')' << endl;
}