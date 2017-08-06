#pragma once
#include "afxwin.h"
#include "resource.h"
#include "DBManager.h"
#include "afxdtctl.h"

// CCustomAddDlg 对话框

class CCustomAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomAddDlg)

public:
	CCustomAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	CCustomAddDlg(CString strUserID, CWnd* pParent = NULL);// 编辑模式
	virtual ~CCustomAddDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CUSTOM_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	bool checkInput();
private:
	// 编辑模式
	BOOL m_bEdit;

	// 数据库接口
	CDBManager m_DBM;
private:
	// ID
	CString m_strID;
	// 会员姓名
	CString m_strName;
	// 注册时间
	CString m_strRegtime;
	// 注册时间
	CDateTimeCtrl m_dtcRegTime;
	// 联系电话
	CString m_strPhone1;
	// 备用电话
	CString m_strPhone2;
	// 会员类型  // ! 关于会员类型的思考 可能既有次数 又有余额
	CComboBox m_cmbType;
	// 会员积分
	CString m_strScore;
	// 次数余额
	CString m_strBalanceCount;
	// 现金余额
	CString m_strBalanceMoney;
	// 宝宝姓名
	CString m_strBabyName;
	// 宝宝性别
	CComboBox m_cmbBabySex;
	// 宝宝年龄
	CString m_strBabyAge;
	// 备注
	CString m_strRemark;


};
