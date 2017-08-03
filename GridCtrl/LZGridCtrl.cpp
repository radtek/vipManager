#include "stdafx.h"
#include "LZGridCtrl.h"
// #include "ZWAddRowsDlg.h"
// #include "ZWInsertRowsDlg.h"
#include "GridCellColorButton.h"
#include "GridCellCombo.h"


IMPLEMENT_DYNAMIC(LZGridCtrl, CGridCtrl)

CMFCRibbonStatusBar *LZGridCtrl::m_pStatusBar = NULL;
CProgressCtrl *LZGridCtrl::m_pProGress = NULL;
int LZGridCtrl::m_nProStep = 0;

LZGridCtrl::LZGridCtrl(void)
{
	m_gType = gNONE;
	m_pView = NULL;
	m_nLastInsert = 0;
	m_nIndexNum = 0;
	m_vecDel.clear();
}

LZGridCtrl::LZGridCtrl( GridType gtp )
{
	m_gType = gtp;
	m_pView = NULL;
	m_nLastInsert = 0;
	m_nIndexNum = 0;
	m_vecDel.clear();
}
BEGIN_MESSAGE_MAP(LZGridCtrl, CGridCtrl)
	//ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

LZGridCtrl::~LZGridCtrl(void)
{
	m_vecDel.clear();
}

bool LZGridCtrl::Init()
{
	DeleteAllItems();
	SetEditable(true);						// 是否可编辑
	SetTextBkColor(RGB(255, 255, 255));	// Cell背景
	SetGridColor(RGB(80,130,170));			// 表格网线的颜色
	SetFixedTextColor(RGB(0,0,64));		// 字段颜色
	SetBkColor(RGB(250,250,250));			// 背景颜色
	SetFixedRowCount(1);					// 表头为一行
	SetFixedColumnCount(1);					// 表头为一列
	SetRowResize(FALSE);					// 是否可以改变行高
	
	updateCellSize(1,60);
	return true;
}

void LZGridCtrl::ClearGrid()
{
	CCellID cell = GetFocusCell();
	CGridCellBase *pCell = GetCell(cell.row, cell.col);
	if (pCell)
		pCell->EndEdit();
	delete this;
}

bool LZGridCtrl::CreateGrid(CWnd* parent,UINT nID)
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!Create(rectDummy,parent,nID,dwViewStyle))
	{
		TRACE0("未能创建表格视图\n");
		return FALSE;      // 未能创建
	}
	ShowWindow(SW_SHOW);
	m_pView = parent;
	AdjustLayout();
	return TRUE;
}

