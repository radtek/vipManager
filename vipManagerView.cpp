// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// vipManagerView.cpp : CvipManagerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "vipManager.h"
#endif

#include "vipManagerDoc.h"
#include "vipManagerView.h"

#include "MysqlManager.h"

#include "LZGridCtrlFlow.h"
#include "LZGridCtrlUser.h"

#include "CustomAddDlg.h"
#include "FlowListDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  IDC_GRIDCTRL 1214
// CvipManagerView


IMPLEMENT_DYNCREATE(CvipManagerView, CFormView)

BEGIN_MESSAGE_MAP(CvipManagerView, CFormView)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CvipManagerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BTN_CUSTOMER, &CvipManagerView::OnBtnCustomer)
	ON_WM_SIZE()
	ON_COMMAND(ID_BTN_MANAGER_FLOW, &CvipManagerView::OnBtnManagerFlow)
	ON_COMMAND(ID_BTN_CUSTOM_FIND, &CvipManagerView::OnBtnCustomFind)
	ON_COMMAND(ID_BTN_CUSTOM_ADD, &CvipManagerView::OnBtnCustomAdd)

	ON_COMMAND(ID_CHECK_CUS_SHOW_BABY, &CvipManagerView::OnCheckCusShowBaby)
	ON_COMMAND(ID_CHECK_CUS_SHOW_TYPE, &CvipManagerView::OnCheckCusShowType)
	ON_COMMAND(ID_CHECK_CUS_SHOW_TIME, &CvipManagerView::OnCheckCusShowTime)
	ON_COMMAND(ID_CHECK_CUS_SHOW_SCORT, &CvipManagerView::OnCheckCusShowScort)
	ON_COMMAND(ID_CHECK_CUS_SHOW_BALANCE, &CvipManagerView::OnCheckCusShowBalance)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CUS_SHOW_BABY, &CvipManagerView::OnUpdateCheckCusShowBaby)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CUS_SHOW_TYPE, &CvipManagerView::OnUpdateCheckCusShowType)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CUS_SHOW_TIME, &CvipManagerView::OnUpdateCheckCusShowTime)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CUS_SHOW_SCORT, &CvipManagerView::OnUpdateCheckCusShowScort)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CUS_SHOW_BALANCE, &CvipManagerView::OnUpdateCheckCusShowBalance)
	ON_COMMAND(ID_EDIT_QFIND_PHONE, &CvipManagerView::OnEditQfindPhone)
	ON_NOTIFY(NM_DBLCLK, IDC_GRIDCTRL, &CvipManagerView::OnDblclkGrid)
	ON_COMMAND(ID_BTN_CUSM_SHOW, &CvipManagerView::OnBtnCusmShow)
	ON_COMMAND(ID_BTN_CUSM_EDIT, &CvipManagerView::OnBtnCusmEdit)
END_MESSAGE_MAP()

// CvipManagerView 构造/析构

CvipManagerView::CvipManagerView()
	: CFormView(IDD_DLG_VIPMANAGER_FROM_VIEW)
{
	// TODO: 在此处添加构造代码
	m_pGridView = NULL;
}

CvipManagerView::~CvipManagerView()
{
}

BOOL CvipManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CFormView::PreCreateWindow(cs);
}


// CvipManagerView 打印

void CvipManagerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


void CvipManagerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CvipManagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}


void CvipManagerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CvipManagerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CvipManagerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

	//
	if (!CreatGridView(LZGridCtrl::gFlow))
		return ;


}
// CvipManagerView 诊断

#ifdef _DEBUG
void CvipManagerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CvipManagerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CvipManagerDoc* CvipManagerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CvipManagerDoc)));
	return (CvipManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CvipManagerView 消息处理程序


void CvipManagerView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_pGridView)
	{
		m_pGridView->AdjustLayout();
	}
	// TODO: 在此处添加消息处理程序代码
}

void CvipManagerView::OnBtnCustomer()
{
	// TODO: 顾客管理 按钮
	AfxMessageBox(_T("呵呵呵"));
}




void CvipManagerView::OnBtnManagerFlow()
{
	// TODO: 工作流 按钮
	AfxInitRichEdit();
	CFlowListDlg fdlg;
	if (fdlg.DoModal() == IDOK)
	{
		;
	}

}



void CvipManagerView::OnBtnCustomFind()
{
	// TODO: 会员管理 查找 按钮
	AfxMessageBox(_T("会员管理 查找 按钮"));
}


void CvipManagerView::OnBtnCustomAdd()
{
	// TODO: 会员管理 添加 按钮
	CCustomAddDlg adDlg;
	if (adDlg.DoModal() == IDOK)
	{
		AfxMessageBox(_T("添加成功!"));
		CreatGridView(m_pGridView->gType());
	}
}



