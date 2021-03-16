#include "../include/vm.h"

VM::VM() {}

VM::VM(int vm_id, string vm_str) {
    vm_id_ = vm_id;
    vm_str_ = vm_str;

}

void VM::PCreatRequest(){
    if(sv_node_ == -1){
        cout<<'('<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<sv_id_<<','<<'A'+sv_node_<<')'<<endl;
    }
}

void VM::PMigration(){
    if(sv_node_ == -1){
        cout<<'('<<vm_id_<<','<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<vm_id_<<','<<sv_id_<<','<< 'A' + sv_node_ <<')'<<endl;
    }
}


void VM::Add(int sv_id, int sv_node,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, Server>& server_resources, 
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes){

    sv_id_ = sv_id;
    sv_node_ = sv_node;
    if (sv_node == -1) {
        server_resources[sv_id].IncreaseUse(vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2,'a',
            server_runs, server_closes);
        server_resources[sv_id].IncreaseUse(vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2, 'b',
            server_runs, server_closes);
    }
    else if (sv_node == 0) {
        server_resources[sv_id].IncreaseUse(vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'a',
            server_runs, server_closes);
    }
    else if (sv_node == 1) {
        server_resources[sv_id].IncreaseUse(vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'b',
            server_runs, server_closes);
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
        server_resources[sv_id_].DecreaseUse(vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2, 'a',
            server_runs, server_closes);
        server_resources[sv_id_].DecreaseUse(vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2, 'b',
            server_runs, server_closes);
    }
    else if(sv_node_ == 0){
        server_resources[sv_id_].DecreaseUse(vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'a',
            server_runs, server_closes);
    }
    else{//sv_node_==1
        server_resources[sv_id_].DecreaseUse(vm_infos[vm_str_].cpu, vm_infos[vm_str_].mem, 'b',
            server_runs, server_closes);
    }

    vm_runs.erase(vm_id_);
}

void CreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes) {

    VM vm(vm_id, vm_str);
    vm_runs[vm_id] = vm;
    int judge = 0;
    for (auto i = server_runs.begin();i != server_runs.end(); ++i) {
        Node a = (*i).second->get_node('a');
        Node b = (*i).second->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                vm.Add((*i).first, -1, vm_infos, server_resources, server_runs, server_closes);
                judge = 1;
                cout <<'('<< vm.sv_id_<< ')' << endl;
                break;
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                vm.Add((*i).first, 0, vm_infos, server_resources,  server_runs, server_closes);
                judge = 1;
                cout << '(' << vm.sv_id_ <<",A"<< ')' << endl;
                break;
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                vm.Add((*i).first, 1, vm_infos, server_resources,  server_runs, server_closes);
                judge = 1;

                cout << '(' << vm.sv_id_ << ",B" << ')' << endl;
                break;
            }
        }
    }
    if (judge == 1) {
        return;
    }
    for (auto i = server_closes.begin(); i != server_closes.end(); ++i) {
        Node a = (*i).second->get_node('a');
        Node b = (*i).second->get_node('b');
        if (vm_infos[vm_str].dual_node == 1) {
            if (a.cpu_res >= vm_infos[vm_str].cpu / 2 && a.mem_res >= vm_infos[vm_str].mem / 2
                && b.cpu_res >= vm_infos[vm_str].cpu / 2 && b.mem_res >= vm_infos[vm_str].mem / 2) {
                vm.Add((*i).first, -1, vm_infos, server_resources,  server_runs, server_closes);
                judge = 1;
                cout << '(' << vm.sv_id_ << ')' << endl;
                break;
            }
        }
        else {
            if (a.cpu_res >= vm_infos[vm_str].cpu && a.mem_res >= vm_infos[vm_str].mem) {
                vm.Add((*i).first, 0, vm_infos, server_resources,  server_runs, server_closes);
                judge = 1;
                cout << '(' << vm.sv_id_ << ",A" << ')' << endl;
                break;
            }
            if (b.cpu_res >= vm_infos[vm_str].cpu && b.mem_res >= vm_infos[vm_str].mem) {
                vm.Add((*i).first, 1, vm_infos, server_resources,  server_runs, server_closes);
                judge = 1;
                judge = 1;
                cout << '(' << vm.sv_id_ << ",B" << ')' << endl;
                break;
            }
        }
    }

    if (judge == 0) {
        cout << "Error:CreatVM" << endl;
    }

}

void MigrationVM(VM vm, int sv_id, int sv_node,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes) {

    vm.Del(vm_infos, vm_runs, server_resources, server_runs, server_closes);
    vm.Add(sv_id, sv_node, vm_infos, server_resources, server_runs, server_closes);
    vm_runs[vm.vm_id_] = vm;
}
