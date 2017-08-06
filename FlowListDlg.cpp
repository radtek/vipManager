// FlowListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "vipManager.h"
#include "FlowListDlg.h"
#include "afxdialogex.h"


// CFlowListDlg 对话框

IMPLEMENT_DYNAMIC(CFlowListDlg, CDialogEx)

CFlowListDlg::CFlowListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_FLOW_LIST, pParent)
{

}

CFlowListDlg::~CFlowListDlg()
{
}

void CFlowListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFlowListDlg, CDialogEx)
END_MESSAGE_MAP()


// CFlowListDlg 消息处理程序
