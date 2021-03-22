#pragma once
#include <iostream>

#include <string>
#include<queue>
#include <unordered_map>
#include "server.h"
#include "vm.h"
#include "request.h"
using namespace std;

/**
 * @brief ����delimiters�ָ��ַ���
 * @param s Դ�ַ���
 * @param tokens �ָ���ַ�������
 * @param delimiters �ָ��/�ָ��ַ���
*/
void Split(const string& s, vector<string>& tokens, const string& delimiters = " ");

/**
 * @brief
 * @param s Դ�ַ���
 * @param brace �����ַ���
 * @return �������ַ���
*/
string Trim(string s);

/**
 * @brief ��ʼ������
 * @param server_info ��������Ϣ����,����Ϊunordered_map<�������ͺ�,������ʵ��>
 * @param vm_infos �������Ϣ����,����Ϊunordered_map<������ͺ�,�����ʵ��>
 * @param requests_set �������м���,����Ϊunordered_map<����,������������>
*/
void InitializeData(unordered_map<string,ServerInfo>& server_infos,unordered_map<string,VMInfo>& vm_infos,
	vector<vector<Request>>& requests_set, const string file_path);

/**
 * @brief ͳ��������Ϣ
 * @param vm_infos ���������
 * @param requests_set ���󼯺�
*/
double StatisticInfo(unordered_map<string, VMInfo>& vm_infos, vector<vector<Request>>& requests_set);


// ���������������Ϣ��
void PrintVMInfos(unordered_map<string, VMInfo>& vm_infos);

// ���������������Ϣ��
void PrintServerInfos(unordered_map<string, ServerInfo>& server_infos);