void LZGridCtrl::AdjustLayout()
{
	if (!m_pView || GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	m_pView->GetClientRect(rectClient);

	SetWindowPos(NULL, rectClient.left , rectClient.top,
		rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER/* | SWP_NOSIZE*/);
}

int LZGridCtrl::GetTableRowCount( const CString& strTbname )
{
	CString mysql;
	int nRet = 0;
	mysql.Format(_T("SELECT COUNT(*) FROM `%s`;"),strTbname);
	if (-1 == theApp.GetDBCon()->ExecutSqlAndReturn(mysql))
	{
		theApp.GetDBCon()->CloseSqlRecords();
		return nRet;
	}
	theApp.GetDBCon()->BeforeFirst();
	if (theApp.GetDBCon()->Next())
	{
		nRet = theApp.GetDBCon()->GetInt32(1);
	}
	theApp.GetDBCon()->CloseSqlRecords();
	return nRet;
}

bool LZGridCtrl::AddRow()
{
	InsertRowEx();
	return TRUE;
}

bool LZGridCtrl::InsRow()
{
	int row = GetFocusCell().row;
	if (!IsRowValid(row))
	{
		AfxMessageBox(_T("请选中行！"));
		return FALSE;
	}
	InsertRowEx(row);
	return TRUE;
}

bool LZGridCtrl::EmpRow()
{
	CCellRange Selection = GetSelectedCellRange();

	int iMinRow = Selection.GetMinRow(); 
	int iMaxRow = Selection.GetMaxRow();

	//设置清空进度
	CProgressCtrl *pProGress = NULL;

	if (!GetPrg(pProGress))
		return FALSE;
	pProGress->SetRange(0,iMaxRow-iMinRow);

	int i = 1;
	for (int row = iMinRow; row <= iMaxRow; row++)
	{
		pProGress->SetPos(i);
		clearRow(row);
		SetFlagEdit(row);
		i++;
	}
	pProGress->SetPos(0);
	Invalidate();
	return TRUE;
}

bool LZGridCtrl::DelRow()
{
	CProgressCtrl *pProGress = NULL;
	if (!GetPrg(pProGress))
		return FALSE;
	CCellRange Selection = GetSelectedCellRange();
	int iMinRow = Selection.GetMinRow();
	int iMaxRow = Selection.GetMaxRow();
	if (iMaxRow < 0)
	{
		AfxMessageBox(_T("请选择要删除的行！"),MB_ICONEXCLAMATION);
		return FALSE;
	}
	int iPos = 1;
	pProGress->SetRange(0,iMaxRow-iMinRow);
	for (int nRow = iMinRow;  nRow <= iMaxRow; nRow++)
	{
		DeleteRowEx(iMinRow);
		pProGress->SetPos(iPos++);
	}
	pProGress->SetPos(0);
	Invalidate();
	return TRUE;
}

bool LZGridCtrl::AddMultiRow()
{
// 	int nRow = GetRowCount();
// 	CZWAddRowsDlg ZWAddRowsDLG(this);
// 	ZWAddRowsDLG.GetuporDown(nRow);;
// 	ZWAddRowsDLG.DoModal();
	return TRUE;
}

bool LZGridCtrl::Save()
{
	return submit();
}


bool LZGridCtrl::InsMultiRow(std::vector<int>& vecRows)
{
// 	ZWInsertRowsDlg dlg;
// 	if (dlg.DoModal() != IDOK)
// 		return TRUE;
// 	vecRows.clear();
// 	int nCount = dlg.GetRowsCount();
// 
// 	int selRow = GetFocusCell().row;
// 	if (!IsRowValid(selRow))
// 	{
// 		AfxMessageBox(_T("请选中行！"));
// 		return FALSE;
// 	}
// 
// 	for (int i = 0; i < nCount ; ++i)
// 	{
// 		InsertRowEx(selRow+i);
// 		vecRows.push_back(GetlastInsert());
// 	}
	return TRUE;
}

BOOL LZGridCtrl::AddRows(const int& nRows,const BOOL& bUp)
{
	StatBarBegin(_T("正在插入表格..."),nRows);
	if (bUp)
	{
		for (int i = 0; i < nRows ; ++i)
		{
			StatBarStep();
			InsertRowEx(i+1);
		}
	}
	else
	{
		for (int i = 0; i < nRows ; ++i)
		{
			StatBarStep();
			InsertRowEx();
		}
	}
	StatBarEnd();
	return TRUE;
}


bool LZGridCtrl::IsChanged(BOOL bDelSame)
{
	// 是否有删除
	bool bchenge = false;
	if (GetVecDel().size()>0)
	{
		bchenge = TRUE;
	}
	int nRowCount = GetRowCount();
	for (int i = 0; i < nRowCount ; ++i)
	{
		int nf = GetItemFlag(i);
		// 是否有修改、增加
		if (nf==4 || nf==3)
		{
			bchenge = TRUE;
			break;
		}
	}
	if (bchenge)
	{
		if (IDYES==MessageBox(_T("数据已修改，是否保存？"),NULL,MB_YESNO|MB_ICONQUESTION ))
		{
			Save();
		}
	}
	return TRUE;
}

bool LZGridCtrl::GetPrg( CProgressCtrl*& pProGress )
{
	CMainFrame *pMainFram=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pMainFram)
	{
		pProGress = pMainFram->getPrgGressCtrl();
	}
	if (!pProGress )
		return FALSE;
	pProGress->SetStep(1);
	return TRUE;
}

