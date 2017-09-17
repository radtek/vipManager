/*============================================================================*
* 文件名:    ZWMenuGrid.h
* 版  权:    Copyright (c) ZWST, All Rights Reserved
* 版  本:    1.0
* 作  者:    lzk
* 描  述:    表格重写说明文件 修改自动对应数据库，*需要数据表有ID主键字段
* 历  史:    
=============================================================================*/
#pragma once
#include "GridCtrl.h"
#include <list>
#include <vector>
#include "boost/function.hpp"


class LZGridCtrl :public CGridCtrl
{
	DECLARE_DYNAMIC(LZGridCtrl)
public:
	enum GridType
	{
		gNONE = 0,
		gFlow,		// 工作流
		gUser,		// 会员管理
		gGoods,		// 商品管理
		gReport		// 报表
	};
public:
	LZGridCtrl(void);
	LZGridCtrl(GridType gtp);
	virtual ~LZGridCtrl(void);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void show();

	virtual void ClearGrid();
	virtual bool CreateGrid(CWnd* parent,UINT nID);
	virtual void AdjustLayout();
	virtual bool IsChanged(BOOL bDelSame = TRUE);
	virtual bool Init();
	// Edit
	virtual bool AddRow();
	virtual bool InsRow();
	virtual bool EmpRow();
	virtual bool DelRow();
	virtual BOOL AddRows(const int& nRows,const BOOL& bUp);
	virtual bool AddMultiRow();
	virtual bool InsMultiRow(std::vector<int>& vecRows);
	virtual bool Save();
	virtual void DbClickGrid(DWORD wp,DWORD lp);

	// 当数据库中的值与表格显示的值需要转换时重载此函数  例如0->零件，1->部件
	virtual CString getCellStr(const int& nRow,const int& nCol);
	virtual void getCellType(const int& nRow,const int& nCol,const std::vector<CString>& vecRowData,std::vector<CString>& vecopt);
	virtual void getCellData(std::list<std::pair<int,std::vector<CString> > > & listDataArry);
	virtual void endCellType(const int& nRow, const int& nCol, const CString& strData) { return; }; // 填充cell结束后的处理
	virtual bool submit_add(const CString& strField){return false;};
	virtual bool submit_del(const std::vector<int>& vecDel){return false;};
	virtual bool submit_edt(const int& nID,const CString& strField,const int& nRow){return false;};

	// 自动调整行高 列宽 ncWidth为参数个数 可选参数为列宽 参数不足时以最后一个为准 设置其余列
	virtual bool updateCellSize(int ncWidth,...); 

	// Action
	virtual void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
	virtual void OnEndEditCell(int nRow, int nCol, CString str);   //重写单元格编辑完毕函数 重载必须先调用父类
	

	GridType gType(){return m_gType;}
	void SetEndEdit();	// 手动解除cell编辑状态 用于无法激活onEndEditCell时使用
	int SetItemTextEx(int nRow, int nCol, LPCTSTR str,int nFlag = 1); // 1初始,2删,3增,4改
private:
	bool submit();
	int InsertRowEx(int nRow = -1);
	bool DeleteRowEx(int nRow);
	int GetlastInsert() {return m_nLastInsert;};
protected:
	// 辅助函数
	bool SetFlagNormal(int nRow);
	bool SetFlagAdd(int nRow);
	bool SetFlagEdit(int nRow);
	void updateCellType(int nRow);
	

	int GetTableRowCount(const CString& strTbname);
	bool GetPrg(CProgressCtrl*& pProGress);
	std::vector<int>& GetVecDel(){return m_vecDel;}
	int GetLineText(int nRow,CString& strRes/*OUT*/);
	// 进度条控制
	static bool StatBarBegin(const CString& strText,int nRange );
	static bool StatBarStep();
	static bool StatBarEnd(const CString& strText = _T(""));

	void SetItemMapID(int nRow,int nMapID);
	void SetItemFlag(int nRow,int nFlag);
	int  GetItemMapID(int nRow);
	int  GetItemFlag(int nRow);
public:
	bool  clearCol(int nCol);                               //清空列
	bool  clearRow(int nRow);                               //清空行													
	void  readOnlyCell(CGridCellBase* cel, bool bEn = true); // 设置单元格为只读
protected:
	GridType m_gType;
	CWnd* m_pView;

	static CMFCRibbonStatusBar *m_pStatusBar;
	static CProgressCtrl *m_pProGress;
	static int m_nProStep;
	CToolTipCtrl m_TTC;  // 气泡提示
private:
	int m_nLastInsert;				// 上一次插入的索引ID
	int m_nIndexNum;				// Grid递增索引
	std::vector<int> m_vecDel;		// 删除的行sql索引
};

