#pragma once

#include "afxwin.h"
#include "resource.h"
#include "DBManager.h"
#include "afxdtctl.h"
#include "afxcmn.h"
// CFlowListDlg 对话框

class CFlowListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlowListDlg)

public:
	CFlowListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlowListDlg();

	CFont m_cFontID;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FLOW_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
private:
	// 购物清单
	CListCtrl m_lstFlow;
	// 总计
	CRichEditCtrl m_rEditTotal;

	// 单价
	CString m_scValue;
	// 数量
	CString m_scCount;
	// 折扣
	CString m_scSale;
	// 折后价
	CString m_scSaleValue;

	// 会员ID
	CString m_scCusmID;
	// 预留电话
	CString m_scCusmPhone;

	// 商品编码
	CString m_scGoodsCode;
	// 商品名称
	CString m_scGoodsName;
	// 商品库存
	CString m_scGoodsRep;
	// 商品信息
	CString m_scGoodsInfo;
	// 商品单价
	CString m_scGoodsPrice;
	// 商品数量
	CString m_scGoodsCount;
private:
	// 数据库接口
	CDBManager m_DBM;

private:
	void updateMainValue();
	void updateGoodsValue();
	void updateTotalValue();
public:
	afx_msg void OnBnClickedCheckFlowlistNormal();
	afx_msg void OnBnClickedBtnFlowlistAdvfind();
	afx_msg void OnBnClickedBtnFlowlistCheck();
	afx_msg void OnBnClickedBtnFlowlistGoodsAdvfind();
	afx_msg void OnBnClickedBtnFlowlistGoodsFind();
	afx_msg void OnBnClickedBtnFlowlistAddgoods();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditFlowlistCount();
	afx_msg void OnEnChangeEditFlowlistSale();
	afx_msg void OnLvnInsertitemListFlowList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListFlowList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListFlowList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIdrMenuFlowListDel();
};
