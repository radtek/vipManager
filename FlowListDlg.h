#pragma once

#include "afxwin.h"
#include "resource.h"
#include "DBManager.h"
#include "afxdtctl.h"
// CFlowListDlg 对话框

class CFlowListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlowListDlg)

public:
	CFlowListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlowListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FLOW_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
