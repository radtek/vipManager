#include "stdafx.h"
#include "LZGridCtrlReport.h"

LZGridCtrlReport::LZGridCtrlReport():
	LZGridCtrl(gGoods)
{
	m_strTblName = _T("manager_goods");
}


LZGridCtrlReport::~LZGridCtrlReport()
{
}

bool LZGridCtrlReport::Init()
{
	
	LZGridCtrl::Init();
	CStringArray strArryColName;
	strArryColName.Add(_T("序号"));
	strArryColName.Add(_T("商品名称"));
	strArryColName.Add(_T("商品类型"));
	if (g_goodsSet._bShowIntoPre)
	{
		strArryColName.Add(_T("商品进价"));
	}
	strArryColName.Add(_T("商品售价"));
	strArryColName.Add(_T("商品库存"));
	strArryColName.Add(_T("商品编码"));
	strArryColName.Add(_T("商品备注"));

	int nCol = strArryColName.GetSize();
	SetColumnCount(nCol);
	SetRowCount(1);
	for (int i = 0; i < nCol; i++)
	{
		CString strColNmae = strArryColName.GetAt(i);
		SetItemText(0, i, strColNmae);
		SetItemFormat(0, i, DT_CENTER);
	}
	updateCellSize(8, 60,120,70,60,60,60,100,150);
	return true;
}

void LZGridCtrlReport::getCellType(const int& nRow, const int& nCol, const std::vector<CString>& vecRowData, std::vector<CString>& vecopt)
{
	if (nRow < 1)
		return;
	CString strField = GetItemText(0, nCol);
	
	if (strField.CompareNoCase(_T("商品进价"))==0 || 
		strField.CompareNoCase(_T("商品售价")) == 0)
	{
		GetCell(nRow, nCol)->SetTextClr(RGB(255, 0, 0));
		SetItemFormat(nRow, nCol, DT_RIGHT);
	}
	readOnlyCell(GetCell(nRow, nCol));
}

void LZGridCtrlReport::endCellType(const int& nRow, const int& nCol, const CString& strData)
{
	if (nRow < 1)
		return;
	CString strField = GetItemText(0, nCol);
	if (strField.CompareNoCase(_T("商品库存")) == 0)
	{
		double dTotle = _ttof(strData);
		int nCl = GetColumnCount();
		for (int i = 1; i < nCl ; ++i)
		{
			if (dTotle < 1)
			{
				// 库存空了 红色
				GetCell(nRow, i)->SetBackClr(RGB(250, 0, 0));
			}
			else if (dTotle < 10)
			{
				// 库存不足 黄色
				GetCell(nRow, i)->SetBackClr(RGB(200, 200, 0));
			}
			else
			{
				GetCell(nRow, i)->SetBackClr(RGB(255, 255, 255));
			}
		}
	}
}


void LZGridCtrlReport::getCellData(std::list<std::pair<int, std::vector<CString>>>& listDataArry)
{
	CString strSql;
	listDataArry.clear();
	strSql.Format(_T("SELECT * FROM `%s`.`%s` ORDER BY `ID`;"), MysqlManager::DBLZManager, m_strTblName);
	
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
			strData = theApp.GetDBCon()->GetString(_T("TITLE"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("TYPE"));		strArry.push_back(strData);
			if (g_goodsSet._bShowIntoPre)
			{
				strData = theApp.GetDBCon()->GetString(_T("INTOPRICE"));		strArry.push_back(strData);
			}
			strData = theApp.GetDBCon()->GetString(_T("PRICE"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("TOTAL"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("CODENUM"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("INFO"));		strArry.push_back(strData);
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

