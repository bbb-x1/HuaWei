#include "../include/vm.h"

VM::VM() {
    vm_id_ = -1;
    vm_str_ = " ";
    sv_id_ = -1;
    sv_node_ = -1;
}

VM::VM(int vm_id, string vm_str) {
    vm_id_ = vm_id;
    vm_str_ = vm_str;

}


void VM::Add(int sv_id, int sv_node,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources, 
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes){

    sv_id_ = sv_id;
    sv_node_ = sv_node;

    
    if (sv_node == 2) {
        server_resources[sv_id].IncreaseUse(vm_id_, vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2,'a',
            server_runs, server_closes);
        server_resources[sv_id].IncreaseUse(vm_id_, vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2, 'b',
            server_runs, server_closes);
        server_resources[sv_id].own_vm.push_back(vm_id_);
    }
    else if (sv_node == 0) {
        server_resources[sv_id].IncreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'a',
            server_runs, server_closes);
        server_resources[sv_id].own_vm.push_back(vm_id_);
    }
    else if (sv_node == 1) {
        server_resources[sv_id].IncreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'b',
            server_runs, server_closes);
        server_resources[sv_id].own_vm.push_back(vm_id_);
    }
    else {
        cout << "Error:sv_node" << endl;
    }
}


void VM::Del(
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes) {
    if (sv_node_ == 2) {
        server_resources[sv_id_].DecreaseUse(vm_id_,vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2, 'a',
            server_runs, server_closes);
        server_resources[sv_id_].DecreaseUse(vm_id_,vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2, 'b',
            server_runs, server_closes);
        
    }
    else if(sv_node_ == 0){
        server_resources[sv_id_].DecreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'a',
            server_runs, server_closes);
        
    }
    else{//sv_node_==1
        server_resources[sv_id_].DecreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'b',
            server_runs, server_closes);
        
    }
    server_resources[sv_id_].own_vm.remove(vm_id_);
    vm_runs.erase(vm_id_);
}


// 输入服务器id，创建1台虚拟机
pair<int, int> CreateOneVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    int sv_id,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes) {
    if(server_resources.size() == 0)
        return std::make_pair(-1, -1);
    VM vm(vm_id, vm_str);
    vm_runs[vm_id] = vm;
    Node a = server_resources[sv_id].get_node('a');
    Node b = server_resources[sv_id].get_node('b');
    if (vm_infos[vm_str].dual_node == 1) {
        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
            vm_runs[vm_id].Add(sv_id, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 2代表双节点虚拟机
            return std::make_pair(vm_runs[vm_id].sv_id_, 2);
        }
    }
    else {
        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
            vm_runs[vm_id].Add(sv_id, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 0代表存放在A节点
            return std::make_pair(vm_runs[vm_id].sv_id_, 0);
        }
        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
            vm_runs[vm_id].Add(sv_id, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 1代表存放在B节点
            return std::make_pair(vm_runs[vm_id].sv_id_, 1);
        }
    }
    return std::make_pair(-1, -1);
}


