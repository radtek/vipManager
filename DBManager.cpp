#include "stdafx.h"
#include "DBManager.h"
#include "vipManager.h"

CDBManager::CDBManager():
	m_pDBM(NULL)
{
	m_pDBM = theApp.GetDBCon();
}


CDBManager::~CDBManager()
{
}

bool CDBManager::init()
{
	m_pDBM = theApp.GetDBCon();
	if (NULL == m_pDBM)
	{
		return false;
	}
	return true;
}

bool CDBManager::cusm_get_last_id(CString &strID)
{

	CString mysql;
	mysql.Format(_T("select `table_name`, `AUTO_INCREMENT` from information_schema.tables where table_name='customer_idx'"));
	try
	{
		m_pDBM->SelectDataBase(MysqlManager::DBLZCustomer);

		if (-1 == m_pDBM->ExecutSqlAndReturn(mysql))
			throw 0;
		m_pDBM->BeforeFirst();
		while (m_pDBM->Next())
		{
			strID = m_pDBM->GetString(_T("AUTO_INCREMENT"));
			m_pDBM->CloseSqlRecords();
			return true;
		}
		throw 0;
	}
	catch (...)
	{
		m_pDBM->CloseSqlRecords();
		return false;
	}
	return true;
}

bool CDBManager::cusm_add_new_user(const CString &strID,const USER_DATA& ud)
{
	CString mysql;
	// 添加记录
	mysql.Format(_T("REPLACE INTO `%s`.`customer_idx` (%s) VALUES (%s)"), 
		MysqlManager::DBLZCustomer, ud._getdb_key(),ud._getdb_val());
	if (!m_pDBM->ExecutSql(mysql))
		return false;
	// 添加对应用户log表
	CString strTableName = _T("log_") + strID; // ud中的id为空 自动递增的原因
	if (!m_pDBM->CopyTable(strTableName, _T("temp_clog"), false, MysqlManager::DBLZCustomer, MysqlManager::DBLZCustomer))
		return false;

	mysql.Format(_T("REPLACE INTO `%s`.`%s` (`ID`,`TYPE`,`TIME`,`LOG`) VALUES (NULL,'系统操作',NOW(),'添加用户')"),
		MysqlManager::DBLZCustomer, strTableName);
	if (!m_pDBM->ExecutSql(mysql))
		return false;


	return true;
}

