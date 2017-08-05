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
