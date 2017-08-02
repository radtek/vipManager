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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  IDC_GRIDCTRL 1214
// CvipManagerView

IMPLEMENT_DYNCREATE(CvipManagerView, CView)

BEGIN_MESSAGE_MAP(CvipManagerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CvipManagerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CvipManagerView 构造/析构

CvipManagerView::CvipManagerView()
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
	CView::PreCreateWindow(cs);

// 	if (!CreatGridView())
// 		return -1;
// 	m_pGridView->Init();
// 	CStringArray strArryColName;
// 	strArryColName.Add(_T("序号"));
// 	strArryColName.Add(_T("类型"));
// 	strArryColName.Add(_T("名称"));
// 	strArryColName.Add(_T("颜色"));
// 	int nCol = strArryColName.GetSize();
// 	m_pGridView->SetColumnCount(nCol);
// 	m_pGridView->SetRowCount(3);
// 	for (int i = 0; i < nCol; i++)
// 	{
// 		CString strColNmae = strArryColName.GetAt(i);
// 		m_pGridView->SetItemText(0, i, strColNmae);
// 	}
// 	m_pGridView->updateCellSize(1, 100);

	return true;
}


// CvipManagerView 绘制

void CvipManagerView::OnDraw(CDC* /*pDC*/)
{
	CvipManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

// CvipManagerView 打印


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

void CvipManagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CvipManagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CvipManagerView 诊断

#ifdef _DEBUG
void CvipManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CvipManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CvipManagerDoc* CvipManagerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CvipManagerDoc)));
	return (CvipManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CvipManagerView 消息处理程序

BOOL CvipManagerView::CreatGridView()
{
	if (m_pGridView)
	{
		m_pGridView->CloseWindow();
		m_pGridView->ClearGrid();
		m_pGridView = NULL;
	}
	m_pGridView = new LZGridCtrl();
	return m_pGridView->CreateGrid(this, IDC_GRIDCTRL);
}