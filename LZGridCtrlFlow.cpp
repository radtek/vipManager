#include "stdafx.h"
#include "LZGridCtrlFlow.h"


LZGridCtrlFlow::LZGridCtrlFlow():
	LZGridCtrl(gFlow)
{
	m_strTblName = _T("manager_flow_temp");
}


LZGridCtrlFlow::~LZGridCtrlFlow()
{
}

bool LZGridCtrlFlow::Init()
{
	LZGridCtrl::Init();
	CStringArray strArryColName;
	strArryColName.Add(_T("序号"));
	strArryColName.Add(_T("工单号"));
	strArryColName.Add(_T("状态"));
	strArryColName.Add(_T("消费时间"));
	strArryColName.Add(_T("完成时间"));
	strArryColName.Add(_T("消费类型"));
	strArryColName.Add(_T("账户类型"));
	strArryColName.Add(_T("应收金额"));
	strArryColName.Add(_T("折扣"));
	strArryColName.Add(_T("实收金额"));
	strArryColName.Add(_T("支付类型"));
	strArryColName.Add(_T("发票"));
	strArryColName.Add(_T("发票号"));
	strArryColName.Add(_T("备注"));
	int nCol = strArryColName.GetSize();
	SetColumnCount(nCol);
	SetRowCount(1);
	for (int i = 0; i < nCol; i++)
	{
		CString strColNmae = strArryColName.GetAt(i);
		SetItemText(0, i, strColNmae);
		SetItemFormat(0, i, DT_CENTER);
	}
	updateCellSize(13,60,60,60,150,150,60,60,60,70,100,70,60,100);
	return true;
}

void LZGridCtrlFlow::getCellType(const int& nRow, const int& nCol, const std::vector<CString>& vecRowData, std::vector<CString>& vecopt)
{
	if (nRow < 1)
		return;

	readOnlyCell(GetCell(nRow, nCol));
}


void LZGridCtrlFlow::endCellType(const int& nRow, const int& nCol, const CString& strData)
{
	if (nRow < 1)
		return;
	CString strField = GetItemText(0, nCol);
	CString strVal = GetItemText(nRow, nCol);
	if (strField.CompareNoCase(_T("实收金额")) == 0 ||
		strField.CompareNoCase(_T("应收金额")) == 0)
	{
		double dv = _wtof(strVal);
		if (dv >= 0)
		{
			GetCell(nRow, nCol)->SetTextClr(RGB(255, 0, 0));
		}
		else
		{
			GetCell(nRow, nCol)->SetTextClr(RGB(0, 0, 255));
		}
		SetItemFormat(nRow, nCol, DT_RIGHT);
	}
	else if (strField.CompareNoCase(_T("折扣")) == 0)
	{
		SetItemFormat(nRow, nCol, DT_RIGHT);
	}
	else if (nCol == 1)
	{
		SetItemFormat(nRow, nCol, DT_CENTER);
	}
	else if (nCol == 2)
	{
		if (strVal.CompareNoCase(_T("已完成")) == 0)
		{
			GetCell(nRow, nCol)->SetTextClr(RGB(0, 190, 0));
		}
		else if (strVal.CompareNoCase(_T("进行中")) == 0)
		{
			GetCell(nRow, nCol)->SetTextClr(RGB(255, 0, 0));
		}
		else if (strVal.CompareNoCase(_T("未完成")) == 0)
		{
			GetCell(nRow, nCol)->SetTextClr(RGB(200, 200, 200));
		}
		SetItemFormat(nRow, nCol, DT_CENTER);
	}
}



void LZGridCtrlFlow::getCellData(std::list<std::pair<int, std::vector<CString>>>& listDataArry)
{
	CString strSql;
	listDataArry.clear();
	if (!theApp.GetDBCon())
		return;
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
			strData = theApp.GetDBCon()->GetString(_T("FLOW_ID"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("STATUS"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("TIME"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("ENDTIME"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("TYPE"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("PAYTYPE"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("VALUE"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("SALE"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("TOTAL"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("VALUETYPE"));	strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("RECEIPT"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("RECEIPTNUM"));	strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("REMARK"));		strArry.push_back(strData);
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
