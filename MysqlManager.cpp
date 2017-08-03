#include "StdAfx.h"
#include "MysqlManager.h"
#include <map>
#include <string>
#include <memory>

#include "ByteCode.h"
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"
#include "boost/lexical_cast.hpp"

#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/tuple/tuple.hpp>

using namespace sql;
using namespace std;
using namespace IUDB;

CString MysqlManager::CoreDBName = _T("LZmanager");


MysqlManager::MysqlManager(const CString& strDbName, const CString& strTcpIp, const int& iPort, const CString& strUesrName, const CString& strPassWord, const CString& dbType)
	: m_stdDbName(strDbName), m_strTcpIp(strTcpIp), m_iPort(iPort), m_strUesrName(strUesrName), m_strPassWord(strPassWord), m_dbType(dbType)
{
	m_nExcueCols = 0;
	m_pDriver = sql::mysql::get_mysql_driver_instance();
	m_pCon = NULL;
	m_pRes = NULL;
	m_pState = NULL;
	m_pPreMst = NULL;
}


MysqlManager::~MysqlManager(void)
{
	if (m_pState)
	{
		delete m_pState;
		m_pState = NULL;
	}
	if (m_pRes)
	{
		delete m_pRes;
		m_pRes = NULL;
	}
	if (m_pPreMst)
	{
		delete m_pPreMst;
		m_pPreMst = NULL;
	}

	if (m_pCon)
	{
		if (!m_pCon->isClosed())
			m_pCon->close();
		delete m_pCon;
		m_pCon = NULL;
	}
}

sql::SQLString MysqlManager::getSqlString(const CString& strString) const
{
	char szBuf[MAX_SQL];
	CByteCode::ToUTF8(szBuf, sizeof(szBuf), strString);
	sql::SQLString strAllTabl;
	strAllTabl.append(szBuf);
	return strAllTabl;
}

sql::SQLString MysqlManager::makeCnName() const
{
	CString strTmp = m_strTcpIp;
	if (m_strTcpIp.CompareNoCase(_T("localhost")) == 0)
		strTmp = _T("127.0.0.1");
	CString strTmp2;
	strTmp2.Format(_T("%d"), m_iPort);
	strTmp = _T("tcp://") + strTmp + _T(":") + strTmp2;
	sql::SQLString str = getSqlString(strTmp);
	return str;
}

CString MysqlManager::getErrorInfo(const sql::SQLException& er) const
{
	CString strTmp1;
	CByteCode::UTF_8ToUnicode(strTmp1, (char*)er.what());
	sql::SQLString strTmp = er.getSQLState();
	CString strTmp2;
	CByteCode::UTF_8ToUnicode(strTmp2, (char*)strTmp.c_str());
	CString strTmp3;
	strTmp3.Format(_T("ERROR:%s <MySQL error code:%d, SQLState:%s>"), strTmp1, er.getErrorCode(), strTmp2);
	return strTmp3;
}

