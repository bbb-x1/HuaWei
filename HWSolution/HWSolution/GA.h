#pragma once
class GA
{
public:
	int sum = 20;//�ܵ�Ⱦɫ������
	int LOOP = 2000;//���ѭ������
	int crossp = 0.7;//�������
	int mp = 0.04;//�������
public:
	static void initiate();
	static void evaluate();
	static void cross();
	static void select();

};

