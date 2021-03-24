#include "../include/solution.h"

Solution::Solution() {}


Solution::Solution(unordered_map<string, ServerInfo> server_infos,
	unordered_map<string, VMInfo> vm_infos,
	int server_number, unordered_map<int, Server> server_resources,
	unordered_map<int, Server*> server_runs, unordered_map<int, Server*> server_closes,
	unordered_map<int, VM> vm_runs, string buy_server_type,
	vector<vector<Request>>::const_iterator& it) {
	this->server_number_ = server_number;
	this->server_resources_ = server_resources;
	for (auto& it : server_runs)
	{
		server_runs_[it.first] = &server_resources_[it.first];
	}
	for (auto& it : server_closes)
	{
		server_closes_[it.first] = &server_resources_[it.first];
	}
	this->vm_runs_ = vm_runs;
	this->day_cost_ = 0;  // �ɱ���ʼ��Ϊ0

	for (auto itv = it->cbegin(); itv != it->cend(); ++itv) {
		if (itv->op_type == ADD) {
			pair<int, int>create_vm = CreateVM(itv->vm_id, itv->vm_type, vm_infos, vm_runs_,
				server_resources_, server_runs_, server_closes_);
			// ����������Դ�������������ʱ
			if (create_vm.second == -1) {
				PurchaseServer(buy_server_type, server_number_, server_infos, server_resources_,
					server_closes_);
				day_cost_ += server_infos[buy_server_type].buy_cost;  // ���ӹ���ɱ�
				// �ڵ��칺��������ֵ���������ķ�����
				if (one_day_purchase_.find(buy_server_type) == one_day_purchase_.end()) {
					one_day_purchase_[buy_server_type] = 1;
				}
				else {
					one_day_purchase_[buy_server_type]++;
				}
				// �ٴγ��Դ��������
				create_vm = CreateVM(itv->vm_id, itv->vm_type, vm_infos, vm_runs_,
					server_resources_, server_runs_, server_closes_);
			}
			one_day_create_vm_.push_back(create_vm);
		}
		else {
			vm_runs_[itv->vm_id].Del(vm_infos, vm_runs_, server_resources_,
				server_runs_, server_closes_);
		}
	}
	// �ѿ����������ĵ�Ѽ���
	for (auto its = server_runs_.cbegin(); its != server_runs_.cend(); ++its) {
		day_cost_ += server_infos[its->second->type_].day_power_cost;
	}
}


Solution::Solution(const Solution& copy) {
	this->server_number_ = copy.server_number_;
	this->server_resources_ = copy.server_resources_;
	for (auto& it : copy.server_runs_)
	{
		server_runs_[it.first] = &server_resources_[it.first];
	}
	for (auto& it : copy.server_closes_)
	{
		server_closes_[it.first] = &server_resources_[it.first];
	}
	this->one_day_purchase_ = copy.one_day_purchase_;
	this->one_day_create_vm_ = copy.one_day_create_vm_;
	this->vm_runs_ = copy.vm_runs_;
	this->day_cost_ = copy.day_cost_;
}


bool _cmp(Solution a, Solution b) {
	return a.day_cost_ < b.day_cost_;
}


// �ҵ�vec�ﵱ��ɱ���͵�solution
Solution GetBestSolution(vector<Solution>& solution_vec) {
	sort(solution_vec.begin(), solution_vec.end(), _cmp);
	Solution best_solution = solution_vec[0];
	return best_solution;
}


// ��solutionʵʩ��main��
void SetSolution(int& server_number, unordered_map<int, Server>& server_resources,
	unordered_map<int, Server*>& server_runs, unordered_map<int, Server*>& server_closes,
	unordered_map<int, VM>& vm_runs, long long& TOTALCOST, Solution solution) {
	server_number = solution.server_number_;
	server_resources = solution.server_resources_;
	for (auto& it : solution.server_runs_)
	{
		server_runs[it.first] = &server_resources[it.first];
	}
	for (auto& it : solution.server_closes_)
	{
		server_closes[it.first] = &server_resources[it.first];
	}
	vm_runs = solution.vm_runs_;
	TOTALCOST += solution.day_cost_;
}
