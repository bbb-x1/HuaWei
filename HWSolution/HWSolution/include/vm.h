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

// 1�������������Ϣ
typedef struct VirtualMachineInfo {
    int cpu = 0;            //CPU����
    int mem = 0;         //�ڴ��С
    int dual_node = -1;       //�Ƿ�˫�ڵ㲿�� 0�ǵ��ڵ� 1��˫�ڵ�
}VMInfo;

class VM{

public:
    int vm_id_ = -1;       //�����ID
    string vm_str_ = " ";     //������ͺ�

    int sv_id_ = -1;       //���ڷ�����ID
    int sv_node_ = -1;     //���ڷ������ڵ㣬=0ʱ��A�ڵ㣬=1ʱ��B�ڵ�, =2ʱ˫��㲿��
   
    VM();
    VM(int vm_id, string vm_str);

    //��������ӵ���������sv_id����sv_node�ڵ�
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

// ���أ��������з��ز������̨��������ָ��
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


// ���������id������1̨�����
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

//pair<�����id��pair<������id���������ڵ�> >
vector<pair<int, pair<int, int> > > MigrateVMFull(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server);

//pair<�����id��pair<������id���������ڵ�> >
vector<pair<int, pair<int, int> > > MigrateVMEmpty(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server,
    vector<Request>& day_requests);

//pair<�����id��pair<������id���������ڵ�> >
vector<pair<int, pair<int, int> > > MigrateVMMiddle(int vm_count,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes,
    list<Server*>& sorted_server,
    vector<Request>& day_requests);


// ��Middle(add��del�������)ʱ�Ĳ������
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