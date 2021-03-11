#include "server.h"

Server::Server(int ID, int cpu, int mem) {
	this->ID_ = ID;
	int node_cpu = cpu / 2;  // 每个节点的cpu
	int node_mem = mem / 2;  // 每个节点的内存
	
	this->a->cpu_res = node_cpu;  // 初始化节点a
	this->a->mem_res = node_mem;
	this->b->cpu_res = node_cpu;  // 初始化节点b
	this->b->mem_res = node_mem;
}


// 增加服务器负载
int Server::increase(int cpu, int mem, char node) {
	if (node == 'a') {
		this->a->cpu_res -= cpu;
		this->a->cpu_used += cpu;  // 减少剩余，增加使用
		this->a->mem_res -= mem;
		this->a->mem_used += mem;
	}
	else if (node == 'b') {
		this->b->cpu_res -= cpu;
		this->b->cpu_used += cpu;
		this->b->mem_res -= mem;
		this->b->mem_used += mem;
	}
	else {
		std::cout << "节点使用错误！" << std::endl;
		return -1;
	}
	return 0;
}


// 减少服务器负载
int Server::decrease(int cpu, int mem, char node) {
	if (node == 'a') {
		this->a->cpu_res += cpu;
		this->a->cpu_used -= cpu;  // 增加剩余，减少使用
		this->a->mem_res += mem;
		this->a->mem_used -= mem;
	}
	else if (node == 'b') {
		this->b->cpu_res += cpu;
		this->b->cpu_used -= cpu;
		this->b->mem_res += mem;
		this->b->mem_used -= mem;
	}
	else {
		std::cout << "节点使用错误！" << std::endl;
		return -1;
	}
	return 0;
}