bool CDBManager::cusm_edit_user(const USER_DATA& nud, const USER_DATA& oud)
{
	if (nud._paID.second.IsEmpty())
		return false;
	CString mysql;
	// 修改记录
	mysql.Format(_T("REPLACE INTO `%s`.`customer_idx` (%s) VALUES (%s)"),
		MysqlManager::DBLZCustomer, nud._getdb_key(), nud._getdb_val());
	if (!m_pDBM->ExecutSql(mysql))
		return false;
	// 保存操作记录
	CString strTableName = _T("log_") + nud._paID.second;
	CString strLog;
	CString strTemp;
	if (nud._paName.second.CompareNoCase(oud._paName.second))
	{
		strTemp.Format(_T("姓名%s改为%s;\r\n"), oud._paName.second, nud._paName.second);
		strLog += strTemp;
	}
	if (nud._paPhone1.second.CompareNoCase(oud._paPhone1.second))
	{
		strTemp.Format(_T("电话%s改为%s;\r\n"), oud._paPhone1.second, nud._paPhone1.second);
		strLog += strTemp;
	}
	if (nud._paPhone2.second.CompareNoCase(oud._paPhone2.second))
	{
		strTemp.Format(_T("备电话%s改为%s;\r\n"), oud._paPhone2.second, nud._paPhone2.second);
		strLog += strTemp;
	}
	if (nud._paBabyName.second.CompareNoCase(oud._paBabyName.second))
	{
		strTemp.Format(_T("宝宝名%s改为%s;\r\n"), oud._paBabyName.second, nud._paBabyName.second);
		strLog += strTemp;
	}
	if (nud._paBabySex.second.CompareNoCase(oud._paBabySex.second))
	{
		strTemp.Format(_T("宝宝性别%s改为%s;\r\n"), oud._paBabySex.second, nud._paBabySex.second);
		strLog += strTemp;
	}
	if (nud._paBabyAge.second.CompareNoCase(oud._paBabyAge.second))
	{
		strTemp.Format(_T("宝宝年龄%s改为%s;\r\n"), oud._paBabyAge.second, nud._paBabyAge.second);
		strLog += strTemp;
	}
	if (nud._paScore.second.CompareNoCase(oud._paScore.second))
	{
		strTemp.Format(_T("积分%s改为%s;\r\n"), oud._paScore.second, nud._paScore.second);
		strLog += strTemp;
	}
	if (nud._paBalanceMoney.second.CompareNoCase(oud._paBalanceMoney.second))
	{
		strTemp.Format(_T("余额%s改为%s;\r\n"), oud._paBalanceMoney.second, nud._paBalanceMoney.second);
		strLog += strTemp;
	}
	if (nud._paBalanceCount.second.CompareNoCase(oud._paBalanceCount.second))
	{
		strTemp.Format(_T("次数%s改为%s;\r\n"), oud._paBalanceCount.second, nud._paBalanceCount.second);
		strLog += strTemp;
	}
	if (nud._paType.second.CompareNoCase(oud._paType.second))
	{
		strTemp.Format(_T("类型%s改为%s;\r\n"), oud._paType.second, nud._paType.second);
		strLog += strTemp;
	}
	mysql.Format(_T("REPLACE INTO `%s`.`%s` (`ID`,`TYPE`,`TIME`,`LOG`) VALUES (NULL,'编辑操作',NOW(),'%s')"),
		MysqlManager::DBLZCustomer, strTableName, strLog);
	m_pDBM->ExecutSql(mysql);
	return true;
}

bool CDBManager::cusm_delete_user(const USER_DATA& ud)
{
	CString mysql;
	mysql.Format(_T("DELETE FROM `%s`.`customer_idx` WHERE `ID` IN (%s)"),
		MysqlManager::DBLZCustomer, ud._paID.second);
	if (!m_pDBM->ExecutSql(mysql))
		return false;
	mysql.Format(_T("DROP TABLE `%s`.`log_%s"), MysqlManager::DBLZCustomer,ud._paID.second);
	m_pDBM->ExecutSql(mysql);
	return true;
}

bool CDBManager::cusm_find_user(const USER_DATA& ud, std::vector<USER_DATA>& vecFindUd)
{
	CString mysql;
	mysql.Format(_T("SELECT * FROM `%s`.`customer_idx` WHERE 1 "),MysqlManager::DBLZCustomer);
	if (!ud._paID.second.IsEmpty())
	{
		mysql += (_T("AND `") + ud._paID.first + _T("`='") + ud._paID.second + _T("'"));
	}

	if (!ud._paName.second.IsEmpty())
	{
		mysql += (_T("AND `") + ud._paName.first + _T("`='") + ud._paName.second + _T("'"));
	}

	if (!ud._paPhone1.second.IsEmpty())
	{
		mysql += (_T("AND `") + ud._paPhone1.first + _T("`='") + ud._paPhone1.second + _T("'"));
	}

	if (!ud._paPhone2.second.IsEmpty())
	{
		mysql += (_T("AND `") + ud._paPhone2.first + _T("`='") + ud._paPhone2.second + _T("'"));
	}

	if (!ud._paType.second.IsEmpty())
	{
		mysql += (_T("AND `") + ud._paType.first + _T("`='") + ud._paType.second + _T("'"));
	}

	mysql += _T(" ORDER BY `ID`");
	try
	{
		if (-1 == m_pDBM->ExecutSqlAndReturn(mysql))
			throw 0;
		m_pDBM->BeforeFirst();
		while (m_pDBM->Next())
		{
			USER_DATA uud = ud;
			uud._paID.second = theApp.GetDBCon()->GetString(uud._paID.first);
			uud._paName.second = theApp.GetDBCon()->GetString(uud._paName.first);
			uud._paPhone1.second = theApp.GetDBCon()->GetString(uud._paPhone1.first);
			uud._paPhone2.second = theApp.GetDBCon()->GetString(uud._paPhone2.first);
			uud._paBabyName.second = theApp.GetDBCon()->GetString(uud._paBabyName.first);
			uud._paBabySex.second = theApp.GetDBCon()->GetString(uud._paBabySex.first);
			uud._paBabyAge.second = theApp.GetDBCon()->GetString(uud._paBabyAge.first);
			uud._paScore.second = theApp.GetDBCon()->GetString(uud._paScore.first);
			uud._paType.second = theApp.GetDBCon()->GetString(uud._paType.first);
			uud._paRegtime.second = theApp.GetDBCon()->GetString(uud._paRegtime.first);
			uud._paBalanceCount.second = theApp.GetDBCon()->GetString(uud._paBalanceCount.first);
			uud._paBalanceMoney.second = theApp.GetDBCon()->GetString(uud._paBalanceMoney.first);
			uud._paRemark.second = theApp.GetDBCon()->GetString(uud._paRemark.first);
			vecFindUd.push_back(uud);
		}
		m_pDBM->CloseSqlRecords();
	}
	catch (...)
	{
		m_pDBM->CloseSqlRecords();
		return false;
	}
	return true;
}

