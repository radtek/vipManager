#pragma once
#include "afxwin.h"
#include "resource.h"
#include "DBManager.h"
#include "afxdtctl.h"

// CGoodsAddDlg 对话框

class CGoodsAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGoodsAddDlg)

public:
	CGoodsAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	CGoodsAddDlg(CString strCodeNum, CWnd* pParent = NULL);// 编辑模式
	virtual ~CGoodsAddDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GOODS_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnGoodsAddDelgoods();
private:
	bool checkInput();
	CString getStrUnixTime();
private:
	// 编辑模式
	BOOL m_bEdit;
	// 数据库接口
	CDBManager m_DBM;
	// 出初始化数据
	goods_data m_initgd;
private:
	// 商品编码
	CString m_scCodeNum;
	// 商品类型
	CString m_scType;
	// 商品类型
	CComboBox m_cmbType;
	// 商品名称
	CString m_scTitle;
	// 商品进价
	CString m_scInterPre;
	// 商品售价
	CString m_scPrice;
	// 库存数量
	CString m_scTotal;
	// 备注信息
	CString m_scInfo;
};
