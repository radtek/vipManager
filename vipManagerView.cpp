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
	m_pGridView->Init();
	m_pGridView->show();

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
	default:m_pGridView = NULL;
		return FALSE;
		break;
	}
	return m_pGridView->CreateGrid(this, IDC_GRIDCTRL);
}


void CvipManagerView::OnBtnCustomer()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("呵呵呵"));
}


void CvipManagerView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_pGridView)
	{
		m_pGridView->AdjustLayout();
	}
	// TODO: 在此处添加消息处理程序代码
}
