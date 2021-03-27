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

    
    if (sv_node == -1) {
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
    if (sv_node_ == -1) {
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
            vm_runs[vm_id].Add(sv_id, -1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 0代表双节点虚拟机
            return std::make_pair(vm_runs[vm_id].sv_id_, 0);
        }
    }
    else {
        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
            vm_runs[vm_id].Add(sv_id, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 1代表存放在A节点
            return std::make_pair(vm_runs[vm_id].sv_id_, 1);
        }
        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
            vm_runs[vm_id].Add(sv_id, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 2代表存放在B节点
            return std::make_pair(vm_runs[vm_id].sv_id_, 2);
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

    VM vm(vm_id, vm_str);
    vm_runs[vm_id] = vm;

    int judge = 0;
    for (auto i = cpu_sorted_server.begin(); i != cpu_sorted_server.end(); ++i) {
        Node a = (*i)->get_node('a');
        Node b = (*i)->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                vm_runs[vm_id].Add((*i)->ID_, -1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                judge = 1;
                // 0代表双节点虚拟机
                return std::make_pair(vm_runs[vm_id].sv_id_, 0);
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].Add((*i)->ID_, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                judge = 1;
                // 1代表存放在A节点
                return std::make_pair(vm_runs[vm_id].sv_id_, 1);
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].Add((*i)->ID_, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                judge = 1;
                // 2代表存放在B节点
                return std::make_pair(vm_runs[vm_id].sv_id_, 2);
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
    int b_temp = 0;
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
        if (iter_temp != iter_empty && path % 10 == 0) {
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
    for (auto iter = iter_output; iter != iter_empty; ++iter) {
        for (auto iter_int = (*iter)->own_vm.begin(); iter_int != (*iter)->own_vm.end(); ++iter_int) {
            vm_output.push_back(make_pair(*iter_int, vm_infos[vm_runs[*iter_int].vm_str_].cpu - vm_infos[vm_runs[*iter_int].vm_str_].mem));
        }
    }
    vm_output.sort([](pair<int, int> a, pair<int, int> b) {
        return a.second < b.second;
        });


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
                    judge_node = -1;
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
            b_temp = result.size();
        }
        else {
            
            cout << "************" << endl;
            break;
        }

    }
    //从利用率低的服务器开始迁移
    for (auto iter_r_s = post_server.rbegin(); mig_nums < max_nums && iter_r_s != post_server.rend(); ++iter_r_s) {
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
                if ((*iter_s)->ID_ == s_id && s_node == -1) {
                    break;
                }
                Node a = (*iter_s)->get_node('a');
                Node b = (*iter_s)->get_node('b');
                if (vm_infos[vm_str].dual_node == 1) {
                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                        fit_server_id = (*iter_s)->ID_;
                        fit_server_node = -1;

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

    cout << result.size() - b_temp << endl;
    return result;
}