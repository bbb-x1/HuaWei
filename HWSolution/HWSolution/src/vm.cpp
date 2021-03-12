#include "../include/vm.h"


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


//void VM::add(string svStr_, int svID_, Server target,VirtualMachineInfo vm_info, int svNode_){
//    svStr = svStr_;
//    svID = svID_;
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