bool CDBManager::manger_get_last_flow(CString &strID)
{
	CString mysql;
	mysql.Format(_T("select `table_name`, `AUTO_INCREMENT` from information_schema.tables where table_name='manager_flow_idx'"));
	try
	{
		m_pDBM->SelectDataBase(MysqlManager::DBLZManager);

		if (-1 == m_pDBM->ExecutSqlAndReturn(mysql))
			throw 0;
		m_pDBM->BeforeFirst();
		while (m_pDBM->Next())
		{
			strID = m_pDBM->GetString(_T("AUTO_INCREMENT"));
			m_pDBM->CloseSqlRecords();
			return true;
		}
		throw 0;
	}
	catch (...)
	{
		m_pDBM->CloseSqlRecords();
		return false;
	}
	return true;
}

bool CDBManager::manger_order_list(const flowlist_data& fld)
{
	CString mysql;
	try
	{
		CString strType;
		CString strEndTime(_T("NULL"));
		if (fld._vecList.size())
		{
			if (fld._vecList[0].first._paCodeNumber.second.CompareNoCase(_T("1")))
			{
				strType = _T("商品");
				strEndTime = _T("NOW()");
			}
			else if (fld._vecList[0].first._paCodeNumber.second.CompareNoCase(_T("1")) == 0 && fld._vecList.size() == 1)
			{
				strType = _T("普通");
			}
			else
			{
				strType = _T("普通&商品");
			}
		}
		else
			return false;
		// 添加 temp
		mysql.Format(_T("REPLACE INTO `%s`.`manager_flow_temp` (`ID`,`FLOW_ID`,`STATUS`,`TYPE`,`PAYTYPE`,`TIME`,`ENDTIME`,`TOTAL`,`VALUETYPE`,`RECEIPT`,`RECEIPTNUM`,`REMARK`) VALUES \
			(NULL,'%s','%d','%s','%s',NOW(),%s,'%s','%s','%s','%s','%s')"),
			MysqlManager::DBLZManager,
			fld._paID.second,
			2, 
			strType, 
			fld._paPayUser.second, 
			strEndTime,
			fld._paTotal.second, 
			fld._paPayType.second, 
			fld._paReceipt.second,
			fld._paReceiptNum.second,
			fld._paRemark.second);
		if (!m_pDBM->ExecutSql(mysql))
			throw CString(_T("初始temp失败!"));
		// 添加 goods
		for (int i = 0; i < fld._vecList.size(); ++i)
		{
			if (fld._vecList[i].first._paCodeNumber.second.CompareNoCase(_T("1"))==0)
				continue;
			CString strTotal;
			strTotal.Format(_T("%.2lf"), _ttof(fld._vecList[i].first._paPrice.second) * 1 * fld._vecList[i].second);
			mysql.Format(_T("REPLACE INTO `%s`.`manager_flow_goods` (`ID`,`FLOW_ID`,`STATUS`,`PAYTYPE`,`TIME`,`TITLE`,`VALUE`,`SALE`,`CODENUM`,`COUNT`,`TOTAL`,`VALUETYPE`) VALUES \
			(NULL,'%s','%d','%s',NOW(),'%s','%s','%d','%s','%g','%s','%s')"),
				MysqlManager::DBLZManager, 
				fld._paID.second, 
				1,
				fld._paPayUser.second, 
				fld._vecList[i].first._paTitle.second, 
				fld._vecList[i].first._paPrice.second, 
				1,
				fld._vecList[i].first._paCodeNumber.second,
				fld._vecList[i].second,
				strTotal,
				fld._paPayType.second);
			if (!m_pDBM->ExecutSql(mysql))
				throw CString(_T("初始goods失败!"));
		}
		// 初始 main
		if (fld._vecList.size() && fld._vecList[0].first._paCodeNumber.second.CompareNoCase(_T("1"))==0)
		{
			CString strTotal;
			strTotal.Format(_T("%.2lf"), _ttof(fld._vecList[0].first._paPrice.second) * 1 * fld._vecList[0].second);
			mysql.Format(_T("REPLACE INTO `%s`.`manager_flow_main` (`ID`,`FLOW_ID`,`STATUS`,`PAYTYPE`,`TIME`,`ENDTIME`,`VALUE`,`SALE`,`COUNT`,`TOTAL`,`VALUETYPE`) VALUES \
			(NULL,'%s','%d','%s',NOW(),%s,'%s','%d','%g','%s','%s')"),
				MysqlManager::DBLZManager,
				fld._paID.second,
				2,
				fld._paPayUser.second,
				strEndTime,
				fld._vecList[0].first._paPrice.second,
				1,
				fld._vecList[0].second,
				strTotal,
				fld._paPayType.second);
			if (!m_pDBM->ExecutSql(mysql))
				throw CString(_T("初始main失败!"));
		}
		// 初始 idx
		mysql.Format(_T("REPLACE INTO `%s`.`manager_flow_idx` (`ID`,`STATUS`,`CARDNUM`,`TIME`,`VALUE`,`RECEIPT`,`RECEIPTNUM`,`REMARK`) VALUES \
			(NULL,'%d','%s',NOW(),'%s','%s','%s','%s')"),
			MysqlManager::DBLZManager,
			2,
			fld._paPayUser.second,
			fld._paTotal.second,
			fld._paReceipt.second,
			fld._paReceiptNum.second,
			fld._paRemark.second);
		if (!m_pDBM->ExecutSql(mysql))
			throw CString(_T("初始idx失败!"));
		// 刷新 flow 外面

	}
	catch (CString stre )
	{
		AfxMessageBox(stre);
		return false;
	}
	catch (...)
	{
		AfxMessageBox(_T("下单错误!!!"));
		return false;
	}

	return true;
}

