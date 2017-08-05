

#include "StdAfx.h"
#include <mysql_connection.h>   
#include <mysql_driver.h>   
#include <statement.h> 
#include "sqlstring.h"
#include "exception.h"

#pragma once
#define MAX_SQL 1024
#define MAX_SQL2 2048

#	ifdef MYSQL_MODULE
#		define MYSQLExport   __declspec(dllexport)
#	else
#		define MYSQLExport   __declspec(dllimport)
#	endif

namespace IUDB
{
	class CMysqlDbConnect;
}

namespace IUDB
{
	enum EnTableMode
	{
		enTableRead,		//读取方式
		enTableWrite,	//写入方式
		enTableModify	//更改方式
	};

	class /*MYSQLExport*/ MysqlManager
	{
	public:
		MysqlManager(const CString& strDbName,const CString& strTcpIp, const int& iPort,const CString& strUesrName,const CString& strPassWord,const CString& dbType);
		~MysqlManager(void);

	public:
		static CString DBLZManager;		// 综合管理库
		static CString DBLZCustomer;	// 用户管理库
		
		const CString& getType(void)const { return m_dbType; }
		const CString& getTcpIp(void)const { return m_strTcpIp; }
		const int& getPort(void)const { return m_iPort; }
		const CString& getUesrName(void)const { return m_strUesrName; }
		const CString& getPassWord(void)const { return m_strPassWord; }
		const CString& getDbName(void)const { return m_stdDbName; }

	public:
		/*!
		 * 创建数据库 并给当前用户创建所有权限
		 * \param [in] dbName 库名
		 */
		bool CreateDb(const CString& dbName);

		/*!
		 * 授予数据库管理员权限
		 */
		bool GrantAllPrivileges(const CString& pszUser);

		/*!
		 * 得到当前正在操作的数据库
		 */
		CString GetCurrentDataBase();

		/*! 
		 * 选择数据库, 切换当前为指定的数据库
		 */
		bool SelectDataBase(const CString& pszDbName);

		/*!
		 * 判断是否存在数据库
		 * \param [in] dbName 库名
		 */
		bool isExitsDataBase(const CString& dbName);

		/*!
		 * 判断是否存在指定表
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tblname 表名
		 */
		bool IsExitTable(const CString& tblName);

		/*!
		 * 判断指定库中是否存在指定表
		 * \param [in] dbName 库名
		 * \param [in] tblName 表名
		 */
		bool IsExitTable(const CString& dbName, const CString& tblName);

		/*!
		 * 判断指定库指定表中是否存在指定字段
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tblName 表名
		 * \param [in] fieldName 字段名
	 	 */
		bool isExitsField(const CString& tblName, const CString& fieldName);

		/*!
		 * 判断指定库指定表中是否存在指定字段
		 * \param [in] dbName 库名
		 * \param [in] tblName 表名
		 * \param [in] fieldName 字段名
	 	 */
		bool isExitsField(const CString& dbName, const CString& tblName, const CString& fieldName);

		/*!
		 * 获取指定表的行数(记录数)
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tableName 表名
		 */
		int GetRowCounts(const CString& tableName);
		
		/*!
		 * 获取指定库指定表的行数(记录数)
		 * \param [in] dbName 库名
		 * \param [in] tableName 表名
		 */
		int GetRowCounts(const CString& dbName,const CString& tableName);
		
		/*!
		 * 获取指定表的列数(字段数)
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tableName 表名
		 */
		int GetColCounts(const CString& tableName);
		
		/*!
		 * 获取指定库指定表的列数(字段数)
		 * \param [in] dbName 库名
		 * \param [in] tableName 表名
		 */
		int GetColCounts(const CString& dbName, const CString& tableName);

		//函数释放不知原因崩溃 禁用
		bool GetColsInfo(const CString& dbName, const CString& tblName, CStringArray& fieldNames,CStringArray& filedTypes);

		/*!
		 * 返回指定表中ID列最大的值
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \note 如果不存在ID列失败
		 * \param [in] tableName 表名
		 */
		int GetLastId(const CString& tableName);

		/*!
		 * 返回指定库指定表中ID列最大的值
		 * \note 如果不存在ID列失败
		 * \param [in] dbName 库名
		 * \param [in] tableName 表名
		 */
		int GetLastId(const CString& dbName, const CString& tableName);
		

		/////////   查询结果操作    //////////////////
		/*!
		 * 将当前行移至结果数据起始行之前(此行无数据只是位置)
		 */
		void BeforeFirst();
		/*!
		 * 将查询结果中指定行设为当前行
		 */
		bool MoveRow(int rowIndex);
		/*!
		 * 移动至下一行
		 */
		bool Next();
		/////////   查询结果操作    //////////////////


		/////////////  批量读  ///////////////////////////////
		/*!
		 * 从查询结果当前行获取浮点数值
		 * \param [in] columnIndex 列索引，从1开始
		 */
		bool GetBoolean(int columnIndex) const;
		bool GetBoolean(const CString& columnLabel) const;

		/*!
		 * 从查询结果当前行获取浮点数值
		 * \param [in] columnIndex 列索引，从1开始
		 */
		long double GetDouble(int columnIndex) const ;
		long double GetDouble(const CString& columnLabel) const;