int LZGridCtrl::SetItemTextEx(int nRow, int nCol, LPCTSTR str,int nFlag)
{
	SetItemText(nRow,nCol,str);

	if (nFlag == 1)
	{
		SetItemFlag(nRow,1);
	}
	else if (nFlag == 3)
	{
		SetItemFlag(nRow,3);
	}
	else if (nFlag == 4)
	{
		SetItemFlag(nRow,4);
	}

	if (m_nIndexNum <= nRow)
	{
		m_nIndexNum = nRow+1;
	}
	return nRow;
}

int LZGridCtrl::InsertRowEx(int nRow /*= -1*/ )
{
	CString strHead;
	strHead.Format(_T("%d"),m_nIndexNum++);
	m_nLastInsert = InsertRow(strHead,nRow);
	if (m_nLastInsert==-1)
		return -1;
	SetFlagAdd(m_nLastInsert);
	updateCellType(m_nLastInsert);
	Invalidate();
	return m_nIndexNum-1;
}

bool LZGridCtrl::DeleteRowEx( int nRow )
{
	int nf = GetItemFlag(nRow);
	if (nf==1 || nf==4)
	{
		int nId = GetItemMapID(nRow);
		m_vecDel.push_back(nId);
	}
	DeleteRow(nRow);
	return TRUE;
}


void LZGridCtrl::SetItemMapID( int nRow,int nMapID )
{
	long ndata = GetItemData(nRow,0);
	long nId   = ndata>>8;
	long nFl   = ndata - (nId<<8);

	nId = nMapID;
	ndata = (nId<<8) + nFl;
	SetItemData(nRow,0,ndata);
}

void LZGridCtrl::SetItemFlag( int nRow,int nFlag )
{
	long ndata = GetItemData(nRow,0);
	long nId   = ndata>>8;
	long nFl   = ndata - (nId<<8);

	nFl = nFlag;
	ndata = (nId<<8) + nFl;
	SetItemData(nRow,0,ndata);
}

int LZGridCtrl::GetItemMapID( int nRow )
{
	long ndata = GetItemData(nRow,0);
	long nId   = ndata>>8;
	long nFl   = ndata - (nId<<8);

	return (int)nId;
}

int LZGridCtrl::GetItemFlag( int nRow )
{
	long ndata = GetItemData(nRow,0);
	long nId   = ndata>>8;
	long nFl   = ndata - (nId<<8);

	return (int)nFl;
}

bool LZGridCtrl::clearCol(int nCol)
{
	int nSize = GetColumnCount();
	for (int i = 1; i < nSize; i++)
	{
		CGridCellBase* pCell = GetCell(i, nCol);
		if (!pCell)
		{
			return FALSE;
		}
		pCell->SetText(_T(""));
	}
	return TRUE;
}

bool LZGridCtrl::clearRow(int nRow)
{
	int nSize = GetColumnCount();
	for (int i = 1; i < nSize; i++)
	{
		CGridCellBase* pCell = GetCell(nRow, i);
		if (!pCell)
		{
			return FALSE;
		}
		pCell->SetText(_T(""));
	}
	return TRUE;
}

void LZGridCtrl::readOnlyCell(CGridCellBase* cel, bool bEn /*= true*/)
{
	if (!cel)
		return;
	DWORD_PTR dstat = cel->GetState();
	if (bEn)
	{
		dstat = dstat | GVIS_READONLY;
	}
	else
	{
		dstat = dstat & (~GVIS_READONLY);
	}
	cel->SetState(dstat);
}

// 不包含索引ID项 字符串被''包含
int LZGridCtrl::GetLineText( int nRow,CString& strRes)
{
	strRes.Empty();
	int nC = GetColumnCount();
	for (int i = 1; i < nC ; ++i)
	{
		CString strTemp;
		strTemp.Format(_T("'%s'"),getCellStr(nRow,i));
		if (!strRes.IsEmpty())
		{
			strRes += _T(",");
		}
		strRes += strTemp;

	}
	return nC-1;
}

bool LZGridCtrl::StatBarBegin( const CString& strText,int nRange )
{
	CMainFrame *pMainFram=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pMainFram)
	{
		m_pProGress = pMainFram->getPrgGressCtrl();
		m_pStatusBar = pMainFram->getStatusBar();
	}
	if (!m_pProGress )
		return false;
	if (!m_pStatusBar)
		return false;
	CMFCRibbonBaseElement* pElement = m_pStatusBar->GetElement(0);

	if (!strText.IsEmpty())
	{
		pElement->SetText(strText);
	}
	m_pProGress->SetRange(0,nRange);
	m_pProGress->SetPos(0);
	pElement->Redraw();
	return true;
}

