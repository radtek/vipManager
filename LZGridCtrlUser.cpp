#include "stdafx.h"
#include "LZGridCtrlUser.h"

LZGridCtrlUser::LZGridCtrlUser():
	LZGridCtrl(gUser)
{
	m_strTblName = _T("customer_idx");
}


LZGridCtrlUser::~LZGridCtrlUser()
{
}

bool LZGridCtrlUser::Init()
{
	
	LZGridCtrl::Init();
	CStringArray strArryColName;
	strArryColName.Add(_T("序号"));
	strArryColName.Add(_T("ID"));
	strArryColName.Add(_T("姓名"));
	strArryColName.Add(_T("电话"));
	strArryColName.Add(_T("备用电话"));
	if (g_customSet._bShowBaby)
	{
		strArryColName.Add(_T("宝宝姓名"));
		strArryColName.Add(_T("宝宝性别"));
		strArryColName.Add(_T("宝宝年龄"));
	}
	if (g_customSet._bShowTime)
	{
		strArryColName.Add(_T("注册时间"));
	}
	if (g_customSet._bShowType)
	{
		strArryColName.Add(_T("会员类型"));
	}
	if (g_customSet._bShowScort)
	{
		strArryColName.Add(_T("积分"));
	}
	if (g_customSet._bShowBalance)
	{
		strArryColName.Add(_T("次数余额"));
		strArryColName.Add(_T("现金余额"));
	}

	int nCol = strArryColName.GetSize();
	SetColumnCount(nCol);
	SetRowCount(1);
	for (int i = 0; i < nCol; i++)
	{
		CString strColNmae = strArryColName.GetAt(i);
		SetItemText(0, i, strColNmae);
		SetItemFormat(0, i, DT_CENTER);
	}
	updateCellSize(10, 60,60,100,120,120,100,60,60,100,60);
	return true;
}

void LZGridCtrlUser::getCellType(const int& nRow, const int& nCol, const std::vector<CString>& vecRowData, std::vector<CString>& vecopt)
{
	if (nRow < 1)
		return;
	CString strField = GetItemText(0, nCol);
// 	if (nCol == 4)
// 	{
// 		SetItemFormat(nRow, nCol, DT_RIGHT);
// 		GetCell(nRow, nCol)->SetTextClr(RGB(255, 0, 0));
// 	}
// 	else if (nCol == 6)
// 	{
// 		SetItemFormat(nRow, nCol, DT_CENTER);
// 	}
	
	if (strField.CompareNoCase(_T("积分"))==0 || 
		strField.CompareNoCase(_T("次数余额")) == 0 ||
		strField.CompareNoCase(_T("现金余额")) == 0)
	{
		GetCell(nRow, nCol)->SetTextClr(RGB(255, 0, 0));
		SetItemFormat(nRow, nCol, DT_RIGHT);
	}
	readOnlyCell(GetCell(nRow, nCol));
}

void LZGridCtrlUser::getCellData(std::list<std::pair<int, std::vector<CString>>>& listDataArry)
{
	CString strSql;
	listDataArry.clear();
	strSql.Format(_T("SELECT * FROM `%s`.`%s` ORDER BY `ID`;"), MysqlManager::DBLZCustomer, m_strTblName);
	if (!g_customSet._sQfindPhone.IsEmpty())
	{
		strSql.Format(_T("SELECT * FROM `%s`.`%s` WHERE `PHONE1` LIKE '%s%%' ORDER BY `ID`;"), MysqlManager::DBLZCustomer, m_strTblName, g_customSet._sQfindPhone);
	}
	
	try
	{
		if (-1 == theApp.GetDBCon()->ExecutSqlAndReturn(strSql))
			throw 0;
		int nRow = 1;
		theApp.GetDBCon()->BeforeFirst();
		while (theApp.GetDBCon()->Next())
		{
			CString strData;
			std::vector<CString> strArry;
			int nID = theApp.GetDBCon()->GetInt32(_T("ID"));
			strData.Format(_T("%d"), nRow++);							strArry.push_back(strData);
			strData.Format(_T("%d"), nID);								strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("NAME"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("PHONE1"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("PHONE2"));		strArry.push_back(strData);
			if (g_customSet._bShowBaby)
			{
				strData = theApp.GetDBCon()->GetString(_T("BABYNAME"));		strArry.push_back(strData);
				strData = theApp.GetDBCon()->GetString(_T("BABYSEX"));		strArry.push_back(strData);
				strData = theApp.GetDBCon()->GetString(_T("BABYAGE"));		strArry.push_back(strData);
			}
			if (g_customSet._bShowTime)
			{
				strData = theApp.GetDBCon()->GetString(_T("REGTIME"));		strArry.push_back(strData);
			}
			if (g_customSet._bShowType)
			{
				strData = theApp.GetDBCon()->GetString(_T("CARDTYPE"));		strArry.push_back(strData);
			}
			if (g_customSet._bShowScort)
			{
				strData = theApp.GetDBCon()->GetString(_T("SCORE"));		strArry.push_back(strData);
			}
			if (g_customSet._bShowBalance)
			{
				strData = theApp.GetDBCon()->GetString(_T("BALANCE_COUNT")); strArry.push_back(strData);
				strData = theApp.GetDBCon()->GetString(_T("BALANCE_MONEY")); strArry.push_back(strData);
			}
			listDataArry.push_back(std::make_pair(nID, strArry));
		}
		theApp.GetDBCon()->CloseSqlRecords();
	}
	catch (...)
	{
		theApp.GetDBCon()->CloseSqlRecords();
		return;
	}
}