bool CDBManager::manger_find_goods(const GOODS_DATA& gd, std::vector<GOODS_DATA>& vecFindGd)
{
	CString mysql;
	mysql.Format(_T("SELECT * FROM `%s`.`manager_goods` WHERE 1 "), MysqlManager::DBLZManager);
	if (!gd._paCodeNumber.second.IsEmpty())
	{
		mysql += (_T("AND `") + gd._paCodeNumber.first + _T("`='") + gd._paCodeNumber.second + _T("'"));
	}

	if (!gd._paTitle.second.IsEmpty())
	{
		mysql += (_T("AND `") + gd._paTitle.first + _T("` LIKE '%") + gd._paTitle.second + _T("%'"));
	}

	if (!gd._paType.second.IsEmpty())
	{
		mysql += (_T("AND `") + gd._paType.first + _T("`='") + gd._paType.second + _T("'"));
	}

	if (!gd._paPrice.second.IsEmpty())
	{
		mysql += (_T("AND `") + gd._paPrice.first + _T("`='") + gd._paPrice.second + _T("'"));
	}

	mysql += _T(" ORDER BY `ID`");
	try
	{
		if (-1 == m_pDBM->ExecutSqlAndReturn(mysql))
			throw 0;
		m_pDBM->BeforeFirst();
		while (m_pDBM->Next())
		{
			GOODS_DATA ggd = gd;
			ggd._paID.second = theApp.GetDBCon()->GetString(ggd._paID.first);
			ggd._paCodeNumber.second = theApp.GetDBCon()->GetString(ggd._paCodeNumber.first);
			ggd._paType.second = theApp.GetDBCon()->GetString(ggd._paType.first);
			ggd._paTitle.second = theApp.GetDBCon()->GetString(ggd._paTitle.first);
			ggd._paPrice.second = theApp.GetDBCon()->GetString(ggd._paPrice.first);
			ggd._paIntoPrice.second = theApp.GetDBCon()->GetString(ggd._paIntoPrice.first);
			ggd._paTotal.second = theApp.GetDBCon()->GetString(ggd._paTotal.first);
			ggd._paInfo.second = theApp.GetDBCon()->GetString(ggd._paInfo.first);
			vecFindGd.push_back(ggd);
		}
		m_pDBM->CloseSqlRecords();
	}
	catch (...)
	{
		m_pDBM->CloseSqlRecords();
		return false;
	}
	return true;
}

