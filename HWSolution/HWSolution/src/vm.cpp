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

//void VM::PCreatRequest(){
//    if(sv_node_ == -1){
//        cout<<'('<<sv_id_<<')'<<endl;
//    }else{
//        cout<<'('<<sv_id_<<','<<'A'+sv_node_<<')'<<endl;
//    }
//}
//
//void VM::PMigration(){
//    if(sv_node_ == -1){
//        cout<<'('<<vm_id_<<','<<sv_id_<<')'<<endl;
//    }else{
//        cout<<'('<<vm_id_<<','<<sv_id_<<','<< 'A' + sv_node_ <<')'<<endl;
//    }
//}


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

//void CreateVM(int vm_id, string vm_str,
//    unordered_map<string, VMInfo>& vm_infos,
//    unordered_map<int, VM>& vm_runs,
//    unordered_map<int, Server>& server_resources,
//    unordered_map<int, Server*>& server_runs,
//    unordered_map<int, Server*>& server_closes) {
//
//    VM vm(vm_id, vm_str);
//    vm_runs[vm_id] = vm;
//    int judge = 0;
//    for (auto i = server_runs.begin();i != server_runs.end(); ++i) {
//        Node a = (*i).second->get_node('a');
//        Node b = (*i).second->get_node('b');
//        if (vm_infos[vm_str].dual_node == 1) {
//            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                vm.Add((*i).first, -1, vm_infos, server_resources, server_runs, server_closes);
//                judge = 1;
//                cout <<'('<< vm.sv_id_<< ')' << endl;
//                break;
//            }
//        }
//        else {
//            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                vm.Add((*i).first, 0, vm_infos, server_resources,  server_runs, server_closes);
//                judge = 1;
//                cout << '(' << vm.sv_id_ <<",A"<< ')' << endl;
//                break;
//            }
//            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                vm.Add((*i).first, 1, vm_infos, server_resources,  server_runs, server_closes);
//                judge = 1;
//
//                cout << '(' << vm.sv_id_ << ",B" << ')' << endl;
//                break;
//            }
//        }
//    }
//    if (judge == 1) {
//        return;
//    }
//    for (auto i = server_closes.begin(); i != server_closes.end(); ++i) {
//        Node a = (*i).second->get_node('a');
//        Node b = (*i).second->get_node('b');
//        if (vm_infos[vm_str].dual_node == 1) {
//            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
//                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
//                vm.Add((*i).first, -1, vm_infos, server_resources,  server_runs, server_closes);
//                judge = 1;
//                cout << '(' << vm.sv_id_ << ')' << endl;
//                break;
//            }
//        }
//        else {
//            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
//                vm.Add((*i).first, 0, vm_infos, server_resources,  server_runs, server_closes);
//                judge = 1;
//                cout << '(' << vm.sv_id_ << ",A" << ')' << endl;
//                break;
//            }
//            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
//                vm.Add((*i).first, 1, vm_infos, server_resources,  server_runs, server_closes);
//                judge = 1;
//                judge = 1;
//                cout << '(' << vm.sv_id_ << ",B" << ')' << endl;
//                break;
//            }
//        }
//    }
//
//    if (judge == 0) {
//        cout << "Error:CreatVM" << endl;
//    }
//
//}


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
    //for (auto i = server_closes.begin(); i != server_closes.end(); ++i) {
    //    Node a = (*i).second->get_node('a');
    //    Node b = (*i).second->get_node('b');
    //    if (vm_infos[vm_str].dual_node == 1) {
    //        if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
    //            && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
    //            vm_runs[vm_id].Add((*i).first, -1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
    //            judge = 1;
    //            // 0代表双节点虚拟机
    //            return std::make_pair(vm_runs[vm_id].sv_id_, 0);
    //        }
    //    }
    //    else {
    //        if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
    //            vm_runs[vm_id].Add((*i).first, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
    //            judge = 1;
    //            // 1代表存放在A节点
    //            return std::make_pair(vm_runs[vm_id].sv_id_, 1);
    //        }
    //        if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
    //            vm_runs[vm_id].Add((*i).first, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
    //            judge = 1;
    //            // 2代表存放在B节点
    //            return std::make_pair(vm_runs[vm_id].sv_id_, 2);
    //        }
    //    }
    //}

    //if (judge == 0) {
    //    // -1代表服务器不够了
    return std::make_pair(-1, -1);
    //}
}

