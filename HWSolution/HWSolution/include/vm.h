#pragma once
#include <iostream>
#include <string>
#include "server.h"
using namespace std;

// 1条虚拟机基本信息
typedef struct VirtualMachineInfo {
    int cpu = 0;            //CPU核数
    int mem = 0;         //内存大小
    int dual_node;       //是否双节点部署
}VMInfo;


class VM{

public:
    int vMID;       //虚拟机ID

    int sv_id_;       //所在服务器ID
    int svNode = -1;     //所在服务器节点，=0时在A节点，=1时在B节点, =-1时双结点部署

    
    //void add(int sv_id);   //将虚拟机加到服务器
    //void del(Server target, VirtualMachineInfo vm_info);
    //void pCreatRequest();   //输出创建请求的部署情况
    //void pMigration();      //输出该虚拟机迁移情况
};



