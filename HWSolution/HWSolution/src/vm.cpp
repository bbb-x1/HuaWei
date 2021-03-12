#include "../include/vm.h"

VM::VM() {}

VM::VM(int vm_id, string vm_str) {
    vm_id_ = vm_id;
    vm_str_ = vm_str;

}

void VM::PCreatRequest(){
    if(sv_node == -1){
        cout<<'('<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<sv_id_<<','<<'A'+sv_node<<')'<<endl;
    }
}

void VM::PMigration(){
    if(sv_node == -1){
        cout<<'('<<vm_id_<<','<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<vm_id_<<','<<sv_id_<<','<< 'A' + sv_node <<')'<<endl;
    }
}


void VM::add(int sv_id, int sv_node,
    unordered_map<int, Server>& server_resources, 
    unordered_map<string, VirtualMachineInfo>& vm_infos,
    unordered_map<int, Server>& server_runs,
    unordered_map<int, Server>& server_closes){

    sv_id_ = sv_id_;
    sv_node_ = sv_node;
    if (vm_infos[vm_str_].dual_node == 1) {
        server_resources[sv_id].IncreaseUse(vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2,'A',
            server_runs, server_closes);
        server_resources[sv_id].IncreaseUse(vm_infos[vm_str_].cpu / 2, vm_infos[vm_str_].mem / 2, 'B',
            server_runs, server_closes);
    }
}
//
//
//void VM::del(Server target, VirtualMachineInfo vm_info) {
//    if (vm_info.dual_node == 0) {
//        target.DecreaseUse(vm_info.cpu, vm_info.mem, svNode + 'A');
//    }
//    else {
//        target.DecreaseUse(vm_info.cpu, vm_info.mem, 'A');
//        target.DecreaseUse(vm_info.cpu, vm_info.mem, 'B');
//    }
//}

