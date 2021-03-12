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
    int vm_id_;       //虚拟机ID
    string vm_str_;     //虚拟机型号

    int sv_id_;       //所在服务器ID
    int sv_node_ = -1;     //所在服务器节点，=0时在A节点，=1时在B节点, =-1时双结点部署
   
    VM();
    VM(int vm_id, string vm_str);

    void add(int sv_id, int sv_node,
        unordered_map<int, Server>& server_resources, 
        unordered_map<string, VirtualMachineInfo>& vm_infos,
        unordered_map<int, Server>& server_runs,
        unordered_map<int, Server>& server_closes);   //将虚拟机加到服务器
    //void del(Server target, VirtualMachineInfo vm_info);

    void PCreatRequest();   //输出创建请求的部署情况
    void PMigration();      //输出该虚拟机迁移情况
};