bool CDBManager::manger_add_new_goods(const goods_data& gd)
{
	CString mysql;
	// 添加记录
	mysql.Format(_T("REPLACE INTO `%s`.`manager_goods` (%s) VALUES (%s)"),
		MysqlManager::DBLZManager, gd._getdb_key(), gd._getdb_val());
	if (!m_pDBM->ExecutSql(mysql))
		return false;
	// 修改log表
	mysql.Format(_T("REPLACE INTO `%s`.`manager_goods_log` (`ID`,`CODENUM`,`TYPE`,`VALUE`,`FLOWID`,`TIME`,`WHO`) VALUES (NULL,'%s','%s','%s',%d,NOW(),'测试操作用户')"),
		MysqlManager::DBLZManager, 
		gd._paCodeNumber.second,
		_T("新品入库"),
		gd._paTotal.second,
		0
	);
	if (!m_pDBM->ExecutSql(mysql))
		return false;
	return true;
}

bool CDBManager::manger_edit_goods(const goods_data& ngd, const goods_data& ogd)
{
	CString mysql;
	// 修改记录
	mysql.Format(_T("REPLACE INTO `%s`.`manager_goods` (%s) VALUES (%s)"),
		MysqlManager::DBLZManager, ngd._getdb_key(), ngd._getdb_val());
	if (!m_pDBM->ExecutSql(mysql))
		return false;
	// 修改log表
	// ...
	// ...
	return true;
}

bool CDBManager::manger_delete_goods(const goods_data& gd)
{
	CString mysql;
	mysql.Format(_T("DELETE FROM `%s`.`manager_goods` WHERE `CODENUM`='%s'"),
		MysqlManager::DBLZManager, gd._paCodeNumber.second);
	if (!m_pDBM->ExecutSql(mysql))
		return false;
	// 修改log表
	// ...
	// ...
	return true;
}

