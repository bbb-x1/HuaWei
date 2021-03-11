#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "include/server.h"
#include "include/input.h"
using namespace std;

int main(int argc, char **argv){
	// 测试数据文件
	const string kFilePath = "resource/training-1.txt";

	// 服务器信息
	unordered_map<string, ServerInfo> server_infos;
	// TODO: 虚拟机信息

	// 已购买的服务器
	int server_number = 0;
	unordered_map<int, Server> server_resources;

	// 当前开机服务器
	vector<ServerInfo> server_runs;
	// TODO: 当前运行虚拟机

	// 成本
	long long BUYCOST = 0, POWERCOST = 0, TOTALCOST = 0;

	InputServerInfos(kFilePath, server_infos);
	PrintServerInfos(server_infos);

	return 0;
}
