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
    //方向第一个不空的服务器
    auto iter_re_empty = cpu_sorted_server.rbegin();
    while (iter_re_empty != cpu_sorted_server.rend()) {

        if ((*iter_re_empty)->get_node('a').cpu_res != 0 || (*iter_re_empty)->get_node('b').cpu_res != 0) {
            break;
        }
        ++iter_re_empty;
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
    for (auto iter_r_s = iter_re_empty; mig_nums < max_nums && iter_r_s != iter_re_full; ++iter_r_s) {

        ////服务器无虚拟机或服务器满，则下一台
        //if ( (*iter_r_s)->own_vm.empty()
        //    ||( (*iter_r_s)->get_node('a').cpu_res==0 && (*iter_r_s)->get_node('b').cpu_res == 0)
        //    ||( (*iter_r_s)->get_node('a').mem_res == 0 && (*iter_r_s)->get_node('b').mem_res == 0)){
        //    continue;
        //}
        if ((*iter_r_s)->get_node('a').cpu_res < 10 && (*iter_r_s)->get_node('b').cpu_res < 10
            || (*iter_r_s)->get_node('a').mem_res < 10 && (*iter_r_s)->get_node('b').mem_res < 10) {
            continue;
        }

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
            for (auto iter_s = iter_full; iter_s != cpu_sorted_server.end(); ++iter_s) {
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
    return result;
}