bool LZGridCtrl::StatBarStep()
{
	if (m_pProGress)
	{
		m_pProGress->SetPos(++m_nProStep);
		return true;
	}
	return false;
}

bool LZGridCtrl::StatBarEnd( const CString& strText )
{
	m_pProGress->SetPos(0);
	CMFCRibbonBaseElement* pElement = m_pStatusBar->GetElement(0);
	if (strText.IsEmpty())
	{
		pElement->SetText(_T("操作显示进度条"));
	}
	else
	{
		pElement->SetText(strText);
	}
	pElement->Redraw();

	m_pProGress = NULL;
	m_pStatusBar= NULL;
	m_nProStep  = 0;
	return true;
}


void LZGridCtrl::OnEditCell( int nRow, int nCol, CPoint point, UINT nChar )
{
	CGridCtrl::OnEditCell(nRow,nCol,point,nChar);
}


void LZGridCtrl::OnEndEditCell( int nRow, int nCol, CString str )
{
	if (GetItemText(nRow, nCol) != str) // 新增加的(3) 不会被标记为修改
	{
		SetFlagEdit(nRow);
	}
	CGridCtrl::OnEndEditCell(nRow,nCol,str);
}

bool LZGridCtrl::SetFlagNormal( int nRow )
{
	SetItemFlag(nRow,1);
	return true;
}

bool LZGridCtrl::SetFlagAdd( int nRow )
{
	SetItemFlag(nRow,3);
	return true;
}

bool LZGridCtrl::SetFlagEdit( int nRow )
{
	if (GetItemFlag(nRow)==1)
	{
		SetItemFlag(nRow,4);
	}
	return true;
}


void LZGridCtrl::updateCellType(int nRow)
{
	int nC = GetColumnCount();
	for (int i = 0; i < nC ; ++i)
	{
		std::vector<CString> vecRowData;
		std::vector<CString> vecopt;
		getCellType(nRow,i,vecRowData,vecopt);
		CGridCell* pCell= dynamic_cast< CGridCell*>(GetCell(nRow,i));
		if (pCell)
		{
			CGridCellColorButton* pCellColor = dynamic_cast< CGridCellColorButton*>(pCell);
			CGridCellCombo*       pCellComb  = dynamic_cast< CGridCellCombo*>(pCell);
			if (pCellColor)
			{
				;
			}
			else if (pCellComb)
			{
				pCellComb->SetOptions(vecopt);
			}
		}
	}
}

bool LZGridCtrl::updateCellSize(int ncWidth,...)
{
	va_list vl = 0;
	va_start(vl,ncWidth);
	std::vector<int> vecWidth;
	int nLast = 100;
	for (int i = 0; i < ncWidth ; ++i)
	{
		nLast = va_arg(vl,int);
		vecWidth.push_back(nLast);
	}
	va_end(vl); 
	int nRow = GetRowCount();
	if (nRow<1)
		return false;
	AutoSize();
	int nCol = GetColumnCount();
	int nHight = GetRowHeight(0);
	for (int i = 0; i < nCol ; ++i)
	{
		if (i<ncWidth)
		{
			if (GetColumnWidth(i)<vecWidth[i])
			{
				SetColumnWidth(i,vecWidth[i]);
			}
		}
		else
		{
			if (GetColumnWidth(i)<nLast)
			{
				SetColumnWidth(i,nLast);
			}
		}

	}
	// 如果行中没有数据的话autosize会显示最小的行高 应该显示和首行等高的
	for (int i = 0; i < nRow ; ++i)
	{
		SetRowHeight(i,nHight);
	}
	return true;
}

void LZGridCtrl::DbClickGrid( DWORD wp,DWORD lp )
{

}

