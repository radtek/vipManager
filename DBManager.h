#pragma once

#include "MysqlManager.h"
#include "DataType.h"
using namespace DataType;

class CDBManager
{
public:
	CDBManager();
	~CDBManager();

private:
	MysqlManager* m_pDBM;

public:
	// 初始化链接
	bool init();
	// 获取下一个注册用户ID
	bool cusm_get_last_id(CString &strID);
	// 添加用户
	bool cusm_add_new_user(const USER_DATA& ud);
};

