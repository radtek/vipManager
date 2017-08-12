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

#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "vipManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define ID_BTN_CMB_LIST 1

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(ID_BTN_CMB_LIST, &CPropertiesWnd::OnBtnSelchangedCmbList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	//m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight-1/* + cyTlb*/, rectClient.Width(), rectClient.Height() -(m_nComboHeight/*+cyTlb*/), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_DBM.init())
	{
		AfxMessageBox(_T("初始化数据库失败！"));
		return -1;
	}

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_BTN_CMB_LIST))
	{
		TRACE0("未能创建属性组合 \n");
		return -1;      // 未能创建
	}

	m_wndObjectCombo.AddString(_T("应用程序")); // 显示单号
	m_wndObjectCombo.AddString(_T("属性窗口"));
	m_wndObjectCombo.SetCurSel(0);
	m_wndObjectCombo.EnableWindow(FALSE);
	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}
	InitPropList();
	//InitPropListSimple();
/*
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE );//已锁定
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE );// 锁定

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
*/
	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::InitPropList()
{

	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("商品列表"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("三维外观"), (_variant_t)false, _T("指定窗口的字体不使用粗体，并且控件将使用三维边框")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一: “无”、“细”、“可调整大小”或“对话框外框”"));
	pProp->AddOption(_T("无"));
	pProp->AddOption(_T("细"));
	pProp->AddOption(_T("可调整大小"));
	pProp->AddOption(_T("对话框外框"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	CMFCPropertyGridProperty* pTitle = new CMFCPropertyGridProperty(_T("标题"), (_variant_t)_T("关于"), _T("指定窗口标题栏中显示的文本"));
	pTitle->AllowEdit(FALSE);
	pGroup1->AddSubItem(pTitle);

	m_wndPropList.AddProperty(pGroup1);
}

void CPropertiesWnd::InitPropListSimple()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("商品列表"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("三维外观"), (_variant_t) false, _T("指定窗口的字体不使用粗体，并且控件将使用三维边框")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一: “无”、“细”、“可调整大小”或“对话框外框”"));
	pProp->AddOption(_T("无"));
	pProp->AddOption(_T("细"));
	pProp->AddOption(_T("可调整大小"));
	pProp->AddOption(_T("对话框外框"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	CMFCPropertyGridProperty* pTitle = new CMFCPropertyGridProperty(_T("标题"), (_variant_t)_T("关于"), _T("指定窗口标题栏中显示的文本"));
	pTitle->AllowEdit(FALSE);
	pGroup1->AddSubItem(pTitle);

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("窗口大小"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定窗口的高度"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("宽度"), (_variant_t) 150l, _T("指定窗口的宽度"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("字体"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("宋体, Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定窗口的默认字体")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定窗口使用“MS Shell Dlg”字体")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("杂项"));
	pProp = new CMFCPropertyGridProperty(_T("(名称)"), _T("应用程序"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("窗口颜色"), RGB(210, 192, 254), NULL, _T("指定默认的窗口颜色"));
	pColorProp->EnableOtherButton(_T("其他..."));
	pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定窗口图标")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("文件夹"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("层次结构"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("第一个子级"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("第二个子级"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 1"), (_variant_t) _T("值 1"), _T("此为说明")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 2"), (_variant_t) _T("值 2"), _T("此为说明")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 3"), (_variant_t) _T("值 3"), _T("此为说明")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::OnBtnSelchangedCmbList()
{
	int nsel = m_wndObjectCombo.GetCurSel();
	CString str;
	m_wndObjectCombo.GetLBText(nsel, str);
	AfxMessageBox(str);
}



void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CPropertiesWnd::clearShow()
{
	m_wndPropList.RemoveAll();
	m_wndObjectCombo.ResetContent();
}

BOOL CPropertiesWnd::showProper(CString strFlowID)
{
	if (strFlowID.IsEmpty())
		return false;
	clearShow();
	m_wndObjectCombo.AddString(strFlowID);
	m_wndObjectCombo.SetCurSel(0);

	flow_idx_data idata;
	flow_main_data mdata;
	flow_goods_data gdata;
	idata._paID.second = strFlowID;
	mdata._paFlowID.second = strFlowID;
	gdata._paFlowID.second = strFlowID;

	std::vector<flow_idx_data> vecIdata;
	std::vector<flow_main_data> vecMdata;
	std::vector<flow_goods_data> vecGdata;
	// 基本信息
	m_DBM.proper_get_idx(idata, vecIdata);
	if (vecIdata.size()!=1)
	{
		AfxMessageBox(_T("工单数据错误！"));
		return false;
	}
	if (vecIdata[0]._paPayType.second.CompareNoCase(_T("0"))==0)
	{
		// 普通账户
	}
	else
	{
		// 会员账户
		user_data ud;
		std::vector<user_data> vecUD;
		ud._paID.second = vecIdata[0]._paPayType.second;
		m_DBM.cusm_find_user(ud, vecUD);
		if (vecUD.size() != 1)
		{
			AfxMessageBox(_T("用户信息错误,请处理!"));
			return false;
		}
		CMFCPropertyGridProperty* pGroupCustom = new CMFCPropertyGridProperty(_T("会员信息"));
		{
			CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("姓名"), (_variant_t)vecUD[0]._paName.second, vecUD[0]._paRemark.second);
			pName->AllowEdit(FALSE);
			pGroupCustom->AddSubItem(pName);

			CMFCPropertyGridProperty* pID = new CMFCPropertyGridProperty(_T("ID"), (_variant_t)vecUD[0]._paID.second, _T("用户的唯一ID号码"));
			pID->AllowEdit(FALSE);
			pGroupCustom->AddSubItem(pID);

			CMFCPropertyGridProperty* pPhone = new CMFCPropertyGridProperty(_T("电话"), (_variant_t)vecUD[0]._paPhone1.second, _T("用户主要电话号码"));
			pPhone->AllowEdit(FALSE);
			pGroupCustom->AddSubItem(pPhone);

			CMFCPropertyGridProperty* pType = new CMFCPropertyGridProperty(_T("会员类型"), (_variant_t)vecUD[0]._paType.second, _T("注册会员类型"));
			pType->AddOption(_T("预付费"));
			pType->AddOption(_T("预付次数"));
			pType->AddOption(_T("折扣会员"));
			pType->AllowEdit(FALSE);
			pGroupCustom->AddSubItem(pType);

			CMFCPropertyGridProperty* pbcount = new CMFCPropertyGridProperty(_T("剩余次数"), (_variant_t)vecUD[0]._paBalanceCount.second, _T("次数类型会员的剩余使用次数"));
			pbcount->AllowEdit(FALSE);
			pGroupCustom->AddSubItem(pbcount);

			CMFCPropertyGridProperty* pbmoney = new CMFCPropertyGridProperty(_T("余额"), (_variant_t)vecUD[0]._paBalanceMoney.second, _T("预付费类型会员的剩余使用金额"));
			pbmoney->AllowEdit(FALSE);
			pGroupCustom->AddSubItem(pbmoney);

			CMFCPropertyGridProperty* pbScoer = new CMFCPropertyGridProperty(_T("积分"), (_variant_t)vecUD[0]._paScore.second, _T("会员积分"));
			pbScoer->AllowEdit(FALSE);
			pGroupCustom->AddSubItem(pbScoer);

		}
		m_wndPropList.AddProperty(pGroupCustom);
	}
	// 普通消费
	m_DBM.proper_get_main(mdata, vecMdata);
	if (vecMdata.size()>1)
	{
		AfxMessageBox(_T("出现重复数据，请处理！"));
		return false;
	}
	else if (vecMdata.size() == 1)
	{
		CMFCPropertyGridProperty* pGroupMain = new CMFCPropertyGridProperty(_T("普通消费"));
		{
			CMFCPropertyGridProperty* pPropStatus = new CMFCPropertyGridProperty(_T("状态"), vecMdata[0]._paStatus.second, _T("当前消费状态进度"));
			pPropStatus->AddOption(_T("已完成"));
			pPropStatus->AddOption(_T("进行中"));
			pPropStatus->AddOption(_T("交易失败"));
			pPropStatus->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pPropStatus);

			CMFCPropertyGridProperty* pSTime = new CMFCPropertyGridProperty(_T("开始时间"), (_variant_t)vecMdata[0]._paTime.second, _T("操作开始的时间"));
			pSTime->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pSTime);

			CMFCPropertyGridProperty* pETime = new CMFCPropertyGridProperty(_T("完成时间"), (_variant_t)vecMdata[0]._paEndTime.second, _T("客人离开的时间"));
			pETime->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pETime);

			CMFCPropertyGridProperty* pValue = new CMFCPropertyGridProperty(_T("单价"), (_variant_t)vecMdata[0]._paValue.second, _T("标准单价"));
			pValue->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pValue);

			CMFCPropertyGridProperty* pSale = new CMFCPropertyGridProperty(_T("折扣"), (_variant_t)vecMdata[0]._paSale.second, _T("折扣0-1"));
			pSale->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pSale);

			CMFCPropertyGridProperty* pCount = new CMFCPropertyGridProperty(_T("数量"), (_variant_t)vecMdata[0]._paCount.second, _T("消费人数"));
			pCount->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pCount);

			CMFCPropertyGridProperty* pTotal = new CMFCPropertyGridProperty(_T("总计"), (_variant_t)vecMdata[0]._paTotal.second, _T("折后的总价格"));
			pTotal->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pTotal);

			CMFCPropertyGridProperty* pvType = new CMFCPropertyGridProperty(_T("支付类型"), (_variant_t)vecMdata[0]._paValueType.second, _T("支付类型(现金刷卡等)"));
			pvType->AllowEdit(FALSE);
			pGroupMain->AddSubItem(pvType);
		}
		m_wndPropList.AddProperty(pGroupMain);
	}

	
	// 商品列表
	m_DBM.proper_get_goods(gdata, vecGdata);
	if (vecGdata.size())
	{
		// 商品信息
		CMFCPropertyGridProperty* pGroupGoods = new CMFCPropertyGridProperty(_T("商品列表"));
		for each (flow_goods_data var in vecGdata)
		{
			goods_data gd;
			std::vector<GOODS_DATA> vecGD;
			gd._paCodeNumber.second = var._paCodeNum.second;
			m_DBM.manger_find_goods(gd, vecGD);
			if (vecGD.size() != 1)
			{
				AfxMessageBox(_T("未找到商品信息！"));
				continue;
			}
			CMFCPropertyGridProperty* pGood = new CMFCPropertyGridProperty(var._paTitle.second, (_variant_t)var._paTotal.second, vecGD[0]._paInfo.second);
			{
				/*
				CMFCPropertyGridProperty* pGoodCodeNum = new CMFCPropertyGridProperty(_T("商品码"), (_variant_t)var._paCodeNum.second, _T("库中记录的商品码"));
				CMFCPropertyGridProperty* pGoodType = new CMFCPropertyGridProperty(_T("商品类型"), (_variant_t)vecGD[0]._paType.second, _T("库中记录的商品类型"));
				CMFCPropertyGridProperty* pGoodValue = new CMFCPropertyGridProperty(_T("单价"), (_variant_t)var._paValue.second, _T("指交易时的单价"));
				CMFCPropertyGridProperty* pGoodCount = new CMFCPropertyGridProperty(_T("数量"), (_variant_t)var._paCount.second, _T("指交易时的商品数量"));
				CMFCPropertyGridProperty* pGoodAllCount = new CMFCPropertyGridProperty(_T("库存"), (_variant_t)vecGD[0]._paTotal.second, _T("指交易时的商品数量"));
				CMFCPropertyGridProperty* pGoodTotal = new CMFCPropertyGridProperty(_T("商品总价"), (_variant_t)var._paCount.second, _T("指此件商品的总价"));
				pGood->AddSubItem(pGoodCodeNum);
				pGood->AddSubItem(pGoodType);
				pGood->AddSubItem(pGoodValue);
				pGood->AddSubItem(pGoodAllCount);
				pGood->AddSubItem(pGoodCount);
				pGood->AddSubItem(pGoodTotal);
				*/
			}
			pGood->AllowEdit(FALSE);
			pGroupGoods->AddSubItem(pGood);
		}

		m_wndPropList.AddProperty(pGroupGoods);
	}
	


	return true;
}

