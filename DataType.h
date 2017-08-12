#pragma once

namespace DataType
{
	typedef struct USER_DATA
	{
		// 用户数据
		// <数据库字段名，字段值>
		USER_DATA();
		// ID
		std::pair<CString, CString> _paID;
		// 会员姓名
		std::pair<CString, CString> _paName;
		// 注册时间
		std::pair<CString, CString> _paRegtime;
		// 联系电话
		std::pair<CString, CString> _paPhone1;
		// 备用电话
		std::pair<CString, CString> _paPhone2;
		// 会员类型  // ! 关于会员类型的思考 可能既有次数 又有余额
		std::pair<CString, CString> _paType;
		// 会员积分
		std::pair<CString, CString> _paScore;
		// 次数余额
		std::pair<CString, CString> _paBalanceCount;
		// 现金余额
		std::pair<CString, CString> _paBalanceMoney;
		// 宝宝姓名
		std::pair<CString, CString> _paBabyName;
		// 宝宝性别
		std::pair<CString, CString> _paBabySex;
		// 宝宝年龄
		std::pair<CString, CString> _paBabyAge;
		// 备注
		std::pair<CString, CString> _paRemark;

		CString _getdb_key() const;
		CString _getdb_val() const;
	} user_data;

	// 商品数据
	typedef struct GOODS_DATA
	{
		// <数据库字段名，字段值>
		GOODS_DATA();
		bool operator==(GOODS_DATA& gd);
		// ID
		std::pair<CString, CString> _paID;
		// 商品码
		std::pair<CString, CString> _paCodeNumber;
		// 商品类型
		std::pair<CString, CString> _paType;
		// 商品名称
		std::pair<CString, CString> _paTitle;
		// 商品售价
		std::pair<CString, CString> _paPrice;
		// 商品进价
		std::pair<CString, CString> _paIntoPrice;
		// 库存数量
		std::pair<CString, CString> _paTotal;
		// 商品信息
		std::pair<CString, CString> _paInfo;

		CString _getdb_key() const;
		CString _getdb_val() const;
	} goods_data;

	// 工单数据
	typedef struct FLOWLIST_DATA
	{
		FLOWLIST_DATA();
		// 单号
		std::pair<CString, CString> _paID;
		// 消费账户
		std::pair<CString, CString> _paPayUser;
		// 操作起始时间
		std::pair<CString, CString> _paPayTime;
		// 总计收入
		std::pair<CString, CString> _paTotal;
		// 支付类型
		std::pair<CString, CString> _paPayType;
		// 是否发票
		std::pair<CString, CString> _paReceipt;
		// 发票号码
		std::pair<CString, CString> _paReceiptNum;
		// 备注
		std::pair<CString, CString> _paRemark;
		// 商品清单 <商品,数量>
		std::vector<std::pair<goods_data, double>> _vecList;

	} flowlist_data;

	// manager_main数据
	typedef struct FLOW_IDX_DATA
	{
		FLOW_IDX_DATA();
		// 单号
		std::pair<CString, CString> _paID;
		// 状态
		std::pair<CString, CString> _paStatus;
		// 消费账户
		std::pair<CString, CString> _paPayType;
		// 操作开始日期
		std::pair<CString, CString> _paTime;
		// 收入
		std::pair<CString, CString> _paValue;
		// 发票
		std::pair<CString, CString> _paReceipt;
		// 发票号
		std::pair<CString, CString> _paReceiptNum;
		// 备注
		std::pair<CString, CString> _paRemark;

	} flow_idx_data;

	// manager_main数据
	typedef struct FLOW_MAIN_DATA 
	{
		FLOW_MAIN_DATA();
		// 单号
		std::pair<CString, CString> _paFlowID;
		// 状态
		std::pair<CString, CString> _paStatus;
		// 消费账户
		std::pair<CString, CString> _paPayType;
		// 操作开始日期
		std::pair<CString, CString> _paTime;
		// 消费结束日期
		std::pair<CString, CString> _paEndTime;
		// 应付金额
		std::pair<CString, CString> _paValue;
		// 折扣
		std::pair<CString, CString> _paSale;
		// 数量
		std::pair<CString, CString> _paCount;
		// 实收
		std::pair<CString, CString> _paTotal;
		// 金额类型(现金刷卡等)
		std::pair<CString, CString> _paValueType;

	} flow_main_data;

	// manager_goods数据
	typedef struct FLOW_GOODS_DATA
	{
		FLOW_GOODS_DATA();
		// 单号
		std::pair<CString, CString> _paFlowID;
		// 状态
		std::pair<CString, CString> _paStatus;
		// 消费账户
		std::pair<CString, CString> _paPayType;
		// 操作开始日期
		std::pair<CString, CString> _paTime;
		// 商品名称
		std::pair<CString, CString> _paTitle;
		// 应付金额
		std::pair<CString, CString> _paValue;
		// 商品码
		std::pair<CString, CString> _paCodeNum;
		// 折扣
		std::pair<CString, CString> _paSale;
		// 数量
		std::pair<CString, CString> _paCount;
		// 实收
		std::pair<CString, CString> _paTotal;
		// 金额类型(现金刷卡等)
		std::pair<CString, CString> _paValueType;
	} flow_goods_data;

	// 用户全局设置结构
	typedef struct CUSTOM_GSET
	{
		CUSTOM_GSET();
		BOOL _bShowBaby;
		BOOL _bShowType;
		BOOL _bShowTime;
		BOOL _bShowScort;
		BOOL _bShowBalance;

		CString _sQfindPhone;
	} custom_gSet;

	// 工作流全局设置结构
	typedef struct FLOW_GSET
	{
		FLOW_GSET();
		BOOL _bComplite;
		BOOL _bRuning;
	} flow_gSet;

	typedef struct GOODS_GSET
	{
		GOODS_GSET();
		BOOL _bShowIntoPre;	// 显示进价
	} goods_gSet;
}
