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
	bool cusm_add_new_user(const CString &strID,const USER_DATA& ud);
	// 修改用户信息
	bool cusm_edit_user(const USER_DATA& nud, const USER_DATA& oud);
	// 删除一个用户 (通过ID)
	bool cusm_delete_user(const USER_DATA& ud);
	// 查找用户
	bool cusm_find_user(const USER_DATA& ud,std::vector<USER_DATA>& vecFindUd);



	// 获取下一个工作流单号
	bool manger_get_last_flow(CString &strID);
	// 下单
	bool manger_order_list(const flowlist_data& fld);

	// 查找商品
	bool manger_find_goods(const GOODS_DATA& gd, std::vector<GOODS_DATA>& vecFindGd);

	// 根据单号 获取基本信息、普通消费信息、商品消费信息
	bool proper_get_idx(const flow_idx_data& findData, std::vector<flow_idx_data>& vecResData);
	bool proper_get_main(const flow_main_data &findData, std::vector<flow_main_data>& vecResData);
	bool proper_get_goods(const flow_goods_data &findData, std::vector<flow_goods_data>& vecResData);
};

