#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <algorithm>
#include "include/server.h"
#include "include/input.h"
#include "include/vm.h"
#include "include/request.h"

using namespace std;


// 测试数据文件
const string kFilePath = "resource/training-2.txt";

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

//虚拟机数量
int vm_count = 0;
// 当前开机虚拟机
// int 为vm_id
unordered_map<int, VM> vm_runs;

// 全部请求序列
vector<vector<Request> > requests_set;

// 以服务器已用cpu排序的服务器列表(大到小)
list<Server*> cpu_sorted_server;

// 成本
long long BUYCOST = 0, POWERCOST = 0, TOTALCOST = 0;

//void Purchase(vector<vector<Request>>::const_iterator& it);
void PrintPurchase(unordered_map<string, int>&);
void PrintMigration(vector<pair<int, pair<int, int> > >& one_day_migrate_vm);
void PrintDeploy(vector<pair<int, int> >);
bool cpu_cmp(Server* first, Server* second);
bool mem_divide_cpu_cmp(pair<int, VM> p1, pair<int, VM>p2);
void Caculator();

int main(int argc, char **argv){
	//初始化数据
	pair<int, int> day_data = InitializeData(server_infos, vm_infos, requests_set, kFilePath);
	//
	for (int day = 0; day<day_data.first; ++day) {
		//

		unordered_map<string, int> one_day_purchase;			// 当天购买的服务器，服务器类型->台数
		vector<pair<int, pair<int, int> > > one_day_migrate_vm;	//当天迁移的虚拟机，虚拟机id
		vector<pair<int, int>> one_day_create_vm;				// 当天部署的虚拟机，服务器id->节点
		//迁移虚拟机
		cpu_sorted_server.sort(cpu_cmp);
		one_day_migrate_vm = MigrateVM(vm_count, vm_infos, vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
		cpu_sorted_server.sort(cpu_cmp);
		//部署虚拟机
		DeployVm(vm_count, server_number, BUYCOST, TOTALCOST,
			requests_set[day], one_day_purchase, one_day_create_vm, vm_infos, vm_runs, 
			server_infos, server_resources, server_runs, server_closes, cpu_sorted_server);
		////
		//Caculator();
		//cout << "总成本为:" << TOTALCOST << endl;
		//cout << day << endl;
		////
		//输出
		PrintPurchase(one_day_purchase);
		PrintMigration(one_day_migrate_vm);
		PrintDeploy(one_day_create_vm);
		//读入未来数据
		if (day_data.second > 0) {
			Future(requests_set);
			--day_data.second;
		}
	}
	return 0;
}

void Caculator(){
	for (auto it = server_runs.begin(); it != server_runs.end(); it++)
	{
		TOTALCOST += server_infos[it->second->type_].day_power_cost;
	}
}

// 输出购买服务器
void PrintPurchase(unordered_map<string, int>& one_day_purchase) {
	int server_kind_num = one_day_purchase.size();
	//for (auto it = one_day_purchase.cbegin(); it != one_day_purchase.cend(); ++it) {
	//	++server_kind_num;
	//}
	cout << '(' << "purchase," << server_kind_num << ')' << endl;
	for (auto it = one_day_purchase.cbegin(); it != one_day_purchase.cend(); ++it) {
		cout << '(' << it->first << "," << it->second << ')' << endl;
	}
}

// 输出迁移
void PrintMigration(vector<pair<int, pair<int, int> > >& one_day_migrate_vm) {
	int nums = one_day_migrate_vm.size();
	cout << '(' << "migration," << nums << ')' << endl;
	for (auto it = one_day_migrate_vm.begin(); it != one_day_migrate_vm.cend(); ++it) {
		if ((*it).second.second == 2) {
			cout << '(' << (*it).first << ',' << (*it).second.first << ')' << endl;
		}
		else {
			char node = 'A' + (*it).second.second;
			cout << '(' << (*it).first << ',' << (*it).second.first << ','<<node<< ')' << endl;
		}
	}

}

// 输出服务器部署
void PrintDeploy(vector<pair<int, int>> one_day_create_vm) {
	for (auto it = one_day_create_vm.cbegin(); it != one_day_create_vm.cend(); ++it) {
		if (it->second == 2) {
			cout << '(' << it->first<< ')' << endl;
		}
		else if (it->second == 0) {
			cout << '(' << it->first << ",A" << ')' << endl;
		}
		else {
			cout << '(' << it->first << ",B" << ')' << endl;
		}
	}
}

bool cpu_cmp(Server* first, Server* second){
	int first_cpu = (*first).get_node('a').cpu_used + (*first).get_node('b').cpu_used;
	int second_cpu = (*second).get_node('a').cpu_used + (*second).get_node('b').cpu_used;
	return first_cpu > second_cpu;
}

bool mem_divide_cpu_cmp(pair<int, VM> p1, pair<int, VM>p2) {
	double p1_mem_divide_cpu = double(vm_infos[p1.second.vm_str_].mem) / 
		double(vm_infos[p1.second.vm_str_].cpu);
	double p2_mem_divide_cpu = double(vm_infos[p2.second.vm_str_].mem) /
		double(vm_infos[p2.second.vm_str_].cpu);
	return p1_mem_divide_cpu > p2_mem_divide_cpu;
}