CString LZGridCtrl::getCellStr( const int& nRow,const int& nCol )
{
	return GetItemText(nRow,nCol);
}
void LZGridCtrl::getCellType( const int& nRow,const int& nCol ,const std::vector<CString>& vecRowData,std::vector<CString>& vecopt)
{
	return;
}

void LZGridCtrl::getCellData( std::list<std::pair<int,std::vector<CString>>>& listDataArry )
{
	return;
}

bool LZGridCtrl::submit()
{
	int nRowCount = GetRowCount();
	try
	{
		CString strMySql;
		CString strNum;
		// 删除
		std::vector<int>& vecDel = GetVecDel();
		if (vecDel.size()>0)
		{
			StatBarBegin(_T("正在删除数据..."),1);
			StatBarStep();
			submit_del(vecDel);
			vecDel.clear();
		}

		// 修改
		StatBarBegin(_T("正在保存修改..."),nRowCount-1);
		for (int i = 1; i < nRowCount ; ++i)
		{
			StatBarStep();
			CString strField;
			int nf = GetItemFlag(i);
			if (nf==4&&GetLineText(i,strField))
			{
				int nId = GetItemMapID(i);
				if(!submit_edt(nId,strField,i))
					continue;
				SetItemFlag(i,1);
			}
		}
		// 增加
		StatBarBegin(_T("正在提交数据..."),nRowCount-1);
		int nLastInsertID = -1;
		for (int i = 1; i < nRowCount ; ++i)
		{
			StatBarStep();
			int nf = GetItemFlag(i);
			CString strField;
			if (nf==3&&GetLineText(i,strField))
			{
				if(!submit_add(strField))
					continue;
				if (nLastInsertID==-1)
				{
					strMySql.Format(_T("SELECT  LAST_INSERT_ID() AS LASTID;"));
					if (-1 == theApp.GetDBCon()->ExecutSqlAndReturn(strMySql))
					{
						theApp.GetDBCon()->CloseSqlRecords();
						throw;
					}
					theApp.GetDBCon()->BeforeFirst();
					if(theApp.GetDBCon()->Next())
					{
						nLastInsertID = theApp.GetDBCon()->GetInt32(_T("LASTID"));
					}
					theApp.GetDBCon()->CloseSqlRecords();
				}
				else if(nLastInsertID>0)
				{
					nLastInsertID++;
				}
				ASSERT(nLastInsertID>0);
				SetItemFlag(i,1);
				SetItemMapID(i,nLastInsertID);
			}
		}
		StatBarEnd();
	}
	catch (...)
	{
		AfxMessageBox(_T("保存失败!"));
		return FALSE;
	}
	return TRUE;
}


void LZGridCtrl::show()
{
	Init();
	std::list<std::pair<int,std::vector<CString>>> listDataArry;
	getCellData(listDataArry);
	int nRow = listDataArry.size();
	int nCol = GetColumnCount();
	SetRowCount(nRow+1);
	nRow = 1;
	for each ( std::pair<int,std::vector<CString>> var in listDataArry)
	{
		for (int j = 0; j < nCol ; ++j)
		{
			std::vector<CString> vecopt;
			getCellType(nRow,j,var.second,vecopt);
			CGridCell* pCell= dynamic_cast< CGridCell*>(GetCell(nRow,j));
			if (pCell)
			{
				CGridCellColorButton* pCellColor = dynamic_cast< CGridCellColorButton*>(pCell);
				CGridCellCombo*       pCellComb  = dynamic_cast< CGridCellCombo*>(pCell);
				if (pCellColor)
				{
					COLORREF cl = _wtoi(var.second[j]);
					pCellColor->SetColor(cl);
				}
				else if (pCellComb)
				{
					pCellComb->SetOptions(vecopt);
					SetItemTextEx(nRow,j,var.second[j]);
				}
				else
				{
					SetItemTextEx(nRow,j,var.second[j]);
				}
			}
		}
		SetItemMapID(nRow,var.first); // 数据库mapID映射
		++nRow;
	}
}

void LZGridCtrl::SetEndEdit()
{
	CCellID cell = GetFocusCell();
	CGridCellBase* pCell = GetCell(cell.row, cell.col);
	if (pCell)
		pCell->EndEdit();
	this->SetFocus();
}


