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
	, m_scValue(_T(""))
	, m_scCount(_T(""))
	, m_scSale(_T(""))
	, m_scSaleValue(_T(""))
	, m_scCusmID(_T(""))
	, m_scCusmPhone(_T(""))
	, m_scGoodsCode(_T(""))
	, m_scGoodsName(_T(""))
	, m_scGoodsRep(_T(""))
	, m_scGoodsInfo(_T(""))
	, m_scGoodsPrice(_T(""))
	, m_scGoodsCount(_T(""))
{

}

CFlowListDlg::~CFlowListDlg()
{
}

void CFlowListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FLOW_LIST, m_lstFlow);
	DDX_Control(pDX, IDC_RICHEDIT_FLOW_TOTAL, m_rEditTotal);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_VALUE, m_scValue);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_COUNT, m_scCount);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_SALE, m_scSale);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_SALEVALUE, m_scSaleValue);
	DDX_Text(pDX, IDC_EDIT_CUSM_ID, m_scCusmID);
	DDX_Text(pDX, IDC_EDIT_CUSM_PHONE, m_scCusmPhone);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_GOODS_CODE, m_scGoodsCode);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_GOODS_NAME, m_scGoodsName);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_GOODS_REPERTORY, m_scGoodsRep);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_GOODS_INFO, m_scGoodsInfo);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_GOODS_PRICE, m_scGoodsPrice);
	DDX_Text(pDX, IDC_EDIT_FLOWLIST_GOODS_COUNT, m_scGoodsCount);
}


BEGIN_MESSAGE_MAP(CFlowListDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_FLOWLIST_NORMAL, &CFlowListDlg::OnBnClickedCheckFlowlistNormal)
	ON_BN_CLICKED(IDC_BTN_FLOWLIST_ADVFIND, &CFlowListDlg::OnBnClickedBtnFlowlistAdvfind)
	ON_BN_CLICKED(IDC_BTN_FLOWLIST_CHECK, &CFlowListDlg::OnBnClickedBtnFlowlistCheck)
	ON_BN_CLICKED(IDC_BTN_FLOWLIST_GOODS_ADVFIND, &CFlowListDlg::OnBnClickedBtnFlowlistGoodsAdvfind)
	ON_BN_CLICKED(IDC_BTN_FLOWLIST_GOODS_FIND, &CFlowListDlg::OnBnClickedBtnFlowlistGoodsFind)
	ON_BN_CLICKED(IDC_BTN_FLOWLIST_ADDGOODS, &CFlowListDlg::OnBnClickedBtnFlowlistAddgoods)
	ON_BN_CLICKED(IDCANCEL, &CFlowListDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CFlowListDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_FLOWLIST_COUNT, &CFlowListDlg::OnEnChangeEditFlowlistCount)
	ON_EN_CHANGE(IDC_EDIT_FLOWLIST_SALE, &CFlowListDlg::OnEnChangeEditFlowlistSale)
	ON_NOTIFY(LVN_INSERTITEM, IDC_LIST_FLOW_LIST, &CFlowListDlg::OnLvnInsertitemListFlowList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FLOW_LIST, &CFlowListDlg::OnLvnItemchangedListFlowList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FLOW_LIST, &CFlowListDlg::OnNMRClickListFlowList)
	ON_COMMAND(ID_IDR_MENU_FLOW_LIST_DEL, &CFlowListDlg::OnIdrMenuFlowListDel)
END_MESSAGE_MAP()


// CFlowListDlg 消息处理程序


HBRUSH CFlowListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_FLOW_ID)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}