pair<int, int> CreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server) {



    int judge = 0;
    for (auto i = cpu_sorted_server.begin(); i != cpu_sorted_server.end(); ++i) {
        Node a = (*i)->get_node('a');
        Node b = (*i)->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                VM vm(vm_id, vm_str);
                vm_runs[vm_id] = vm;
                vm_runs[vm_id].Add((*i)->ID_, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                judge = 1;
                // 2代表双节点虚拟机
                return std::make_pair(vm_runs[vm_id].sv_id_, 2);
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                VM vm(vm_id, vm_str);
                vm_runs[vm_id] = vm;
                vm_runs[vm_id].Add((*i)->ID_, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                judge = 1;
                // 0代表存放在A节点
                return std::make_pair(vm_runs[vm_id].sv_id_, 0);
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                VM vm(vm_id, vm_str);
                vm_runs[vm_id] = vm;
                vm_runs[vm_id].Add((*i)->ID_, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                judge = 1;
                // 1代表存放在B节点
                return std::make_pair(vm_runs[vm_id].sv_id_, 1);
            }
        }
    }
    return std::make_pair(-1, -1);
}

//pair<虚拟机id，pair<服务器id，服务器节点> >
vector<pair<int, pair<int, int> > > MigrateVM(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server) {

    vector<pair<int, pair<int, int> > > result;
    int max_nums = int(floor(vm_count * 0.005));
    int mig_nums = 0;
    //正向第一个不满的服务器
    auto iter_not_full = cpu_sorted_server.begin();
    while (iter_not_full != cpu_sorted_server.end()) {

        if ((*iter_not_full)->get_node('a').cpu_res != 0 || (*iter_not_full)->get_node('b').cpu_res != 0) {
            break;
        }
        ++iter_not_full;

    }
    //正向第一个空的服务器
    auto iter_empty = cpu_sorted_server.begin();
    while (iter_empty != cpu_sorted_server.end()) {

        if ((*iter_empty)->get_node('a').cpu_used == 0 && (*iter_empty)->get_node('b').cpu_used == 0) {
            break;
        }
        ++iter_empty;

    }

    auto iter_output = iter_not_full;
    auto iter_temp = iter_not_full;

    list<Server*> pre_server;
    list<Server*> post_server;
    int path = 0;
    while (iter_temp != iter_empty) {
        if ((*iter_output)->get_node('a').mem_res != 0 || (*iter_output)->get_node('b').mem_res != 0) {
            pre_server.push_back(*iter_output);
        }
        ++iter_output;
        ++iter_temp;
        if (iter_temp != iter_empty && path % 2== 0) {
            ++iter_temp;
        }
        ++path;
    }
    for (auto iter = iter_output; iter != iter_empty; ++iter) {
        if ((*iter)->get_node('a').mem_res != 0 || (*iter)->get_node('b').mem_res != 0) {
            post_server.push_back(*iter);
        }
    }

    //虚拟机迁移列表(cpu从小到大)
    list<pair<int, int> > vm_output;
    //初始化vm_output
    iter_temp = iter_output;
    path = 0;
    while (iter_temp != iter_empty) {
        ++iter_output;
        ++iter_temp;
        if (iter_temp != iter_empty && path % 10== 0) {
            ++iter_temp;
        }
        ++path;
    }
    for (auto iter = iter_output; iter != iter_empty; ++iter) {
        for (auto iter_int = (*iter)->own_vm.begin(); iter_int != (*iter)->own_vm.end(); ++iter_int) {
            vm_output.push_back(make_pair(*iter_int, vm_infos[vm_runs[*iter_int].vm_str_].cpu - vm_infos[vm_runs[*iter_int].vm_str_].mem));
        }
    }
    vm_output.sort([](pair<int, int> a, pair<int, int> b) {
        return a.second < b.second;
        });

    int judge_pre = 0;

    for (auto output_iter = vm_output.begin(); mig_nums < max_nums && output_iter != vm_output.end(); output_iter = vm_output.erase(output_iter)) {
        //虚拟机信息
        int vm_id = (*output_iter).first;
        string vm_str = vm_runs[vm_id].vm_str_;
        int dual_node = vm_infos[vm_str].dual_node;

        //若找到了合适的服务器，judge_find =1；
        int judge_find = 0;
        //记录找到服务器的id,和节点
        int judge_id = -1;
        int judge_node = -2;
        //从第一个不满的开始找到合适的服务器
        auto iter_s = pre_server.begin();
        while (iter_s != pre_server.end()) {
            Node a = (*iter_s)->get_node('a');
            Node b = (*iter_s)->get_node('b');
            if (dual_node == 1) {
                if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                    && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                    judge_id = (*iter_s)->ID_;
                    judge_node = 2;
                    judge_find = 1;
                    break;
                }
            }
            else {
                if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                    judge_id = (*iter_s)->ID_;
                    judge_node = 0;
                    judge_find = 1;
                    break;

                }
                if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                    judge_id = (*iter_s)->ID_;
                    judge_node = 1;
                    judge_find = 1;
                    break;
                }
            }
            ++iter_s;
        }
        if (judge_find == 1) {
            //将虚拟机从服务器中去除
            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
            VM vm(vm_id, vm_str);
            vm_runs[vm_id] = vm;
            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            ++mig_nums;
            result.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
            if ((*iter_s)->get_node('a').cpu_res == 0 && (*iter_s)->get_node('b').cpu_res == 0) {
                pre_server.erase(iter_s);
            }
            
        }
        else {
            judge_pre = 1;
            break;
        }

    }
    //从post_server的服务器开始迁移
    for (auto iter_r_s = post_server.rbegin(); mig_nums < max_nums && iter_r_s != post_server.rend() && (*iter_r_s) != (*iter_output); ++iter_r_s) {
        list<int> iter_int;
        for (auto iter = (*iter_r_s)->own_vm.begin(); iter != (*iter_r_s)->own_vm.end(); ++iter) {
            iter_int.push_back(*iter);
        }
        auto iter_v = iter_int.begin();
        while (iter_v != iter_int.end() && mig_nums < max_nums) {
            //--temp_num;
            int vm_id = *iter_v;
            string vm_str = vm_runs[*iter_v].vm_str_;
            int s_id = vm_runs[*iter_v].sv_id_;
            int s_node = vm_runs[*iter_v].sv_node_;
            //找到合适的服务器则为1
            int judge = 0;
            int fit_server_id;
            int fit_server_node;
            //找到合适的服务器插入
            for (auto iter_s = post_server.begin(); iter_s != post_server.end(); ++iter_s) {
                if ((*iter_s)->ID_ == s_id && s_node == 2) {
                    break;
                }
                Node a = (*iter_s)->get_node('a');
                Node b = (*iter_s)->get_node('b');
                if (vm_infos[vm_str].dual_node == 1) {
                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                        fit_server_id = (*iter_s)->ID_;
                        fit_server_node = 2;

                        if (s_id != (*iter_s)->ID_) {
                            judge = 1;
                        }
                        break;
                    }
                }
                else {
                    if ((*iter_s)->ID_ == s_id && s_node == 0) {
                        break;
                    }
                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                        fit_server_id = (*iter_s)->ID_;
                        fit_server_node = 0;

                        if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 0)) {
                            judge = 1;
                        }
                        break;

                    }
                    if ((*iter_s)->ID_ == s_id && s_node == 1) {
                        break;
                    }
                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                        fit_server_id = (*iter_s)->ID_;
                        fit_server_node = 1;
                        if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 1)) {
                            judge = 1;
                        }
                        break;
                    }
                }
            }
            if (judge == 1) {
                vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
                VM vm(vm_id, vm_str);
                vm_runs[vm_id] = vm;
                vm_runs[vm_id].Add(fit_server_id, fit_server_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                result.push_back(make_pair(vm_id, make_pair(fit_server_id, fit_server_node)));
                ++mig_nums;
            }
            ++iter_v;
        }
    }

    if (judge_pre == 0) {
        //从pre_server的服务器开始迁移
        for (auto iter_r_s = pre_server.rbegin(); mig_nums < max_nums && iter_r_s != pre_server.rend(); ++iter_r_s) {
            
            list<int> iter_int;
            for (auto iter = (*iter_r_s)->own_vm.begin(); iter != (*iter_r_s)->own_vm.end(); ++iter) {
                iter_int.push_back(*iter);
            }
            auto iter_v = iter_int.begin();
            while (iter_v != iter_int.end() && mig_nums < max_nums) {
                //--temp_num;
                int vm_id = *iter_v;
                string vm_str = vm_runs[*iter_v].vm_str_;
                int s_id = vm_runs[*iter_v].sv_id_;
                int s_node = vm_runs[*iter_v].sv_node_;
                //找到合适的服务器则为1
                int judge = 0;
                int fit_server_id;
                int fit_server_node;
                //找到合适的服务器插入
                for (auto iter_s = post_server.begin(); iter_s != post_server.end(); ++iter_s) {
                    if ((*iter_s)->ID_ == s_id && s_node == 2) {
                        break;
                    }
                    Node a = (*iter_s)->get_node('a');
                    Node b = (*iter_s)->get_node('b');
                    if (vm_infos[vm_str].dual_node == 1) {
                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                            fit_server_id = (*iter_s)->ID_;
                            fit_server_node = 2;
                            if (s_id != (*iter_s)->ID_) {
                                judge = 1;
                            }
                            break;
                        }
                    }
                    else {
                        if ((*iter_s)->ID_ == s_id && s_node == 0) {
                            break;
                        }
                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                            fit_server_id = (*iter_s)->ID_;
                            fit_server_node = 0;
                            if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 0)) {
                                judge = 1;
                            }
                            break;
                        }
                        if ((*iter_s)->ID_ == s_id && s_node == 1) {
                            break;
                        }
                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                            fit_server_id = (*iter_s)->ID_;
                            fit_server_node = 1;
                            if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 1)) {
                                judge = 1;
                            }
                            break;
                        }
                    }
                }
                if (judge == 1) {
                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    VM vm(vm_id, vm_str);
                    vm_runs[vm_id] = vm;
                    vm_runs[vm_id].Add(fit_server_id, fit_server_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    result.push_back(make_pair(vm_id, make_pair(fit_server_id, fit_server_node)));
                    ++mig_nums;
                }
                ++iter_v;
            }
        }
    }
    return result;
}