// 
BOOL CvipManagerView::CreatGridView(LZGridCtrl::GridType gtp)
{
	if (m_pGridView)
	{
		m_pGridView->CloseWindow();
		m_pGridView->ClearGrid();
		m_pGridView = NULL;
	}
	switch (gtp)
	{
	case LZGridCtrl::gFlow: m_pGridView = new LZGridCtrlFlow(); break;
	case LZGridCtrl::gUser: m_pGridView = new LZGridCtrlUser(); break;
	default:m_pGridView = NULL;
		return false;
		break;
	}
	if (m_pGridView->CreateGrid(this, IDC_GRIDCTRL))
	{
		m_pGridView->Init();
		m_pGridView->show();
		return true;
	}

	return false;
}




void CvipManagerView::OnCheckCusShowBaby()
{
	// TODO: 在此添加命令处理程序代码
	g_customSet._bShowBaby = !g_customSet._bShowBaby;
 	CreatGridView(m_pGridView->gType());	
}


void CvipManagerView::OnCheckCusShowType()
{
	// TODO: 在此添加命令处理程序代码
	g_customSet._bShowType = !g_customSet._bShowType;
	CreatGridView(m_pGridView->gType());
}


void CvipManagerView::OnCheckCusShowTime()
{
	// TODO: 在此添加命令处理程序代码
	g_customSet._bShowTime = !g_customSet._bShowTime;
	CreatGridView(m_pGridView->gType());
}


void CvipManagerView::OnCheckCusShowScort()
{
	// TODO: 在此添加命令处理程序代码
	g_customSet._bShowScort = !g_customSet._bShowScort;
	CreatGridView(m_pGridView->gType());
}


void CvipManagerView::OnCheckCusShowBalance()
{
	// TODO: 在此添加命令处理程序代码
	g_customSet._bShowBalance = !g_customSet._bShowBalance;
	CreatGridView(m_pGridView->gType());
}


void CvipManagerView::OnUpdateCheckCusShowBaby(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_customSet._bShowBaby);
}


void CvipManagerView::OnUpdateCheckCusShowType(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_customSet._bShowType);
}


void CvipManagerView::OnUpdateCheckCusShowTime(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_customSet._bShowTime);
}


void CvipManagerView::OnUpdateCheckCusShowScort(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_customSet._bShowScort);
}


void CvipManagerView::OnUpdateCheckCusShowBalance(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_customSet._bShowBalance);
}


void CvipManagerView::OnEditQfindPhone()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMainF = dynamic_cast<CMainFrame*>(theApp.GetMainWnd());
	if (pMainF)
	{
		CMFCRibbonBar* pRibbonBar = pMainF->getWndRibbonBar();
		if (pRibbonBar)
		{
			CMFCRibbonEdit* pEdit = dynamic_cast<CMFCRibbonEdit *>(pRibbonBar->FindByID(ID_EDIT_QFIND_PHONE));
			if (pEdit)
			{
				CString strText = pEdit->GetEditText();
				g_customSet._sQfindPhone = strText;
				CreatGridView(m_pGridView->gType());
			}

		}
	}
}

void CvipManagerView::OnDblclkGrid(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	OnBtnCusmEdit();
}


void CvipManagerView::OnBtnCusmShow()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMainF = dynamic_cast<CMainFrame*>(theApp.GetMainWnd());
	if (pMainF)
	{
		CMFCRibbonBar* pRibbonBar = pMainF->getWndRibbonBar();
		if (pRibbonBar)
		{
			CMFCRibbonEdit* pEdit = dynamic_cast<CMFCRibbonEdit *>(pRibbonBar->FindByID(ID_EDIT_QFIND_PHONE));
			if (pEdit)
			{
				pEdit->SetEditText(_T(""));
				g_customSet._sQfindPhone = _T("");
			}

		}
	}
	CreatGridView(m_pGridView->gType());
}


void CvipManagerView::OnBtnCusmEdit()
{
	// TODO: 在此添加命令处理程序代码
	CCellID cid = m_pGridView->GetFocusCell();
	CMainFrame* pMainF = dynamic_cast<CMainFrame*>(theApp.GetMainWnd());
	if (!pMainF)
		return;

	if (2 == pMainF->getCategoryIndex() && cid.IsValid() && cid.row > 0)
	{
		CString strId = m_pGridView->GetItemText(cid.row, 1);

		if (!strId.IsEmpty())
		{
			CCustomAddDlg adDlg(strId);
			if (adDlg.DoModal() == IDOK)
			{
				AfxMessageBox(_T("操作成功!"));
				CreatGridView(m_pGridView->gType());
			}
		}
	}
}
