#include "../include/server.h"

Server::Server() {

}


Server::Server(string type, int ID, int cpu, int mem) {
	this->type_ = type;  // �������ͺ�
	this->ID_ = ID;
	int node_cpu = cpu / 2;  // ÿ���ڵ��cpu
	int node_mem = mem / 2;  // ÿ���ڵ���ڴ�
	
	this->a.cpu_res = node_cpu;  // ��ʼ���ڵ�a
	this->a.mem_res = node_mem;
	this->b.cpu_res = node_cpu;  // ��ʼ���ڵ�b
	this->b.mem_res = node_mem;
}


// ����
void Server::_Open(unordered_map<int, Server>& server_runs,
	unordered_map<int, Server>& server_closes) {
	server_runs[this->ID_] = *this;
	server_closes.erase(this->ID_);
}

// �ػ�
void Server::_Close(unordered_map<int, Server>& server_runs,
	unordered_map<int, Server>& server_closes) {
	server_closes[this->ID_] = *this;
	server_runs.erase(this->ID_);
}


// ���ӷ���������
int Server::IncreaseUse(int cpu, int mem, char node, 
	unordered_map<int, Server>& server_runs,
	unordered_map<int, Server>& server_closes) {
	// ���Ӹ���ǰ�������գ��򿪻�
	if (this->a.cpu_used == 0 && this->a.mem_used == 0
		&& this->b.cpu_used == 0 && this->b.mem_used == 0)
		_Open(server_runs, server_closes);
	if (node == 'a') {
		this->a.cpu_res -= cpu;
		this->a.cpu_used += cpu;  // ����ʣ�࣬����ʹ��
		this->a.mem_res -= mem;
		this->a.mem_used += mem;
	}
	else if (node == 'b') {
		this->b.cpu_res -= cpu;
		this->b.cpu_used += cpu;
		this->b.mem_res -= mem;
		this->b.mem_used += mem;
	}
	else {
		std::cout << "�ڵ�ʹ�ô���" << std::endl;
		return -1;
	}
	return 0;
}


// ���ٷ���������
int Server::DecreaseUse(int cpu, int mem, char node,
	unordered_map<int, Server>& server_runs,
	unordered_map<int, Server>& server_closes) {
	if (node == 'a') {
		this->a.cpu_res += cpu;
		this->a.cpu_used -= cpu;  // ����ʣ�࣬����ʹ��
		this->a.mem_res += mem;
		this->a.mem_used -= mem;
	}
	else if (node == 'b') {
		this->b.cpu_res += cpu;
		this->b.cpu_used -= cpu;
		this->b.mem_res += mem;
		this->b.mem_used -= mem;
	}
	else {
		std::cout << "�ڵ�ʹ�ô���" << std::endl;
		return -1;
	}
	// ���ٸ��غ�������գ���ػ�
	if (this->a.cpu_used == 0 && this->a.mem_used == 0
		&& this->b.cpu_used == 0 && this->b.mem_used == 0)
		_Close(server_runs, server_closes);
	return 0;
}


// ��ȡ�ڵ�״̬
Node Server::get_node(char node) {
	if (node == 'a') {
		return this->a;
	}
	return this->b;
}


void PurchaseServer(string& server_str, int &server_number, 
	unordered_map<string, ServerInfo> &server_infos, 
	unordered_map<int, Server>& server_resources,
	unordered_map<int, Server>& server_closes,
	long long& BUYCOST, long long& TOTALCOST) {
	Server purchased_server(server_str, server_number, server_infos[server_str].cpu, server_infos[server_str].mem);
	server_resources[server_number] = purchased_server;
	server_closes[server_number] = purchased_server;
	++server_number;
	BUYCOST += server_infos[server_str].buy_cost;
	TOTALCOST += server_infos[server_str].buy_cost;
}