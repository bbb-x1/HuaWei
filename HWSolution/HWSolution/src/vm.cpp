#include "../include/vm.h"

VM::VM() {}

VM::VM(int vm_id, string vm_str) {
    vm_id_ = vm_id;
    vm_str_ = vm_str;

}

void VM::PCreatRequest(){
    if(svNode == -1){
        cout<<'('<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<sv_id_<<','<<'A'+svNode<<')'<<endl;
    }
}

void VM::PMigration(){
    if(svNode == -1){
        cout<<'('<<vm_id_<<','<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<vm_id_<<','<<sv_id_<<','<< 'A' + svNode <<')'<<endl;
    }
}


//void VM::add(int sv_id){
//    sv_id_ = sv_id_;
//    svNode = svNode_;
//
//    if (vm_info.dual_node == 0) {
//        target.IncreaseUse(vm_info.cpu, vm_info.mem, svNode +'A');
//    }
//    else {
//        target.IncreaseUse(vm_info.cpu, vm_info.mem, 'A');
//        target.IncreaseUse(vm_info.cpu, vm_info.mem, 'B');
//    }
//}
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

