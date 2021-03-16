#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
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
	int ID_;  // 服务器ID
	Node  a;  // 节点a
	Node  b;  // 节点b
	// 开机
	void _Open(unordered_map<int, Server*>& server_runs,unordered_map<int, Server*>& server_closes);
	// 关机
	void _Close(unordered_map<int, Server*>& server_runs, unordered_map<int, Server*>& server_closes);
public:
	Server();
	Server(string type, int ID, int cpu, int mem);  // 初始化节点a, b
	string type_;  // 服务器型号
	int IncreaseUse(int cpu, int mem, char node, unordered_map<int, Server*>& server_runs,
		unordered_map<int, Server*>& server_closes);  // 增加服务器负载
	int DecreaseUse(int cpu, int mem, char node, unordered_map<int, Server*>& server_runs,
		unordered_map<int, Server*>& server_closes);  // 减少服务器负载
	Node get_node(char node);  // 获取节点状态
};

void PurchaseServer(string& server_str, int& server_number,
	unordered_map<string, ServerInfo>& server_infos,
	unordered_map<int, Server>& server_resources,
	unordered_map<int, Server*>& server_closes,
	long long& BUYCOST, long long& TOTALCOST);