void DeployVm(int &vm_count,int& server_number,
    long long& BUYCOST, long long& TOTALCOST,
    vector<Request>& day_requests,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server) 
{
    pair<int, int> judge_purchase;              // 判断是否需要买服务器
    unordered_map<int, Request*> extra_add;     // 放在新服务器的add请求(顺序下标-请求)
    unordered_map<int, Request*> extra_del_add; // 放在新服务器且删除的add请求
    unordered_map<int, int> judge_del;          // 用于判断del里有没有今天add(vmid,index)
    int add_index = 0;                          // one_day_create_vm数组的下标
    //已有服务器上的部署
    for (auto req = day_requests.begin(); req != day_requests.end(); ++req) {
        //
        if (req->op_type == ADD) {
            ++vm_count;
            judge_purchase = CreateVM(req->vm_id,req->vm_type,
                vm_infos,vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
            if (judge_purchase.second == -1) {
                extra_add[add_index] = &(*req);
                judge_del[req->vm_id] = add_index;
            }
            one_day_create_vm.push_back(judge_purchase);
            ++add_index;
        }
        else {
            --vm_count;
            auto add_del = judge_del.find(req->vm_id);
            if (add_del != judge_del.end()) {//放在新服务器且删除的add请求
                extra_del_add[ add_del->second ] = extra_add[ add_del->second ];
                extra_add.erase( add_del->second );
            }
            else {
                vm_runs[req->vm_id].Del(
                    vm_infos, vm_runs, server_resources, server_runs, server_closes);
            }
        }
    }
    //
    int mem_max = 0;
    int cpu_max = 0;                // 虚拟机最大cpu和mem
    double mem_cpu_ratio = 0;               // extra_need中mem/cpu
    DayCaculate(mem_max, cpu_max, mem_cpu_ratio, extra_add, 
        vm_infos);
    string buy_server_type = " ";           // 要购买的服务器类型
    SelectPurchaseServer(buy_server_type,mem_max, cpu_max, mem_cpu_ratio,
        server_infos);
    list<Server*> new_server;               //新服务器列表
    //
    //新服务器上的add
    for (auto extra = extra_add.begin(); extra != extra_add.end(); ) {
        //
        judge_purchase = CreateVM(extra->second->vm_id, extra->second->vm_type,
            vm_infos,vm_runs, server_resources, server_runs, server_closes, new_server);
        //
        if (judge_purchase.second == -1) {  //买服务器
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST,server_infos, server_resources,server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //部署成功
            one_day_create_vm[extra->first] = judge_purchase;
            ++extra;
        }
    }
    //新服务器上且删除的add
    for (auto extra_da = extra_del_add.begin(); extra_da != extra_del_add.end(); ++extra_da) {
        judge_purchase = CreateVM(extra_da->second->vm_id, extra_da->second->vm_type,
            vm_infos, vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
        if (judge_purchase.second == -1) {  //买服务器
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //部署成功
            one_day_create_vm[extra_da->first] = judge_purchase;
            ++extra_da;
            vm_runs[extra_da->second->vm_id].Del(
                vm_infos, vm_runs, server_resources, server_runs, server_closes);

        }
    }
}

//计算mem_max,cpu_max,mem_cpu_ratio
void DayCaculate(int& mem_max, int& cpu_max, double& mem_cpu_ratio, 
    unordered_map<int, Request*>& extra_need,
    unordered_map<string, VMInfo>& vm_infos) {
    double mem_total = 0;
    double cpu_total = 0;
    for (auto extra = extra_need.begin(); extra != extra_need.end(); ++extra) {

        int mem_temp = vm_infos[extra->second->vm_type].mem;
        int cpu_temp = vm_infos[extra->second->vm_type].cpu;

        mem_total += double(mem_temp);
        cpu_total += double(cpu_temp);

        if (mem_temp > mem_max) {
            mem_max = mem_temp;
        }
        if (cpu_temp > cpu_max) {
            cpu_max = cpu_temp;
        }
    }
    mem_cpu_ratio = mem_total / cpu_total;
}