#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <cmath>
#include "server.h"
#include "request.h"
using namespace std;

typedef enum VMTYPE
{
    SMALL,
    MEDIUM,
    BIG
}VMTYPE;

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

// 重载，从引用中返回插入的那台服务器的指针
pair<int, int> CreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server,
    Server*& deployed_server);


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

void DayCaculate(int& mem_max, int& cpu_max, double& mem_cpu_ratio,
    unordered_map<int, Request*>& extra_need,
    unordered_map<string, VMInfo>& vm_infos);

void DeployVm(int& vm_count, int& server_number,
    long long& BUYCOST, long long& TOTALCOST,
    vector<Request>& day_requests,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server,
    list<Server*>& cpu_mem_server);

void DeployVmAdd(int& vm_count, int& server_number,
    long long& BUYCOST, long long& TOTALCOST, int& actual_vm_count,
    vector<Request>& day_requests,
    list<pair<int, Request*> >& list_del,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    vector<pair<int, pair<int, int> > >& one_day_migrate_vm,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server,
    int& max_cpu, int& max_mem, int& remain_day, pair<int, int>record,
    string& buy_server_type);


void DeployVmDel(int& vm_count, int& server_number,
    long long& BUYCOST, long long& TOTALCOST, int& actual_vm_count,
    list<pair<int, Request*> > list_add,
    list<Request*> list_del,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    vector<pair<int, pair<int, int> > > one_day_migrate_vm,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server,
    list<Server*>& full_server,
    list<Server*>& cpu_mem_server,
    list<Server*>& empty_server,
    string& buy_server_type);

pair<int, int> NewCreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    vector<Server*>& result_server,
    int& length);

pair<int, int> rCreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server);


void DeployVmFull(int& vm_count, int& server_number,
    long long& BUYCOST, long long& TOTALCOST,
    vector<Request>& day_requests,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    int& remain_day,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server);

//pair<虚拟机id，pair<服务器id，服务器节点> >
vector<pair<int, pair<int, int> > > MigrateVMFull(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server);

//pair<虚拟机id，pair<服务器id，服务器节点> >
vector<pair<int, pair<int, int> > > MigrateVMEmpty(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server,
    vector<Request>& day_requests);

//pair<虚拟机id，pair<服务器id，服务器节点> >
vector<pair<int, pair<int, int> > > MigrateVMMiddle(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server,
    vector<Request>& day_requests);


// 在Middle(add和del数量差不多)时的部署策略
void DeployVmMiddle(int& vm_count, int& server_number,
    long long& BUYCOST, long long& TOTALCOST,
    vector<Request>& day_requests,
    unordered_map<string, int>& one_day_purchase,
    vector<pair<int, int>>& one_day_create_vm,
    int& remain_day,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<string, ServerInfo>& server_infos,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& cpu_sorted_server);