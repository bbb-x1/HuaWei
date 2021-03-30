#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include "server.h"
using namespace std;

// 1条虚拟机基本信息
typedef struct VirtualMachineInfo {
    int cpu = 0;            //CPU核数
    int mem = 0;         //内存大小
    int dual_node = -1;       //是否双节点部署 0是单节点 1是双节点
}VMInfo;

class VM{

public:
    int vm_id_ = -1;       //虚拟机ID
    string vm_str_ = " ";     //虚拟机型号

    int sv_id_ = -1;       //所在服务器ID
    int sv_node_ = -1;     //所在服务器节点，=0时在A节点，=1时在B节点, =2时双结点部署
   
    VM();
    VM(int vm_id, string vm_str);

    //将虚拟机加到服务器（sv_id）的sv_node节点
    void Add(int sv_id, int sv_node,
        unordered_map<string, VMInfo>& vm_infos,
        unordered_map<int, VM>& vm_runs,
        unordered_map<int, Server>& server_resources, 
        unordered_map<int, Server*>& server_runs,
        unordered_map<int, Server*>& server_closes);   
    
    
        void Del(
        unordered_map<string, VMInfo>& vm_infos,
        unordered_map<int, VM>& vm_runs,
        unordered_map<int, Server>& server_resources,
        unordered_map<int, Server*>& server_runs,
        unordered_map<int, Server*>& server_closes);
};



pair<int, int> CreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server);

vector<pair<int, pair<int, int> > > MigrateVM(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server);


// 输入服务器id，创建1台虚拟机
pair<int, int> CreateOneVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    int sv_id,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes);