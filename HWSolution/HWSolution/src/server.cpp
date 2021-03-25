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
	this->a.cpu_used = 0;
	this->a.mem_used = 0;
	this->b.cpu_res = node_cpu;  // ��ʼ���ڵ�b
	this->b.mem_res = node_mem;
	this->b.cpu_used = 0;
	this->b.mem_used = 0;
}


// ����
void Server::_Open(unordered_map<int, Server*>& server_runs,
	unordered_map<int, Server*>& server_closes) {
	server_runs[this->ID_] = this;
	server_closes.erase(this->ID_);
}

// �ػ�
void Server::_Close(unordered_map<int, Server*>& server_runs,
	unordered_map<int, Server*>& server_closes) {
	server_closes[this->ID_] = this;
	server_runs.erase(this->ID_);
}


// ���ӷ���������
int Server::IncreaseUse(int vm_id, int cpu, int mem, char node,
	unordered_map<int, Server*>& server_runs,
	unordered_map<int, Server*>& server_closes) {

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
int Server::DecreaseUse(int vm_id, int cpu, int mem, char node,
	unordered_map<int, Server*>& server_runs,
	unordered_map<int, Server*>& server_closes) {
	
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
	unordered_map<int, Server*>& server_closes,
	list<Server*>& cpu_re_sorted_server,
	list<Server*>& cpu_sorted_server,
	long long& BUYCOST, long long& TOTALCOST) {
	int sn = server_number;
	Server purchased_server(server_str, sn, server_infos[server_str].cpu, server_infos[server_str].mem);
	server_resources[sn] = purchased_server;
	server_closes[sn] = &server_resources[sn];
	cpu_re_sorted_server.push_back(&server_resources[sn]);
	cpu_sorted_server.push_back(&server_resources[sn]);
	++server_number;
	BUYCOST += server_infos[server_str].buy_cost;
	TOTALCOST += server_infos[server_str].buy_cost;
}

bool sort_compare(const pair<string, double>& item1, const pair<string, double>& item2) {
	return item1.second < item2.second;
}


string SelectPurchaseServer(double mem_cpu_ratio, 
	unordered_map<string, ServerInfo> server_infos, 
	int single_need_cpu, int single_need_mem) {
	string max_server_name;
	int sever_map_len = server_infos.size();
	vector<pair<string, double>> ratio_array(sever_map_len);
	int index = 0;
	for (auto iter = server_infos.cbegin(); iter != server_infos.cend(); ++iter) {
		ratio_array[index].first = iter->first;
		ratio_array[index].second = double(iter->second.mem) / double(iter->second.cpu);
		++index;
	}
	sort(ratio_array.begin(),ratio_array.end(),sort_compare);

	pair<string, double> temp = { " ", mem_cpu_ratio };
	auto rivet = lower_bound(ratio_array.begin(), ratio_array.end(),temp,sort_compare);
	string result_server = (*rivet).first;
	double min = double(server_infos[result_server].buy_cost) / (double(server_infos[result_server].cpu) + server_infos[result_server].mem);
	int limit = 5;
	for (auto iter = rivet + 1; iter != ratio_array.end() && limit!=0; ++iter) {
		if (server_infos[iter->first].cpu < single_need_cpu *2 || server_infos[iter->first].mem < single_need_mem*2)
			continue;
		double current_min = double(server_infos[(*iter).first].buy_cost) / (double(server_infos[(*iter).first].cpu) + server_infos[(*iter).first].mem);
		if (min > current_min) {
			min = current_min;
			result_server = (*iter).first;
		}
		--limit;
	}

	return result_server;
}