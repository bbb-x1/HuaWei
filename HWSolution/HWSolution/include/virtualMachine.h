#include<string>

using namespace std;

// 1��������������Ϣ
typedef struct VirtualMachineInfo{
    int CPU = 0;            //CPU����
    int Memory = 0;         //�ڴ��С
    int dualNode;       //�Ƿ�˫�ڵ㲿��
}


class VM{
private:
    string svStr;   //���ڷ������ͺ�
    int svID;       //���ڷ�����ID
    int svNode = -1;     //���ڷ������ڵ㣬=0ʱ��A�ڵ㣬=1ʱ��B�ڵ�, =-1ʱ˫��㲿��

public:

    void add(string svStr_, int svID, int svNode = -1);   //��������ӵ�������

    void pCreatRequest();   //�����������Ĳ������
    void pMigration();      //����������Ǩ�����

};
