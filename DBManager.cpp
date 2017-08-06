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

bool CDBManager::cusm_add_new_user(const USER_DATA& ud)
{
	CString mysql;
	mysql.Format(_T("REPLACE INTO `%s`.`customer_idx` (%s) VALUES (%s)"), 
		MysqlManager::DBLZCustomer, ud._getdb_key(),ud._getdb_val());
	if (!m_pDBM->ExecutSql(mysql))
		return false;
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
