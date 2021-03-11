#pragma once
#include <iostream>

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
	Server(int ID, int cpu, int mem);  // 初始化节点a, b
	int increase(int cpu, int mem, char node);  // 增加服务器负载
	int decrease(int cpu, int mem, char node);  // 减少服务器负载
};