		/*!
		 * 从查询结果当前行获取整数值
		 * \param [in] columnIndex 列索引，从1开始
		 */
		int GetInt32(int columnIndex) const;
		int GetInt32(const CString& columnLabel) const;

		/*!
		 * 从查询结果当前行获取字符串
		 * \param [in] columnIndex 列索引，从1开始
		 */
		CString GetString(int columnIndex)  const ;
		CString GetString(const CString& columnLabel) const ;

		/*!
		 * 从查询结果当前行获取数据流
		 * \param [in] columnIndex 列索引，从1开始
		 */
		std::istream* GetBlob(int columnIndex) const;
		std::istream* GetBlob(const CString& columnLabel)const;
		/////////////  end 批量读入  ///////////////////////////

		///////////////  批量写入  ////////////////////////////////
		void SetInt(unsigned int parameterIndex, int val, bool bNull=false);
		void SetString(unsigned int parameterIndex, const CString& val, bool bNull=false);
		void SetDouble(unsigned int parameterIndex, double val, bool bNull=false);
		void SetBoolean(unsigned int parameterIndex, bool val, bool bNull=false);
		void SetBlob(unsigned int parameterIndex, std::istream * blob, bool bNull=false);
		////////////// end 批量写入  ////////////////////////////////

		///////////////      单记录操作    /////////////////////////
		/*!
		 * 添加一行
		 * \param [in] 表名
		 * \param [in] 列名
		 * \param [in] 当前行记录值
		 */
		bool addOneRow(const CString& tblName,const CStringArray&colNames, const CStringArray&rowVals);
		
		/*!
		 * 更新行
		 * \param [in] 表名
		 * \param [in] 行索引
		 * \param [in] 列名
		 * \param [in] 当前记录值
		 */
		bool updateOneRow(const  CString& tblName, const int& rowIndex,
			const CStringArray&colNames, const CStringArray&rowVals);
		
		/*!
		 * 删除行
		 * \param [in] 表名
		 * \param [in] 航索引
		 */
		bool deleteOneRow(const CString& pTblName,const int rowIndex);
		///////////////   end   单记录操作    /////////////////////////

		////////////////     列操作  //////////////////////////////////////
		/*!
		 * 向指定表增加列
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tblName 表名
		 * \param [in] columnName 列名
		 * \param [in] columType 列类型
		 */
		bool AddColumn(const CString& tblName, const CString& columnName, 
			const CString& columType);
		/*!
		 * 向指定表增加列
		 * \param [in] dbName 库名
		 * \param [in] tblName 表名
		 * \param [in] columnName 列名
		 * \param [in] columType 列类型
		 */
		bool AddColumn(const CString& dbName, const CString& tblName, const CString& columnName, 
			const CString& columType);
		
		/*!
		 * 修改指定表指定列的列名
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tblName 表名
		 * \param [in] oldColumnName 旧列名
		 * \param [in] newColmName 新列名
		 * \param [in] columnType 列类型
		 */
		bool ModifyColumName(const CString& tblName,const CString& oldColumnName,
			const CString& newColmName,const CString& columType);
		/*!
		 * 修改指定库指定表指定列的列名
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] dbName 库名
		 * \param [in] tblName 表名
		 * \param [in] oldColumnName 旧列名
		 * \param [in] newColmName 新列名
		 * \param [in] columnType 列类型
		 */
		bool ModifyColumName(const CString& dbName, const CString& tblName,const CString& oldColumnName,
			const CString& newColmName,const CString& columType);
		
		/*!
		 * 修改指定表指定列的类型
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \note [in] tblName 表名
		 * \note [in] columnName 列名
		 * \note [in] newColmType 列类型
		 */
		bool ModifyColumType(const CString& tblName,const CString& columnName,
			const CString& newColmType);
		/*!
		 * 修改指定库指定表指定列的类型
		 * \note [in] dbName 库名
		 * \note [in] tblName 表名
		 * \note [in] columnName 列名
		 * \note [in] newColmType 列类型
		 */
		bool ModifyColumType(const CString& dbName, const CString& tblName,const CString& columnName,
			const CString& newColmType);
		////////////////   end  列操作  //////////////////////////////////////
		
		//////////////////      表操作  /////////////////////////////////
		bool CreateCustomTable(const CString& strSql);

#ifndef NDEBUG
		//该接口可能存在问题，暂时先不要使用
		bool CreateTable(const CString& dbName, const CString& tblName);	

		//该接口可能存在问题，暂时先不要使用
		//primarkKey:主键
		bool CreateTable(const  CString& tblName, const CStringArray& colNames,
			const CStringArray& colTypes,const  CString& primarkKey);
#endif
		
		/*!
		 * 删除指定表
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tblName 表名
		 */
		bool DropTable(const CString& tblName);

		/*!
		 * 删除指定库指定表
		 * \param [in] dbname 库名
		 * \param [in] tblName 表名
		 */
		bool DropTable(const CString& dbName, const CString& tblName);

