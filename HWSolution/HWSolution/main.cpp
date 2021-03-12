#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "include/server.h"
#include "include/input.h"
#include "include/virtualMachine.h"
using namespace std;


// 测试数据文件
const string kFilePath = "resource/training-1.txt";

// 服务器信息
unordered_map<string, ServerInfo> server_infos;
// 虚拟机信息
unordered_map<string, VirtualMachineInfo> vm_infos;


// 已购买的服务器
int server_number = 0;
unordered_map<int, Server> server_resources;

// 当前开机服务器
vector<Server> server_runs;
// TODO: 当前运行虚拟机

// 全部请求序列
unordered_map<int, vector<Request>> requests_set;

// 成本
long long BUYCOST = 0, POWERCOST = 0, TOTALCOST = 0;


int main(int argc, char **argv){

	//初始化数据
	InitialzieData(server_infos, vm_infos, requests_set);

	//统计数据
	StatisticInfo(vm_infos, requests_set);
	system("pause");
	return 0;
}

