#include "../include/virtualMachine.h"

VM::VM() {}

VM::VM(string vM_str, int vM_id) {
    vMStr = vM_str;
    vMID = vM_id;
}

void VM::pCreatRequest(){
    if(svNode == -1){
        cout<<'('<<svID<<')'<<endl;
    }else{
        cout<<'('<<svID<<','<<'A'+svNode<<')'<<endl;
    }

}


void VM::pMigration(){
    if(svNode == -1){
        cout<<'('<<vMID<<','<<svID<<')'<<endl;
    }else{
        cout<<'('<<vMID<<','<<svID<<','<< 'A' + svNode <<')'<<endl;
    }
}


void VM::add(int sv_id, Server target,VirtualMachineInfo vm_info, int svNode_){
    svStr = target.type_;
    svID = sv_id;
    svNode = svNode_;

    if (vm_info.dual_node == 0) {
        target.IncreaseUse(vm_info.cpu, vm_info.mem, svNode +'A');
    }
    else {
        target.IncreaseUse(vm_info.cpu, vm_info.mem, 'A');
        target.IncreaseUse(vm_info.cpu, vm_info.mem, 'B');
    }
}


void VM::del(Server target, VirtualMachineInfo vm_info) {
    if (vm_info.dual_node == 0) {
        target.DecreaseUse(vm_info.cpu, vm_info.mem, svNode + 'A');
    }
    else {
        target.DecreaseUse(vm_info.cpu, vm_info.mem, 'A');
        target.DecreaseUse(vm_info.cpu, vm_info.mem, 'B');
    }
}


void CreateVM(string vm_str, int vm_id, Server& target, unordered_map<int, VM> &vm_resources, int server_number, unordered_map<int, Server> &server_resources, unordered_map<string, VirtualMachineInfo> &vm_infos) {
    VM vm(vm_str, vm_id);
    vm_resources[vm_id] = vm;
    int i;
    for (i = 0; i < server_number; ++i) {
        if (vm_infos[vm_str].dual_node == 1){
            if (server_resources[i].a->cpu_res >= vm_infos[vm_str].cpu &&
                server_resources[i].a->mem_res >= vm_infos[vm_str].mem &&
                server_resources[i].b->cpu_res >= vm_infos[vm_str].cpu &&
                server_resources[i].b->mem_res >= vm_infos[vm_str].mem) {
                vm.add(i, server_resources[i], vm_infos[vm_str]);
                break;
            }
        }
        else {
            if (server_resources[i].a->cpu_res >= vm_infos[vm_str].cpu &&
                server_resources[i].a->mem_res >= vm_infos[vm_str].mem) {
                vm.add(i, server_resources[i], vm_infos[vm_str], 0);
                break;
            }
            else if (server_resources[i].b->cpu_res >= vm_infos[vm_str].cpu &&
                server_resources[i].b->mem_res >= vm_infos[vm_str].mem) {
                vm.add(i, server_resources[i], vm_infos[vm_str], 1);
                break;
            }
        }
    }

    if (i == server_number) {
        cout << "fail" << endl;
    }
}

void DeleteVM(int vm_id, unordered_map<int, VM>& vm_resources, unordered_map<int, Server> server_resources, unordered_map<string, VirtualMachineInfo> vm_infos) {
    vm_resources[vm_id].del(server_resources[vm_resources[vm_id].svID], vm_infos[vm_resources[vm_id].vMStr]);
    vm_resources.erase(vm_id);
}