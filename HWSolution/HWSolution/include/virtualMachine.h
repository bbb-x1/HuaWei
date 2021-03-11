#include<string>

using namespace std;

// 1条服务器基本信息
typedef struct VirtualMachineInfo{
    int CPU = 0;            //CPU核数
    int Memory = 0;         //内存大小
    int dualNode;       //是否双节点部署
}


class VM{
private:
    string svStr;   //所在服务器型号
    int svID;       //所在服务器ID
    int svNode = -1;     //所在服务器节点，=0时在A节点，=1时在B节点, =-1时双结点部署

public:

    void add(string svStr_, int svID, int svNode = -1);   //将虚拟机加到服务器

    void pCreatRequest();   //输出创建请求的部署情况
    void pMigration();      //输出该虚拟机迁移情况

};