bool MysqlManager::CreateDb(const CString& dbName)
{
	try
	{
		//表名替换
		char szCh[128];
		CString strCsTmp = _T("`") + dbName + _T("`");
		CByteCode::ToUTF8(szCh, sizeof(szCh), strCsTmp);

		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());

		//创建数据库
		sql::SQLString strSql;
		strSql.append("Create Database ");
		strSql.append(szCh);
		strSql.append(" CHARACTER SET utf8 COLLATE utf8_general_ci");
		pSta->executeUpdate(strSql);

		//授予管理员权限，可以操作所有数据库
		sql::SQLString strAdmin;
		strAdmin.append("GRANT ALL ON ");
		strAdmin.append(szCh);
		strAdmin.append(".*");
		strAdmin.append(" TO ");
		strAdmin.append("'");
		sql::SQLString strTmp = getSqlString(m_strUesrName);
		strAdmin.append(strTmp);
		strAdmin.append("'");
		strAdmin.append("@");
		strAdmin.append("'");
		strAdmin.append("%");
		strAdmin.append("'");
		pSta->executeUpdate(strAdmin);
	}
	catch (SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("创建数据库%s失败!"),pszDbName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::GrantAllPrivileges(const CString& pszUser)
{
	try
	{
		//表名替换
		char szCh[128];
		CByteCode::ToUTF8(szCh, sizeof(szCh), pszUser);

		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());

		//授予管理员权限，可以操作所有数据库
		sql::SQLString strAdmin;
		strAdmin.append("GRANT GRANT OPTION ON *.* TO ");
		strAdmin.append("'");
		strAdmin.append(szCh);
		strAdmin.append("'");
		strAdmin.append("@");
		strAdmin.append("'");
		strAdmin.append("%");
		strAdmin.append("'");
		pSta->executeUpdate(strAdmin);

		sql::SQLString strGrant;
		strGrant.append("GRANT UPDATE, DELETE, CREATE, DROP, RELOAD, SHUTDOWN, PROCESS, FILE, REFERENCES,");
		strGrant.append("INDEX, ALTER, SHOW DATABASES, SUPER, CREATE TEMPORARY TABLES, LOCK TABLES, EXECUTE,");
		strGrant.append("REPLICATION SLAVE, REPLICATION CLIENT, CREATE VIEW, SHOW VIEW, CREATE ROUTINE, ALTER ROUTINE,");
		strGrant.append("CREATE USER, EVENT, TRIGGER ON *.* TO 'zwdb'@'%'");

		pSta->executeUpdate(strGrant);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("授予用户%s所有权限失败!"),pszUser);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

CString MysqlManager::GetCurrentDataBase()
{
	if (m_pCon)
	{
		sql::SQLString dbName = m_pCon->getSchema();
		TCHAR wszBuf[128];
		const int kTempBufSize = sizeof(wszBuf) / sizeof(wszBuf[0]);
		memset(wszBuf, 0, sizeof(wszBuf));
		CByteCode::ToUCS2(wszBuf, kTempBufSize, dbName.c_str());
		return wszBuf;
	}
	return _T("");
}

bool MysqlManager::SelectDataBase(const CString& pszDbName)
{
	if (!m_pCon->isValid())
		reconnectSvc();
	std::string strdbName;
	m_stdDbName = pszDbName;
	try
	{
		CByteCode::UnicodeToUTF_8(strdbName, m_stdDbName);
		m_pCon->setSchema(strdbName.c_str());
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("选择数据库%s失败!"),pszDbName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

int MysqlManager::GetRowCounts(const CString& dbName, const CString& tableName)
{
	// 查information_schema表貌似有点慢
	// 	try
	// 	{
	// 		if (dbName.IsEmpty() && tableName.IsEmpty())
	// 			return -1;
	// 		std::auto_ptr<sql::Statement>pSta(m_pCon->createStatement());
	// 		CString strSql;//该语句在数据库名称和表名完全由字母、下划线和数字构成时，速度更快
	// 		strSql.Format( _T("select NUM_ROWS from `information_schema`.`INNODB_SYS_TABLESTATS` WHERE name = '%s/%s'"), dbName, tableName);
	// 		char szSql[128];
	// 		CByteCode::ToUTF8(szSql,sizeof(szSql),strSql);
	// 		std::auto_ptr<sql::ResultSet>pRes (pSta->executeQuery(szSql));
	// 		pRes->next();
	// 
	// 		return pRes->getInt(1);
	// 	} 
	// 	catch (const sql::SQLException& er)
	// 	{
	// 		//CString strError = getErrorInfo(er);
	// 		//CString strError; 
	// 		//strError.Format(_T("获取表%s总行数失败!"),tblName);
	// 		CString strError = getErrorInfo(er);
	// #if defined(_DEBUG)&&defined(_DEBUG_SQL)
	// 		AfxMessageBox(strError);
	// #endif
	// 		//return -1;  
	// 	}

	try
	{
		if (dbName.IsEmpty() && tableName.IsEmpty())
			return -1;
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		CString strSql;
		strSql.Format(_T("SELECT count(*) FROM `%s`.`%s`"), dbName, tableName);
		char szSql[128];
		CByteCode::ToUTF8(szSql, sizeof(szSql), strSql);
		std::auto_ptr<sql::ResultSet> pRes(pSta->executeQuery(szSql));
		pRes->next();

		return pRes->getInt(1);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("获取表%s总行数失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return -1;
	}
	return -1;
}

int MysqlManager::GetRowCounts(const CString& tableName)
{
	return GetRowCounts(m_stdDbName, tableName);
}

int MysqlManager::GetColCounts(const CString& dbName, const CString& tableName)
{
	try
	{
		if (dbName.IsEmpty() || tableName.IsEmpty())
			return -1;
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		CString strSql;
		strSql.Format(_T("select count(*) from `information_schema`.`columns` where table_schema='%s' and table_name='%s'"), dbName, tableName);
		char szSql[128];
		CByteCode::ToUTF8(szSql, sizeof(szSql), strSql);
		std::auto_ptr<sql::ResultSet> pRes(pSta->executeQuery(szSql));
		pRes->next();

		return pRes->getInt(1);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("获取表%s总行数失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return -1;
	}
	return -1;
}

int MysqlManager::GetColCounts(const CString& tableName)
{
	return GetColCounts(m_stdDbName, tableName);
}

bool MysqlManager::GetColsInfo(const CString& dbName, const CString& tblName, CStringArray& fieldNames, CStringArray& filedTypes)
{
	if (tblName.IsEmpty())
		return false;

	CString strSql;
	strSql.Format(_T("select * from `%s`.`%s`"), dbName, tblName);

	sql::SQLString strAllTabl = getSqlString(strSql);
	//step1: colms op
	try
	{
		sql::PreparedStatement* pPreStaTem = m_pCon->prepareStatement(strAllTabl);
		if (pPreStaTem)
		{
			sql::ResultSetMetaData* pResMet = pPreStaTem->getMetaData();
			if (pResMet)
			{
				int nCnt = pResMet->getColumnCount();
				for (int i = 1; i <= nCnt; i++)
				{
					TCHAR wszBuf[128];
					const int kTempBufSize = sizeof(wszBuf) / sizeof(wszBuf[0]);
					memset(wszBuf, 0, sizeof(wszBuf));
					CByteCode::ToUCS2(wszBuf, kTempBufSize, pResMet->getColumnName(i).c_str());
					fieldNames.Add(wszBuf);
					memset(wszBuf, 0, sizeof(wszBuf));
					CByteCode::ToUCS2(wszBuf, kTempBufSize, pResMet->getColumnTypeName(i).c_str());
					filedTypes.Add(wszBuf);
				}
			}
			delete pPreStaTem;
		}
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("获取表%s列信息失败!"),pTblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}

	return true;
}

//SELECT MAX(ID) FROM BOOKID 
int MysqlManager::GetLastId(const CString& dbName, const CString& tableName)
{
	try
	{
		if (dbName.IsEmpty() || tableName.IsEmpty())
			return false;

		CString strSql;
		strSql.Format(_T("SELECT MAX(ID) FROM `%s`.`%s`"), dbName, tableName);

		sql::SQLString strQery = getSqlString(strSql);
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		std::auto_ptr<sql::ResultSet> pRes(pSta->executeQuery(strQery));
		pRes->next();
		return pRes->getInt(1);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("获取表%s最后一个ID失败!"),pTblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return -1;
	}
}

int MysqlManager::GetLastId(const CString& tableName)
{
	return GetLastId(m_stdDbName, tableName);
}

void MysqlManager::BeforeFirst()
{
	if (m_enMode == enTableRead)
	{
		if (m_pRes)
			return m_pRes->beforeFirst();
	}
}

bool MysqlManager::MoveRow(int rowIndex)
{
	if (m_enMode == enTableRead)
	{
		if (m_pRes)
			return m_pRes->absolute(rowIndex);
	}
	else if (m_enMode == enTableWrite)
	{
		return false;
	}
	else if (m_enMode == enTableModify)
	{
		std::string strId = boost::lexical_cast<std::string>(rowIndex);
		sql::SQLString strTemSql;
		strTemSql.append(m_sqlScript);
		strTemSql.append(strId.c_str());

		if (InitWriteCache())
		{
			m_pPreMst = m_pCon->prepareStatement(strTemSql);
			return true;
		}
		return false;
	}
	return false;
}

bool MysqlManager::InitWriteCache()
{
	if (m_pCon == NULL)
		return false;

	if (m_pPreMst)
	{
		delete m_pPreMst;
		m_pPreMst = NULL;
		SQLlogger(_T("写入表未关闭！"));
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(_T("写入表未关闭！"));
#endif
	}
	return true;
}

bool MysqlManager::Next()
{
	try
	{
		if (m_enMode == enTableRead)
		{
			if (m_pRes)
				return m_pRes->next();
		}
		else if (m_enMode == enTableWrite)
		{
			if (m_pPreMst)
				m_pPreMst->executeUpdate();
			return true;
		}
		else if (m_enMode == enTableModify)
		{
			if (m_pPreMst)
			{
				m_pPreMst->executeUpdate();
				delete m_pPreMst;
				m_pPreMst = NULL;
			}
			return false;
		}
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("游标移动到下一行失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return false;
}

bool MysqlManager::GetBoolean(int columnIndex) const
{
	if (m_pRes == NULL || m_nExcueCols < columnIndex)
		return false;
	return m_pRes->getBoolean(columnIndex);
}

bool MysqlManager::GetBoolean(const CString& columnLabel) const
{
	sql::SQLString strT = getSqlString(columnLabel);
	if (m_pRes == NULL || m_pRes->findColumn(strT) < 1)
		return false;
	return m_pRes->getBoolean(strT);
}

int MysqlManager::GetInt32(int columnIndex) const
{
	if (m_pRes == NULL || m_nExcueCols < columnIndex)
		return -1;
	return m_pRes->getInt(columnIndex);
}

int MysqlManager::GetInt32(const CString& columnLabel) const
{
	sql::SQLString strT = getSqlString(columnLabel);
	if (m_pRes == NULL || m_pRes->findColumn(strT) < 1)
		return -1;
	return m_pRes->getInt(strT);
}


long double MysqlManager::GetDouble(int columnIndex) const
{
	if (m_pRes == NULL || m_nExcueCols < columnIndex)
		return -1;
	return m_pRes->getDouble(columnIndex);
}

long double MysqlManager::GetDouble(const CString& columnLabel) const
{
	sql::SQLString strT = getSqlString(columnLabel);
	if (m_pRes == NULL || m_pRes->findColumn(strT) < 1)
		return -1;
	return m_pRes->getDouble(strT);
}

CString MysqlManager::GetString(int columnIndex) const
{
	CString str;
	if (m_pRes == NULL || m_nExcueCols < columnIndex)
		return str;
	sql::SQLString strTmp = m_pRes->getString(columnIndex);
	CByteCode::UTF_8ToUnicode(str, (char*)strTmp.c_str());
	return str;
}


CString MysqlManager::GetString(const CString& columnLabel) const
{
	CString str;
	sql::SQLString strT = getSqlString(columnLabel);
	if (m_pRes == NULL || m_pRes->findColumn(strT) < 1)
		return str;
	sql::SQLString strTmp = m_pRes->getString(strT);
	CByteCode::UTF_8ToUnicode(str, (char*)strTmp.c_str());
	return str;
}

std::istream* MysqlManager::GetBlob(int columnIndex) const
{
	if (m_pRes == NULL || m_nExcueCols < columnIndex)
		return NULL;
	return m_pRes->getBlob(columnIndex);
}

std::istream* MysqlManager::GetBlob(const CString& columnLabel) const
{
	sql::SQLString strT = getSqlString(columnLabel);
	if (m_pRes == NULL || m_pRes->findColumn(strT) < 1)
		return NULL;
	return m_pRes->getBlob(strT);
}

void MysqlManager::SetInt(unsigned int parameterIndex, int val, bool bNull)
{
	if (m_pPreMst && m_nExcueCols >= (int)parameterIndex)
	{
		if (!bNull)
			m_pPreMst->setInt(parameterIndex, val);
		else
			m_pPreMst->setNull(parameterIndex, sql::DataType::SQLNULL);
	}
}

void MysqlManager::SetString(unsigned int parameterIndex, const CString& val, bool bNull)
{
	if (m_pPreMst && m_nExcueCols >= (int)parameterIndex)
	{
		CString tval(val);
		if (tval.IsEmpty())
		{
			CString filedType = m_filedTypes.GetAt(parameterIndex - 1);
			if (filedType.CompareNoCase(_T("DECIMAL")) == 0 || filedType.CompareNoCase(_T("SMALLINT")) == 0 ||
				filedType.CompareNoCase(_T("INT")) == 0 || filedType.CompareNoCase(_T("REAL")) == 0 ||
				filedType.CompareNoCase(_T("DOUBLE")) == 0 || filedType.CompareNoCase(_T("FLOAT")) == 0 ||
				filedType.CompareNoCase(_T("TINYINT")) == 0 || filedType.CompareNoCase(_T("INTEGER")) == 0 || filedType.CompareNoCase(_T("BIGINT")) == 0)
			{
				tval = _T("0");
			}
		}

		int dwNum = WideCharToMultiByte(CP_UTF8,NULL, tval, -1,NULL, 0,NULL,FALSE);
		if (!bNull && dwNum > 0 /*&& (!val.IsEmpty())*/)
		{
			sql::SQLString strT = getSqlString(tval);
			m_pPreMst->setString(parameterIndex, strT);
		}
		else
		{
			m_pPreMst->setNull(parameterIndex, sql::DataType::SQLNULL);
		}
	}
}

void MysqlManager::SetDouble(unsigned int parameterIndex, double val, bool bNull)
{
	if (m_pPreMst && m_nExcueCols >= (int)parameterIndex)
	{
		if (!bNull)
			m_pPreMst->setDouble(parameterIndex, val);
		else
			m_pPreMst->setNull(parameterIndex, sql::DataType::SQLNULL);
	}
}

void MysqlManager::SetBoolean(unsigned int parameterIndex, bool val, bool bNull)
{
	if (m_pPreMst && m_nExcueCols >= (int)parameterIndex)
	{
		if (!bNull)
			m_pPreMst->setBoolean(parameterIndex, val);
		else
			m_pPreMst->setNull(parameterIndex, sql::DataType::SQLNULL);
	}
}

void MysqlManager::SetBlob(unsigned int parameterIndex, std::istream* blob, bool bNull)
{
	if (m_pPreMst && m_nExcueCols >= (int)parameterIndex)
	{
		if (!bNull)
			m_pPreMst->setBlob(parameterIndex, blob);
		else
			m_pPreMst->setNull(parameterIndex, sql::DataType::SQLNULL);
	}
}

//插入操作
//INSERT INTO `a` (`NameNew`) VALUES ('100')
//////////////////////////////////////////////////////////////////////////
bool MysqlManager::addOneRow(const CString& tblName, const CStringArray& colNames, const CStringArray& rowVals)
{
	try
	{
		if (tblName.IsEmpty())
			return false;
		if ((colNames.GetSize() != rowVals.GetSize()) || colNames.GetSize() == 0 || rowVals.GetSize() == 0)
		{
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("列数与行记录不相等,请重新检查!"));
#endif
			return false;
		}
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		CString strSql = _T("INSERT INTO `") + tblName + _T("`(");
		CString strRowVals = _T("(");

		for (int i = 0; i < colNames.GetSize(); i++)
		{
			strSql += colNames[i];
			strRowVals += _T("'") + rowVals[i] + _T("'");
			if (i != (colNames.GetSize() - 1))
			{
				strSql += _T(",");
				strRowVals += _T(",");
			}
		}
		strSql += _T(")");
		strRowVals += _T(")");

		strSql += _T("VALUES");
		strSql += strRowVals;

		sql::SQLString strNewSql = getSqlString(strSql);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("表%s添加行失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::deleteOneRow(const CString& pTblName, const int rowIndex)
{
	try
	{
		if (pTblName.IsEmpty())
			return false;
		CString strSql;
		strSql = _T("delete from `") + pTblName + _T("` where ID= ");
		strSql.Format(_T("%s%d"), strSql, rowIndex);
		sql::SQLString strNewSql = getSqlString(strSql);
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("表%s删除行失败!"),pTblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

//UPDATE `a` SET `NameNew`='200.003', `ITEM`='dd1' WHERE (`ID`='4')
bool MysqlManager::updateOneRow(const CString& tblName, const int& rowIndex,
                                    const CStringArray& colNames, const CStringArray& rowVals)
{
	try
	{
		if (tblName.IsEmpty())
			return false;
		if ((colNames.GetSize() != rowVals.GetSize()) || colNames.GetSize() == 0 || rowVals.GetSize() == 0)
		{
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("列数与行记录不相等,请重新检查!"));
#endif
			return false;
		}
		CString strSql = _T("UPDATE `") + tblName + _T("` SET ");
		for (int i = 0; i < colNames.GetSize(); i++)
		{
			strSql += colNames[i];
			strSql += _T("=");
			strSql += _T("'");
			strSql += rowVals[i];
			strSql += _T("'");
			if (i != (colNames.GetSize() - 1))
			{
				strSql += _T(",");
			}
		}
		strSql += _T(" where ID=");
		CString strId;
		strId.Format(_T("%d"), rowIndex);
		strSql += strId;
		sql::SQLString strNewSql = getSqlString(strSql);
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("表%s更新行失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::AddColumn(const CString& tblName, const CString& columnName,
                                 const CString& columType)
{
	return AddColumn(m_stdDbName, tblName, columnName, columType);
}

bool MysqlManager::AddColumn(const CString& dbName, const CString& tblName, const CString& columnName,
                                 const CString& columType)
{
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty() || columnName.IsEmpty() || columType.IsEmpty())
			return false;
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		CString strOrg;
		strOrg.Format(_T("ALTER TABLE `%s`.`%s` ADD COLUMN %s %s"), dbName, tblName, columnName, columType);
		sql::SQLString strNewSql = getSqlString(strOrg);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("表%s添加列失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

//ALTER TABLE `a`
//CHANGE COLUMN `Name` `NameR`  double(8,4) NULL DEFAULT NULL AFTER `ID`;
//1. 修改列名
bool MysqlManager::ModifyColumName(const CString& tblName, const CString& oldColumnName,
                                       const CString& newColmName, const CString& columType)
{
	return ModifyColumName(m_stdDbName, tblName, oldColumnName, newColmName, columType);
}

bool MysqlManager::ModifyColumName(const CString& dbName, const CString& tblName, const CString& oldColumnName,
                                       const CString& newColmName, const CString& columType)
{
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty() || newColmName.IsEmpty() || columType.IsEmpty() || oldColumnName.IsEmpty())
			return false;
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		CString strOrg;
		strOrg.Format(_T("ALTER TABLE `%s`.`%s` CHANGE COLUMN %s %s %s"), dbName, tblName, oldColumnName, newColmName, columType);

		sql::SQLString strNewSql = getSqlString(strOrg);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("表%s修改列名失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

//ALTER TABLE `a`
//	MODIFY COLUMN `Name`  int(8) NULL DEFAULT NULL AFTER `ID`;
//2. 修改列类型
bool MysqlManager::ModifyColumType(const CString& tblName, const CString& columnName,
                                       const CString& newColmType)
{
	return ModifyColumType(m_stdDbName, tblName, columnName, newColmType);
}

bool MysqlManager::ModifyColumType(const CString& dbName, const CString& tblName, const CString& columnName,
                                       const CString& newColmType)
{
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty() || newColmType.IsEmpty() || columnName.IsEmpty())
			return false;
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		CString strOrg;
		strOrg.Format(_T("ALTER TABLE `%s`.`%s` MODIFY COLUMN %s %s"), dbName, tblName, columnName, newColmType);

		sql::SQLString strNewSql = getSqlString(strOrg);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("表%s修改列类型失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

//创建操作
//////////////////////////////////////////////////////////////////////////
bool MysqlManager::CreateCustomTable(const CString& strSql)
{
	auto_ptr<sql::Statement> pSta;
	try
	{
		if (strSql.IsEmpty())
			return false;
		sql::SQLString strNewSql = getSqlString(strSql);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("创建自定义表失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::isExitsDataBase(const CString& dbName)
{
	try
	{
		if (dbName.IsEmpty())
		{
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("数据库名不能为空!"));
#endif
			return false;
		}

		CString strSql;
		//std::wstring pSql;
		strSql.Format(_T("show databases like '%s'"), dbName);

		CString strDbName = GetCurrentDataBase();
		sql::SQLString strNewSql = getSqlString(strSql);
		if (!m_pState)
		{
			std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
			std::auto_ptr<sql::ResultSet> pRes(pSta->executeQuery(strNewSql));
			size_t nCnt = pRes->rowsCount();
			if (nCnt > 0)
			{
				return true;
			}
			else
				return false;
		}
		else
		{
			std::auto_ptr<sql::ResultSet> pRes(m_pState->executeQuery(strNewSql));
			size_t nCnt = pRes->rowsCount();
			if (nCnt > 0)
			{
				return true;
			}
			else
				return false;
		}
	}

	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("判断表%s是否存在失败!"),pszTable);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::isExitsField(const CString& tblName, const CString& fieldName)
{
	return isExitsField(m_stdDbName, tblName, fieldName);
}

bool MysqlManager::isExitsField(const CString& dbName, const CString& tblName, const CString& fieldName)
{
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty() || fieldName.IsEmpty())
		{
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("不能为空!"));
#endif
			return false;
		}

		CString strSql;
		//std::wstring pSql;
		strSql.Format(_T("show columns from `%s`.`%s` like '%s'"), dbName, tblName, fieldName);

		CString strDbName = GetCurrentDataBase();
		sql::SQLString strNewSql = getSqlString(strSql);
		if (!m_pState)
		{
			std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
			std::auto_ptr<sql::ResultSet> pRes(pSta->executeQuery(strNewSql));
			size_t nCnt = pRes->rowsCount();
			if (nCnt > 0)
			{
				return true;
			}
			else
				return false;
		}
		else
		{
			std::auto_ptr<sql::ResultSet> pRes(m_pState->executeQuery(strNewSql));
			size_t nCnt = pRes->rowsCount();
			if (nCnt > 0)
			{
				return true;
			}
			else
				return false;
		}
	}

	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("判断表%s是否存在失败!"),pszTable);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

//select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA='dbname' and TABLE_NAME='tablename' ;
//SHOW TABLES LIKE '%gb_stud%' 查询自己的数据库
bool MysqlManager::IsExitTable(const CString& tblName)
{
	return IsExitTable(m_stdDbName, tblName);
}

bool MysqlManager::IsExitTable(const CString& dbName, const CString& tblName)
{
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty())
		{
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("表名不能为空!"));
#endif
			return false;
		}

		CString strSql;
		strSql.Format(_T("SELECT table_name FROM `information_schema`.`tables` WHERE table_schema ='%s' AND table_name = '%s'"), dbName, tblName);

		CString strDbName = GetCurrentDataBase();
		sql::SQLString strNewSql = getSqlString(strSql);
		if (!m_pState && m_pCon)
		{
			std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
			std::auto_ptr<sql::ResultSet> pRes(pSta->executeQuery(strNewSql));
			size_t nCnt = pRes->rowsCount();
			if (nCnt > 0)
			{
				return true;
			}
			else
				return false;
		}
		else
		{
			std::auto_ptr<sql::ResultSet> pRes(m_pState->executeQuery(strNewSql));
			size_t nCnt = pRes->rowsCount();
			if (nCnt > 0)
			{
				return true;
			}
			else
				return false;
		}
	}

	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("判断表%s是否存在失败!"),pszTable);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

#ifndef NDEBUG
bool MysqlManager::CreateTable(const CString& dbName, const CString& tblName)
{
	CString columnsAndOtherStuff = _T("(ID int(11) NOT NULL default '0', PRIMARY KEY  (ID)) ENGINE=InnoDB CHARACTER SET utf8 COLLATE utf8_general_ci");
	sql::Statement* pSta;
	try
	{
		if (!IsExitTable(dbName, tblName))
		{
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("表不存在"));
#endif
			return false;
		}

		pSta = m_pCon->createStatement();

		CString createSql;
		createSql.Format(_T("CREATE TABLE `%s`.`%s` %s"), dbName, tblName, columnsAndOtherStuff);

		sql::SQLString strSql = getSqlString(createSql);
		pSta->execute(strSql);
		if (pSta)
			delete pSta;
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("创建表%s失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}

	return true;
}

bool MysqlManager::CreateTable(const CString& tblName, const CStringArray& colNames,
                                   const CStringArray& colTypes, const CString& primarkKey)
{
	INT_PTR nColNams = colNames.GetSize();
	INT_PTR nColTypes = colTypes.GetSize();
	if (nColNams <= 0 || nColTypes <= 0 || (nColNams != nColTypes))
	{
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(_T("列数与列类型数不匹配或为空!") );
#endif
		return false;
	}

	CString columnsAndOtherStuff = _T("(");
	for (int i = 0; i < nColNams; i++)
		columnsAndOtherStuff += colNames[i] + _T(" ") + colTypes[i] + _T(", ");

	if (_tcslen(primarkKey) != 0)
	{
		columnsAndOtherStuff += _T("PRIMARY KEY (") + primarkKey + _T(")");
	}
	columnsAndOtherStuff += _T(") ");
	columnsAndOtherStuff += _T("ENGINE=InnoDB CHARACTER SET utf8 COLLATE utf8_general_ci");

	sql::Statement* pSta = NULL;
	try
	{
		pSta = m_pCon->createStatement();
		//step1:

		CString pSql = _T("select TABLE_NAME from `INFORMATION_SCHEMA`.`TABLES` where TABLE_NAME= `") + tblName + _T("`");

		sql::SQLString strNewSql = getSqlString(pSql);
		auto_ptr<sql::ResultSet> pRes(pSta->executeQuery(strNewSql));
		if (!pRes->rowsCount())
		{
			if (pSta)
				delete pSta;
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("表不存在!"));
#endif
			return false;
		}

		//step2
		CString createSql = _T("CREATE TABLE `") + tblName + _T("` ") + columnsAndOtherStuff;
		strNewSql = getSqlString(createSql);
		pSta->execute(strNewSql);

		if (pSta)
			delete pSta;
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("创建表%s失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}

	return true;
}
#endif // !NDEBUG

bool MysqlManager::DropTable(const CString& tblName)
{
	return DropTable(m_stdDbName, tblName);
}

bool MysqlManager::DropTable(const CString& dbName, const CString& tblName)
{
	sql::Statement* pSta;
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty())
			return false;

		pSta = m_pCon->createStatement();

		char szKf0[32], szKf1[32];
		CByteCode::ToUTF8(szKf0, sizeof(szKf0),_T("SET foreign_key_checks = 0"));
		pSta->execute(szKf0);

		CString strSql;
		strSql.Format(_T("DROP TABLE IF EXISTS `%s`.`%s`"), dbName, tblName);
		sql::SQLString strNewSql = getSqlString(strSql);
		pSta->executeUpdate(strNewSql);

		CByteCode::ToUTF8(szKf1, sizeof(szKf1),_T("SET foreign_key_checks = 1"));
		pSta->execute(szKf1);

		if (pSta)
			delete pSta;
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("删除表%s失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}

	return true;
}

bool MysqlManager::CreateIndex(const CString& tblName, const CString& colName, const CString& idxName)
{
	return CreateIndex(m_stdDbName, tblName, colName, idxName);
}

bool MysqlManager::CreateIndex(const CString& dbName, const CString& tblName, const CString& colName, const CString& idxName)
{
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty() || colName.IsEmpty() || idxName.IsEmpty())
			return false;
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());

		CString strSql;
		strSql.Format(_T("CREATE UNIQUE INDEX %s ON `%s`.`%s` (%s ASC)"), idxName, dbName, tblName, colName);
		sql::SQLString strNewSql = getSqlString(strSql);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("创建表%s索引失败!"),tblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::ClearTable(const CString& tblName)
{
	return ClearTable(m_stdDbName, tblName);
}

bool MysqlManager::ClearTable(const CString& dbName, const CString& tblName)
{
	try
	{
		if (dbName.IsEmpty() || tblName.IsEmpty())
			return false;
		CString strSql;
		strSql.Format(_T("delete from `%s`.`%s`"), dbName, tblName);
		sql::SQLString strNewSql = getSqlString(strSql);
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("清空表%s失败!"),ptblName);
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::CopyTable(const CString& destTblName, const CString& orgTblName, bool isContainData, const CString& destDBName, const CString& orgDBName)
{
	try
	{
		CString realDestDBName = destDBName;
		if (destDBName.IsEmpty())
		{
			realDestDBName = m_stdDbName;
		}
		CString realOrgDBName = orgDBName;
		if (orgDBName.IsEmpty())
		{
			realOrgDBName = m_stdDbName;
		}
		if (realDestDBName.IsEmpty() || realOrgDBName.IsEmpty() || destTblName.IsEmpty() || orgTblName.IsEmpty())
		{
			SQLlogger(_T("表名不能为空!"));
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("表名不能为空!"));
#endif
			return false;
		}

		if (!isContainData)
		{
			CString strSql;
			strSql.Format(_T("CREATE TABLE `%s`.`%s` LIKE `%s`.`%s`"), realDestDBName, destTblName, realOrgDBName, orgTblName);

			sql::SQLString strNewSql = getSqlString(strSql);
			std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
			pSta->execute(strNewSql);
		}
		else
		{
			std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());
			CString strSql;
			strSql.Format(_T("CREATE TABLE `%s`.`%s` LIKE `%s`.`%s`"), realDestDBName, destTblName, realOrgDBName, orgTblName);
			sql::SQLString strNewSql = getSqlString(strSql);
			pSta->execute(strNewSql);

			CString strSqlEx;
			strSqlEx.Format(_T(" INSERT INTO `%s`.`%s` SELECT * FROM `%s`.`%s`"), realDestDBName, destTblName, realOrgDBName, orgTblName);

			strNewSql = getSqlString(strSqlEx);
			pSta->execute(strNewSql);
		}
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("复制表失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::CopyTableData(const CString& pDestTblName, const CString& pSelectSql)
{
	try
	{
		if (_tcslen(pSelectSql) == 0 || _tcslen(pDestTblName) == 0)
		{
			SQLlogger(_T("表名不能为空!"));
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("表名不能为空!"));
#endif
			return false;
		}
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());

		CString strSqlEx = _T(" INSERT INTO `") + pDestTblName + _T("` ") + pSelectSql;
		sql::SQLString strNewSql = getSqlString(strSqlEx);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("复制表数据失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::CopyTableDataByDefCols(const CString& pDestTblName, const CStringArray& colNames,
                                              const CString& pSelectSql)
{
	try
	{
		if (_tcslen(pSelectSql) == 0 || _tcslen(pDestTblName) == 0)
		{
			SQLlogger(_T("表名不能为空!"));
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(_T("表名不能为空!"));
#endif
			return false;
		}
		std::auto_ptr<sql::Statement> pSta(m_pCon->createStatement());

		CString strSqlEx = _T(" INSERT INTO `") + pDestTblName + _T("` (");

		for (int i = 0; i < colNames.GetSize(); i++)
		{
			strSqlEx += colNames[i];
			if (i != colNames.GetSize() - 1)
				strSqlEx += _T(",");
		}
		strSqlEx += _T(") ") + pSelectSql;

		sql::SQLString strNewSql = getSqlString(strSqlEx);
		pSta->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("复制表数据失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::InitReadCache()
{
	if (m_pCon == NULL)
		return false;
	if (m_pRes)
	{
		delete m_pRes;
		m_pRes = NULL;
		SQLlogger(_T("查询表未关闭！"));
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(_T("查询表未关闭！"));
#endif
	}
	if (m_pState)
	{
		delete m_pState;
		m_pState = NULL;
		SQLlogger(_T("查询表未关闭！"));
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(_T("查询表未关闭！"));
#endif
	}
	return true;
}

bool MysqlManager::CloseReadCache()
{
	if (m_pState)
	{
		m_pState->close();
		delete m_pState;
		m_pState = NULL;
	}
	if (m_pRes)
	{
		m_pRes->close();
		delete m_pRes;
		m_pRes = NULL;
	}
	return true;
}

int MysqlManager::ExecutSqlAndReturn(const CString& sqlScript)
{
	try
	{
		m_enMode = enTableRead;
		if (InitReadCache())
		{
			if (_tcslen(sqlScript) == 0) return false;

			m_pState = m_pCon->createStatement();
			CString str = sqlScript;
			str.Replace(_T("\\"),_T("\\\\"));
			sql::SQLString strNewSql = getSqlString(str);
			m_pRes = m_pState->executeQuery(strNewSql);
			sql::ResultSetMetaData* pResMet = m_pRes->getMetaData();
			if (pResMet)
				m_nExcueCols = pResMet->getColumnCount();
			return (int)m_pRes->rowsCount();
		}
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		// 		CString strError; 
		// 		strError.Format(_T("查询选择集失败!"));
		CloseReadCache();
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return -1;
	}
	return -1;
}

bool MysqlManager::CloseSqlRecords()
{
	return CloseReadCache();
}

bool MysqlManager::CloseWriteCache()
{
	bool isAutoCmmit = m_pCon->getAutoCommit();
	if (!isAutoCmmit)
		m_pCon->setAutoCommit(true);

	if (m_pPreMst)
	{
		m_pPreMst->close();
		delete m_pPreMst;
		m_pPreMst = NULL;
	}
	return true;
}

bool MysqlManager::connectSvc()
{
	try
	{
		if (m_pCon)
		{
			if (m_pCon->isValid())
			{
				return true;
			}
		}
		else
		{
			sql::SQLString strCnName = makeCnName();
			sql::SQLString strUserName = getSqlString(m_strUesrName);
			sql::SQLString strPassWord = getSqlString(m_strPassWord);
			m_pCon = m_pDriver->connect(strCnName, strUserName, strPassWord);
		}
		if (!openDb())
		{
			return false;
		}
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("连接数据库失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::disconnectSvc()
{
	try
	{
		if (m_pCon)
		{
			m_pCon->close();
		}
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("断开数据库连接失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::reconnectSvc()
{
	try
	{
		if (m_pCon)
		{
			if (m_pCon->isValid())
			{
				return true;
			}
			else
			{
				m_pCon->reconnect();
			}
		}
		else
		{
			return false;
		}
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("连接数据库失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::openDb()
{
	std::string strdbName;
	try
	{
		sql::SQLString strdbName = getSqlString(m_stdDbName);
		m_pCon->setSchema(strdbName);
	}

	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("连接数据库失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

bool MysqlManager::ExecutSql(const CString& sqlScript)
{
	try
	{
		if (_tcslen(sqlScript) == 0) return false;
		std::auto_ptr<sql::Statement> pStam(m_pCon->createStatement());
		sql::SQLString strNewSql = getSqlString(sqlScript);
		pStam->execute(strNewSql);
	}
	catch (const sql::SQLException& er)
	{
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("查询选择集失败!"));
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return false;
	}
	return true;
}

int MysqlManager::OpenTable(const CString& tableName, EnTableMode enM)
{
	return OpenTable(m_stdDbName, tableName, enM);
}

int MysqlManager::OpenTable(const CString& dbName, const CString& tableName, EnTableMode enM)
{
	if (dbName.IsEmpty() || tableName.IsEmpty())
		return -1;
	try
	{
		m_fieldNames.RemoveAll();
		m_filedTypes.RemoveAll();
		GetColsInfo(dbName, tableName, m_fieldNames, m_filedTypes);
		if (enM == enTableRead)
		{
			m_enMode = enTableRead;
			if (InitReadCache())
			{
				m_pState = m_pCon->createStatement();

				CString strQuery;
				strQuery.Format(_T("select * from `%s`.`%s`"), dbName, tableName);
				sql::SQLString strNewSql = getSqlString(strQuery);
				m_pRes = m_pState->executeQuery(strNewSql);
				sql::ResultSetMetaData* pResMet = m_pRes->getMetaData();
				if (pResMet)
					m_nExcueCols = pResMet->getColumnCount();
				return (int)m_pRes->rowsCount();
			}
		}
		else if (enM == enTableWrite)
		{
			m_enMode = enTableWrite;
			if (InitWriteCache())
			{
				CString strSql;
				strSql.Format(_T("select * from `%s`.`%s`"), dbName, tableName);
				sql::SQLString strNewSql = getSqlString(strSql);
				//step1: colms op
				m_pCon->setAutoCommit(false);
				sql::PreparedStatement* pPreStaTem = m_pCon->prepareStatement(strNewSql);
				sql::ResultSetMetaData* pResMet = pPreStaTem->getMetaData();
				int nCnt = pResMet->getColumnCount();
				m_nExcueCols = nCnt;
				//step2: insert op		
				strSql.Format(_T("insert into `%s`.`%s` values"), dbName, tableName);
				CString strSep = _T("(");

				for (int i = 1; i <= nCnt; i++)
				{
					strSep += _T("?");
					if (i != nCnt)
					{
						strSep += _T(",");
					}
				}
				strSep += _T(")");
				strSql += strSep;
				if (pPreStaTem)
				{
					delete pPreStaTem;
					pPreStaTem = NULL;
				}
				strNewSql = getSqlString(strSql);
				m_pPreMst = m_pCon->prepareStatement(strNewSql);
				return 0;
			}
		}
		else if (enM == enTableModify)
		{
			m_enMode = enTableModify;
			//更新记录集模式
			if (InitWriteCache())
			{
				CString strSql;
				strSql.Format(_T("select * from `%s`.`%s`"), dbName, tableName);
				//step1: colms op
				sql::SQLString strAllTabl = getSqlString(strSql);
				sql::PreparedStatement* pPreStaTem = m_pCon->prepareStatement(strAllTabl);
				sql::ResultSetMetaData* pResMet = pPreStaTem->getMetaData();
				int nCnt = pResMet->getColumnCount();
				m_nExcueCols = nCnt;
				//step2: insert op
				strSql.Format(_T("UPDATE `%s`.`%s` SET "), dbName, tableName);
				for (int i = 1; i < nCnt + 1; ++i)
				{
					CString strTmp;
					sql::SQLString name = pResMet->getColumnName(i);
					CByteCode::UTF_8ToUnicode(strTmp, (char*)name.c_str());
					strSql += strTmp + _T("=?");
					if (i != nCnt)
					{
						strSql += _T(",");
					}
				}
				strSql += _T(" where ID=");
				if (pPreStaTem)
				{
					delete pPreStaTem;
					pPreStaTem = NULL;
				}
				m_sqlScript = getSqlString(strSql);
				return 1;
			}
		}
	}
	catch (const sql::SQLException& er)
	{
		bool isAutoCmmit = m_pCon->getAutoCommit();
		if (!isAutoCmmit)
			m_pCon->setAutoCommit(true);
		//CString strError = getErrorInfo(er);
		//CString strError; 
		//strError.Format(_T("打开表失败!"));
		CloseTable();
		CString strError = getErrorInfo(er);
		SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
		AfxMessageBox(strError);
#endif
		return -1;
	}
	return -1;
}

int MysqlManager::OpenTable(const CString& dbName, const CString& tableName, const CStringArray& colName, EnTableMode enM)
{
	return OpenTable(dbName, tableName, enM);
}

bool MysqlManager::CloseTable()
{
	if (m_enMode == enTableWrite)
	{
		try
		{
			m_pCon->commit();
			CloseWriteCache();
			return true;
		}
		catch (const sql::SQLException& er)
		{
			//CString strError = getErrorInfo(er);
			//CString strError; 
			//strError.Format(_T("关闭表失败!"));
			CloseWriteCache();
			m_pCon->rollback();
			CString strError = getErrorInfo(er);
			SQLlogger(strError);
#if defined(_DEBUG)&&defined(_DEBUG_SQL)
			AfxMessageBox(strError);
#endif
			return false;
		}
	}
	else if (m_enMode == enTableRead)
	{
		return CloseReadCache();
	}
	else if (m_enMode == enTableModify)
	{
		return CloseWriteCache();
	}
	return false;
}

bool IUDB::MysqlManager::SQLlogger( const CString strContent,const CString strFile/*=_T("./zwdb.err")*/ )
{
	time_t curTime;  
	struct tm mt; 
	curTime = time(NULL);  
	errno_t rt = localtime_s(&mt,&curTime);

	CString strTmp;
	strTmp.Format(_T("[%d-%02d-%02d %02d:%02d:%02d] %s"),mt.tm_year+1900,mt.tm_mon+1,mt.tm_mday,mt.tm_hour,mt.tm_min,mt.tm_sec,strContent);
	CString strPath = strFile;
	FILE* file = NULL;
	 _tfopen_s(&file,(LPTSTR)(LPCTSTR)strPath, _T("a+"));
	if (file)
	{
		_ftprintf(file, _T("%s\n"), strTmp.GetBuffer());
		fclose(file);
		file = NULL;
	}
	return true;
}
