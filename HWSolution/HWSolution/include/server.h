#pragma once
#include <iostream>
#include <string>
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
	Node* a;  // 节点a
	Node* b;  // 节点b
public:
	string type_;  // 服务器型号
	Server(string type, int ID, int cpu, int mem);  // 初始化节点a, b
	int IncreaseUse(int cpu, int mem, char node);  // 增加服务器负载
	int DecreaseUse(int cpu, int mem, char node);  // 减少服务器负载
};

void PurchaseServer(string server_str);