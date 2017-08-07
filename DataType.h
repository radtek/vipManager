#pragma once

namespace DataType
{
	struct USER_DATA
	{
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
	};

	struct GOODS_DATA 
	{
		// <数据库字段名，字段值>
		GOODS_DATA();
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
		// 库存数量
		std::pair<CString, CString> _paTotal;
		// 商品信息
		std::pair<CString, CString> _paInfo;
	};

	struct CUSTOM_GSET
	{
		CUSTOM_GSET();
		BOOL _bShowBaby;
		BOOL _bShowType;
		BOOL _bShowTime;
		BOOL _bShowScort;
		BOOL _bShowBalance;

		CString _sQfindPhone;
	};
}
