#pragma once
#include "LZGridCtrl.h"
#include "DataType.h"
using namespace  DataType;

extern CUSTOM_GSET g_customSet;

class LZGridCtrlUser :
	public LZGridCtrl
{
public:
	LZGridCtrlUser();
	~LZGridCtrlUser();

private:
	CString   m_strTblName;	// 标准库表名

public:
	virtual bool Init();			// 必须重载 初始化
									// 行中包含特殊单元格时 重载
	virtual void getCellType(const int& nRow, const int& nCol, const std::vector<CString>& vecRowData, std::vector<CString>& vecopt);
									// 显示数据 重载
	virtual void getCellData(std::list<std::pair<int, std::vector<CString>>>& listDataArry);

};

