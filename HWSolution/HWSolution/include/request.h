#pragma once
#include<string>
typedef enum OPTYPE
{
	ADD,
	DELETE
}OPTYPE;
typedef struct Request
{
	OPTYPE op_type;
	std::string vm_type;
	int vm_id;
}Request;
