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
        server_resources[sv_id].cown_vm.push_back(vm_id_);
    }
    else if (sv_node == 0) {
        server_resources[sv_id].IncreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'a',
            server_runs, server_closes);
        server_resources[sv_id].own_vm.push_back(vm_id_);
        server_resources[sv_id].aown_vm.push_back(vm_id_);
    }
    else if (sv_node == 1) {
        server_resources[sv_id].IncreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'b',
            server_runs, server_closes);
        server_resources[sv_id].own_vm.push_back(vm_id_);
        server_resources[sv_id].bown_vm.push_back(vm_id_);
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
        server_resources[sv_id_].cown_vm.remove(vm_id_);
        
    }
    else if(sv_node_ == 0){
        server_resources[sv_id_].DecreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'a',
            server_runs, server_closes);
        server_resources[sv_id_].aown_vm.remove(vm_id_);
    }
    else{//sv_node_==1
        server_resources[sv_id_].DecreaseUse(vm_id_, vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'b',
            server_runs, server_closes);
        server_resources[sv_id_].bown_vm.remove(vm_id_);
    }
    server_resources[sv_id_].own_vm.remove(vm_id_);
    vm_runs.erase(vm_id_);
}


// ���������id������1̨�����
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
            // 2����˫�ڵ������
            return std::make_pair(vm_runs[vm_id].sv_id_, 2);
        }
    }
    else {
        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
            vm_runs[vm_id].Add(sv_id, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 0��������A�ڵ�
            return std::make_pair(vm_runs[vm_id].sv_id_, 0);
        }
        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
            vm_runs[vm_id].Add(sv_id, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            // 1��������B�ڵ�
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
    for (auto i = cpu_sorted_server.begin(); i != cpu_sorted_server.end(); ++i) {
        Node a = (*i)->get_node('a');
        Node b = (*i)->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 2����˫�ڵ������
                return std::make_pair(vm_runs[vm_id].sv_id_, 2);
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 0��������A�ڵ�
                return std::make_pair(vm_runs[vm_id].sv_id_, 0);
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 1��������B�ڵ�
                return std::make_pair(vm_runs[vm_id].sv_id_, 1);
            }
        }
    }
    return std::make_pair(-1, -1);
}

// ���أ��������з��ز������̨��������ָ��
pair<int, int> CreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server,
    Server*& deployed_server) {
    for (auto i = cpu_sorted_server.begin(); i != cpu_sorted_server.end(); ++i) {
        Node a = (*i)->get_node('a');
        Node b = (*i)->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                deployed_server = *i;
                // 2����˫�ڵ������
                return std::make_pair(vm_runs[vm_id].sv_id_, 2);
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                deployed_server = *i;
                // 0��������A�ڵ�
                return std::make_pair(vm_runs[vm_id].sv_id_, 0);
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                deployed_server = *i;
                // 1��������B�ڵ�
                return std::make_pair(vm_runs[vm_id].sv_id_, 1);
            }
        }
    }
    return std::make_pair(-1, -1);
}

//pair<�����id��pair<������id���������ڵ�> >
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
    //�����һ�������ķ�����
    auto iter_not_full = cpu_sorted_server.begin();
    while (iter_not_full != cpu_sorted_server.end()) {

        if ((*iter_not_full)->get_node('a').cpu_res != 0 || (*iter_not_full)->get_node('b').cpu_res != 0) {
            break;
        }
        ++iter_not_full;

    }
    //�����һ���յķ�����
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

    //�����Ǩ���б�(cpu��С����)
    list<pair<int, int> > vm_output;
    //��ʼ��vm_output
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
        //�������Ϣ
        int vm_id = (*output_iter).first;
        string vm_str = vm_runs[vm_id].vm_str_;
        int dual_node = vm_infos[vm_str].dual_node;

        //���ҵ��˺��ʵķ�������judge_find =1��
        int judge_find = 0;
        //��¼�ҵ���������id,�ͽڵ�
        int judge_id = -1;
        int judge_node = -2;
        //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
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
            //��������ӷ�������ȥ��
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
    //��post_server�ķ�������ʼǨ��
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
            //�ҵ����ʵķ�������Ϊ1
            int judge = 0;
            int fit_server_id;
            int fit_server_node;
            //�ҵ����ʵķ���������
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
        //��pre_server�ķ�������ʼǨ��
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
                //�ҵ����ʵķ�������Ϊ1
                int judge = 0;
                int fit_server_id;
                int fit_server_node;
                //�ҵ����ʵķ���������
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
    pair<int, int> judge_purchase;              // �ж��Ƿ���Ҫ�������
    unordered_map<int, Request*> extra_add;     // �����·�������add����(˳���±�-����)
    unordered_map<int, Request*> extra_del_add; // �����·�������ɾ����add����
    unordered_map<int, int> judge_del;          // �����ж�del����û�н���add(vmid,index)
    int add_index = 0;                          // one_day_create_vm������±�
    //���з������ϵĲ���
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
            if (add_del != judge_del.end()) {//�����·�������ɾ����add����
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
    int cpu_max = 0;                // ��������cpu��mem
    double mem_cpu_ratio = 0;               // extra_need��mem/cpu
    DayCaculate(mem_max, cpu_max, mem_cpu_ratio, extra_add, 
        vm_infos);
    string buy_server_type = " ";           // Ҫ����ķ���������
    SelectPurchaseServer(buy_server_type,mem_max, cpu_max, mem_cpu_ratio,
        server_infos);
    list<Server*> new_server;               //�·������б�
    //
    //�·������ϵ�add
    for (auto extra = extra_add.begin(); extra != extra_add.end(); ) {
        //
        judge_purchase = CreateVM(extra->second->vm_id, extra->second->vm_type,
            vm_infos,vm_runs, server_resources, server_runs, server_closes, new_server);
        //
        if (judge_purchase.second == -1) {  //�������
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST,server_infos, server_resources,server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //����ɹ�
            one_day_create_vm[extra->first] = judge_purchase;
            ++extra;
        }
    }
    //�·���������ɾ����add
    for (auto extra_da = extra_del_add.begin(); extra_da != extra_del_add.end(); ++extra_da) {
        judge_purchase = CreateVM(extra_da->second->vm_id, extra_da->second->vm_type,
            vm_infos, vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
        if (judge_purchase.second == -1) {  //�������
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //����ɹ�
            one_day_create_vm[extra_da->first] = judge_purchase;
            ++extra_da;
            vm_runs[extra_da->second->vm_id].Del(
                vm_infos, vm_runs, server_resources, server_runs, server_closes);

        }
    }
}

//����mem_max,cpu_max,mem_cpu_ratio
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
    mem_cpu_ratio = cpu_total / mem_total;
}


