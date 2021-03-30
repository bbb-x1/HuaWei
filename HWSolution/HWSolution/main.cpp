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
	InitializeData(server_infos, vm_infos, requests_set, kFilePath);
	double mem_cpu_ratio = StatisticInfo(vm_infos, requests_set);

	// 拿到虚拟机里最大的cpu和内存需求
	int single_need_cpu = 0;
	int single_need_mem = 0;
	for (auto& it : vm_infos){
		if (it.second.dual_node == 0) {
			if (it.second.cpu > single_need_cpu){
				single_need_cpu = it.second.cpu;
			}
			if (it.second.mem > single_need_mem) {
				single_need_mem = it.second.mem;
			}
		}
	}

	// 要购买的服务器类型
	string buy_server_type = SelectPurchaseServer(mem_cpu_ratio,server_infos, single_need_cpu, single_need_mem);
	//每天的工作
	int day = 0;  // 天数
	for (auto it = requests_set.cbegin(); it != requests_set.cend(); ++it) {
		day++;
		// 所有需要购买新服务器的虚拟机，按mem/cpu的顺序排序
		// 在one_day_create_vm中的下标，vm类指针
		list<pair<int, VM>> mem_divide_cpu_sorted_vms;
		// 每一天涉及新购买服务器的删除请求
		vector<Request> res_del_requests_set;

		// 购买服务器和部署虚拟机
		// 当天购买的服务器，服务器类型->台数
		unordered_map<string, int> one_day_purchase;
		//当天迁移的虚拟机，虚拟机id
		vector<pair<int, pair<int, int> > > one_day_migrate_vm;
		// 当天部署的虚拟机，服务器id->节点
		vector<pair<int, int>> one_day_create_vm;
		int one_day_create_vm_index = 0;

		cpu_sorted_server.sort(cpu_cmp);
		//迁移虚拟机
		one_day_migrate_vm = MigrateVM(vm_count, vm_infos, vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
		cpu_sorted_server.sort(cpu_cmp);

		for (auto itv = it->cbegin(); itv != it->cend(); ++itv) {
			if (itv->op_type == ADD) {
				++vm_count;
				// 创建要排序的虚拟机表mem_divide_cpu_sorted_vms
				VM vm(itv->vm_id, itv->vm_type);
				mem_divide_cpu_sorted_vms.push_back(make_pair(one_day_create_vm_index, vm));
				// 初始化当天要创建的虚拟机表one_day_create_vm
				one_day_create_vm.push_back(make_pair(-1, -1));
				++one_day_create_vm_index;
			}
			else {
				--vm_count;
				res_del_requests_set.push_back(*itv);
			}	
		}
		// 按mem除以cpu从大到小排序
		mem_divide_cpu_sorted_vms.sort(mem_divide_cpu_cmp);
		for (auto it = mem_divide_cpu_sorted_vms.begin(); it != mem_divide_cpu_sorted_vms.end(); ){
			auto itr = mem_divide_cpu_sorted_vms.back();
			// 先从头创建一个
			//pair<int, int>create_vm_begin = CreateOneVM(it->second.vm_id_, it->second.vm_str_,
			//	vm_infos, vm_runs, server_number-1, server_resources, server_runs, server_closes);
			pair<int, int>create_vm_begin = CreateVM(it->second.vm_id_, it->second.vm_str_, vm_infos, 
				vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
			while (create_vm_begin.second == -1) {
				PurchaseServer(buy_server_type, server_number, server_infos, server_resources,
					server_closes, cpu_sorted_server, BUYCOST, TOTALCOST);
				// 在当天购买服务器字典里加入刚买的服务器
				if (one_day_purchase.find(buy_server_type) == one_day_purchase.end()) {
					one_day_purchase[buy_server_type] = 1;
				}
				else {
					one_day_purchase[buy_server_type]++;
				}
				//create_vm_begin = CreateOneVM(it->second.vm_id_, it->second.vm_str_,
				//	vm_infos, vm_runs, server_number - 1, server_resources, server_runs, server_closes);
				create_vm_begin = CreateVM(it->second.vm_id_, it->second.vm_str_, vm_infos,
					vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
			}
			one_day_create_vm[it->first] = create_vm_begin;
			if (it->second.vm_id_ == itr.second.vm_id_)
				break;
			// 再从尾创建一个
			//pair<int, int>create_vm_end = CreateOneVM(itr.second.vm_id_, itr.second.vm_str_,
			//	vm_infos, vm_runs, server_number - 1, server_resources, server_runs, server_closes);
			pair<int, int>create_vm_end = CreateVM(itr.second.vm_id_, itr.second.vm_str_, vm_infos,
				vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
			while (create_vm_end.second == -1) {
				PurchaseServer(buy_server_type, server_number, server_infos, server_resources,
					server_closes, cpu_sorted_server, BUYCOST, TOTALCOST);
				// 在当天购买服务器字典里加入刚买的服务器
				if (one_day_purchase.find(buy_server_type) == one_day_purchase.end()) {
					one_day_purchase[buy_server_type] = 1;
				}
				else {
					one_day_purchase[buy_server_type]++;
				}
				//create_vm_end = CreateOneVM(itr.second.vm_id_, itr.second.vm_str_,
				//	vm_infos, vm_runs, server_number - 1, server_resources, server_runs, server_closes);
				create_vm_end = CreateVM(itr.second.vm_id_, itr.second.vm_str_, vm_infos,
					vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
			}
			one_day_create_vm[itr.first] = create_vm_end;
			// 迭代器向后走，把创建好的虚拟机从表中删除
			it++;
			mem_divide_cpu_sorted_vms.pop_front();
			if (mem_divide_cpu_sorted_vms.size() == 0 || it->second.vm_id_ == itr.second.vm_id_)
				break;
			mem_divide_cpu_sorted_vms.pop_back();
		}
		// 最后执行删除操作
		for (auto& it : res_del_requests_set)
		{
			vm_runs[it.vm_id].Del(vm_infos, vm_runs, server_resources,
				server_runs, server_closes);
		}

		//Caculator();
		//// 输出
		//cout << "总成本为:" << TOTALCOST << endl;
		PrintPurchase(one_day_purchase);
		PrintMigration(one_day_migrate_vm);
		PrintDeploy(one_day_create_vm);
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