#pragma once
#include <unordered_map>
#include <algorithm>
#include<random>
#include "server.h"
#include "vm.h"
#include "request.h"


// 1���������
class Solution {
public:
	// �ѹ���ķ�����
	int server_number_;
	unordered_map<int, Server> server_resources_;
	// ��ǰ����������
	unordered_map<int, Server*> server_runs_;
	// ��ǰ�ػ�������
	unordered_map<int, Server*> server_closes_;
	// ���칺��ķ�����������������->̨��
	unordered_map<string, int> one_day_purchase_;
	// ���첿����������������id->�ڵ�
	vector<pair<int, int>> one_day_create_vm_;
	// ��ǰ���������
	// int Ϊvm_id
	unordered_map<int, VM> vm_runs_;

	// ����ɱ�
	int day_cost_;
	
	Solution();
	Solution(unordered_map<string, ServerInfo> server_infos,
		unordered_map<string, VMInfo> vm_infos,
		int server_number,unordered_map<int, Server> server_resources,
		unordered_map<int, Server*> server_runs, unordered_map<int, Server*> server_closes,
		unordered_map<int, VM> vm_runs, string buy_server_type,
		vector<vector<Request>>::const_iterator& it);
	Solution(const Solution& copy);
};

// �ҵ�vec�ﵱ��ɱ���͵�solution
Solution GetBestSolution(vector<Solution>& solution_vec);

// ��solutionʵʩ��main��
void SetSolution(int& server_number, unordered_map<int, Server>& server_resources,
	unordered_map<int, Server*>& server_runs, unordered_map<int, Server*>& server_closes,
	unordered_map<int, VM>& vm_runs, long long& TOTALCOST, Solution solution);
