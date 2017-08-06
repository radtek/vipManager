#include "stdafx.h"
#include "LZGridCtrlFlow.h"


LZGridCtrlFlow::LZGridCtrlFlow():
	LZGridCtrl(gFlow)
{
	m_strTblName = _T("manager_flow");
}


LZGridCtrlFlow::~LZGridCtrlFlow()
{
}

bool LZGridCtrlFlow::Init()
{
	LZGridCtrl::Init();
	CStringArray strArryColName;
	strArryColName.Add(_T("序号"));
	strArryColName.Add(_T("操作类型"));
	strArryColName.Add(_T("会员账户"));
	strArryColName.Add(_T("操作时间"));
	strArryColName.Add(_T("金额(人民币)"));
	strArryColName.Add(_T("金额类型"));
	strArryColName.Add(_T("开发票"));
	strArryColName.Add(_T("发票号"));
	strArryColName.Add(_T("备注"));
	int nCol = strArryColName.GetSize();
	SetColumnCount(nCol);
	SetRowCount(3);
	for (int i = 0; i < nCol; i++)
	{
		CString strColNmae = strArryColName.GetAt(i);
		SetItemText(0, i, strColNmae);
		SetItemFormat(0, i, DT_CENTER);
	}
	updateCellSize(9, 60,100,60,180,80,60,60,150,200);
	return true;
}

void LZGridCtrlFlow::getCellType(const int& nRow, const int& nCol, const std::vector<CString>& vecRowData, std::vector<CString>& vecopt)
{
	if (nRow < 1)
		return;
	if (nCol == 4)
	{
		SetItemFormat(nRow, nCol, DT_RIGHT);
		GetCell(nRow, nCol)->SetTextClr(RGB(255, 0, 0));
	}
	else if (nCol == 6)
	{
		SetItemFormat(nRow, nCol, DT_CENTER);
	}
	readOnlyCell(GetCell(nRow, nCol));
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
			strData = theApp.GetDBCon()->GetString(_T("TYPE"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("CARDNUM"));		strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("TIME"));			strArry.push_back(strData);
			strData = theApp.GetDBCon()->GetString(_T("VALUE"));		strArry.push_back(strData);
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
