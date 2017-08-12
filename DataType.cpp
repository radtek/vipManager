#include "stdafx.h"
#include "DataType.h"

namespace DataType
{
	USER_DATA::USER_DATA()
	{
		_paID			= std::make_pair(_T("ID"), _T(""));
		_paName			= std::make_pair(_T("NAME"), _T(""));
		_paRegtime		= std::make_pair(_T("REGTIME"), _T(""));
		_paPhone1		= std::make_pair(_T("PHONE1"), _T(""));
		_paPhone2		= std::make_pair(_T("PHONE2"), _T(""));
		_paType			= std::make_pair(_T("CARDTYPE"), _T(""));
		_paScore		= std::make_pair(_T("SCORE"), _T("0,"));
		_paBalanceCount	= std::make_pair(_T("BALANCE_COUNT"), _T("0"));
		_paBalanceMoney	= std::make_pair(_T("BALANCE_MONEY"), _T("0.00"));
		_paBabyName		= std::make_pair(_T("BABYNAME"), _T(""));
		_paBabySex		= std::make_pair(_T("BABYSEX"), _T(""));
		_paBabyAge		= std::make_pair(_T("BABYAGE"), _T(""));
		_paRemark		= std::make_pair(_T("REMARK"), _T(""));
	}

	CString USER_DATA::_getdb_key() const
	{
		CString str;
		str += (_T("`") + _paID.first + _T("`,"));
		str += (_T("`") + _paName.first + _T("`,"));
		str += (_T("`") + _paRegtime.first + _T("`,"));
		str += (_T("`") + _paPhone1.first + _T("`,"));
		str += (_T("`") + _paPhone2.first + _T("`,"));
		str += (_T("`") + _paType.first + _T("`,"));
		str += (_T("`") + _paScore.first + _T("`,"));
		str += (_T("`") + _paBalanceCount.first + _T("`,"));
		str += (_T("`") + _paBalanceMoney.first + _T("`,"));
		str += (_T("`") + _paBabyName.first + _T("`,"));
		str += (_T("`") + _paBabySex.first + _T("`,"));
		str += (_T("`") + _paBabyAge.first + _T("`,"));
		str += (_T("`") + _paRemark.first + _T("`"));
		return str;
	}

	CString USER_DATA::_getdb_val() const
	{
		CString str;
		if (_paID.second.IsEmpty())
		{
			str += (_T("NULL,"));
		}
		else
		{
			str += (_T("'") + _paID.second + _T("',"));
		}
		str += (_T("'") + _paName.second + _T("',"));
		if (_paRegtime.second.IsEmpty())
		{
			str += (_T(" NOW(),"));
		}
		else
		{
			str += (_T("'") + _paRegtime.second + _T("',"));
		}
		str += (_T("'") + _paPhone1.second + _T("',"));
		str += (_T("'") + _paPhone2.second + _T("',"));
		str += (_T("'") + _paType.second + _T("',"));
		str += (_T("'") + _paScore.second + _T("',"));
		str += (_T("'") + _paBalanceCount.second + _T("',"));
		str += (_T("'") + _paBalanceMoney.second + _T("',"));
		str += (_T("'") + _paBabyName.second + _T("',"));
		str += (_T("'") + _paBabySex.second + _T("',"));
		str += (_T("'") + _paBabyAge.second + _T("',"));
		str += (_T("'") + _paRemark.second + _T("'"));
		return str;
	}

	GOODS_DATA::GOODS_DATA()
	{
		_paID				= std::make_pair(_T("ID"), _T(""));
		_paCodeNumber		= std::make_pair(_T("CODENUM"), _T(""));
		_paType				= std::make_pair(_T("TYPE"), _T(""));
		_paTitle			= std::make_pair(_T("TITLE"), _T(""));
		_paPrice			= std::make_pair(_T("PRICE"), _T(""));
		_paIntoPrice		= std::make_pair(_T("INTOPRICE"), _T(""));
		_paTotal			= std::make_pair(_T("TOTAL"), _T(""));
		_paInfo				= std::make_pair(_T("INFO"), _T(""));
	}

	bool GOODS_DATA::operator==(GOODS_DATA & gd)
	{
		// TODO: 在此处插入 return 语句
		if (_paID != gd._paID)
			return false;
		if (_paCodeNumber != gd._paCodeNumber)
			return false;
		if (_paCodeNumber != gd._paCodeNumber)
			return false;
		if (_paType != gd._paType)
			return false;
		if (_paTitle != gd._paTitle)
			return false;
		if (_paPrice != gd._paPrice)
			return false;
		if (_paIntoPrice != gd._paIntoPrice)
			return false;
		if (_paTotal != gd._paTotal)
			return false;
		if (_paInfo != gd._paInfo)
			return false;
		return true;
	}