bool CDBManager::proper_get_idx(const flow_idx_data& findData, std::vector<flow_idx_data>& vecResData)
{
	CString mysql;
	mysql.Format(_T("SELECT * FROM `%s`.`manager_flow_idx` WHERE 1 "), MysqlManager::DBLZManager);
	if (!findData._paID.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paID.first + _T("`='") + findData._paID.second + _T("'"));
	}

	if (!findData._paStatus.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paStatus.first + _T("`='") + findData._paStatus.second + _T("'"));
	}

	if (!findData._paPayType.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paPayType.first + _T("`='") + findData._paPayType.second + _T("'"));
	}

	if (!findData._paTime.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paTime.first + _T("`='") + findData._paTime.second + _T("'"));
	}

	if (!findData._paReceipt.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paReceipt.first + _T("`='") + findData._paReceipt.second + _T("'"));
	}
	if (!findData._paReceiptNum.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paReceiptNum.first + _T("`='") + findData._paReceiptNum.second + _T("'"));
	}

	if (!findData._paRemark.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paRemark.first + _T("` LIKE '%") + findData._paRemark.second + _T("%'"));
	}

	mysql += (_T(" ORDER BY `") + findData._paID.first +_T("`"));
	try
	{
		if (-1 == m_pDBM->ExecutSqlAndReturn(mysql))
			throw 0;
		m_pDBM->BeforeFirst();
		while (m_pDBM->Next())
		{
			flow_idx_data ggd = findData;
			ggd._paID.second = theApp.GetDBCon()->GetString(ggd._paID.first);
			ggd._paStatus.second = theApp.GetDBCon()->GetString(ggd._paStatus.first);
			ggd._paPayType.second = theApp.GetDBCon()->GetString(ggd._paPayType.first);
			ggd._paTime.second = theApp.GetDBCon()->GetString(ggd._paTime.first);
			ggd._paValue.second = theApp.GetDBCon()->GetString(ggd._paValue.first);
			ggd._paReceipt.second = theApp.GetDBCon()->GetString(ggd._paReceipt.first);
			ggd._paReceiptNum.second = theApp.GetDBCon()->GetString(ggd._paReceiptNum.first);
			ggd._paRemark.second = theApp.GetDBCon()->GetString(ggd._paRemark.first);
			vecResData.push_back(ggd);
		}
		m_pDBM->CloseSqlRecords();
	}
	catch (...)
	{
		m_pDBM->CloseSqlRecords();
		return false;
	}

	return true;
}

bool CDBManager::proper_get_main(const flow_main_data &findData, std::vector<flow_main_data>& vecResData)
{
	CString mysql;
	mysql.Format(_T("SELECT * FROM `%s`.`manager_flow_main` WHERE 1 "), MysqlManager::DBLZManager);
	if (!findData._paFlowID.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paFlowID.first + _T("`='") + findData._paFlowID.second + _T("'"));
	}

	if (!findData._paStatus.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paStatus.first + _T("`='") + findData._paStatus.second + _T("'"));
	}

	if (!findData._paPayType.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paPayType.first + _T("`='") + findData._paPayType.second + _T("'"));
	}

	if (!findData._paTime.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paTime.first + _T("`='") + findData._paTime.second + _T("'"));
	}

	if (!findData._paEndTime.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paEndTime.first + _T("`='") + findData._paEndTime.second + _T("'"));
	}

	if (!findData._paValue.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paValue.first + _T("`='") + findData._paValue.second + _T("'"));
	}
	if (!findData._paSale.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paSale.first + _T("`='") + findData._paSale.second + _T("'"));
	}

	if (!findData._paCount.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paCount.first + _T("`='") + findData._paCount.second + _T("'"));
	}

	if (!findData._paTotal.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paTotal.first + _T("`='") + findData._paTotal.second + _T("'"));
	}

	if (!findData._paValueType.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paValueType.first + _T("`='") + findData._paValueType.second + _T("'"));
	}
	mysql += (_T(" ORDER BY `") + findData._paFlowID.first + _T("`"));
	try
	{
		if (-1 == m_pDBM->ExecutSqlAndReturn(mysql))
			throw 0;
		m_pDBM->BeforeFirst();
		while (m_pDBM->Next())
		{
			CString strTemp;
			flow_main_data ggd = findData;
			ggd._paFlowID.second = theApp.GetDBCon()->GetString(ggd._paFlowID.first);
			strTemp = theApp.GetDBCon()->GetString(ggd._paStatus.first);
			if (strTemp.CompareNoCase(_T("1")) == 0)
				ggd._paStatus.second = _T("已完成");
			else if (strTemp.CompareNoCase(_T("2")) == 0)
				ggd._paStatus.second = _T("进行中");
			else
				ggd._paStatus.second = _T("交易失败");
			ggd._paPayType.second = theApp.GetDBCon()->GetString(ggd._paPayType.first);
			ggd._paTime.second = theApp.GetDBCon()->GetString(ggd._paTime.first);
			ggd._paEndTime.second = theApp.GetDBCon()->GetString(ggd._paEndTime.first);
			ggd._paValue.second = theApp.GetDBCon()->GetString(ggd._paValue.first);
			ggd._paSale.second = theApp.GetDBCon()->GetString(ggd._paSale.first);
			ggd._paCount.second = theApp.GetDBCon()->GetString(ggd._paCount.first);
			ggd._paTotal.second = theApp.GetDBCon()->GetString(ggd._paTotal.first);
			ggd._paValueType.second = theApp.GetDBCon()->GetString(ggd._paValueType.first);
			vecResData.push_back(ggd);
		}
		m_pDBM->CloseSqlRecords();
	}
	catch (...)
	{
		m_pDBM->CloseSqlRecords();
		return false;
	}

	return true;
}

