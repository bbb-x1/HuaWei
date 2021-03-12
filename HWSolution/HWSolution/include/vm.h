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
private:
    int vMID;       //虚拟机ID

    string svStr;   //所在服务器型号
    int svID;       //所在服务器ID
    int svNode = -1;     //所在服务器节点，=0时在A节点，=1时在B节点, =-1时双结点部署

public:
    void add(string svStr_, int svID, Server target,VirtualMachineInfo vm_info, int svNode = -1);   //将虚拟机加到服务器
    void del(Server target, VirtualMachineInfo vm_info);

    void pCreatRequest();   //输出创建请求的部署情况
    void pMigration();      //输出该虚拟机迁移情况
};