BOOL CFlowListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (!m_DBM.init())
	{
		AfxMessageBox(_T("初始化数据库失败！"));
		CDialogEx::OnCancel();
		return FALSE;
	}
	CString strFlowID;
	m_DBM.cusm_get_last_flow(strFlowID);
	GetDlgItem(IDC_STATIC_FLOW_ID)->SetWindowTextW(strFlowID);
	// TODO:  在此添加额外的初始化
	m_cFontID.CreatePointFont(180, _T("黑体"));
	GetDlgItem(IDC_STATIC_FLOW_ID)->SetFont(&m_cFontID);
	//
	m_lstFlow.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstFlow.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 40);	//插入列
	m_lstFlow.InsertColumn(1, _T("商品名称"), LVCFMT_LEFT,150);
	m_lstFlow.InsertColumn(2, _T("商品单价"), LVCFMT_RIGHT, 80);
	m_lstFlow.InsertColumn(3, _T("商品数量"), LVCFMT_RIGHT, 80);
	m_lstFlow.InsertColumn(4, _T("商品总价"), LVCFMT_RIGHT, 80);

	int nRow = m_lstFlow.InsertItem(0, _T("1"));//插入行
	m_lstFlow.SetItemText(nRow,1, _T("小苹果"));//设置数据
	m_lstFlow.SetItemText(nRow,2, _T("8.5"));//设置数据
	m_lstFlow.SetItemText(nRow,3, _T("3"));//设置数据
	m_lstFlow.SetItemText(nRow,4, _T("25.5"));//设置数据
	nRow = m_lstFlow.InsertItem(nRow+1, _T("2"));//插入行
	m_lstFlow.SetItemText(nRow, 1, _T("儿童玩具"));//设置数据
	m_lstFlow.SetItemText(nRow, 2, _T("15"));//设置数据
	m_lstFlow.SetItemText(nRow, 3, _T("1"));//设置数据
	m_lstFlow.SetItemText(nRow, 4, _T("15"));//设置数据


	m_scValue = _T("30");
	m_scCount = _T("1");

	m_scSaleValue = _T("30");
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



void CFlowListDlg::OnBnClickedCheckFlowlistNormal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_FLOWLIST_NORMAL))->GetCheck())
	{
		UpdateData(FALSE);
		m_lstFlow.InsertItem(0, _T("1"));			//插入行
		m_lstFlow.SetItemText(0, 1, _T("普通消费"));// 名称
		m_lstFlow.SetItemText(0, 2, m_scValue);		// 单价
		m_lstFlow.SetItemText(0, 3, m_scCount);		// 数量
		m_lstFlow.SetItemText(0, 4, m_scSaleValue);	// 总价
	}
	else
	{
		m_lstFlow.DeleteItem(0);
	}
	int nC = m_lstFlow.GetItemCount();
	for (int i = 0; i < nC; ++i)
	{
		CString str;
		str.Format(_T("%d"), i + 1);
		m_lstFlow.SetItemText(i, 0, str);//设置数据
	}
}


void CFlowListDlg::OnBnClickedBtnFlowlistAdvfind()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CFlowListDlg::OnBnClickedBtnFlowlistCheck()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CFlowListDlg::OnBnClickedBtnFlowlistGoodsAdvfind()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CFlowListDlg::OnBnClickedBtnFlowlistGoodsFind()
{
	// TODO: 在此添加控件通知处理程序代码
	m_scGoodsCode = _T("");
	m_scGoodsRep = _T("");
	m_scGoodsInfo = _T("");
	m_scGoodsPrice = _T("");
	m_scGoodsCount = _T("");
	GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_NAME)->GetWindowTextW(m_scGoodsName);
	GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_CODE)->SetWindowTextW(m_scGoodsCode);
	GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_REPERTORY)->SetWindowTextW(m_scGoodsRep);
	GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_INFO)->SetWindowTextW(m_scGoodsInfo);
	GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_PRICE)->SetWindowTextW(m_scGoodsPrice);
	GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_COUNT)->SetWindowTextW(m_scGoodsCount);

	GOODS_DATA gd;
	gd._paTitle.second = m_scGoodsName;
	std::vector<GOODS_DATA> ggd;
	m_DBM.manger_find_goods(gd, ggd);
	if (ggd.size() == 0)
	{
		AfxMessageBox(_T("未找到商品！"));
	}
	else if (ggd.size() == 1)
	{
		GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_CODE)->SetWindowTextW(ggd[0]._paCodeNumber.second);
		GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_NAME)->SetWindowTextW(ggd[0]._paTitle.second);
		GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_REPERTORY)->SetWindowTextW(ggd[0]._paTotal.second);
		GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_INFO)->SetWindowTextW(ggd[0]._paInfo.second);
		GetDlgItem(IDC_EDIT_FLOWLIST_GOODS_PRICE)->SetWindowTextW(ggd[0]._paPrice.second);
		m_scGoodsCode = ggd[0]._paCodeNumber.second;
		m_scGoodsName = ggd[0]._paTitle.second;
		m_scGoodsRep = ggd[0]._paTotal.second;
		m_scGoodsInfo = ggd[0]._paInfo.second;
		m_scGoodsPrice = ggd[0]._paPrice.second;
	}
	else
	{
		AfxMessageBox(_T("找到多个商品！"));
	}
}


void CFlowListDlg::OnBnClickedBtnFlowlistAddgoods()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_scGoodsCount.IsEmpty())
	{
		AfxMessageBox(_T("商品数量不能为空!"));
		return;
	}
	updateGoodsValue();
}


void CFlowListDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CFlowListDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CFlowListDlg::OnEnChangeEditFlowlistCount()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_FLOWLIST_COUNT)->GetWindowTextW(m_scCount);
	updateMainValue();
}


void CFlowListDlg::OnEnChangeEditFlowlistSale()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_FLOWLIST_SALE)->GetWindowTextW(m_scSale);
	updateMainValue();
}

void CFlowListDlg::updateMainValue()
{
	// 转换需要使用boost完善
	CString strValue, strCount, strSale, strSalevalue;
	double dValue(0), dCount(0), dSale(0), dSalevalue(0);
	GetDlgItem(IDC_EDIT_FLOWLIST_VALUE)->GetWindowTextW(strValue);
	GetDlgItem(IDC_EDIT_FLOWLIST_COUNT)->GetWindowTextW(strCount);
	GetDlgItem(IDC_EDIT_FLOWLIST_SALE)->GetWindowTextW(strSale);
	dValue = _wtof(strValue);
	dCount = _wtof(strCount);
	dSale = _wtof(strSale);
	if (dCount > 9999 || dValue > 99999)
	{
		return;
	}
	if ((dSale > -0.00001 && dSale < 0.00001) || dSale < 0)
	{
		// 不打折
		strSalevalue.Format(_T("%.2lf"), dValue*dCount);
	}
	else
	{
		// 打折
		strSalevalue.Format(_T("%.2lf"), dValue*dSale*dCount);
	}

	GetDlgItem(IDC_EDIT_FLOWLIST_SALEVALUE)->SetWindowTextW(strSalevalue);
	m_scSaleValue = strSalevalue;
}



void CFlowListDlg::updateGoodsValue()
{
	CString strvalue;
	double dValue(0), dCount(0), dSale(0), dSalevalue(0);
	// 暂不处理商品折扣
	dValue = _wtof(m_scGoodsPrice);
	dCount = _wtof(m_scGoodsCount);
	strvalue.Format(_T("%.2lf"), dValue*dCount);

	
	int nC = m_lstFlow.GetItemCount();
	int nRow = m_lstFlow.InsertItem(nC+1, _T("1"));
	m_lstFlow.SetItemText(nRow, 1, m_scGoodsName);// 名称
	m_lstFlow.SetItemText(nRow, 2, m_scGoodsPrice);		// 单价
	m_lstFlow.SetItemText(nRow, 3, m_scGoodsCount);		// 数量
	m_lstFlow.SetItemText(nRow, 4, strvalue);	// 总价

	nC = m_lstFlow.GetItemCount();
	for (int i = 0; i < nC; ++i)
	{
		CString str;
		str.Format(_T("%d"), i + 1);
		m_lstFlow.SetItemText(i, 0, str);//设置数据
	}

}

void CFlowListDlg::updateTotalValue()
{
	int nCount = m_lstFlow.GetItemCount();
	double dTotal = 0;
	for (int i = 0; i < nCount; ++i)
	{
		dTotal += _wtof(m_lstFlow.GetItemText(i, 4));
	}
	CString str;
	str.Format(_T("%.2lf"), dTotal);
	m_rEditTotal.SetWindowTextW(str);
}

void CFlowListDlg::OnLvnInsertitemListFlowList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CFlowListDlg::OnLvnItemchangedListFlowList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	updateTotalValue();
	*pResult = 0;
}


void CFlowListDlg::OnNMRClickListFlowList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lstFlow.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if (pos == NULL)
		return;
	int nItem = m_lstFlow.GetNextSelectedItem(pos);
	if (nItem == 0 && ((CButton*)GetDlgItem(IDC_CHECK_FLOWLIST_NORMAL))->GetCheck())
		return;
	*pResult = 0;
	CMenu menu;
	POINT pt = { 0 };
	GetCursorPos(&pt);//得到鼠标点击位置 
	menu.LoadMenu(IDR_MENU_FLOW_LIST);//菜单资源ID 
	menu.GetSubMenu(0)->TrackPopupMenu(0, pt.x, pt.y, this); //m_newListCtrl是CListCtrl对象
}


void CFlowListDlg::OnIdrMenuFlowListDel()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_lstFlow.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if (pos == NULL)
		return;
	int nItem = m_lstFlow.GetNextSelectedItem(pos);
	m_lstFlow.DeleteItem(nItem);

	updateTotalValue();
	int nC = m_lstFlow.GetItemCount();
	for (int i = 0; i < nC; ++i)
	{
		CString str;
		str.Format(_T("%d"), i + 1);
		m_lstFlow.SetItemText(i, 0, str);//设置数据
	}

}
