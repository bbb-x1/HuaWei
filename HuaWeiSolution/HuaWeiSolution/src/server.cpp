#include "server.h"

Server::Server(int ID, int cpu, int mem) {
	this->ID_ = ID;
	int node_cpu = cpu / 2;  // ÿ���ڵ��cpu
	int node_mem = mem / 2;  // ÿ���ڵ���ڴ�
	
	this->a->cpu_res = node_cpu;  // ��ʼ���ڵ�a
	this->a->mem_res = node_mem;
	this->b->cpu_res = node_cpu;  // ��ʼ���ڵ�b
	this->b->mem_res = node_mem;
}


// ���ӷ���������
int Server::increase(int cpu, int mem, char node) {
	if (node == 'a') {
		this->a->cpu_res -= cpu;
		this->a->cpu_used += cpu;  // ����ʣ�࣬����ʹ��
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
		std::cout << "�ڵ�ʹ�ô���" << std::endl;
		return -1;
	}
	return 0;
}


// ���ٷ���������
int Server::decrease(int cpu, int mem, char node) {
	if (node == 'a') {
		this->a->cpu_res += cpu;
		this->a->cpu_used -= cpu;  // ����ʣ�࣬����ʹ��
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
		std::cout << "�ڵ�ʹ�ô���" << std::endl;
		return -1;
	}
	return 0;
}