//void DeployVmAdd(int& vm_count, int& server_number,
//    long long& BUYCOST, long long& TOTALCOST, int& actual_vm_count,
//    vector<Request>& day_requests,
//    list<pair<int, Request*> >& list_del,
//    unordered_map<string, int>& one_day_purchase,
//    vector<pair<int, int>>& one_day_create_vm,
//    vector<pair<int, pair<int, int> > >& one_day_migrate_vm,
//    unordered_map<string, VMInfo>& vm_infos,
//    unordered_map<int, VM>& vm_runs,
//    unordered_map<string, ServerInfo>& server_infos,
//    unordered_map<int, Server>& server_resources,
//    unordered_map<int, Server*>& server_runs,
//    unordered_map<int, Server*>& server_closes,
//    list<Server*>& cpu_sorted_server,
//    int& max_cpu,int& max_mem, int& remain_day,pair<int, int>record,
//    string& buy_server_type)
//{
//    vector<Server*> full_server(server_number);     //��������
//    //
//    //��Ǩ��*********************************************************************************************
//    //��һ��,������������
//    vector<Server*> cpu_server(server_number);      //cpu>mem�ķ�����
//    vector<Server*> mem_server(server_number);      //mem<cpu�ķ�����
//    vector<Server*> out_server(server_number);      //��Ǩ��������ķ�����
//    int wait_out = 0;                               //�ȴ�Ǩ�Ƶ����������
//    int out_edge = server_number - 1;
//    while (out_edge >= 0) {
//        wait_out += server_resources[out_edge].own_vm.size();
//        if (wait_out > actual_vm_count) {
//            break;
//        }
//        out_server[out_edge] = &server_resources[out_edge];
//        --out_edge;
//    }
//    for (int i = 0; i <= out_edge; ++i) {
//        if ((server_resources[i].get_node('a').cpu_res + server_resources[i].get_node('b').cpu_res == 0) >
//            (server_resources[i].get_node('a').mem_res + server_resources[i].get_node('b').mem_res == 0)) {
//            cpu_server[i] = &server_resources[i];
//        }
//        else {
//            mem_server[i] = &server_resources[i];
//        }
//    }
//    //�ڶ�������out_server�е������Ǩ�Ƶ�cpu_server��mem_server��
//    for (int out_index = out_edge + 1; out_index < server_number; ++out_index) {
//        list<int> temp_id;
//        for (auto iter_int = out_server[out_index]->own_vm.begin(); iter_int != out_server[out_index]->own_vm.end(); ++iter_int) {
//            temp_id.push_back(*iter_int);
//        }
//        for (auto iter_int = temp_id.begin(); iter_int != temp_id.end(); ++iter_int) {
//            //�������Ϣ
//            int vm_id = *iter_int;
//            string vm_str = vm_runs[vm_id].vm_str_;
//            int dual_node = vm_infos[vm_str].dual_node;
//            //���ҵ��˺��ʵķ�������judge_find =1��
//            int judge_find = 0;
//            //��¼�ҵ���������id,�ͽڵ�
//            int judge_id = -1;
//            int judge_node = -2;
//            if (vm_infos[vm_str].cpu > vm_infos[vm_str].mem) {
//                //��cpu_server�ҵ����ʵķ�����
//                for (int cpu_index = 0; cpu_index <= out_edge; ++cpu_index) {
//                    if (cpu_server[cpu_index] == nullptr) {
//                        continue;
//                    }
//                    Node a = cpu_server[cpu_index]->get_node('a');
//                    Node b = cpu_server[cpu_index]->get_node('b');
//                    if (dual_node == 1) {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                            judge_id = cpu_index;
//                            judge_node = 2;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    else {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = cpu_index;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = cpu_index;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                }
//                if (judge_find == 1) {
//                    //��������ӷ�������ȥ��
//                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    vm_runs[vm_id].vm_id_ = vm_id;
//                    vm_runs[vm_id].vm_str_ = vm_str;
//                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    --actual_vm_count;
//                    one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
//                    if ((cpu_server[judge_id]->get_node('a').cpu_res + cpu_server[judge_id]->get_node('b').cpu_res) <
//                        (cpu_server[judge_id]->get_node('a').mem_res + cpu_server[judge_id]->get_node('b').mem_res)) {
//                        mem_server[judge_id] = cpu_server[judge_id];
//                        cpu_server[judge_id] = nullptr;
//                    }
//                }
//                else {
//                    //��mem_server�ҵ����ʵķ�����
//                    for (int mem_index = 0; mem_index <= out_edge; ++mem_index) {
//                        if (mem_server[mem_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = mem_server[mem_index]->get_node('a');
//                        Node b = mem_server[mem_index]->get_node('b');
//                        if (dual_node == 1) {
//                            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                                judge_id = mem_index;
//                                judge_node = 2;
//                                judge_find = 1;
//                                break;
//                            }
//                        }
//                        else {
//                            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                                judge_id = mem_index;
//                                judge_node = 0;
//                                judge_find = 1;
//                                break;
//
//                            }
//                            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                                judge_id = mem_index;
//                                judge_node = 1;
//                                judge_find = 1;
//                                break;
//                            }
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
//                        if ((mem_server[judge_id]->get_node('a').cpu_res + mem_server[judge_id]->get_node('b').cpu_res) >
//                            (mem_server[judge_id]->get_node('a').mem_res + mem_server[judge_id]->get_node('b').mem_res)) {
//                            cpu_server[judge_id] = mem_server[judge_id];
//                            mem_server[judge_id] = nullptr;
//                        }
//                    }
//                    else {
//                        //��out_server�ҵ����ʵķ�����
//                        for (int out_index_front = out_edge + 1; out_index_front < out_index; ++out_index_front) {
//                            Node a = out_server[out_index_front]->get_node('a');
//                            Node b = out_server[out_index_front]->get_node('b');
//                            if (dual_node == 1) {
//                                if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                                    && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                                    judge_id = out_index_front;
//                                    judge_node = 2;
//                                    judge_find = 1;
//                                    break;
//                                }
//                            }
//                            else {
//                                if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                                    judge_id = out_index_front;
//                                    judge_node = 0;
//                                    judge_find = 1;
//                                    break;
//
//                                }
//                                if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                                    judge_id = out_index_front;
//                                    judge_node = 1;
//                                    judge_find = 1;
//                                    break;
//                                }
//                            }
//                        }
//                        if (judge_find == 1) {
//                            //��������ӷ�������ȥ��
//                            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                            vm_runs[vm_id].vm_id_ = vm_id;
//                            vm_runs[vm_id].vm_str_ = vm_str;
//                            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                            --actual_vm_count;
//                            one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
//                        }
//                    }
//                }
//            }
//            else {
//                //��mem_server�ҵ����ʵķ�����
//                for (int mem_index = 0; mem_index <= out_edge; ++mem_index) {
//                    if (mem_server[mem_index] == nullptr) {
//                        continue;
//                    }
//                    Node a = mem_server[mem_index]->get_node('a');
//                    Node b = mem_server[mem_index]->get_node('b');
//                    if (dual_node == 1) {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                            judge_id = mem_index;
//                            judge_node = 2;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    else {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = mem_index;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = mem_index;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                }
//                if (judge_find == 1) {
//                    //��������ӷ�������ȥ��
//                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    vm_runs[vm_id].vm_id_ = vm_id;
//                    vm_runs[vm_id].vm_str_ = vm_str;
//                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    --actual_vm_count;
//                    one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                    if ((mem_server[judge_id]->get_node('a').cpu_res + mem_server[judge_id]->get_node('b').cpu_res) >
//                        (mem_server[judge_id]->get_node('a').mem_res + mem_server[judge_id]->get_node('b').mem_res)) {
//                        cpu_server[judge_id] = mem_server[judge_id];
//                        mem_server[judge_id] = nullptr;
//                    }
//                }
//                else {
//                    //��cpu_server�ҵ����ʵķ�����
//                    for (int cpu_index = 0; cpu_index <= out_edge; ++cpu_index) {
//                        if (cpu_server[cpu_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = cpu_server[cpu_index]->get_node('a');
//                        Node b = cpu_server[cpu_index]->get_node('b');
//                        if (dual_node == 1) {
//                            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                                judge_id = cpu_index;
//                                judge_node = 2;
//                                judge_find = 1;
//                                break;
//                            }
//                        }
//                        else {
//                            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                                judge_id = cpu_index;
//                                judge_node = 0;
//                                judge_find = 1;
//                                break;
//
//                            }
//                            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                                judge_id = cpu_index;
//                                judge_node = 1;
//                                judge_find = 1;
//                                break;
//                            }
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                        if ((cpu_server[judge_id]->get_node('a').cpu_res + cpu_server[judge_id]->get_node('b').cpu_res) <
//                            (cpu_server[judge_id]->get_node('a').mem_res + cpu_server[judge_id]->get_node('b').mem_res)) {
//                            mem_server[judge_id] = cpu_server[judge_id];
//                            cpu_server[judge_id] = nullptr;
//                        }
//                    }
//                    else {
//                        //��out_server�ҵ����ʵķ�����
//                        for (int out_index_front = out_edge + 1; out_index_front < out_index; ++out_index_front) {
//                            Node a = out_server[out_index_front]->get_node('a');
//                            Node b = out_server[out_index_front]->get_node('b');
//                            if (dual_node == 1) {
//                                if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                                    && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                                    judge_id = out_index_front;
//                                    judge_node = 2;
//                                    judge_find = 1;
//                                    break;
//                                }
//                            }
//                            else {
//                                if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                                    judge_id = out_index_front;
//                                    judge_node = 0;
//                                    judge_find = 1;
//                                    break;
//
//                                }
//                                if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                                    judge_id = out_index_front;
//                                    judge_node = 1;
//                                    judge_find = 1;
//                                    break;
//                                }
//                            }
//                        }
//                        if (judge_find == 1) {
//                            //��������ӷ�������ȥ��
//                            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                            vm_runs[vm_id].vm_id_ = vm_id;
//                            vm_runs[vm_id].vm_str_ = vm_str;
//                            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                            --actual_vm_count;
//                            one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
//                        }
//                    }
//                }
//            }
//        }
//    }
//    //����������out_server���䵽empty_server
//    vector<Server*> empty_server(server_number);    //�շ�����
//    int empty_edge = server_number - 1;
//    while (empty_edge >= 0) {
//        if (!(server_resources[empty_edge].get_node('a').cpu_used == 0 && server_resources[empty_edge].get_node('b').cpu_used == 0
//            && server_resources[empty_edge].get_node('a').mem_used == 0 && server_resources[empty_edge].get_node('b').mem_used == 0)) {
//            break;
//        }
//        empty_server[empty_edge] = &server_resources[empty_edge];
//        --empty_edge;
//    }
//    //������Ҫ�����̨������***************************************************************************
//    //��һ�������������Ҫ��cpu���ڴ�
//    for (int i = empty_edge + 1; i < server_number; ++i) {
//        record.first -= server_infos[empty_server[i]->type_].cpu;
//        record.second -= server_infos[empty_server[i]->type_].mem;
//    }
//    //�ڶ�����ȷ���������������
//    SelectPurchaseServerNew(buy_server_type, max_cpu, max_mem, remain_day, record, server_infos);
//    ////��������ȷ����������
//    //int demand = record.first + record.second;
//    //int single_resource = server_infos[buy_server_type].cpu + server_infos[buy_server_type].mem;
//    //while (demand > 0) {//�Ϲ�������֮����޸�
//    //    PurchaseServerNew(buy_server_type,
//    //        server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes,
//    //        cpu_sorted_server, empty_server);
//    //    full_server.push_back(nullptr);
//    //    cpu_server.push_back(nullptr);
//    //    mem_server.push_back(nullptr);
//    //    out_server.push_back(nullptr);
//    //    one_day_purchase[buy_server_type]++;
//    //    demand -= single_resource;
//    //}
//    
//    //Ǩ��**********************************************************************************************
//    //��һ������ɾ����������Ƶ�ĩβ������
//    auto iter_del_front = list_del.begin();
//    while (iter_del_front != list_del.end()){
//        if (iter_del_front->first != 10000) {
//            break;
//        }
//    }
//    auto iter_del_back = list_del.end();
//    int del_exchange = 0;
//    auto iter_del = iter_del_front;
//    while (iter_del_front != iter_del_back && actual_vm_count > 0) {
//        if (del_exchange % 2 == 0) {
//            iter_del = iter_del_front;
//            ++iter_del_front;
//        }
//        else {
//            --iter_del_back;
//            iter_del = iter_del_back;
//        }
//        ++del_exchange;
//        //�������Ϣ
//        int vm_id = iter_del->second->vm_id;
//        string vm_str = vm_runs[vm_id].vm_str_;
//        int dual_node = vm_infos[vm_str].dual_node;
//        //���ҵ��˺��ʵķ�������judge_find =1��
//        int judge_find = 0;
//        //��¼�ҵ���������id,�ͽڵ�
//        int judge_id = -1;
//        int judge_node = -2;
//        //��empty_serverĩβ��ʼ
//        for (int empty_index = server_number - 1; empty_index > empty_edge; --empty_index) {
//            Node a = empty_server[empty_index]->get_node('a');
//            Node b = empty_server[empty_index]->get_node('b');
//            if (dual_node == 1) {
//                if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                    && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                    judge_id = empty_index;
//                    judge_node = 2;
//                    judge_find = 1;
//                    break;
//                }
//            }
//            else {
//                if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                    judge_id = empty_index;
//                    judge_node = 0;
//                    judge_find = 1;
//                    break;
//
//                }
//                if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                    judge_id = empty_index;
//                    judge_node = 1;
//                    judge_find = 1;
//                    break;
//                }
//            }
//        }
//        if (judge_find == 1) {
//            //��������ӷ�������ȥ��
//            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);//������cpu��mem�ĸ���
//            vm_runs[vm_id].vm_id_ = vm_id;
//            vm_runs[vm_id].vm_str_ = vm_str;
//            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//            --actual_vm_count;
//            one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
//        }
//
//    }
//    //�ڶ���,�ǿշ���������
//    
//    //for (int i = 0; i < server_number; ++i) {
//    //    full_server[i] = nullptr;
//    //    cpu_server[i] = nullptr;
//    //    mem_server[i] = nullptr;
//    //    out_server[i] = nullptr;
//    //}
//    //wait_out = 0;                               //�ȴ�Ǩ�Ƶ����������
//    //out_edge = empty_edge;                      //out_server�����
//    //while (out_edge >= 0) {
//    //    wait_out += server_resources[out_edge].own_vm.size();
//    //    if (wait_out > actual_vm_count) {
//    //        break;
//    //    }
//    //    out_server[out_edge] = &server_resources[out_edge];
//    //    --out_edge;
//    //}
//    //for (int i = 0; i <= out_edge; ++i) {
//    //    if ((server_resources[i].get_node('a').cpu_res == 0 || server_resources[i].get_node('a').mem_res == 0)
//    //        && (server_resources[i].get_node('b').cpu_res == 0 || server_resources[i].get_node('b').mem_res == 0)) {
//    //        full_server[i] = &server_resources[i];
//    //    }
//    //    else if ((server_resources[i].get_node('a').cpu_res + server_resources[i].get_node('b').cpu_res == 0) >
//    //        (server_resources[i].get_node('a').mem_res + server_resources[i].get_node('b').mem_res == 0)) {
//    //        cpu_server[i] = &server_resources[i];
//    //    }
//    //    else {
//    //        mem_server[i] = &server_resources[i];
//    //    }
//    //}
//    
//    ////����������out_server�е������Ǩ�Ƶ�cpu_server��mem_server��
//  
//    //for (int out_index = out_edge + 1; out_index <= empty_edge; ++out_index) {
//    //    list<int> temp_id;
//    //    for (auto iter_int = out_server[out_index]->own_vm.begin(); iter_int != out_server[out_index]->own_vm.end(); ++iter_int) {
//    //        temp_id.push_back(*iter_int);
//    //    }
//    //    for (auto iter_int = temp_id.begin(); iter_int != temp_id.end(); ++iter_int) {
//    //        //�������Ϣ
//    //        int vm_id = *iter_int;
//    //        string vm_str = vm_runs[vm_id].vm_str_;
//    //        int dual_node = vm_infos[vm_str].dual_node;
//    //        //���ҵ��˺��ʵķ�������judge_find =1��
//    //        int judge_find = 0;
//    //        //��¼�ҵ���������id,�ͽڵ�
//    //        int judge_id = -1;
//    //        int judge_node = -2;
//    //        if (vm_infos[vm_runs[*iter_int].vm_str_].cpu > vm_infos[vm_runs[*iter_int].vm_str_].mem) {
//    //            //��cpu_server�ҵ����ʵķ�����
//    //            for (int cpu_index = 0; cpu_index < server_number; ++cpu_index) {
//    //                if (cpu_server[cpu_index] == nullptr) {
//    //                    continue;
//    //                }
//    //                Node a = cpu_server[cpu_index]->get_node('a');
//    //                Node b = cpu_server[cpu_index]->get_node('b');
//    //                if (dual_node == 1) {
//    //                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//    //                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//    //                        judge_id = cpu_index;
//    //                        judge_node = 2;
//    //                        judge_find = 1;
//    //                        break;
//    //                    }
//    //                }
//    //                else {
//    //                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//    //                        judge_id = cpu_index;
//    //                        judge_node = 0;
//    //                        judge_find = 1;
//    //                        break;
//    //                    }
//    //                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//    //                        judge_id = cpu_index;
//    //                        judge_node = 1;
//    //                        judge_find = 1;
//    //                        break;
//    //                    }
//    //                }
//    //            }
//    //            if (judge_find == 1) {
//    //                //��������ӷ�������ȥ��
//    //                vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                vm_runs[vm_id].vm_id_ = vm_id;
//    //                vm_runs[vm_id].vm_str_ = vm_str;
//    //                vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                --actual_vm_count;
//    //                one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//    //                if ((cpu_server[judge_id]->get_node('a').cpu_res + cpu_server[judge_id]->get_node('b').cpu_res) <
//    //                    (cpu_server[judge_id]->get_node('a').mem_res + cpu_server[judge_id]->get_node('b').mem_res)) {
//    //                    mem_server[judge_id] = cpu_server[judge_id];
//    //                    cpu_server[judge_id] = nullptr;
//    //                }
//    //            }
//    //            else {
//    //                //��out_server�ҵ����ʵķ�����
//    //                for (int out_index_front = out_edge + 1; out_index_front < out_index; ++out_index_front) {
//    //                    Node a = out_server[out_index_front]->get_node('a');
//    //                    Node b = out_server[out_index_front]->get_node('b');
//    //                    if (dual_node == 1) {
//    //                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//    //                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//    //                            judge_id = out_index_front;
//    //                            judge_node = 2;
//    //                            judge_find = 1;
//    //                            break;
//    //                        }
//    //                    }
//    //                    else {
//    //                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//    //                            judge_id = out_index_front;
//    //                            judge_node = 0;
//    //                            judge_find = 1;
//    //                            break;
//    //                        }
//    //                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//    //                            judge_id = out_index_front;
//    //                            judge_node = 1;
//    //                            judge_find = 1;
//    //                            break;
//    //                        }
//    //                    }
//    //                }
//    //                if (judge_find == 1) {
//    //                    //��������ӷ�������ȥ��
//    //                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                    vm_runs[vm_id].vm_id_ = vm_id;
//    //                    vm_runs[vm_id].vm_str_ = vm_str;
//    //                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                    --actual_vm_count;
//    //                    one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//    //                }
//    //            }
//    //        }
//    //        else {
//    //            //��mem_server�ҵ����ʵķ�����
//    //            for (int mem_index = 0; mem_index < server_number; ++mem_index) {
//    //                if (mem_server[mem_index] == nullptr) {
//    //                    continue;
//    //                }
//    //                Node a = mem_server[mem_index]->get_node('a');
//    //                Node b = mem_server[mem_index]->get_node('b');
//    //                if (dual_node == 1) {
//    //                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//    //                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//    //                        judge_id = mem_index;
//    //                        judge_node = 2;
//    //                        judge_find = 1;
//    //                        break;
//    //                    }
//    //                }
//    //                else {
//    //                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//    //                        judge_id = mem_index;
//    //                        judge_node = 0;
//    //                        judge_find = 1;
//    //                        break;
//    //                    }
//    //                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//    //                        judge_id = mem_index;
//    //                        judge_node = 1;
//    //                        judge_find = 1;
//    //                        break;
//    //                    }
//    //                }
//    //            }
//    //            if (judge_find == 1) {
//    //                //��������ӷ�������ȥ��
//    //                vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                vm_runs[vm_id].vm_id_ = vm_id;
//    //                vm_runs[vm_id].vm_str_ = vm_str;
//    //                vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                --actual_vm_count;
//    //                one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//    //                if ((mem_server[judge_id]->get_node('a').cpu_res + mem_server[judge_id]->get_node('b').cpu_res) >
//    //                    (mem_server[judge_id]->get_node('a').mem_res + mem_server[judge_id]->get_node('b').mem_res)) {
//    //                    cpu_server[judge_id] = mem_server[judge_id];
//    //                    mem_server[judge_id] = nullptr;
//    //                }
//    //            }
//    //            else {
//    //                //��out_server�ҵ����ʵķ�����
//    //                for (int out_index_front = out_edge + 1; out_index_front < out_index; ++out_index_front) {
//    //                    Node a = out_server[out_index_front]->get_node('a');
//    //                    Node b = out_server[out_index_front]->get_node('b');
//    //                    if (dual_node == 1) {
//    //                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//    //                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//    //                            judge_id = out_index_front;
//    //                            judge_node = 2;
//    //                            judge_find = 1;
//    //                            break;
//    //                        }
//    //                    }
//    //                    else {
//    //                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//    //                            judge_id = out_index_front;
//    //                            judge_node = 0;
//    //                            judge_find = 1;
//    //                            break;
//    //                        }
//    //                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//    //                            judge_id = out_index_front;
//    //                            judge_node = 1;
//    //                            judge_find = 1;
//    //                            break;
//    //                        }
//    //                    }
//    //                }
//    //                if (judge_find == 1) {
//    //                    //��������ӷ�������ȥ��
//    //                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                    vm_runs[vm_id].vm_id_ = vm_id;
//    //                    vm_runs[vm_id].vm_str_ = vm_str;
//    //                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    //                    --actual_vm_count;
//    //                    one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//    //                }
//    //            }
//    //        }
//    //    }
//    //}
//
//    ////���Ĳ�����out_server���䵽cpu_server��mem_server��empty_server
//    
//    //int empty_edge_old = empty_edge;
//    //for (int out_index = out_edge + 1; out_index <= empty_edge_old; ++out_index) {
//    //    if (out_server[out_index] == nullptr) {
//    //        continue;
//    //    }
//    //    if (out_server[out_index]->get_node('a').cpu_used == 0 && out_server[out_index]->get_node('a').mem_used == 0
//    //        && out_server[out_index]->get_node('b').cpu_used == 0 && out_server[out_index]->get_node('b').mem_used == 0) {
//    //        empty_server[out_index] = out_server[out_index];
//    //        empty_edge = out_index - 1;
//    //    }
//    //    else if ((out_server[out_index]->get_node('a').cpu_res + out_server[out_index]->get_node('b').cpu_res)
//    //                > (out_server[out_index]->get_node('a').mem_res + out_server[out_index]->get_node('b').mem_res)) {
//    //        cpu_server[out_index] = out_server[out_index];
//    //    }
//    //    else {
//    //        mem_server[out_index] = out_server[out_index];
//    //    }
//    //    out_server[out_index] = nullptr;
//    //}
//    
//    //���岽����full_server�ռ��ͷ�
//    for (int full_index = 0; full_index <= empty_edge && actual_vm_count>0; ++full_index) {
//        if (full_server[full_index] == nullptr) {
//            continue;
//        }
//        double a_cpu_res = full_server[full_index]->get_node('a').cpu_res;
//        double a_mem_res = full_server[full_index]->get_node('a').mem_res;
//        double b_cpu_res = full_server[full_index]->get_node('b').cpu_res;
//        double b_mem_res = full_server[full_index]->get_node('b').mem_res;
//
//        int judge_find = 0;
//        //Ǩ��˫�ڵ�
//        if (((a_cpu_res - a_mem_res) > 40 && (b_cpu_res - b_mem_res) > 40) ||
//            ((a_mem_res - a_cpu_res) > 40 && (b_mem_res - b_cpu_res) > 40)) {
//            double judge_dual =abs( log((a_cpu_res + b_cpu_res) / (a_mem_res + b_mem_res)));
//            //�������Ϣ
//            int vm_id = -1;
//            for (auto dual_vm = full_server[full_index]->cown_vm.begin(); dual_vm != full_server[full_index]->cown_vm.end(); ++dual_vm) {
//                double temp_dual =abs( log((a_cpu_res + b_cpu_res + vm_infos[vm_runs[*dual_vm].vm_str_].cpu) / (a_mem_res + b_mem_res + vm_infos[vm_runs[*dual_vm].vm_str_].mem)));
//                if (temp_dual < judge_dual) {
//                    judge_dual = temp_dual;
//                    vm_id = *dual_vm;
//                }
//            }
//            if (vm_id != -1) {
//                //��¼�ҵ���������id,�ͽڵ�
//                int judge_id = -1;
//                string vm_str = vm_runs[vm_id].vm_str_;
//                if (vm_infos[vm_str].cpu > vm_infos[vm_str].mem) {
//                    //��cpu_server�ҵ����ʵķ�����
//                    for (int cpu_index = 0; cpu_index <= empty_edge; ++cpu_index) {
//                        if (cpu_server[cpu_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = cpu_server[cpu_index]->get_node('a');
//                        Node b = cpu_server[cpu_index]->get_node('b');
//                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                            judge_id = cpu_index;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                        if ((cpu_server[judge_id]->get_node('a').cpu_res + cpu_server[judge_id]->get_node('b').cpu_res) <
//                            (cpu_server[judge_id]->get_node('a').mem_res + cpu_server[judge_id]->get_node('b').mem_res)) {
//                            mem_server[judge_id] = cpu_server[judge_id];
//                            cpu_server[judge_id] = nullptr;
//                        }
//
//                    }
//                }
//                else {
//                    //��mem_server�ҵ����ʵķ�����
//                    for (int mem_index = 0; mem_index <= empty_edge; ++mem_index) {
//                        if (mem_server[mem_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = mem_server[mem_index]->get_node('a');
//                        Node b = mem_server[mem_index]->get_node('b');
//                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                            judge_id = mem_index;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                        if ((mem_server[judge_id]->get_node('a').cpu_res + mem_server[judge_id]->get_node('b').cpu_res) >
//                            (mem_server[judge_id]->get_node('a').mem_res + mem_server[judge_id]->get_node('b').mem_res)) {
//                            cpu_server[judge_id] = mem_server[judge_id];
//                            mem_server[judge_id] = nullptr;
//                        }
//                    }
//                }
//            }
//        }
//        //Ǩ��A�ڵ�
//        if (judge_find == 0 && abs(a_cpu_res - a_mem_res) > 40) {
//            double judge_dual = abs(log(a_cpu_res / a_mem_res));
//            //�������Ϣ
//            int vm_id = -1;
//            for (auto a_vm = full_server[full_index]->aown_vm.begin(); a_vm != full_server[full_index]->aown_vm.end(); ++a_vm) {
//                double temp_dual = abs(log((a_cpu_res+ vm_infos[vm_runs[*a_vm].vm_str_].cpu) / (a_mem_res+ vm_infos[vm_runs[*a_vm].vm_str_].mem)));
//                if (temp_dual < judge_dual) {
//                    judge_dual = temp_dual;
//                    vm_id = *a_vm;
//                }
//            }
//            if (vm_id != -1) {
//                //��¼�ҵ���������id,�ͽڵ�
//                int judge_id = -1;
//                int judge_node = -1;
//                string vm_str = vm_runs[vm_id].vm_str_;
//                if (vm_infos[vm_str].cpu > vm_infos[vm_str].mem) {
//                    //��cpu_server�ҵ����ʵķ�����
//                    for (int cpu_index = 0; cpu_index <= empty_edge; ++cpu_index) {
//                        if (cpu_server[cpu_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = cpu_server[cpu_index]->get_node('a');
//                        Node b = cpu_server[cpu_index]->get_node('b');
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = cpu_index;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = cpu_index;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                        if ((cpu_server[judge_id]->get_node('a').cpu_res + cpu_server[judge_id]->get_node('b').cpu_res) <
//                            (cpu_server[judge_id]->get_node('a').mem_res + cpu_server[judge_id]->get_node('b').mem_res)) {
//                            mem_server[judge_id] = cpu_server[judge_id];
//                            cpu_server[judge_id] = nullptr;
//                        }
//
//                    }
//                }
//                else {
//                    //��mem_server�ҵ����ʵķ�����
//                    for (int mem_index = 0; mem_index <= empty_edge; ++mem_index) {
//                        if (mem_server[mem_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = mem_server[mem_index]->get_node('a');
//                        Node b = mem_server[mem_index]->get_node('b');
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = mem_index;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = mem_index;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                        if ((mem_server[judge_id]->get_node('a').cpu_res + mem_server[judge_id]->get_node('b').cpu_res) >
//                            (mem_server[judge_id]->get_node('a').mem_res + mem_server[judge_id]->get_node('b').mem_res)) {
//                            cpu_server[judge_id] = mem_server[judge_id];
//                            mem_server[judge_id] = nullptr;
//                        }
//                    }
//                }
//            }
//        }
//        //Ǩ��B�ڵ�
//        if (judge_find == 0 && abs(b_cpu_res - b_mem_res) > 40) {
//            double judge_dual = abs(log(b_cpu_res / b_mem_res));
//            //�������Ϣ
//            int vm_id = -1;
//            for (auto b_vm = full_server[full_index]->bown_vm.begin(); b_vm != full_server[full_index]->bown_vm.end(); ++b_vm) {
//                double temp_dual = abs(log((b_cpu_res + vm_infos[vm_runs[*b_vm].vm_str_].cpu) / (b_mem_res + vm_infos[vm_runs[*b_vm].vm_str_].mem)));
//                if (temp_dual < judge_dual) {
//                    judge_dual = temp_dual;
//                    vm_id = *b_vm;
//                }
//            }
//            if (vm_id != -1) {
//                //��¼�ҵ���������id,�ͽڵ�
//                int judge_id = -1;
//                int judge_node = -1;
//                string vm_str = vm_runs[vm_id].vm_str_;
//                if (vm_infos[vm_str].cpu > vm_infos[vm_str].mem) {
//                    //��cpu_server�ҵ����ʵķ�����
//                    for (int cpu_index = 0; cpu_index <= empty_edge; ++cpu_index) {
//                        if (cpu_server[cpu_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = cpu_server[cpu_index]->get_node('a');
//                        Node b = cpu_server[cpu_index]->get_node('b');
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = cpu_index;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = cpu_index;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                        if ((cpu_server[judge_id]->get_node('a').cpu_res + cpu_server[judge_id]->get_node('b').cpu_res) <
//                            (cpu_server[judge_id]->get_node('a').mem_res + cpu_server[judge_id]->get_node('b').mem_res)) {
//                            mem_server[judge_id] = cpu_server[judge_id];
//                            cpu_server[judge_id] = nullptr;
//                        }
//
//                    }
//                }
//                else {
//                    //��mem_server�ҵ����ʵķ�����
//                    for (int mem_index = 0; mem_index <= empty_edge; ++mem_index) {
//                        if (mem_server[mem_index] == nullptr) {
//                            continue;
//                        }
//                        Node a = mem_server[mem_index]->get_node('a');
//                        Node b = mem_server[mem_index]->get_node('b');
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = mem_index;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = mem_index;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    if (judge_find == 1) {
//                        //��������ӷ�������ȥ��
//                        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        vm_runs[vm_id].vm_id_ = vm_id;
//                        vm_runs[vm_id].vm_str_ = vm_str;
//                        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        --actual_vm_count;
//                        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                        if ((mem_server[judge_id]->get_node('a').cpu_res + mem_server[judge_id]->get_node('b').cpu_res) >
//                            (mem_server[judge_id]->get_node('a').mem_res + mem_server[judge_id]->get_node('b').mem_res)) {
//                            cpu_server[judge_id] = mem_server[judge_id];
//                            mem_server[judge_id] = nullptr;
//                        }
//                    }
//                }
//            }
//        }
//
//        if (judge_find == 1) {
//            if ((full_server[full_index]->get_node('a').cpu_res + full_server[full_index]->get_node('b').cpu_res) >
//                (full_server[full_index]->get_node('a').mem_res + full_server[full_index]->get_node('b').mem_res)) {
//                cpu_server[full_index] = full_server[full_index];
//            }
//            else {
//                mem_server[full_index] = full_server[full_index];
//            }
//            full_server[full_index] = nullptr;
//        }
//
//    }
//    //����*************************************************************************************************************
//    for (auto req = day_requests.begin(); req != day_requests.end(); ++req) {
//        int vm_id = req->vm_id;
//        if (req->op_type == ADD) {
//            ++vm_count;
//            string vm_str = req->vm_type;
//            //��������cpu����mem
//            if (vm_infos[vm_str].cpu > vm_infos[vm_str].mem) {
//                //��cpu_server��ʼ
//                pair<int, int> judge_add = NewCreateVM(vm_id, vm_str,
//                    vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                    cpu_server, server_number);
//                if (judge_add.first == -1) {
//                    //��mem_server��ʼ
//                    judge_add = NewCreateVM(vm_id, vm_str,
//                        vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                        mem_server, server_number);
//                    if (judge_add.first == -1) {
//                        //��empty_server��ʼ
//                        judge_add = NewCreateVM(vm_id, vm_str,
//                            vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                            empty_server, server_number);
//                        if (judge_add.first == -1) {
//                            PurchaseServerNew(buy_server_type,
//                                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes,
//                                cpu_sorted_server, empty_server);
//                            one_day_purchase[buy_server_type]++;
//                            vm_runs[vm_id].vm_id_ = vm_id;
//                            vm_runs[vm_id].vm_str_ = vm_str;
//                            cpu_server.push_back(nullptr);
//                            mem_server.push_back(nullptr);
//                            if (vm_infos[vm_str].dual_node == 1) {
//                                vm_runs[vm_id].Add(server_number - 1, 2,
//                                    vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                                one_day_create_vm.push_back(make_pair(server_number - 1, 2));
//                            }
//                            else {
//                                vm_runs[vm_id].Add(server_number - 1, 0,
//                                    vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                                one_day_create_vm.push_back(make_pair(server_number - 1, 0));
//                            }
//                            int temp_num = server_number - 1;
//                            if ((empty_server[temp_num]->get_node('a').cpu_res + empty_server[temp_num]->get_node('b').cpu_res) >
//                                (empty_server[temp_num]->get_node('a').mem_res + empty_server[temp_num]->get_node('b').mem_res)) {
//                                cpu_server[temp_num] = empty_server[temp_num];
//                            }
//                            else {
//                                mem_server[temp_num] = empty_server[temp_num];
//                            }
//                            empty_server[temp_num] = nullptr;
//                        }
//                        else {
//                            one_day_create_vm.push_back(judge_add);
//                            if ((empty_server[judge_add.first]->get_node('a').cpu_res + empty_server[judge_add.first]->get_node('b').cpu_res) >
//                                (empty_server[judge_add.first]->get_node('a').mem_res + empty_server[judge_add.first]->get_node('b').mem_res)) {
//                                cpu_server[judge_add.first] = empty_server[judge_add.first];
//                            }
//                            else {
//                                mem_server[judge_add.first] = empty_server[judge_add.first];
//                            }
//                            empty_server[judge_add.first] = nullptr;
//                        }
//                    }
//                    else {
//                        one_day_create_vm.push_back(judge_add);
//                        if ((mem_server[judge_add.first]->get_node('a').cpu_res + mem_server[judge_add.first]->get_node('b').cpu_res) >
//                            (mem_server[judge_add.first]->get_node('a').mem_res + mem_server[judge_add.first]->get_node('b').mem_res)) {
//                            cpu_server[judge_add.first] = mem_server[judge_add.first];
//                            mem_server[judge_add.first] = nullptr;
//                        }
//                    }
//                }
//                else {
//                    one_day_create_vm.push_back(judge_add);
//                    if ((cpu_server[judge_add.first]->get_node('a').cpu_res + cpu_server[judge_add.first]->get_node('b').cpu_res) <
//                        (cpu_server[judge_add.first]->get_node('a').mem_res + cpu_server[judge_add.first]->get_node('b').mem_res)) {
//                        mem_server[judge_add.first] = cpu_server[judge_add.first];
//                        cpu_server[judge_add.first] = nullptr;
//                    }
//                }
//            }
//            else {
//                //��mem_server��ʼ
//                pair<int, int> judge_add = NewCreateVM(vm_id, vm_str,
//                    vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                    mem_server, server_number);
//                if (judge_add.first == -1) {
//                    //��cpu_server��ʼ
//                    judge_add = NewCreateVM(vm_id, vm_str,
//                        vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                        cpu_server, server_number);
//                    if (judge_add.first == -1) {
//                        //��empty_server��ʼ
//                        judge_add = NewCreateVM(vm_id, vm_str,
//                            vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                            empty_server, server_number);
//                        if (judge_add.first == -1) {
//                            PurchaseServerNew(buy_server_type,
//                                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes,
//                                cpu_sorted_server, empty_server);
//                            one_day_purchase[buy_server_type]++;
//                            vm_runs[vm_id].vm_id_ = vm_id;
//                            vm_runs[vm_id].vm_str_ = vm_str;
//                            full_server.push_back(nullptr);
//                            cpu_server.push_back(nullptr);
//                            mem_server.push_back(nullptr);
//                            if (vm_infos[vm_str].dual_node == 1) {
//                                vm_runs[vm_id].Add(server_number - 1, 2,
//                                    vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                                one_day_create_vm.push_back(make_pair(server_number - 1, 2));
//                            }
//                            else {
//                                vm_runs[vm_id].Add(server_number - 1, 0,
//                                    vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                                one_day_create_vm.push_back(make_pair(server_number - 1, 0));
//                            }
//                            if ((empty_server[server_number - 1]->get_node('a').cpu_res + empty_server[server_number - 1]->get_node('b').cpu_res) >
//                                (empty_server[server_number - 1]->get_node('a').mem_res + empty_server[server_number - 1]->get_node('b').mem_res)) {
//                                cpu_server[server_number - 1] = empty_server[server_number - 1];
//                            }
//                            else {
//                                mem_server[server_number - 1] = empty_server[server_number - 1];
//                            }
//                            empty_server[server_number - 1] = nullptr;
//                        }
//                        else {
//                            one_day_create_vm.push_back(judge_add);
//                            if ((empty_server[judge_add.first]->get_node('a').cpu_res + empty_server[judge_add.first]->get_node('b').cpu_res) >
//                                (empty_server[judge_add.first]->get_node('a').mem_res + empty_server[judge_add.first]->get_node('b').mem_res)) {
//                                cpu_server[judge_add.first] = empty_server[judge_add.first];
//                            }
//                            else {
//                                mem_server[judge_add.first] = empty_server[judge_add.first];
//                            }
//                            empty_server[judge_add.first] = nullptr;
//                        }
//                    }
//                    else {
//                        one_day_create_vm.push_back(judge_add);
//                        if ((cpu_server[judge_add.first]->get_node('a').cpu_res + cpu_server[judge_add.first]->get_node('b').cpu_res) <
//                            (cpu_server[judge_add.first]->get_node('a').mem_res + cpu_server[judge_add.first]->get_node('b').mem_res)) {
//                            mem_server[judge_add.first] = cpu_server[judge_add.first];
//                            cpu_server[judge_add.first] = nullptr;
//                        }
//                    }
//                }
//                else {
//                    one_day_create_vm.push_back(judge_add);
//                    if ((mem_server[judge_add.first]->get_node('a').cpu_res + mem_server[judge_add.first]->get_node('b').cpu_res) <
//                        (mem_server[judge_add.first]->get_node('a').mem_res + mem_server[judge_add.first]->get_node('b').mem_res)) {
//                        cpu_server[judge_add.first] = mem_server[judge_add.first];
//                        mem_server[judge_add.first] = nullptr;
//                    }
//                }
//            }
//        }
//        else {
//            --vm_count;
//            vm_runs[vm_id].Del(
//                vm_infos, vm_runs, server_resources, server_runs, server_closes);
//        }
//
//    }
//
//}


//void DeployVmDel(int& vm_count, int& server_number,
//    long long& BUYCOST, long long& TOTALCOST, int& actual_vm_count,
//    list<pair<int, Request*> > list_add,
//    list<Request*> list_del,
//    unordered_map<string, int>& one_day_purchase,
//    vector<pair<int, int>>& one_day_create_vm,
//    vector<pair<int, pair<int, int> > > one_day_migrate_vm,
//    unordered_map<string, VMInfo>& vm_infos,
//    unordered_map<int, VM>& vm_runs,
//    unordered_map<string, ServerInfo>& server_infos,
//    unordered_map<int, Server>& server_resources,
//    unordered_map<int, Server*>& server_runs,
//    unordered_map<int, Server*>& server_closes,
//    list<Server*>& cpu_sorted_server,
//    list<Server*>& full_server,
//    list<Server*>& cpu_mem_server,
//    list<Server*>& empty_server,
//    string& buy_server_type)
//{
//    list<Server*> temp_empty;
//    //Ǩ��
//    //����del��empty��
//    for (auto iter_del = list_del.rbegin(); iter_del != list_del.rend() && actual_vm_count > 0; ++iter_del) {
//        auto iter_vm = vm_runs.find((*iter_del)->vm_id);
//        if (iter_vm == vm_runs.end()) {
//            continue;
//        }
//        else {
//            //�������Ϣ
//            int vm_id = iter_vm->first;;
//            string vm_str = vm_runs[vm_id].vm_str_;
//            int dual_node = vm_infos[vm_str].dual_node;
//
//            //���ҵ��˺��ʵķ�������judge_find =1��
//            int judge_find = 0;
//            //��¼�ҵ���������id,�ͽڵ�
//            int judge_id = -1;
//            int judge_node = -2;
//
//            for (auto iter_temp_em = temp_empty.begin(); iter_temp_em != temp_empty.end(); ++iter_temp_em) {
//                Node a = (*iter_temp_em)->get_node('a');
//                Node b = (*iter_temp_em)->get_node('b');
//                if (dual_node == 1) {
//                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                        judge_id = (*iter_temp_em)->ID_;
//                        judge_node = 2;
//                        judge_find = 1;
//                        break;
//                    }
//                }
//                else {
//                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                        judge_id = (*iter_temp_em)->ID_;
//                        judge_node = 0;
//                        judge_find = 1;
//                        break;
//
//                    }
//                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                        judge_id = (*iter_temp_em)->ID_;
//                        judge_node = 1;
//                        judge_find = 1;
//                        break;
//                    }
//                }
//            }
//            if (judge_find == 1) {
//                //��������ӷ�������ȥ��
//                vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);//������cpu��mem�ĸ���
//                vm_runs[vm_id].vm_id_ = vm_id;
//                vm_runs[vm_id].vm_str_ = vm_str;
//                vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                --actual_vm_count;
//                one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
//            }
//            else {
//                auto iter_empty = empty_server.begin();
//                for (; iter_empty != empty_server.end(); ++iter_empty) {
//                    Node a = (*iter_empty)->get_node('a');
//                    Node b = (*iter_empty)->get_node('b');
//                    if (dual_node == 1) {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                            judge_id = (*iter_empty)->ID_;
//                            judge_node = 2;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    else {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = (*iter_empty)->ID_;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = (*iter_empty)->ID_;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                }
//                if (judge_find == 1) {
//                    //��������ӷ�������ȥ��
//                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);//������cpu��mem�ĸ���
//                    vm_runs[vm_id].vm_id_ = vm_id;
//                    vm_runs[vm_id].vm_str_ = vm_str;
//                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    --actual_vm_count;
//                    one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
//                    temp_empty.push_back(*iter_empty);
//                    empty_server.erase(iter_empty);
//                }
//            }
//
//        }
//    }
//    //����full
//    for (auto iter_full = full_server.begin(); iter_full != full_server.end();) {
//        if ( ( (*iter_full)->get_node('a').cpu_res != 0 && (*iter_full)->get_node('a').mem_res != 0)
//            ||( (*iter_full)->get_node('b').cpu_res != 0 && (*iter_full)->get_node('b').mem_res != 0)) {
//            cpu_mem_server.push_back(*iter_full);
//            iter_full = full_server.erase(iter_full);
//        }
//        else {
//            ++iter_full;
//        }
//    }
//    // 
//    //����cpu_mem_server*************************************************************************************************
//    auto server_cmp = [](Server* first, Server* second) {
//        int first_cm = first->get_node('a').cpu_res + first->get_node('b').cpu_res
//            - first->get_node('a').mem_res - first->get_node('b').mem_res;
//        int second_cm = second->get_node('a').cpu_res + second->get_node('b').cpu_res
//            - second->get_node('a').mem_res - second->get_node('b').mem_res;
//        return first_cm > second_cm;
//    };
//
//    list<Server*> wait_server;
//    list<Server*> mem_cpu_server;
//    list<pair<int, int> > vm_output;
//    int wait_vm = 0;
//    for (auto iter_cpu = cpu_mem_server.begin(); iter_cpu != cpu_mem_server.end(); ) {
//        if (((*iter_cpu)->get_node('a').mem_res + (*iter_cpu)->get_node('b').mem_res) -
//            ((*iter_cpu)->get_node('a').cpu_res + (*iter_cpu)->get_node('b').cpu_res) > 0) {
//            mem_cpu_server.push_back(*iter_cpu);
//            iter_cpu = cpu_mem_server.erase(iter_cpu);
//        }
//        else {
//            ++iter_cpu;
//        }
//    }
//
//    cpu_mem_server.sort([](Server* first, Server* second) {
//        int first_m = first->get_node('a').cpu_res + first->get_node('b').cpu_res;
//        int second_m = second->get_node('a').cpu_res + second->get_node('b').cpu_res;
//        //if (first_m == second_m) {
//        //    int first_c = first->get_node('a').mem_res + first->get_node('b').mem_res;
//        //    int second_c = second->get_node('a').mem_res + second->get_node('b').mem_res;
//        //    return first_c > second_c;
//        //}
//        //else {
//        return first_m > second_m;
//        });
//    mem_cpu_server.sort([](Server* first, Server* second) {
//        int first_m = first->get_node('a').mem_res + first->get_node('b').mem_res;
//        int second_m = second->get_node('a').mem_res + second->get_node('b').mem_res;
//        //if (first_m == second_m) {
//        //    int first_c = first->get_node('a').cpu_res + first->get_node('b').cpu_res;
//        //    int second_c = second->get_node('a').cpu_res + second->get_node('b').cpu_res;
//        //    return first_c > second_c;
//        //}
//        return first_m > second_m;
//        });
//
//    auto iter_wait_cpu = cpu_mem_server.begin();
//    auto iter_wait_mem = mem_cpu_server.begin();
//    if (iter_wait_cpu != cpu_mem_server.end()) {
//        wait_vm += (*iter_wait_cpu)->aown_vm.size() + (*iter_wait_cpu)->bown_vm.size() + (*iter_wait_cpu)->cown_vm.size();
//
//        while (wait_vm < actual_vm_count && iter_wait_mem != mem_cpu_server.end()) {
//            wait_server.push_back(*iter_wait_cpu);
//            iter_wait_cpu = cpu_mem_server.erase(iter_wait_cpu);
//            wait_vm += (*iter_wait_mem)->aown_vm.size() + (*iter_wait_mem)->bown_vm.size() + (*iter_wait_mem)->cown_vm.size();
//            if (wait_vm > actual_vm_count || iter_wait_cpu == cpu_mem_server.end()) {
//                break;
//            }
//            wait_server.push_back(*iter_wait_mem);
//            iter_wait_mem = mem_cpu_server.erase(iter_wait_mem);
//            wait_vm += (*iter_wait_cpu)->aown_vm.size() + (*iter_wait_cpu)->bown_vm.size() + (*iter_wait_cpu)->cown_vm.size();
//        }
//    }
//    //�������Ż��������Ȳ�����
//    for (auto iter_wait = wait_server.begin(); iter_wait != wait_server.end();) {
//        list<int> temp_id;
//        for (auto iter_int = (*iter_wait)->own_vm.begin(); iter_int != (*iter_wait)->own_vm.end(); ++iter_int) {
//            temp_id.push_back(*iter_int);
//        }
//
//        for (auto iter_int = temp_id.begin(); iter_int != temp_id.end(); ++iter_int) {
//            //�������Ϣ
//            int vm_id = *iter_int;
//            string vm_str = vm_runs[vm_id].vm_str_;
//            int dual_node = vm_infos[vm_str].dual_node;
//            //���ҵ��˺��ʵķ�������judge_find =1��
//            int judge_find = 0;
//            //��¼�ҵ���������id,�ͽڵ�
//            int judge_id = -1;
//            int judge_node = -2;
//            if (vm_infos[vm_runs[*iter_int].vm_str_].cpu > vm_infos[vm_runs[*iter_int].vm_str_].mem) {
//                //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
//                for (auto iter_s = cpu_mem_server.rbegin(); iter_s != cpu_mem_server.rend();) {
//                    Node a = (*iter_s)->get_node('a');
//                    Node b = (*iter_s)->get_node('b');
//                    if (dual_node == 1) {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                            judge_id = (*iter_s)->ID_;
//                            judge_node = 2;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    else {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = (*iter_s)->ID_;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = (*iter_s)->ID_;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    ++iter_s;
//                }
//                if (judge_find == 1) {
//                    //��������ӷ�������ȥ��
//                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    vm_runs[vm_id].vm_id_ = vm_id;
//                    vm_runs[vm_id].vm_str_ = vm_str;
//                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    --actual_vm_count;
//                    one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                }
//                //else {
//                //    for (auto iter_s = mem_cpu_server.rbegin(); iter_s != mem_cpu_server.rend();) {
//                //        Node a = (*iter_s)->get_node('a');
//                //        Node b = (*iter_s)->get_node('b');
//                //        if (dual_node == 1) {
//                //            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                //                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                //                judge_id = (*iter_s)->ID_;
//                //                judge_node = 2;
//                //                judge_find = 1;
//                //                break;
//                //            }
//                //        }
//                //        else {
//                //            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                //                judge_id = (*iter_s)->ID_;
//                //                judge_node = 0;
//                //                judge_find = 1;
//                //                break;
//                //            }
//                //            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                //                judge_id = (*iter_s)->ID_;
//                //                judge_node = 1;
//                //                judge_find = 1;
//                //                break;
//                //            }
//                //        }
//                //        ++iter_s;
//                //    }
//                //    if (judge_find == 1) {
//                //        //��������ӷ�������ȥ��
//                //        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                //        vm_runs[vm_id].vm_id_ = vm_id;
//                //        vm_runs[vm_id].vm_str_ = vm_str;
//                //        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                //        --actual_vm_count;
//                //        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                //    }
//                //}
//            }
//            else {
//                //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
//                for (auto iter_s = mem_cpu_server.rbegin(); iter_s != mem_cpu_server.rend();) {
//                    Node a = (*iter_s)->get_node('a');
//                    Node b = (*iter_s)->get_node('b');
//                    if (dual_node == 1) {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                            judge_id = (*iter_s)->ID_;
//                            judge_node = 2;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    else {
//                        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = (*iter_s)->ID_;
//                            judge_node = 0;
//                            judge_find = 1;
//                            break;
//
//                        }
//                        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                            judge_id = (*iter_s)->ID_;
//                            judge_node = 1;
//                            judge_find = 1;
//                            break;
//                        }
//                    }
//                    ++iter_s;
//                }
//                if (judge_find == 1) {
//                    //��������ӷ�������ȥ��
//                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    vm_runs[vm_id].vm_id_ = vm_id;
//                    vm_runs[vm_id].vm_str_ = vm_str;
//                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    --actual_vm_count;
//                    one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                }
//                //else {
//                //    for (auto iter_s = cpu_mem_server.rbegin(); iter_s != cpu_mem_server.rend();) {
//                //        Node a = (*iter_s)->get_node('a');
//                //        Node b = (*iter_s)->get_node('b');
//                //        if (dual_node == 1) {
//                //            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                //                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                //                judge_id = (*iter_s)->ID_;
//                //                judge_node = 2;
//                //                judge_find = 1;
//                //                break;
//                //            }
//                //        }
//                //        else {
//                //            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                //                judge_id = (*iter_s)->ID_;
//                //                judge_node = 0;
//                //                judge_find = 1;
//                //                break;
//                //            }
//                //            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                //                judge_id = (*iter_s)->ID_;
//                //                judge_node = 1;
//                //                judge_find = 1;
//                //                break;
//                //            }
//                //        }
//                //        ++iter_s;
//                //    }
//                //    if (judge_find == 1) {
//                //        //��������ӷ�������ȥ��
//                //        vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                //        vm_runs[vm_id].vm_id_ = vm_id;
//                //        vm_runs[vm_id].vm_str_ = vm_str;
//                //        vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                //        --actual_vm_count;
//                //        one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
//                //    }
//                //}
//            }
//        }
//        if ((*iter_wait)->get_node('a').cpu_used == 0 && (*iter_wait)->get_node('b').cpu_used == 0
//            && (*iter_wait)->get_node('a').mem_used == 0 && (*iter_wait)->get_node('b').mem_used == 0) {
//            empty_server.push_back(*iter_wait);
//            iter_wait = wait_server.erase(iter_wait);
//        }
//        else {
//            ++iter_wait;
//        }
//    }
//
//    //��wait_server�ķ�������ʼǨ��
//    for (auto iter_r_s = wait_server.rbegin(); 0 < actual_vm_count && iter_r_s != wait_server.rend(); ++iter_r_s) {
//        list<int> iter_int;
//        for (auto iter = (*iter_r_s)->own_vm.begin(); iter != (*iter_r_s)->own_vm.end(); ++iter) {
//            iter_int.push_back(*iter);
//        }
//        auto iter_v = iter_int.begin();
//        while (iter_v != iter_int.end() && 0 < actual_vm_count) {
//            //--temp_num;
//            int vm_id = *iter_v;
//            string vm_str = vm_runs[*iter_v].vm_str_;
//            int s_id = vm_runs[*iter_v].sv_id_;
//            int s_node = vm_runs[*iter_v].sv_node_;
//            //�ҵ����ʵķ�������Ϊ1
//            int judge = 0;
//            int fit_server_id;
//            int fit_server_node;
//            //�ҵ����ʵķ���������
//            for (auto iter_s = wait_server.begin(); iter_s != wait_server.end(); ++iter_s) {
//                if ((*iter_s)->ID_ == s_id && s_node == 2) {
//                    break;
//                }
//                Node a = (*iter_s)->get_node('a');
//                Node b = (*iter_s)->get_node('b');
//                if (vm_infos[vm_str].dual_node == 1) {
//                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                        fit_server_id = (*iter_s)->ID_;
//                        fit_server_node = 2;
//
//                        if (s_id != (*iter_s)->ID_) {
//                            judge = 1;
//                        }
//                        break;
//                    }
//                }
//                else {
//                    if ((*iter_s)->ID_ == s_id && s_node == 0) {
//                        break;
//                    }
//                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                        fit_server_id = (*iter_s)->ID_;
//                        fit_server_node = 0;
//
//                        if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 0)) {
//                            judge = 1;
//                        }
//                        break;
//
//                    }
//                    if ((*iter_s)->ID_ == s_id && s_node == 1) {
//                        break;
//                    }
//                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                        fit_server_id = (*iter_s)->ID_;
//                        fit_server_node = 1;
//                        if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 1)) {
//                            judge = 1;
//                        }
//                        break;
//                    }
//                }
//            }
//            if (judge == 1) {
//                vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                VM vm(vm_id, vm_str);
//                vm_runs[vm_id] = vm;
//                vm_runs[vm_id].Add(fit_server_id, fit_server_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                one_day_migrate_vm.push_back(make_pair(vm_id, make_pair(fit_server_id, fit_server_node)));
//                --actual_vm_count;
//            }
//            ++iter_v;
//        }
//    }
//    cpu_mem_server.splice(cpu_mem_server.begin(), wait_server);
//    cpu_mem_server.splice(cpu_mem_server.begin(), mem_cpu_server);
//    cpu_mem_server.sort(server_cmp);
//
//    //����
//    if (!list_add.empty()) {
//        auto iter_list_add = list_add.begin();
//        auto iter_list_end = list_add.end();
//        --iter_list_end;
//        while (iter_list_add != iter_list_end) {
//            int vm_id = iter_list_add->second->vm_id;
//            string vm_str = iter_list_add->second->vm_type;
//            pair<int, int> judge_add = CreateVM(vm_id, vm_str,
//                vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                cpu_mem_server);
//            if (judge_add.first == -1) {
//                judge_add = CreateVM(vm_id, vm_str,
//                    vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                    empty_server);
//                if (judge_add.first == -1) {
//                    PurchaseServerNew(buy_server_type,
//                        server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes,
//                        cpu_sorted_server, cpu_mem_server);
//                    one_day_purchase[buy_server_type]++;
//                    vm_runs[vm_id].vm_id_ = vm_id;
//                    vm_runs[vm_id].vm_str_ = vm_str;
//                    if (vm_infos[vm_str].dual_node == 1) {
//                        vm_runs[vm_id].Add(server_number - 1, 2,
//                            vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        one_day_create_vm[iter_list_add->first] = make_pair(server_number - 1, 2);
//                    }
//                    else {
//                        vm_runs[vm_id].Add(server_number - 1, 0,
//                            vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        one_day_create_vm[iter_list_add->first] = make_pair(server_number - 1, 0);
//                    }
//                    ++iter_list_add;
//                }
//                else {
//                    one_day_create_vm[iter_list_add->first] = judge_add;
//                    ++iter_list_add;
//                }
//            }
//            else {
//                one_day_create_vm[iter_list_add->first] = judge_add;
//                ++iter_list_add;
//            }
//            if (iter_list_add == iter_list_end) {
//                break;
//            }
//            vm_id = iter_list_end->second->vm_id;
//            vm_str = iter_list_end->second->vm_type;
//            judge_add = rCreateVM(vm_id, vm_str,
//                vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                cpu_mem_server);
//            if (judge_add.first == -1) {
//                judge_add = CreateVM(vm_id, vm_str,
//                    vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                    empty_server);
//                if (judge_add.first == -1) {
//                    PurchaseServer(buy_server_type,
//                        server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes,
//                        cpu_sorted_server, cpu_mem_server);
//                    one_day_purchase[buy_server_type]++;
//                    vm_runs[vm_id].vm_id_ = vm_id;
//                    vm_runs[vm_id].vm_str_ = vm_str;
//                    if (vm_infos[vm_str].dual_node == 1) {
//                        vm_runs[vm_id].Add(server_number - 1, 2,
//                            vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        one_day_create_vm[iter_list_end->first] = make_pair(server_number - 1, 2);
//                    }
//                    else {
//                        vm_runs[vm_id].Add(server_number - 1, 0,
//                            vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                        one_day_create_vm[iter_list_end->first] = make_pair(server_number - 1, 0);
//                    }
//                    --iter_list_end;
//                }
//                else {
//                    one_day_create_vm[iter_list_end->first] = judge_add;
//                    --iter_list_end;
//                }
//            }
//            else {
//                one_day_create_vm[iter_list_end->first] = judge_add;
//                --iter_list_end;
//            }
//        }
//        int vm_id = iter_list_add->second->vm_id;
//        string vm_str = iter_list_add->second->vm_type;
//        pair<int, int> judge_add = CreateVM(vm_id, vm_str,
//            vm_infos, vm_runs, server_resources, server_runs, server_closes,
//            cpu_mem_server);
//        if (judge_add.first == -1) {
//            judge_add = CreateVM(vm_id, vm_str,
//                vm_infos, vm_runs, server_resources, server_runs, server_closes,
//                empty_server);
//            if (judge_add.first == -1) {
//                PurchaseServerNew(buy_server_type,
//                    server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes,
//                    cpu_sorted_server, cpu_mem_server);
//                one_day_purchase[buy_server_type]++;
//                vm_runs[vm_id].vm_id_ = vm_id;
//                vm_runs[vm_id].vm_str_ = vm_str;
//                if (vm_infos[vm_str].dual_node == 1) {
//                    vm_runs[vm_id].Add(server_number - 1, 2,
//                        vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    one_day_create_vm[iter_list_add->first] = make_pair(server_number - 1, 2);
//                }
//                else {
//                    vm_runs[vm_id].Add(server_number - 1, 0,
//                        vm_infos, vm_runs, server_resources, server_runs, server_closes);
//                    one_day_create_vm[iter_list_add->first] = make_pair(server_number - 1, 0);
//                }
//                ++iter_list_add;
//            }
//            else {
//                one_day_create_vm[iter_list_add->first] = judge_add;
//                ++iter_list_add;
//            }
//        }
//        else {
//            one_day_create_vm[iter_list_add->first] = judge_add;
//            ++iter_list_add;
//        }
//
//    }
//
//    for (auto iter_list_del = list_del.begin(); iter_list_del != list_del.end(); ++iter_list_del) {
//        vm_runs[(*iter_list_del)->vm_id].Del(
//            vm_infos, vm_runs, server_resources, server_runs, server_closes);
//    }
//    for (auto iter_empty = empty_server.begin(); iter_empty != empty_server.end(); ) {
//        if (!((*iter_empty)->get_node('a').cpu_used == 0 && (*iter_empty)->get_node('a').mem_used == 0
//            && (*iter_empty)->get_node('b').cpu_used == 0 && (*iter_empty)->get_node('b').mem_used == 0)) {
//            cpu_mem_server.push_back(*iter_empty);
//            iter_empty = empty_server.erase(iter_empty);
//        }
//        else {
//            ++iter_empty;
//        }
//    }
//    empty_server.splice(empty_server.begin(), temp_empty);
//    for (auto iter_cpu = cpu_mem_server.begin(); iter_cpu != cpu_mem_server.end();) {
//        if ((*iter_cpu)->get_node('a').cpu_used == 0 && (*iter_cpu)->get_node('a').mem_used == 0
//            && (*iter_cpu)->get_node('b').cpu_used == 0 && (*iter_cpu)->get_node('b').mem_used == 0) {
//            empty_server.push_back(*iter_cpu);
//            iter_cpu = cpu_mem_server.erase(iter_cpu);
//        }
//        else if (((*iter_cpu)->get_node('a').cpu_res == 0 || (*iter_cpu)->get_node('a').mem_res == 0)
//            && ((*iter_cpu)->get_node('b').cpu_res == 0 || (*iter_cpu)->get_node('b').mem_res == 0)) {
//            full_server.push_back(*iter_cpu);
//            iter_cpu = cpu_mem_server.erase(iter_cpu);
//        }
//        else {
//            ++iter_cpu;
//        }
//    }
//}

pair<int, int> NewCreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    vector<Server*>& result_server,
    int& length) {
    for (int result_index = 0; result_index < length; ++result_index) {
        if (result_server[result_index] == nullptr) {
            continue;
        }
        Node a = result_server[result_index]->get_node('a');
        Node b = result_server[result_index]->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add(result_index, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 2����˫�ڵ������
                return std::make_pair(result_index, 2);
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add(result_index, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 0��������A�ڵ�
                return std::make_pair(result_index, 0);
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add(result_index, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 1��������B�ڵ�
                return std::make_pair(result_index, 1);
            }
        }
    }
    return std::make_pair(-1, -1);
}

pair<int, int> rCreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server) {
    for (auto i = cpu_sorted_server.rbegin(); i != cpu_sorted_server.rend(); ++i) {
        Node a = (*i)->get_node('a');
        Node b = (*i)->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 2����˫�ڵ������
                return std::make_pair(vm_runs[vm_id].sv_id_, 2);
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 0��������A�ڵ�
                return std::make_pair(vm_runs[vm_id].sv_id_, 0);
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                vm_runs[vm_id].vm_id_ = vm_id;
                vm_runs[vm_id].vm_str_ = vm_str;
                vm_runs[vm_id].Add((*i)->ID_, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // 1��������B�ڵ�
                return std::make_pair(vm_runs[vm_id].sv_id_, 1);
            }
        }
    }
    return std::make_pair(-1, -1);
}


void DeployVmFull(int& vm_count, int& server_number,
    long long& BUYCOST, long long& TOTALCOST,
    vector<Request>& day_requests,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    int& remain_day,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server)
{
    pair<int, int> judge_purchase;              // �ж��Ƿ���Ҫ�������
    unordered_map<int, Request*> extra_add;     // �����·�������add����(˳���±�-����)
    unordered_map<int, Request*> extra_del_add; // �����·�������ɾ����add����
    unordered_map<int, int> judge_del;          // �����ж�del����û�н���add(vmid,index)
    int add_index = 0;                          // one_day_create_vm������±�
    //���з������ϵĲ���
    for (auto req = day_requests.begin(); req != day_requests.end(); ++req) {
        //
        if (req->op_type == ADD) {
            ++vm_count;
            judge_purchase = CreateVM(req->vm_id, req->vm_type,
                vm_infos, vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
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
            if (add_del != judge_del.end()) {//�����·�������ɾ����add����
                extra_del_add[add_del->second] = extra_add[add_del->second];
                extra_add.erase(add_del->second);
            }
            else {
                vm_runs[req->vm_id].Del(
                    vm_infos, vm_runs, server_resources, server_runs, server_closes);
            }
        }
    }
    //
    int mem_max = 0;
    int cpu_max = 0;                // ��������cpu��mem
    double mem_cpu_ratio = 0;               // extra_need��mem/cpu
    DayCaculate(mem_max, cpu_max, mem_cpu_ratio, extra_add,
        vm_infos);
    string buy_server_type = " ";           // Ҫ����ķ���������
    SelectPurchaseServerNew(buy_server_type, mem_max, cpu_max,remain_day, mem_cpu_ratio,
        server_infos);
    list<Server*> new_server;               //�·������б�
    //
    //�·������ϵ�add
    for (auto extra = extra_add.begin(); extra != extra_add.end(); ) {
        //
        judge_purchase = CreateVM(extra->second->vm_id, extra->second->vm_type,
            vm_infos, vm_runs, server_resources, server_runs, server_closes, new_server);
        //
        if (judge_purchase.second == -1) {  //�������
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //����ɹ�
            one_day_create_vm[extra->first] = judge_purchase;
            ++extra;
        }
    }
    //�·���������ɾ����add
    for (auto extra_da = extra_del_add.begin(); extra_da != extra_del_add.end(); ++extra_da) {
        judge_purchase = CreateVM(extra_da->second->vm_id, extra_da->second->vm_type,
            vm_infos, vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
        if (judge_purchase.second == -1) {  //�������
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //����ɹ�
            one_day_create_vm[extra_da->first] = judge_purchase;
            ++extra_da;
            vm_runs[extra_da->second->vm_id].Del(
                vm_infos, vm_runs, server_resources, server_runs, server_closes);

        }
    }
}




//pair<�����id��pair<������id���������ڵ�> >
vector<pair<int, pair<int, int> > > MigrateVMEmpty(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server,
    vector<Request>& day_requests)
{
    vector<pair<int, pair<int, int> > > result;
    int max_nums = int(floor(vm_count * 0.03));
    //��һ��������������
    list<Server*> wait_server;
    list<Server*> out_server;
    int out_count = 0;          //����out_server�����������
    auto sorted_iter = sorted_server.rbegin();
    while (sorted_iter != sorted_server.rend()) {
        out_count += (*sorted_iter)->own_vm.size();
        if (out_count > max_nums) {
            break;
        }
        out_server.push_front(*sorted_iter);
        ++sorted_iter;
    }
    int wait_num = sorted_server.size() - out_server.size();
    while (sorted_iter != sorted_server.rend()) {
        wait_server.push_front(*sorted_iter);
        ++sorted_iter;
    }
    //��1.5������del����
    list<int> del_out;
    for (auto req = day_requests.begin(); req != day_requests.end(); ++req) {
        if (req->op_type == DELETE) {
            if (vm_runs.find(req->vm_id) != vm_runs.end()) {
                if ((vm_runs[req->vm_id].sv_id_ < wait_num)) {
                    del_out.push_back(req->vm_id);
                }
            }
        }
    }
    //�٣������Ǩ���б�����(��Դ����)
    list<pair<int, int> > vm_output;
    //��ʼ��vm_output
    for (auto iter_out = out_server.begin(); iter_out != out_server.end(); ++iter_out) {
        for (auto iter_int = (*iter_out)->own_vm.begin(); iter_int != (*iter_out)->own_vm.end(); ++iter_int) {
            vm_output.push_front(make_pair(*iter_int, vm_infos[vm_runs[*iter_int].vm_str_].cpu + vm_infos[vm_runs[*iter_int].vm_str_].mem));
        }
    }
    //vm_output.sort([](pair<int, int> a, pair<int, int> b) {
    //    return a.second < b.second;
    //    });
        //�ڣ�ʵʩǨ��
    for (auto output_iter = vm_output.begin(); max_nums > 0 && output_iter != vm_output.end();) {
        //�������Ϣ
        int vm_id = (*output_iter).first;
        string vm_str = vm_runs[vm_id].vm_str_;
        int dual_node = vm_infos[vm_str].dual_node;

        //���ҵ��˺��ʵķ�������judge_find =1��
        int judge_find = 0;
        //��¼�ҵ���������id,�ͽڵ�
        int judge_id = -1;
        int judge_node = -2;
        //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
        auto iter_s = wait_server.begin();
        while (iter_s != wait_server.end()) {
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
            //��������ӷ�������ȥ��
            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
            VM vm(vm_id, vm_str);
            vm_runs[vm_id] = vm;
            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            --max_nums;
            result.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
            output_iter = vm_output.erase(output_iter);
        }
        else {
            ++output_iter;
        }
    }
    //���岽��del_outǨ��out_server
    for (auto iter_del = del_out.begin(); max_nums > 0 && iter_del != del_out.end(); ++iter_del) {
        //�������Ϣ
        int vm_id = (*iter_del);
        string vm_str = vm_runs[vm_id].vm_str_;
        int dual_node = vm_infos[vm_str].dual_node;

        //���ҵ��˺��ʵķ�������judge_find =1��
        int judge_find = 0;
        //��¼�ҵ���������id,�ͽڵ�
        int judge_id = -1;
        int judge_node = -2;
        //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
        auto iter_s = out_server.begin();
        while (iter_s != out_server.end()) {
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
            //��������ӷ�������ȥ��
            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
            VM vm(vm_id, vm_str);
            vm_runs[vm_id] = vm;
            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            --max_nums;
            result.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
        }
    }

    //�ڣ�ʵʩǨ��
    for (auto output_iter = vm_output.begin(); max_nums > 0 && output_iter != vm_output.end(); ++output_iter) {
        //�������Ϣ
        int vm_id = (*output_iter).first;
        string vm_str = vm_runs[vm_id].vm_str_;
        int dual_node = vm_infos[vm_str].dual_node;

        //���ҵ��˺��ʵķ�������judge_find =1��
        int judge_find = 0;
        //��¼�ҵ���������id,�ͽڵ�
        int judge_id = -1;
        int judge_node = -2;
        //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
        auto iter_s = wait_server.begin();
        while (iter_s != wait_server.end()) {
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
            //��������ӷ�������ȥ��
            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
            VM vm(vm_id, vm_str);
            vm_runs[vm_id] = vm;
            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            --max_nums;
            result.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
            if (((*iter_s)->get_node('a').cpu_res == 0 || (*iter_s)->get_node('a').mem_res == 0)
                && ((*iter_s)->get_node('b').cpu_res == 0 || (*iter_s)->get_node('b').mem_res == 0)) {
                wait_server.erase(iter_s);
            }
        }
    }

    ////���Ĳ���out_server����ǰǨ��

    //for (auto iter_r_s = out_server.rbegin(); max_nums > 0 && iter_r_s != out_server.rend(); ++iter_r_s) {
    //    list<int> iter_int;
    //    for (auto iter = (*iter_r_s)->own_vm.begin(); iter != (*iter_r_s)->own_vm.end(); ++iter) {
    //        iter_int.push_back(*iter);
    //    }
    //    auto iter_v = iter_int.begin();
    //    while (iter_v != iter_int.end() && max_nums > 0) {
    //        //--temp_num;
    //        int vm_id = *iter_v;
    //        string vm_str = vm_runs[*iter_v].vm_str_;
    //        int s_id = vm_runs[*iter_v].sv_id_;
    //        int s_node = vm_runs[*iter_v].sv_node_;
    //        //�ҵ����ʵķ�������Ϊ1
    //        int judge = 0;
    //        int fit_server_id;
    //        int fit_server_node;
    //        //�ҵ����ʵķ���������
    //        for (auto iter_s = out_server.begin(); iter_s != out_server.end(); ++iter_s) {
    //            if ((*iter_s)->ID_ == s_id && s_node == 2) {
    //                break;
    //            }
    //            Node a = (*iter_s)->get_node('a');
    //            Node b = (*iter_s)->get_node('b');
    //            if (vm_infos[vm_str].dual_node == 1) {
    //                if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
    //                    && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
    //                    fit_server_id = (*iter_s)->ID_;
    //                    fit_server_node = 2;
    //                    if (s_id != (*iter_s)->ID_) {
    //                        judge = 1;
    //                    }
    //                    break;
    //                }
    //            }
    //            else {
    //                if ((*iter_s)->ID_ == s_id && s_node == 0) {
    //                    break;
    //                }
    //                if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
    //                    fit_server_id = (*iter_s)->ID_;
    //                    fit_server_node = 0;
    //                    if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 0)) {
    //                        judge = 1;
    //                    }
    //                    break;
    //                }
    //                if ((*iter_s)->ID_ == s_id && s_node == 1) {
    //                    break;
    //                }
    //                if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
    //                    fit_server_id = (*iter_s)->ID_;
    //                    fit_server_node = 1;
    //                    if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 1)) {
    //                        judge = 1;
    //                    }
    //                    break;
    //                }
    //            }
    //        }
    //        if (judge == 1) {
    //            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
    //            VM vm(vm_id, vm_str);
    //            vm_runs[vm_id] = vm;
    //            vm_runs[vm_id].Add(fit_server_id, fit_server_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
    //            result.push_back(make_pair(vm_id, make_pair(fit_server_id, fit_server_node)));
    //            --max_nums;
    //        }
    //        ++iter_v;
    //    }
    //}

    return result;
}

//pair<�����id��pair<������id���������ڵ�> >
vector<pair<int, pair<int, int> > > MigrateVMMiddle(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server,
    vector<Request>& day_requests)
{
    vector<pair<int, pair<int, int> > > result;
    int max_nums = int(floor(vm_count * 0.03));
    //��һ��������������
    list<Server*> full_server;
    list<Server*> wait_server;
    list<Server*> out_server;
    int out_count = 0;          //����out_server�����������
    auto sorted_iter = sorted_server.rbegin();
    while (sorted_iter != sorted_server.rend()) {
        out_count += (*sorted_iter)->own_vm.size();
        if (out_count > max_nums) {
            break;
        }
        out_server.push_front(*sorted_iter);
        ++sorted_iter;
    }
    int wait_num = sorted_server.size() - out_server.size();
    //��1.5������del����
    list<int> del_out;
    for (auto req = day_requests.begin(); req != day_requests.end(); ++req) {
        if (req->op_type == DELETE) {
            if (vm_runs.find(req->vm_id) != vm_runs.end()) {
                if ((vm_runs[req->vm_id].sv_id_ < wait_num)) {
                    del_out.push_back(req->vm_id);
                }
            }
        }
    }
    //����full_server
    while (sorted_iter != sorted_server.rend()) {
        if (((*sorted_iter)->get_node('a').cpu_res == 0 || (*sorted_iter)->get_node('a').mem_res == 0) &&
            ((*sorted_iter)->get_node('b').cpu_res == 0 || (*sorted_iter)->get_node('b').mem_res == 0)) {
            full_server.push_front(*sorted_iter);
        }
        else {
            wait_server.push_front(*sorted_iter);
        }
        ++sorted_iter;
    }
    //�ڶ������ͷ�full_server
    for (auto iter_full = full_server.begin(); iter_full != full_server.end() && max_nums > 0; ++iter_full) {
        double a_cpu_res = (*iter_full)->get_node('a').cpu_res;
        double a_mem_res = (*iter_full)->get_node('a').mem_res;
        double b_cpu_res = (*iter_full)->get_node('b').cpu_res;
        double b_mem_res = (*iter_full)->get_node('b').mem_res;
        int judge_find = 0;
        //Ǩ��˫�ڵ�
        if (((a_cpu_res - a_mem_res) > 40 && (b_cpu_res - b_mem_res) > 40) ||
            ((a_mem_res - a_cpu_res) > 40 && (b_mem_res - b_cpu_res) > 40)) {
            double judge_dual = abs(log((a_cpu_res + b_cpu_res) / (a_mem_res + b_mem_res)));
            //�������Ϣ
            int vm_id = -1;
            for (auto dual_vm = (*iter_full)->cown_vm.begin(); dual_vm != (*iter_full)->cown_vm.end(); ++dual_vm) {
                double temp_dual = abs(log((a_cpu_res + b_cpu_res + vm_infos[vm_runs[*dual_vm].vm_str_].cpu) / (a_mem_res + b_mem_res + vm_infos[vm_runs[*dual_vm].vm_str_].mem)));
                if (temp_dual < judge_dual) {
                    judge_dual = temp_dual;
                    vm_id = *dual_vm;
                }
            }
            if (vm_id != -1) {
                //��¼�ҵ���������id,�ͽڵ�
                int judge_id = -1;
                string vm_str = vm_runs[vm_id].vm_str_;
                //�Ż��㣺��Ǩ�ƺ���������ô��***********************************************************************************
                for (auto iter_wait = wait_server.begin(); iter_wait != wait_server.end(); ++iter_wait) {
                    Node a = (*iter_wait)->get_node('a');
                    Node b = (*iter_wait)->get_node('b');
                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                        judge_id = (*iter_wait)->ID_;
                        judge_find = 1;
                        break;
                    }
                }
                if (judge_find == 1) {
                    //��������ӷ�������ȥ��
                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    vm_runs[vm_id].vm_id_ = vm_id;
                    vm_runs[vm_id].vm_str_ = vm_str;
                    vm_runs[vm_id].Add(judge_id, 2, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    --max_nums;
                    result.push_back(make_pair(vm_id, make_pair(judge_id, 2)));
                }
            }
        }
        //Ǩ��A�ڵ�
        if (judge_find == 0 && abs(a_cpu_res - a_mem_res) > 40) {
            double judge_dual = abs(log(a_cpu_res / a_mem_res));
            //�������Ϣ
            int vm_id = -1;
            for (auto a_vm = (*iter_full)->aown_vm.begin(); a_vm != (*iter_full)->aown_vm.end(); ++a_vm) {
                double temp_dual = abs(log((a_cpu_res + vm_infos[vm_runs[*a_vm].vm_str_].cpu) / (a_mem_res + vm_infos[vm_runs[*a_vm].vm_str_].mem)));
                if (temp_dual < judge_dual) {
                    judge_dual = temp_dual;
                    vm_id = *a_vm;
                }
            }
            if (vm_id != -1) {
                //��¼�ҵ���������id,�ͽڵ�
                int judge_id = -1;
                int judge_node = -1;
                string vm_str = vm_runs[vm_id].vm_str_;
                for (auto iter_wait = wait_server.begin(); iter_wait != wait_server.end(); ++iter_wait) {
                    Node a = (*iter_wait)->get_node('a');
                    Node b = (*iter_wait)->get_node('b');
                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                        judge_id = (*iter_wait)->ID_;
                        judge_node = 0;
                        judge_find = 1;
                        break;

                    }
                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                        judge_id = (*iter_wait)->ID_;
                        judge_node = 1;
                        judge_find = 1;
                        break;
                    }
                }
                if (judge_find == 1) {
                    //��������ӷ�������ȥ��
                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    vm_runs[vm_id].vm_id_ = vm_id;
                    vm_runs[vm_id].vm_str_ = vm_str;
                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    --max_nums;
                    result.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
                }
            }
        }
        //Ǩ��B�ڵ�
        if (judge_find == 0 && abs(b_cpu_res - b_mem_res) > 40) {
            double judge_dual = abs(log(b_cpu_res / b_mem_res));
            //�������Ϣ
            int vm_id = -1;
            for (auto b_vm = (*iter_full)->bown_vm.begin(); b_vm != (*iter_full)->bown_vm.end(); ++b_vm) {
                double temp_dual = abs(log((b_cpu_res + vm_infos[vm_runs[*b_vm].vm_str_].cpu) / (b_mem_res + vm_infos[vm_runs[*b_vm].vm_str_].mem)));
                if (temp_dual < judge_dual) {
                    judge_dual = temp_dual;
                    vm_id = *b_vm;
                }
            }
            if (vm_id != -1) {
                //��¼�ҵ���������id,�ͽڵ�
                int judge_id = -1;
                int judge_node = -1;
                string vm_str = vm_runs[vm_id].vm_str_;
                for (auto iter_wait = wait_server.begin(); iter_wait != wait_server.end(); ++iter_wait) {
                    Node a = (*iter_wait)->get_node('a');
                    Node b = (*iter_wait)->get_node('b');
                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                        judge_id = (*iter_wait)->ID_;
                        judge_node = 0;
                        judge_find = 1;
                        break;

                    }
                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                        judge_id = (*iter_wait)->ID_;
                        judge_node = 1;
                        judge_find = 1;
                        break;
                    }
                }
                if (judge_find == 1) {
                    //��������ӷ�������ȥ��
                    vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    vm_runs[vm_id].vm_id_ = vm_id;
                    vm_runs[vm_id].vm_str_ = vm_str;
                    vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                    --max_nums;
                    result.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
                }
            }
        }
    }
    //��������Ǩ��out_server��wait_server
    //�٣������Ǩ���б�����(��Դ����)
    list<pair<int, int> > vm_output;
    //��ʼ��vm_output
    for (auto iter_out = out_server.begin(); iter_out != out_server.end(); ++iter_out) {
        for (auto iter_int = (*iter_out)->own_vm.begin(); iter_int != (*iter_out)->own_vm.end(); ++iter_int) {
            vm_output.push_front(make_pair(*iter_int, vm_infos[vm_runs[*iter_int].vm_str_].cpu + vm_infos[vm_runs[*iter_int].vm_str_].mem));
        }
    }
    //vm_output.sort([](pair<int, int> a, pair<int, int> b) {
    //    return a.second < b.second;
    //    });

    //���岽��del_outǨ��out_server
    for (auto iter_del = del_out.begin(); max_nums > 0 && iter_del != del_out.end(); ++iter_del) {
        //�������Ϣ
        int vm_id = (*iter_del);
        string vm_str = vm_runs[vm_id].vm_str_;
        int dual_node = vm_infos[vm_str].dual_node;

        //���ҵ��˺��ʵķ�������judge_find =1��
        int judge_find = 0;
        //��¼�ҵ���������id,�ͽڵ�
        int judge_id = -1;
        int judge_node = -2;
        //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
        auto iter_s = out_server.begin();
        while (iter_s != out_server.end()) {
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
            //��������ӷ�������ȥ��
            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
            VM vm(vm_id, vm_str);
            vm_runs[vm_id] = vm;
            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            --max_nums;
            result.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
        }
    }

    //�ڣ�ʵʩǨ��
    for (auto output_iter = vm_output.begin(); max_nums > 0 && output_iter != vm_output.end(); ++output_iter) {
        //�������Ϣ
        int vm_id = (*output_iter).first;
        string vm_str = vm_runs[vm_id].vm_str_;
        int dual_node = vm_infos[vm_str].dual_node;

        //���ҵ��˺��ʵķ�������judge_find =1��
        int judge_find = 0;
        //��¼�ҵ���������id,�ͽڵ�
        int judge_id = -1;
        int judge_node = -2;
        //�ӵ�һ�������Ŀ�ʼ�ҵ����ʵķ�����
        auto iter_s = wait_server.begin();
        while (iter_s != wait_server.end()) {
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
            //��������ӷ�������ȥ��
            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
            VM vm(vm_id, vm_str);
            vm_runs[vm_id] = vm;
            vm_runs[vm_id].Add(judge_id, judge_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
            --max_nums;
            result.push_back(make_pair(vm_id, make_pair(judge_id, judge_node)));
            if (((*iter_s)->get_node('a').cpu_res == 0 || (*iter_s)->get_node('a').mem_res == 0)
                && ((*iter_s)->get_node('b').cpu_res == 0 || (*iter_s)->get_node('b').mem_res == 0)) {
                wait_server.erase(iter_s);
            }
        }
    }

    ////���Ĳ���out_server����ǰǨ��

    //for (auto iter_r_s = out_server.rbegin(); max_nums > 0 && iter_r_s != out_server.rend(); ++iter_r_s) {
    //    list<int> iter_int;
    //    for (auto iter = (*iter_r_s)->own_vm.begin(); iter != (*iter_r_s)->own_vm.end(); ++iter) {
    //        iter_int.push_back(*iter);
    //    }
    //    auto iter_v = iter_int.begin();
    //    while (iter_v != iter_int.end() && max_nums > 0) {
    //        //--temp_num;
    //        int vm_id = *iter_v;
    //        string vm_str = vm_runs[*iter_v].vm_str_;
    //        int s_id = vm_runs[*iter_v].sv_id_;
    //        int s_node = vm_runs[*iter_v].sv_node_;
    //        //�ҵ����ʵķ�������Ϊ1
    //        int judge = 0;
    //        int fit_server_id;
    //        int fit_server_node;
    //        //�ҵ����ʵķ���������
    //        for (auto iter_s = out_server.begin(); iter_s != out_server.end(); ++iter_s) {
    //            if ((*iter_s)->ID_ == s_id && s_node == 2) {
    //                break;
    //            }
    //            Node a = (*iter_s)->get_node('a');
    //            Node b = (*iter_s)->get_node('b');
    //            if (vm_infos[vm_str].dual_node == 1) {
    //                if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
    //                    && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
    //                    fit_server_id = (*iter_s)->ID_;
    //                    fit_server_node = 2;
    //                    if (s_id != (*iter_s)->ID_) {
    //                        judge = 1;
    //                    }
    //                    break;
    //                }
    //            }
    //            else {
    //                if ((*iter_s)->ID_ == s_id && s_node == 0) {
    //                    break;
    //                }
    //                if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
    //                    fit_server_id = (*iter_s)->ID_;
    //                    fit_server_node = 0;
    //                    if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 0)) {
    //                        judge = 1;
    //                    }
    //                    break;
    //                }
    //                if ((*iter_s)->ID_ == s_id && s_node == 1) {
    //                    break;
    //                }
    //                if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
    //                    fit_server_id = (*iter_s)->ID_;
    //                    fit_server_node = 1;
    //                    if (s_id != (*iter_s)->ID_ || (s_id == (*iter_s)->ID_ && s_node != 1)) {
    //                        judge = 1;
    //                    }
    //                    break;
    //                }
    //            }
    //        }
    //        if (judge == 1) {
    //            vm_runs[vm_id].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
    //            VM vm(vm_id, vm_str);
    //            vm_runs[vm_id] = vm;
    //            vm_runs[vm_id].Add(fit_server_id, fit_server_node, vm_infos, vm_runs, server_resources, server_runs, server_closes);
    //            result.push_back(make_pair(vm_id, make_pair(fit_server_id, fit_server_node)));
    //            --max_nums;
    //        }
    //        ++iter_v;
    //    }
    //}

    return result;
}

// ��Middle(add��del�������)ʱ�Ĳ������
void DeployVmMiddle(int& vm_count, int& server_number,
    long long& BUYCOST, long long& TOTALCOST,
    vector<Request>& day_requests,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    int& remain_day,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server)
{
    /*1. ����������*/
    list<Server*> full_servers;  // ��
    int small_threshold = 20;
    list<Server*> small_servers;  // �����ڵ�cpu_res < 20
    list<Server*> medium_servers;  // 20<cpu_res<100
    int big_threshold = 100;
    list<Server*> big_servers;  // 100<cpu_res

    for (auto& sorted_it : cpu_sorted_server)
    {
        int a_cpu_res = sorted_it->get_node('a').cpu_res;
        int a_mem_res = sorted_it->get_node('a').mem_res;
        int b_cpu_res = sorted_it->get_node('b').cpu_res;
        int b_mem_res = sorted_it->get_node('b').mem_res;
        if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {
            full_servers.push_back(&(*sorted_it));
        }
        else if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {
            small_servers.push_back(&(*sorted_it));
        }
        else if (a_cpu_res > big_threshold || b_cpu_res > big_threshold) {
            big_servers.push_back(&(*sorted_it));
        }
        else {
            medium_servers.push_back(&(*sorted_it));
        }
    }

    pair<int, int> judge_purchase;              // �ж��Ƿ���Ҫ�������
    unordered_map<int, Request*> extra_add;     // �����·�������add����(˳���±�-����)
    unordered_map<int, Request*> extra_del_add; // �����·�������ɾ����add����
    unordered_map<int, int> judge_del;          // �����ж�del����û�н���add(vmid,index)
    int add_index = 0;                          // one_day_create_vm������±�
    //���з������ϵĲ���
    for (auto req = day_requests.begin(); req != day_requests.end(); ++req) {
        /*2. ����ADD��������ֳ�3��*/
        if (req->op_type == ADD) {
            ++vm_count;
            VMTYPE type;
            if (vm_infos[req->vm_type].dual_node == 1) {
                if (vm_infos[req->vm_type].cpu / 2 < small_threshold) type = SMALL;
                else if (vm_infos[req->vm_type].cpu / 2 > big_threshold) type = BIG;
                else type = MEDIUM;
            }
            else {
                if (vm_infos[req->vm_type].cpu < small_threshold) type = SMALL;
                else if (vm_infos[req->vm_type].cpu > big_threshold) type = BIG;
                else type = MEDIUM;
            }
            /*3. ���շ���ȷ������˳��*/
            Server* deployed_server;  // ָ�򱻲���ķ�����
            // SMALL�����
            if (type == SMALL) {
                // 1��small_servers
                judge_purchase = CreateVM(req->vm_id, req->vm_type,
                    vm_infos, vm_runs, server_resources, server_runs, server_closes, small_servers, deployed_server);
                if (judge_purchase.first != -1) {
                    int a_cpu_res = server_resources[judge_purchase.first].get_node('a').cpu_res;
                    int a_mem_res = server_resources[judge_purchase.first].get_node('a').mem_res;
                    int b_cpu_res = server_resources[judge_purchase.first].get_node('b').cpu_res;
                    int b_mem_res = server_resources[judge_purchase.first].get_node('b').mem_res;
                    // small_servers�ܱ���
                    if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {
                        small_servers.remove(deployed_server);
                        full_servers.push_back(deployed_server);
                    }
                }
                else {
                    // 2��medium_servers
                    judge_purchase = CreateVM(req->vm_id, req->vm_type,
                        vm_infos, vm_runs, server_resources, server_runs, server_closes, medium_servers, deployed_server);
                    if (judge_purchase.first != -1) {
                        int a_cpu_res = server_resources[judge_purchase.first].get_node('a').cpu_res;
                        int a_mem_res = server_resources[judge_purchase.first].get_node('a').mem_res;
                        int b_cpu_res = server_resources[judge_purchase.first].get_node('b').cpu_res;
                        int b_mem_res = server_resources[judge_purchase.first].get_node('b').mem_res;
                        // medium_servers�ܱ�small
                        if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {
                            medium_servers.remove(deployed_server);
                            small_servers.push_back(deployed_server);
                        }
                        // medium_servers�ܱ�full
                        else if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {
                            medium_servers.remove(deployed_server);
                            full_servers.push_back(deployed_server);
                        }
                    }
                    else {
                        // 3��big_servers
                        judge_purchase = CreateVM(req->vm_id, req->vm_type,
                            vm_infos, vm_runs, server_resources, server_runs, server_closes, big_servers, deployed_server);
                        if (judge_purchase.first != -1) {
                            int a_cpu_res = server_resources[judge_purchase.first].get_node('a').cpu_res;
                            int a_mem_res = server_resources[judge_purchase.first].get_node('a').mem_res;
                            int b_cpu_res = server_resources[judge_purchase.first].get_node('b').cpu_res;
                            int b_mem_res = server_resources[judge_purchase.first].get_node('b').mem_res;
                            // big_servers�ܱ�medium
                            if (small_threshold < a_cpu_res < big_threshold ||
                                small_threshold < b_cpu_res < big_threshold) {
                                big_servers.remove(deployed_server);
                                medium_servers.push_back(deployed_server);
                            }
                            // big_servers�ܱ�small
                            else if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {
                                big_servers.remove(deployed_server);
                                small_servers.push_back(deployed_server);
                            }
                            // big_servers�ܱ�full
                            else if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {
                                big_servers.remove(deployed_server);
                                full_servers.push_back(deployed_server);
                            }
                        }
                        else {
                            // ȷʵװ����
                            extra_add[add_index] = &(*req);
                            judge_del[req->vm_id] = add_index;
                        }
                    }
                }
                one_day_create_vm.push_back(judge_purchase);
                ++add_index;
            }
            else if (type == MEDIUM) {
                // 1��medium_servers
                judge_purchase = CreateVM(req->vm_id, req->vm_type,
                    vm_infos, vm_runs, server_resources, server_runs, server_closes, medium_servers, deployed_server);
                if (judge_purchase.first != -1) {
                    int a_cpu_res = server_resources[judge_purchase.first].get_node('a').cpu_res;
                    int a_mem_res = server_resources[judge_purchase.first].get_node('a').mem_res;
                    int b_cpu_res = server_resources[judge_purchase.first].get_node('b').cpu_res;
                    int b_mem_res = server_resources[judge_purchase.first].get_node('b').mem_res;
                    // medium_servers�ܱ�small
                    if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {
                        medium_servers.remove(deployed_server);
                        small_servers.push_back(deployed_server);
                    }
                    // medium_servers�ܱ�full
                    else if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {
                        medium_servers.remove(deployed_server);
                        full_servers.push_back(deployed_server);
                    }
                }
                else {
                    // 2��big_servers
                    judge_purchase = CreateVM(req->vm_id, req->vm_type,
                        vm_infos, vm_runs, server_resources, server_runs, server_closes, big_servers, deployed_server);
                    if (judge_purchase.first != -1) {
                        int a_cpu_res = server_resources[judge_purchase.first].get_node('a').cpu_res;
                        int a_mem_res = server_resources[judge_purchase.first].get_node('a').mem_res;
                        int b_cpu_res = server_resources[judge_purchase.first].get_node('b').cpu_res;
                        int b_mem_res = server_resources[judge_purchase.first].get_node('b').mem_res;
                        // big_servers�ܱ�medium
                        if (small_threshold < a_cpu_res < big_threshold ||
                            small_threshold < b_cpu_res < big_threshold) {
                            big_servers.remove(deployed_server);
                            medium_servers.push_back(deployed_server);
                        }
                        // big_servers�ܱ�small
                        else if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {
                            big_servers.remove(deployed_server);
                            small_servers.push_back(deployed_server);
                        }
                        // big_servers�ܱ�full
                        else if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {
                            big_servers.remove(deployed_server);
                            full_servers.push_back(deployed_server);
                        }
                    }
                    else {
                        // ȷʵװ����
                        extra_add[add_index] = &(*req);
                        judge_del[req->vm_id] = add_index;
                    }
                }
                one_day_create_vm.push_back(judge_purchase);
                ++add_index;
            }
            // type == BIG
            else {
                // 1��big_servers
                judge_purchase = CreateVM(req->vm_id, req->vm_type,
                    vm_infos, vm_runs, server_resources, server_runs, server_closes, big_servers, deployed_server);
                if (judge_purchase.first != -1) {
                    int a_cpu_res = server_resources[judge_purchase.first].get_node('a').cpu_res;
                    int a_mem_res = server_resources[judge_purchase.first].get_node('a').mem_res;
                    int b_cpu_res = server_resources[judge_purchase.first].get_node('b').cpu_res;
                    int b_mem_res = server_resources[judge_purchase.first].get_node('b').mem_res;
                    // big_servers�ܱ�medium
                    if (small_threshold < a_cpu_res < big_threshold ||
                        small_threshold < b_cpu_res < big_threshold) {
                        big_servers.remove(deployed_server);
                        medium_servers.push_back(deployed_server);
                    }
                    // big_servers�ܱ�small
                    else if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {
                        big_servers.remove(deployed_server);
                        small_servers.push_back(deployed_server);
                    }
                    // big_servers�ܱ�full
                    else if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {
                        big_servers.remove(deployed_server);
                        full_servers.push_back(deployed_server);
                    }
                }
                else {
                    // ȷʵװ����
                    extra_add[add_index] = &(*req);
                    judge_del[req->vm_id] = add_index;
                }
                one_day_create_vm.push_back(judge_purchase);
                ++add_index;
            }
        }
        /*3. ����DEL*/
        else {
            --vm_count;
            auto add_del = judge_del.find(req->vm_id);
            if (add_del != judge_del.end()) {//�����·�������ɾ����add����
                extra_del_add[add_del->second] = extra_add[add_del->second];
                extra_add.erase(add_del->second);
            }
            else {// ���漰�·���������ͨdel
                int sv_id = vm_runs[req->vm_id].sv_id_;
                Server* del_server = &server_resources[sv_id];  // Ҫִ��ɾ���ķ�����
                // �ж�ɾ��ǰ����
                int a_cpu_res = (*del_server).get_node('a').cpu_res;
                int a_mem_res = (*del_server).get_node('a').mem_res;
                int b_cpu_res = (*del_server).get_node('b').cpu_res;
                int b_mem_res = (*del_server).get_node('b').mem_res;
                if ((a_cpu_res == 0 || a_mem_res == 0) && (b_cpu_res == 0 || b_mem_res == 0)) {// FULL
                    full_servers.remove(del_server);
                }
                else if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {// SMALL
                    small_servers.remove(del_server);
                }
                else if (a_cpu_res > big_threshold || b_cpu_res > big_threshold) {// BIG
                    big_servers.remove(del_server);
                }
                else {// MEDIUM
                    medium_servers.remove(del_server);
                }
                // ִ��ɾ��
                vm_runs[req->vm_id].Del(
                    vm_infos, vm_runs, server_resources, server_runs, server_closes);
                // �ж�ɾ��������
                a_cpu_res = (*del_server).get_node('a').cpu_res;
                a_mem_res = (*del_server).get_node('a').mem_res;
                b_cpu_res = (*del_server).get_node('b').cpu_res;
                b_mem_res = (*del_server).get_node('b').mem_res;
                if (a_cpu_res < small_threshold && b_cpu_res < small_threshold) {// SMALL
                    small_servers.push_back(del_server);
                }
                else if (a_cpu_res > big_threshold || b_cpu_res > big_threshold) {// BIG
                    big_servers.push_back(del_server);
                }
                else {// MEDIUM
                    medium_servers.push_back(del_server);
                }
            }
        }
    }
    //
    /*4. �����·������ϵ�ADD��DEL*/
    int mem_max = 0;
    int cpu_max = 0;                // ��������cpu��mem
    double mem_cpu_ratio = 0;               // extra_need��mem/cpu
    DayCaculate(mem_max, cpu_max, mem_cpu_ratio, extra_add,
        vm_infos);
    string buy_server_type = " ";           // Ҫ����ķ���������
    SelectPurchaseServerNew(buy_server_type, mem_max, cpu_max, remain_day, mem_cpu_ratio,
        server_infos);
    list<Server*> new_server;               //�·������б�
    //
    //�·������ϵ�add
    for (auto extra = extra_add.begin(); extra != extra_add.end(); ) {
        //
        judge_purchase = CreateVM(extra->second->vm_id, extra->second->vm_type,
            vm_infos, vm_runs, server_resources, server_runs, server_closes, new_server);
        //
        if (judge_purchase.second == -1) {  //�������
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //����ɹ�
            one_day_create_vm[extra->first] = judge_purchase;
            ++extra;
        }
    }
    //�·���������ɾ����add
    for (auto extra_da = extra_del_add.begin(); extra_da != extra_del_add.end(); ++extra_da) {
        judge_purchase = CreateVM(extra_da->second->vm_id, extra_da->second->vm_type,
            vm_infos, vm_runs, server_resources, server_runs, server_closes, cpu_sorted_server);
        if (judge_purchase.second == -1) {  //�������
            PurchaseServer(buy_server_type,
                server_number, BUYCOST, TOTALCOST, server_infos, server_resources, server_closes, cpu_sorted_server);
            one_day_purchase[buy_server_type]++;
            new_server.push_back(*cpu_sorted_server.rbegin());
        }
        else {                              //����ɹ�
            one_day_create_vm[extra_da->first] = judge_purchase;
            ++extra_da;
            vm_runs[extra_da->second->vm_id].Del(
                vm_infos, vm_runs, server_resources, server_runs, server_closes);

        }
    }
}