// GoodsAddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "vipManager.h"
#include "GoodsAddDlg.h"
#include "afxdialogex.h"


// CGoodsAddDlg 对话框

IMPLEMENT_DYNAMIC(CGoodsAddDlg, CDialogEx)

CGoodsAddDlg::CGoodsAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_GOODS_ADD, pParent)
	, m_bEdit(FALSE)
	, m_scCodeNum(_T(""))
	, m_scType(_T(""))
	, m_scTitle(_T(""))
	, m_scInterPre(_T(""))
	, m_scPrice(_T(""))
	, m_scTotal(_T(""))
	, m_scInfo(_T(""))
{

}

CGoodsAddDlg::CGoodsAddDlg(CString strCodeNum, CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_DLG_GOODS_ADD, pParent)
	, m_bEdit(TRUE)
	, m_scCodeNum(strCodeNum)
	, m_scType(_T(""))
	, m_scTitle(_T(""))
	, m_scInterPre(_T(""))
	, m_scPrice(_T(""))
	, m_scTotal(_T(""))
{

}

CGoodsAddDlg::~CGoodsAddDlg()
{
}

void CGoodsAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GOODS_ADD_CODENUM, m_scCodeNum);
	DDX_CBString(pDX, IDC_CMB_GOODS_ADD_TYPE, m_scType);
	DDX_Text(pDX, IDC_EDIT_GOODS_ADD_TITLE, m_scTitle);
	DDX_Text(pDX, IDC_EDIT_GOODS_ADD_INTERPRE, m_scInterPre);
	DDX_Text(pDX, IDC_EDIT_GOODS_ADD_PRICE, m_scPrice);
	DDX_Text(pDX, IDC_EDIT_GOODS_ADD_TOTAL, m_scTotal);
	DDX_Control(pDX, IDC_CMB_GOODS_ADD_TYPE, m_cmbType);
	DDX_Text(pDX, IDC_EDIT_GOODS_ADD_REMARK, m_scInfo);
}


BEGIN_MESSAGE_MAP(CGoodsAddDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGoodsAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_GOODS_ADD_DELGOODS, &CGoodsAddDlg::OnBnClickedBtnGoodsAddDelgoods)
END_MESSAGE_MAP()


// CGoodsAddDlg 消息处理程序


BOOL CGoodsAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_DBM.init())
	{
		AfxMessageBox(_T("初始化数据库失败！"));
		CDialogEx::OnCancel();
		return FALSE;
	}
	m_scType = _T("泳衣");
	m_cmbType.AddString(_T("泳衣"));
	m_cmbType.AddString(_T("泳圈"));
	m_cmbType.AddString(_T("泳冒"));
	m_cmbType.AddString(_T("其他"));
	
	if (!m_bEdit)
	{
		// 添加模式初始化
		GetDlgItem(IDC_EDIT_GOODS_ADD_CODENUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GOODS_ADD_DELGOODS)->ShowWindow(SW_HIDE);
		m_scCodeNum = getStrUnixTime(); // 生成唯一商品编号
	}
	else
	{
		// 编辑模式初始化
		GetDlgItem(IDC_EDIT_GOODS_ADD_CODENUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GOODS_ADD_DELGOODS)->ShowWindow(SW_SHOW);

		goods_data gd;
		gd._paCodeNumber.second = m_scCodeNum;
		std::vector<goods_data> vecGD;
		if (!m_DBM.manger_find_goods(gd, vecGD) || vecGD.size() == 0)
		{
			AfxMessageBox(_T("用户数据错误"));
			CDialogEx::OnCancel();
			return FALSE;
		}
		m_initgd = vecGD[0];
		m_scInfo = m_initgd._paInfo.second;
		m_scInterPre = m_initgd._paIntoPrice.second;
		m_scPrice = m_initgd._paPrice.second;
		m_scTitle = m_initgd._paTitle.second;
		m_scTotal = m_initgd._paTotal.second;
		m_scType = m_initgd._paType.second;
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CGoodsAddDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CGoodsAddDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	// 数据检查
	if (!checkInput())
	{
		return;
	}

	if (m_bEdit)
	{
		goods_data dd;
		dd._paID.second = m_initgd._paID.second;
		dd._paCodeNumber.second = m_scCodeNum;
		dd._paInfo.second = m_scInfo;
		dd._paIntoPrice.second = m_scInterPre;
		dd._paPrice.second = m_scPrice;
		dd._paTitle.second = m_scTitle;
		dd._paTotal.second = m_scTotal;
		dd._paType.second = m_scType;
		if (dd == m_initgd)
			return CDialogEx::OnCancel();
		if (AfxMessageBox(_T("确认修改?"), MB_YESNO) == IDNO)
			return;
		if (!m_DBM.manger_edit_goods(dd, m_initgd))
		{
			AfxMessageBox(_T("入库失败！"));
			return;
		}
	}
	else
	{
		goods_data gd;
		std::vector<goods_data> vecGd;
		gd._paCodeNumber.second = m_scCodeNum;
		m_DBM.manger_find_goods(gd, vecGd);
		if (vecGd.size())
		{
			AfxMessageBox(_T("已经存在的商品编号!"));
			return;
		}

		if (AfxMessageBox(_T("确认入库?"), MB_YESNO) == IDNO)
			return;
		gd._paCodeNumber.second = m_scCodeNum;
		gd._paInfo.second = m_scInfo;
		gd._paIntoPrice.second = m_scInterPre;
		gd._paPrice.second = m_scPrice;
		gd._paTitle.second = m_scTitle;
		gd._paTotal.second = m_scTotal;
		gd._paType.second = m_scType;
		if (!m_DBM.manger_add_new_goods(gd))
		{
			AfxMessageBox(_T("入库失败！"));
			return;
		}
	}

	CDialogEx::OnOK();
}

bool CGoodsAddDlg::checkInput()
{
	m_scCodeNum.Trim(_T(" "));
	m_scType.Trim(_T(" "));
	m_scTitle.Trim(_T(" "));
	if (m_scCodeNum.IsEmpty())
	{
		AfxMessageBox(_T("编号不许为空!"));
		return false;
	}
	if (m_scType.IsEmpty())
	{
		AfxMessageBox(_T("类型不许为空!"));
		return false;
	}
	if (m_scTitle.IsEmpty())
	{
		AfxMessageBox(_T("标题不许为空!"));
		return false;
	}
	return true;
}

CString CGoodsAddDlg::getStrUnixTime()
{
	CString nowTime;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CTime mtTime(sysTime);
	time_t unixTime = mtTime.GetTime();
	nowTime.Format(_T("%ld"), unixTime);	//输出UNIX时间戳字符串
	return nowTime;
}

void CGoodsAddDlg::OnBnClickedBtnGoodsAddDelgoods()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox(_T("删除后不可恢复,确认删除?"), MB_YESNO) == IDNO)
		return;
	goods_data gd;
	gd._paCodeNumber.second = m_scCodeNum;
	if (!m_DBM.manger_delete_goods(gd))
	{
		AfxMessageBox(_T("删除失败!"));
	}
	else
	{
		return CDialogEx::OnOK();
	}
}
