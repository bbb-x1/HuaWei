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
    int vMID;       //�����ID

    int sv_id_;       //���ڷ�����ID
    int svNode = -1;     //���ڷ������ڵ㣬=0ʱ��A�ڵ㣬=1ʱ��B�ڵ�, =-1ʱ˫��㲿��

    
    //void add(int sv_id);   //��������ӵ�������
    //void del(Server target, VirtualMachineInfo vm_info);
    //void pCreatRequest();   //�����������Ĳ������
    //void pMigration();      //����������Ǩ�����
};



