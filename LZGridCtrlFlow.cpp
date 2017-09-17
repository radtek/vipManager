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
	strArryColName.Add(_T("单号"));
	strArryColName.Add(_T("状态"));
	strArryColName.Add(_T("消费类型"));
	strArryColName.Add(_T("客户类型"));
	strArryColName.Add(_T("消费时间"));
	strArryColName.Add(_T("完成时间"));
	strArryColName.Add(_T("实收金额"));
	strArryColName.Add(_T("支付类型"));
	strArryColName.Add(_T("操作员"));
	strArryColName.Add(_T("流水号"));
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
	updateCellSize(11,60,60,60,60,150,150,70,70,100,70,60);
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
	CString strVal = strData;
	CString strStatus = GetItemText(nRow, 1);
	if (strField.CompareNoCase(_T("实收金额")) == 0)
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
	else if (nCol == 1 || nCol == 2)
	{
		SetItemFormat(nRow, nCol, DT_CENTER);

		if (strStatus.CompareNoCase(_T("已完成")) == 0)
			GetCell(nRow, 1)->SetBackClr(RGB(200, 200, 200));
	}

	if (strStatus.CompareNoCase(_T("已完成")) == 0)
	{
		GetCell(nRow, 1)->SetTextClr(RGB(0, 190, 0));
		GetCell(nRow, nCol)->SetBackClr(RGB(200, 200, 200));
	}
	else if (strStatus.CompareNoCase(_T("进行中")) == 0)
	{
		GetCell(nRow, 1)->SetTextClr(RGB(255, 0, 0));
	}
	else if (strStatus.CompareNoCase(_T("未完成")) == 0)
	{
		GetCell(nRow, 1)->SetTextClr(RGB(100, 100, 100));
	}

}



void LZGridCtrlFlow::getCellData(std::list<std::pair<int, std::vector<CString>>>& listDataArry)
{
	CString strSql;
	listDataArry.clear();
	if (!theApp.GetDBCon())
		return;
	strSql.Format(_T("SELECT * FROM `%s`.`%s` ORDER BY `CUSNUM`;"), MysqlManager::DBLZManager, m_strTblName);
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
			int nID = theApp.GetDBCon()->GetInt32(_T("CUSNUM"));
			//strData.Format(_T("%d"), nRow++);							strArry.push_back(strData);
			strData.Format(_T("%d"), nID);							strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("STATUS"));
			if (strData.CompareNoCase(_T("1")) == 0)
			{
				strArry.push_back(_T("已完成"));
				if (!g_flowSet._bComplite)
					continue;
			}
			else if (strData.CompareNoCase(_T("2")) == 0)
			{
				strArry.push_back(_T("进行中"));
				if (!g_flowSet._bRuning)
					continue;
			}
			else
				strArry.push_back(_T("交易失败"));
			strData = theApp.GetDBCon()->GetString(_T("TYPE"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("PAYTYPE"));
			if (strData.CompareNoCase(_T("0")) == 0)
				strArry.push_back(_T("普通"));
			else
				strArry.push_back(_T("会员"));

			strData = theApp.GetDBCon()->GetString(_T("TIME"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("ENDTIME"));		strArry.push_back(strData);

			strData = theApp.GetDBCon()->GetString(_T("TOTAL"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("VALUETYPE"));	strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("WORKER"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("FLOW_ID"));		strArry.push_back(strData);
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
