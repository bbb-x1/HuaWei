#include "../include/vm.h"


void VM::PCreatRequest(){
    if(svNode == -1){
        cout<<'('<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<sv_id_<<','<<'A'+svNode<<')'<<endl;
    }
}

void VM::PMigration(){
    if(svNode == -1){
        cout<<'('<<vMID<<','<<sv_id_<<')'<<endl;
    }else{
        cout<<'('<<vMID<<','<<sv_id_<<','<< 'A' + svNode <<')'<<endl;
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

