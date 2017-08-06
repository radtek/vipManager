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
	// 添加一个用户
	bool cusm_add_new_user(const USER_DATA& ud);
	// 删除一个用户 (通过ID)
	bool cusm_delete_user(const USER_DATA& ud);
	// 查找用户
	bool cusm_find_user(const USER_DATA& ud,std::vector<USER_DATA>& vecFindUd);
};