bool CDBManager::proper_get_goods(const flow_goods_data &findData, std::vector<flow_goods_data>& vecResData)
{
	CString mysql;
	mysql.Format(_T("SELECT * FROM `%s`.`manager_flow_goods` WHERE 1 "), MysqlManager::DBLZManager);
	if (!findData._paFlowID.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paFlowID.first + _T("`='") + findData._paFlowID.second + _T("'"));
	}

	if (!findData._paStatus.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paStatus.first + _T("`='") + findData._paStatus.second + _T("'"));
	}

	if (!findData._paPayType.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paPayType.first + _T("`='") + findData._paPayType.second + _T("'"));
	}

	if (!findData._paTime.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paTime.first + _T("`='") + findData._paTime.second + _T("'"));
	}

	if (!findData._paTitle.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paTitle.first + _T("`='") + findData._paTitle.second + _T("'"));
	}

	if (!findData._paValue.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paValue.first + _T("`='") + findData._paValue.second + _T("'"));
	}

	if (!findData._paCodeNum.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paCodeNum.first + _T("`='") + findData._paCodeNum.second + _T("'"));
	}

	if (!findData._paSale.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paSale.first + _T("`='") + findData._paSale.second + _T("'"));
	}

	if (!findData._paCount.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paCount.first + _T("`='") + findData._paCount.second + _T("'"));
	}

	if (!findData._paTotal.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paTotal.first + _T("`='") + findData._paTotal.second + _T("'"));
	}

	if (!findData._paValueType.second.IsEmpty())
	{
		mysql += (_T("AND `") + findData._paValueType.first + _T("`='") + findData._paValueType.second + _T("'"));
	}
	mysql += (_T(" ORDER BY `") + findData._paFlowID.first + _T("`"));
	try
	{
		if (-1 == m_pDBM->ExecutSqlAndReturn(mysql))
			throw 0;
		m_pDBM->BeforeFirst();
		while (m_pDBM->Next())
		{
			flow_goods_data ggd = findData;
			ggd._paFlowID.second = theApp.GetDBCon()->GetString(ggd._paFlowID.first);
			ggd._paStatus.second = theApp.GetDBCon()->GetString(ggd._paStatus.first);
			ggd._paPayType.second = theApp.GetDBCon()->GetString(ggd._paPayType.first);
			ggd._paTime.second = theApp.GetDBCon()->GetString(ggd._paTime.first);
			ggd._paTitle.second = theApp.GetDBCon()->GetString(ggd._paTitle.first);
			ggd._paValue.second = theApp.GetDBCon()->GetString(ggd._paValue.first);
			ggd._paCodeNum.second = theApp.GetDBCon()->GetString(ggd._paCodeNum.first);
			ggd._paSale.second = theApp.GetDBCon()->GetString(ggd._paSale.first);
			ggd._paCount.second = theApp.GetDBCon()->GetString(ggd._paCount.first);
			ggd._paTotal.second = theApp.GetDBCon()->GetString(ggd._paTotal.first);
			ggd._paValueType.second = theApp.GetDBCon()->GetString(ggd._paValueType.first);
			vecResData.push_back(ggd);
		}
		m_pDBM->CloseSqlRecords();
	}
	catch (...)
	{
		m_pDBM->CloseSqlRecords();
		return false;
	}
	return true;
}