	CString GOODS_DATA::_getdb_key() const
	{
		CString str;
		str += (_T("`") + _paID.first + _T("`,"));
		str += (_T("`") + _paCodeNumber.first + _T("`,"));
		str += (_T("`") + _paType.first + _T("`,"));
		str += (_T("`") + _paTitle.first + _T("`,"));
		str += (_T("`") + _paPrice.first + _T("`,"));
		str += (_T("`") + _paIntoPrice.first + _T("`,"));
		str += (_T("`") + _paTotal.first + _T("`,"));
		str += (_T("`") + _paInfo.first + _T("`"));
		return str;
	}

	CString GOODS_DATA::_getdb_val() const
	{
		CString str;
		if (_paID.second.IsEmpty())
		{
			str += (_T("NULL,"));
		}
		else
		{
			str += (_T("'") + _paID.second + _T("',"));
		}
		str += (_T("'") + _paCodeNumber.second + _T("',"));
		str += (_T("'") + _paType.second + _T("',"));
		str += (_T("'") + _paTitle.second + _T("',"));
		str += (_T("'") + _paPrice.second + _T("',"));
		str += (_T("'") + _paIntoPrice.second + _T("',"));
		str += (_T("'") + _paTotal.second + _T("',"));
		str += (_T("'") + _paInfo.second + _T("'"));
		return str;
	}

	FLOW_IDX_DATA::FLOW_IDX_DATA()
	{
		_paID				= std::make_pair(_T("ID"), _T(""));
		_paStatus			= std::make_pair(_T("STATUS"), _T(""));
		_paPayType			= std::make_pair(_T("CARDNUM"), _T(""));
		_paTime				= std::make_pair(_T("TIME"), _T(""));
		_paValue			= std::make_pair(_T("VALUE"), _T(""));
		_paReceipt			= std::make_pair(_T("RECEIPT"), _T(""));
		_paReceiptNum		= std::make_pair(_T("RECEIPTNUM"), _T(""));
		_paRemark			= std::make_pair(_T("REMARK"), _T(""));
	}

	FLOW_MAIN_DATA::FLOW_MAIN_DATA()
	{
		_paFlowID			= std::make_pair(_T("FLOW_ID"), _T(""));
		_paStatus			= std::make_pair(_T("STATUS"), _T(""));
		_paPayType			= std::make_pair(_T("PAYTYPE"), _T(""));
		_paTime				= std::make_pair(_T("TIME"), _T(""));
		_paEndTime			= std::make_pair(_T("ENDTIME"), _T(""));
		_paValue			= std::make_pair(_T("VALUE"), _T(""));
		_paSale				= std::make_pair(_T("SALE"), _T(""));
		_paCount			= std::make_pair(_T("COUNT"), _T(""));
		_paTotal			= std::make_pair(_T("TOTAL"), _T(""));
		_paValueType		= std::make_pair(_T("VALUETYPE"), _T(""));
	}

	FLOW_GOODS_DATA::FLOW_GOODS_DATA()
	{
		_paFlowID			= std::make_pair(_T("FLOW_ID"), _T(""));
		_paStatus			= std::make_pair(_T("STATUS"), _T(""));
		_paPayType			= std::make_pair(_T("PAYTYPE"), _T(""));
		_paTime				= std::make_pair(_T("TIME"), _T(""));
		_paTitle			= std::make_pair(_T("TITLE"), _T(""));
		_paValue			= std::make_pair(_T("VALUE"), _T(""));
		_paCodeNum			= std::make_pair(_T("CODENUM"), _T(""));
		_paSale				= std::make_pair(_T("SALE"), _T(""));
		_paCount			= std::make_pair(_T("COUNT"), _T(""));
		_paTotal			= std::make_pair(_T("TOTAL"), _T(""));
		_paValueType		= std::make_pair(_T("VALUETYPE"), _T(""));
	}

	CUSTOM_GSET::CUSTOM_GSET()
	{
		_bShowBaby		 = FALSE;
		_bShowType		 = TRUE;
		_bShowTime		 = TRUE;
		_bShowScort		 = FALSE;
		_bShowBalance	 = TRUE;

		_sQfindPhone	 = _T("");
	}

	FLOW_GSET::FLOW_GSET()
	{
		_bComplite		 = TRUE;
		_bRuning		 = TRUE;
	}

	GOODS_GSET::GOODS_GSET()
	{
		_bShowIntoPre	 = TRUE;
	}

	FLOWLIST_DATA::FLOWLIST_DATA()
	{
		_paID			= std::make_pair(_T("FLOW_ID"), _T(""));
		_paPayUser		= std::make_pair(_T("PAYTYPE"), _T(""));
		_paPayTime		= std::make_pair(_T("TIME"), _T(""));
		_paTotal		= std::make_pair(_T("TOTAL"), _T(""));
		_paPayType		= std::make_pair(_T("VALUETYPE"), _T(""));
		_paReceipt		= std::make_pair(_T("RECEIPT"), _T(""));
		_paReceiptNum	= std::make_pair(_T("RECEIPTNUM"), _T(""));
		_paRemark		= std::make_pair(_T("REMARK"), _T(""));
	}

}