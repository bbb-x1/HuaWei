#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <float.h>
using namespace std;


// 1条服务器基本信息
typedef struct ServerInfo{
	int cpu = 0;  // CPU核数
	int mem = 0;  // 内存大小
	int buy_cost = 0;  // 购买成本
	int day_power_cost = 0;  // 每日能耗成本
}ServerInfo;

typedef struct ServerNode{
	int cpu_res;  // CPU剩余
	int cpu_used;  // CPU使用
	int mem_res;  // 内存剩余
	int mem_used;  // 内存使用
}Node;

class Server {
private:

	Node  a;  // 节点a
	Node  b;  // 节点b

	// 开机
	void _Open(unordered_map<int, Server*>& server_runs,unordered_map<int, Server*>& server_closes);
	// 关机
	void _Close(unordered_map<int, Server*>& server_runs, unordered_map<int, Server*>& server_closes);
public:
	Server();
	Server(string type, int ID, int cpu, int mem);  // 初始化节点a, b
	int ID_;  // 服务器ID
	string type_;  // 服务器型号
	list<int> own_vm; //一台服务器拥有的虚拟机，以虚拟机内存/CPU排序
	list<int> aown_vm;	//a节点拥有的虚拟机
	list<int> bown_vm;	//b节点拥有的虚拟机
	list<int> cown_vm;	//双节点虚拟机
	int IncreaseUse(int vm_id, int cpu, int mem, char node, unordered_map<int, Server*>& server_runs,
		unordered_map<int, Server*>& server_closes);  // 增加服务器负载
	int DecreaseUse(int vm_id, int cpu, int mem, char node, unordered_map<int, Server*>& server_runs,
		unordered_map<int, Server*>& server_closes);  // 减少服务器负载
	Node get_node(char node);  // 获取节点状态
};

void PurchaseServer(string& server_str, int& server_number,
	long long& BUYCOST, long long& TOTALCOST,
	unordered_map<string, ServerInfo>& server_infos,
	unordered_map<int, Server>& server_resources,
	unordered_map<int, Server*>& server_closes,
	list<Server*>& cpu_sorted_server);

void SelectPurchaseServer(string& max_server_name,
	int& single_need_mem, int& single_need_cpu, double& mem_cpu_ratio,
	unordered_map<string, ServerInfo>& server_infos);

void SelectPurchaseServerNew(string& buy_server_type,
	int& single_need_mem, int& single_need_cpu, int& remain_day, double& mem_cpu_ratio,
	unordered_map<string, ServerInfo>& server_infos);

void PurchaseServerNew(string& server_str, int& server_number,
	long long& BUYCOST, long long& TOTALCOST,
	unordered_map<string, ServerInfo>& server_infos,
	unordered_map<int, Server>& server_resources,
	unordered_map<int, Server*>& server_closes,
	list<Server*>& cpu_sorted_server,
	vector<Server*>& cpu_mem_server);