#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include "include/server.h"
#include "include/input.h"
#include "include/vm.h"
#include "include/request.h"
#include "include/solution.h"
using namespace std;
constexpr auto SOLUTION_NUM = 4;  // 每一天随机尝试多少次
const string kFilePath = "resource/training-1.txt";  // 测试数据文件


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

// 互斥锁
mutex m;


// 函数声明
//void Purchase(vector<vector<Request>>::const_iterator& it);
void PushSolution(unordered_map<string, ServerInfo> server_infos,
	unordered_map<string, VMInfo> vm_infos,
	int server_number, unordered_map<int, Server> server_resources,
	unordered_map<int, Server*> server_runs, unordered_map<int, Server*> server_closes,
	unordered_map<int, VM> vm_runs, string buy_server_type,
	vector<vector<Request>>::const_iterator& it, vector<Solution>& solution_vec);
void PrintPurchase(unordered_map<string, int>&);
void PrintMigration();
void PrintDeploy(vector<pair<int, int>>);


int main(int argc, char **argv){

	//初始化数据
	InitializeData(server_infos, vm_infos, requests_set, kFilePath);
	double mem_cpu_ratio = StatisticInfo(vm_infos, requests_set);

	// 要购买的服务器类型
	string buy_server_type = SelectPurchaseServer(mem_cpu_ratio,server_infos);

	// 每天的工作
	for (auto it = requests_set.cbegin(); it != requests_set.cend(); ++it) {
		thread threads[SOLUTION_NUM];  // 多线程数组
		// 当天所有的解决方案
		vector<Solution> solution_vec;
		// 随机找到SOLUTION_NUM个解决方案
		for (int i = 0; i < SOLUTION_NUM; i++) {
			// 多线程产生解决方案
			threads[i] = std::thread(PushSolution, server_infos, vm_infos, server_number, server_resources, server_runs, 
				server_closes, vm_runs, buy_server_type, std::ref(it), std::ref(solution_vec));
		}
		for (auto& it : threads)
		{
			it.join();
		}
		// 得到当天成本最低的解决方案
		Solution best_solution = GetBestSolution(solution_vec);
		// 把最好的方案实施到main中
		SetSolution(server_number, server_resources, server_runs, server_closes, vm_runs,
			TOTALCOST, best_solution);

		// 输出
		PrintPurchase(best_solution.one_day_purchase_);
		PrintMigration();
		PrintDeploy(best_solution.one_day_create_vm_);
		solution_vec.clear();
		solution_vec.shrink_to_fit();
	}

	return 0;
}

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

// 将solution存入solution_vec
void PushSolution(unordered_map<string, ServerInfo> server_infos,
	unordered_map<string, VMInfo> vm_infos,
	int server_number, unordered_map<int, Server> server_resources,
	unordered_map<int, Server*> server_runs, unordered_map<int, Server*> server_closes,
	unordered_map<int, VM> vm_runs, string buy_server_type,
	vector<vector<Request>>::const_iterator& it, vector<Solution>& solution_vec) {
	Solution solution(server_infos, vm_infos, server_number, server_resources, server_runs,
		server_closes, vm_runs, buy_server_type, it);
	{
		lock_guard<mutex> g1(m);
		solution_vec.push_back(solution);
	}
}