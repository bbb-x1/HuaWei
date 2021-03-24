#pragma once
#include <iostream>
#include <string>
#include "server.h"
using namespace std;

// 1�������������Ϣ
typedef struct VirtualMachineInfo {
    int cpu = 0;            //CPU����
    int mem = 0;         //�ڴ��С
    int dual_node;       //�Ƿ�˫�ڵ㲿��
}VMInfo;


class VM{

public:
    int vm_id_;       //�����ID
    string vm_str_;     //������ͺ�

    int sv_id_;       //���ڷ�����ID
    int sv_node_  = 0;     //���ڷ������ڵ㣬=0ʱ��A�ڵ㣬=1ʱ��B�ڵ�, =-1ʱ˫��㲿��
   
    VM();
    VM(int vm_id, string vm_str);

    //��������ӵ���������sv_id����sv_node�ڵ�
    void Add(int sv_id, int sv_node,
        unordered_map<string, VMInfo>& vm_infos,
        unordered_map<int, Server>& server_resources, 
        unordered_map<int, Server*>& server_runs,
        unordered_map<int, Server*>& server_closes);   
    
    
    void Del(
        unordered_map<string, VMInfo>& vm_infos,
        unordered_map<int, VM>& vm_runs,
        unordered_map<int, Server>& server_resources,
        unordered_map<int, Server*>& server_runs,
        unordered_map<int, Server*>& server_closes);

    //void PCreatRequest();   //�����������Ĳ������
    //void PMigration();      //����������Ǩ�����
};


//pair<int, int> CreateVM(int vm_id, string vm_str,
//    unordered_map<string, VMInfo>& vm_infos,
//    unordered_map<int, VM>& vm_runs,
//    unordered_map<int, Server>& server_resources,
//    unordered_map<int, Server*>& server_runs,
//    unordered_map<int, Server*>& server_closes);

vector<pair<int, int>> CreateVM(int vm_id, string vm_str,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes);

//�������vmǨ�Ƶ���������sv_id����sv_node�ڵ�
void MigrationVM(VM vm, int sv_id, int sv_node,
    unordered_map<string, VMInfo>& vm_infos,
    unordered_map<int, VM>& vm_runs,
    unordered_map<int, Server>& server_resources,
    unordered_map<int, Server*>& server_runs,
    unordered_map<int, Server*>& server_closes);