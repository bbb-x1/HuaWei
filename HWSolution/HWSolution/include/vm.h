#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include "server.h"
using namespace std;

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