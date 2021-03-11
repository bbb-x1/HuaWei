#include "../include/virtualMachine.h"
#include <iostream>
#include "../include/server.h"



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


void VM::add(string svStr_, int svID_, Server target,VirtualMachineInfo vm_info, int svNode_){
    svStr = svStr_;
    svID = svID_;
    svNode = svNode_;

    target.IncreaseUse(vm_info.cpu, vm_info.mem, vm_info.cpu + 'A');
    

}
