#pragma once
class GA
{
public:
	int sum = 20;//总的染色体数量
	int LOOP = 2000;//最大循环次数
	int crossp = 0.7;//交叉概率
	int mp = 0.04;//变异概率
public:
	static void initiate();
	static void evaluate();
	static void cross();
	static void select();

};

