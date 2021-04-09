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

const string kFilePath = "resource/training-2.txt";// 测试数据文件

unordered_map<string, ServerInfo> server_infos;// 服务器信息
unordered_map<string, VMInfo> vm_infos;// 虚拟机信息

int server_number = 0;
unordered_map<int, Server> server_resources;// 已购买的服务器
unordered_map<int, Server*> server_runs;// 当前开机服务器
unordered_map<int, Server*> server_closes;// 当前关机服务器
int vm_count = 0;//虚拟机数量
unordered_map<int, VM> vm_runs;// 当前开机虚拟机// int 为vm_id
vector<vector<Request> > requests_set;// 全部请求序列
list<Server*> cpu_sorted_server;// 以服务器已用cpu排序的服务器列表(大到小)
long long BUYCOST = 0, POWERCOST = 0, TOTALCOST = 0;// 成本

//void Purchase(vector<vector<Request>>::const_iterator& it);
void PrintPurchase(unordered_map<string, int>&);
void PrintMigration(vector<pair<int, pair<int, int> > >& one_day_migrate_vm);
void PrintDeploy(vector<pair<int, int> >);
bool cpu_cmp(Server* first, Server* second);
void Caculator();
void FindMax(int& max_cpu, int& max_mem);
int main(int argc, char **argv){
	vector<int> record;
	string buy_server_type = " ";
	int max_cpu = 0;
	int max_mem = 0;
	//初始化数据
	pair<int, int> day_data = InitializeData(server_infos, vm_infos, requests_set, record, kFilePath);
	FindMax(max_cpu, max_mem);
	//
	int add_plus = 0;
	int del_plus = 0;
	int middle_plus = 0;
	for (int day = 0; day<day_data.first; ++day) {
		//
		unordered_map<string, int> one_day_purchase;			// 当天购买的服务器，服务器类型->台数
		vector<pair<int, pair<int, int> > > one_day_migrate_vm;	//当天迁移的虚拟机，虚拟机id
		vector<pair<int, int>> one_day_create_vm;				// 当天部署的虚拟机，服务器id->节点
		//
		int remain_day = day_data.first - day;
		//
		if (record[day] == 0) {//add 多
			one_day_migrate_vm = MigrateVMMiddle(vm_count, vm_infos, vm_runs,
				server_resources, server_runs, server_closes, cpu_sorted_server, requests_set[day]);
			DeployVmFull(vm_count, server_number, BUYCOST, TOTALCOST,requests_set[day],
				one_day_purchase, one_day_create_vm, remain_day,
				vm_infos, vm_runs,server_infos, server_resources, server_runs, server_closes, cpu_sorted_server);
			++add_plus;
		}
		else if(record[day] == 2){ //del多
			one_day_migrate_vm = MigrateVMEmpty(vm_count, vm_infos, vm_runs,
				server_resources, server_runs, server_closes, cpu_sorted_server, requests_set[day]);
			DeployVmFull(vm_count, server_number, BUYCOST, TOTALCOST, requests_set[day],
				one_day_purchase, one_day_create_vm, remain_day,
				vm_infos, vm_runs, server_infos, server_resources, server_runs, server_closes, cpu_sorted_server);
			++del_plus;
		}
		else {//相近
			one_day_migrate_vm = MigrateVMMiddle(vm_count, vm_infos, vm_runs,
				server_resources, server_runs, server_closes, cpu_sorted_server,requests_set[day]);
			DeployVmMiddle(vm_count, server_number, BUYCOST, TOTALCOST, requests_set[day],
				one_day_purchase, one_day_create_vm, remain_day,
				vm_infos, vm_runs, server_infos, server_resources, server_runs, server_closes, cpu_sorted_server);
			++middle_plus;

		}
		//int actual_vm_count = vm_count * 3 / 100;
		////处理每日请求***************************************************************************
		////list<pair<int, Request*> > list_add;    // 所有add请求(顺序下标，请求)
		//list<pair<int, Request*> > list_del;    //所有del请求(cpu-内存，请求)
		//int add_index = 0;						// one_day_create_vm数组的下标
		////第一步，del分类
		//for (auto req = requests_set[day].begin(); req != requests_set[day].end(); ++req) {
		//	//
		//	//if (req->op_type == ADD) {
		//	//	++vm_count;
		//	//	list_add.push_back(make_pair(add_index, &(*req)));
		//	//	one_day_create_vm.push_back(make_pair(-1, -1));
		//	//	++add_index;
		//	//}
		//	if (req->op_type == DELETE) {
		//		if (vm_runs.find(req->vm_id) == vm_runs.end()) {
		//			list_del.push_front(make_pair(10000, &(*req)));
		//		}
		//		else {
		//			list_del.push_front(make_pair(vm_infos[vm_runs[req->vm_id].vm_str_].cpu - vm_infos[vm_runs[req->vm_id].vm_str_].mem, &(*req)));
		//		}
		//	}
		//}
		//////第二步，排序add请求（cpu大的在前）
		////list_add.sort([](pair<int, Request*> first, pair<int, Request*> second) {
		////	int first_cm = vm_infos[first.second->vm_type].cpu - vm_infos[first.second->vm_type].mem;
		////	int second_cm = vm_infos[second.second->vm_type].cpu - vm_infos[second.second->vm_type].mem;
		////	return first_cm > second_cm;
		////	});
		//
		////第二步，排序del请求（cpu大的在前）
		//list_del.sort([](pair<int, Request*> first, pair<int, Request*> second) {
		//	return first.first > second.first;
		//	});
		//
		////迁移***********************************************************************************************
		//
		Caculator();
		cout << "总成本为:" << TOTALCOST << endl;
		cout << day << endl;
		//
		 ////输出
		 //PrintPurchase(one_day_purchase);
		 //PrintMigration(one_day_migrate_vm);
		 //PrintDeploy(one_day_create_vm);
		//读入未来数据
		if (day_data.second > 0) {
			Future(requests_set, record);
			--day_data.second;
		}
	}
	cout << add_plus << endl;
	cout << del_plus << endl;
	cout << middle_plus << endl;
	return 0;
}

void FindMax(int& max_cpu, int& max_mem) {
	for (auto iter = vm_infos.begin(); iter != vm_infos.end(); ++iter) {
		if (iter->second.dual_node == 1) {
			if (iter->second.cpu / 2 > max_cpu) {
				max_cpu = iter->second.cpu / 2;
			}
			if (iter->second.mem / 2 > max_mem) {
				max_mem = iter->second.mem/ 2;
			}
		}
		else {
			if (iter->second.cpu > max_cpu) {
				max_cpu = iter->second.cpu;
			}
			if (iter->second.mem > max_mem) {
				max_mem = iter->second.mem / 2;
			}
		}
	}
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