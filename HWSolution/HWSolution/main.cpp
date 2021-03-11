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
vector<ServerInfo> server_runs;
// TODO: 当前运行虚拟机

// 成本
long long BUYCOST = 0, POWERCOST = 0, TOTALCOST = 0;


int main(int argc, char **argv){
	// 输入重定向
	FILE* stream1;
	freopen_s(&stream1, kFilePath.c_str(), "rb", stdin);

	InputServerInfos(server_infos);
	PrintServerInfos(server_infos);

	InputVMInfos(vm_infos);
	PrintVMInfos(vm_infos);

	fclose(stdin);
	return 0;
}
