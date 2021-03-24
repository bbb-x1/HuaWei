#pragma once
#include <unordered_map>
#include <algorithm>
#include<random>
#include "server.h"
#include "vm.h"
#include "request.h"


// 1个解决方案
class Solution {
public:
	// 已购买的服务器
	int server_number_;
	unordered_map<int, Server> server_resources_;
	// 当前开机服务器
	unordered_map<int, Server*> server_runs_;
	// 当前关机服务器
	unordered_map<int, Server*> server_closes_;
	// 当天购买的服务器，服务器类型->台数
	unordered_map<string, int> one_day_purchase_;
	// 当天部署的虚拟机，服务器id->节点
	vector<pair<int, int>> one_day_create_vm_;
	// 当前开机虚拟机
	// int 为vm_id
	unordered_map<int, VM> vm_runs_;

	// 当天成本
	int day_cost_;
	
	Solution();
	Solution(unordered_map<string, ServerInfo> server_infos,
		unordered_map<string, VMInfo> vm_infos,
		int server_number,unordered_map<int, Server> server_resources,
		unordered_map<int, Server*> server_runs, unordered_map<int, Server*> server_closes,
		unordered_map<int, VM> vm_runs, string buy_server_type,
		vector<vector<Request>>::const_iterator& it);
	Solution(const Solution& copy);
};

// 找到vec里当天成本最低的solution
Solution GetBestSolution(vector<Solution>& solution_vec);

// 把solution实施到main中
void SetSolution(int& server_number, unordered_map<int, Server>& server_resources,
	unordered_map<int, Server*>& server_runs, unordered_map<int, Server*>& server_closes,
	unordered_map<int, VM>& vm_runs, long long& TOTALCOST, Solution solution);