//pair<虚拟机id，pair<服务器id，服务器节点> >
vector<pair<int, pair<int, int> > > MigrateVM(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server) {

    int max_nums = int(floor(vm_count * 0.005));
    vector<pair<int, pair<int, int> > > result;
    int mig_nums = 0;
    //反向第一个满的服务器
    auto iter_re_full = cpu_sorted_server.rbegin();
    while (iter_re_full != cpu_sorted_server.rend()) {

        if ((*iter_re_full)->get_node('a').cpu_res == 0 && (*iter_re_full)->get_node('b').cpu_res == 0) {
            break;
        }
        ++iter_re_full;
    }
    //正向第一个不满的服务器
    auto iter_full = cpu_sorted_server.begin();
    while (iter_full != cpu_sorted_server.end()) {

        if ((*iter_full)->get_node('a').cpu_res != 0 || (*iter_full)->get_node('b').cpu_res != 0) {
            break;
        }
        ++iter_full;
    }




    //从利用率低的服务器开始迁移
    for (auto iter_r_s = cpu_sorted_server.rbegin(); mig_nums < max_nums && iter_r_s != iter_re_full; ++iter_r_s) {

        //服务器无虚拟机，则下一台
        if ((*iter_r_s)->own_vm.empty()) {
            continue;
        }

        ////当前服务器的虚拟机数量
        //int temp_num = (*iter_r_s)->own_vm.size();
        list<int> iter_int;
        for (auto iter = (*iter_r_s)->own_vm.begin(); iter != (*iter_r_s)->own_vm.end(); ++iter) {
            iter_int.push_back(*iter);
        }
        auto iter_v = iter_int.begin();


        while (iter_v != iter_int.end() && mig_nums < max_nums) {
            //--temp_num;

            int vm_id = vm_runs[*iter_v].vm_id_;
            string vm_str = vm_runs[*iter_v].vm_str_;
            int s_id = vm_runs[*iter_v].sv_id_;
            int s_node = vm_runs[*iter_v].sv_node_;

            //找到合适的服务器则为1
            int judge = 0;

            //将虚拟机从服务器中去除
            vm_runs[*iter_v].Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);

            VM vm(vm_id, vm_str);
            vm_runs[vm_id] = vm;

            //找到合适的服务器插入
            for (auto iter_s = iter_full; iter_s != cpu_sorted_server.end(); ++iter_s) {
                Node a = (*iter_s)->get_node('a');
                Node b = (*iter_s)->get_node('b');
                if (vm_infos[vm_str].dual_node == 1) {
                    if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                        && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                        vm_runs[vm_id].Add((*iter_s)->ID_, -1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                        if (s_id != vm_runs[vm_id].sv_id_) {
                            result.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
                            ++mig_nums;
                        }
                        break;
                    }
                }
                else {
                    if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                        vm_runs[vm_id].Add((*iter_s)->ID_, 0, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                        if (s_id != vm_runs[vm_id].sv_id_ || (s_id == vm_runs[vm_id].sv_id_ && s_node != vm_runs[vm_id].sv_node_)) {
                            result.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
                            ++mig_nums;
                        }
                        break;

                    }
                    if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                        vm_runs[vm_id].Add((*iter_s)->ID_, 1, vm_infos, vm_runs, server_resources, server_runs, server_closes);
                        if (s_id != vm_runs[vm_id].sv_id_ || (s_id == vm_runs[vm_id].sv_id_ && s_node != vm_runs[vm_id].sv_node_)) {
                            result.push_back(make_pair(vm_id, make_pair(vm_runs[vm_id].sv_id_, vm_runs[vm_id].sv_node_)));
                            ++mig_nums;
                        }
                        break;
                    }
                }
            }

            ++iter_v;
        }
    }
    return result;
}