		/*!
		 * 为指定表指定列创建索引
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tblName 表名
		 * \param [in] colName 列名
		 * \param [in] idexName 索引列名
		 */
		bool CreateIndex(const CString& tblName, const CString& colName,const CString& idxName);	

		/*!
		 * 为指定库指定表指定列创建索引
		 * \param [in] dbName 库名
		 * \param [in] tblName 表名
		 * \param [in] colName 列名
		 * \param [in] idexName 索引列名
		 */
		bool CreateIndex(const CString& dbName, const CString& tblName, const CString& colName,const CString& idxName);	

		/*!
		 * 清空表内数据
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] tblname 表名
		 */
		bool ClearTable(const CString& tblName);

		/*!
		 * 清空指定库中指定表内的数据
		 * \param [in] dbName 库名
		 * \param [in] tblName 表名
		 */
		bool ClearTable(const CString& dbName, const CString& tblName);

		/*!
		 * 拷贝表
		 * \note 数据库为空时，使用之前调用SelectDataBase()选择的库
		 * \param [in] destTblName 目标表名
		 * \param [in] orgTblNaME 源表名
		 * \param [in] isContainData 是否同时拷贝表内数据
		 * \param [in] destDBName 目标库
		 * \param [in] orgDBName 源库
		 */
		bool CopyTable(const CString& destTblName,
			const CString& orgTblName,
			bool isContainData=false,
			const CString& destDBName = _T(""),
			const CString& orgDBName = _T(""));

		/*!
		 * 根据限定条件拷贝表(只拷贝表数据)
		 * \note 使用前，请保证已调用过SelectDataBase()
		 */
		bool CopyTableData(const CString& pDestTblName,const CString& pSelectSql);

		/*!
		 * copy表的数据到目标表的指定列中(无限定条件，未进行异常处理)
		 * \note 使用前，请保证已调用过SelectDataBase()
		 */
		bool CopyTableDataByDefCols(const CString& pDestTblName,const CStringArray& colNames,
			const CString& pSelectSql);

		/*!
		 * 打开指定表
		 * \note 使用前，请保证已调用过SelectDataBase()
		 * \param [in] 表名
		 * \param [in] 打开模式
		 */
		int OpenTable(const CString& tableName, EnTableMode enM);

		/*!
		 * 打开指定库指定表
		 * \param [in] 库名
		 * \param [in] 表名
		 * \param [in] 打开模式
		 */
		int OpenTable(const CString& dbName, const CString& pTblName, EnTableMode enM);

		/*!
		 * 修改表记录时使用, 这种方式比上一个打开表的函数效率高,但需要传入所有列名
		 * \param [in] 库名
		 * \param [in] 表名
		 * \param [in] 列名
		 * \param [in] 打开模式
		 * \return TRUE;FALSE
		 */
		int OpenTable(const CString& dbName, const CString& tableName,const CStringArray& colName,EnTableMode enM);

		//	@bref: 查询完必须调用，否则容易内存泄露
		bool CloseTable();
		//////////////////  end  表操作    /////////////////////////////////

		////////////////  sql脚本操作  //////////////////////////
		/*!
		 *执行sql记录集, 执行完毕需要关闭 eg:  可以通过sql语句
		 *			直接查询当前查询的所有记录, 也可以指定单条记录
		 * \param [in] sqlScript sql脚本
		 */
		int ExecutSqlAndReturn(const CString& sqlScript);

		/*!
		 * 关闭sql记录集
		 */
		bool CloseSqlRecords();

		/*!
		 * 执行指定sql脚本
		 * \param [in] sqlScript 脚本
		 */
		bool ExecutSql(const CString& sqlScript);
		//////////////// end  sql脚本操作  //////////////////////////


		/////////////////////////////////////////////////////////////////////
		bool connectSvc();
		bool disconnectSvc();
		bool reconnectSvc();

		static bool SQLlogger(const CString strContent, const CString strFile = _T("./zwdb.err")); // 错误日志
	protected:
		sql::SQLString makeCnName() const;
		sql::SQLString getSqlString(const CString& strString) const;
		CString getErrorInfo(const sql::SQLException& er) const;

		bool InitWriteCache();		// 初始化缓存
		bool InitReadCache();	//初始化读缓存
		bool CloseReadCache();	//关闭读缓存

		bool CloseWriteCache();		// 关闭缓存
		bool openDb();

		
	protected:
		CString m_strTcpIp;  //IP地址
		int m_iPort;	//数据库端口
		CString m_strUesrName;	//用户名
		CString m_strPassWord;	//密码
		CString m_dbType;
		CString m_stdDbName;	//连接或切换数据库后为当前使用的数据库名称
		int m_nExcueCols;
		CStringArray m_fieldNames;
		CStringArray m_filedTypes;
	private:
		sql::Driver *m_pDriver;
		sql::Connection* m_pCon;
		sql::Statement* m_pState;	//OpoenTable或ExecutSqlAndReturn后关闭钱，不为空
		sql::ResultSet* m_pRes;		
		sql::PreparedStatement * m_pPreMst;
		EnTableMode m_enMode;				//当前模式
		sql::SQLString m_sqlScript;		//sql 临时脚本
		
	};
}

