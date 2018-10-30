#ifndef _SQL_EXECUTE_RESULT_H_
#define _SQL_EXECUTE_RESULT_H_

#include <string>
#include <iostream>
#include <vector>
#include "noteMysql.h"
#include "string.h"

class CSqlExecuteResult
{
public:
    virtual ~CSqlExecuteResult(){}

    //赋予SQL所需参数
    virtual void assginParameter(std::vector<std::string>& vecQueryElement) = 0;
    //查询入口，执行SQL语句并保存执行结果
    virtual int executeAndGetResult(std::string &result) = 0;

protected:
    //初始化数据库连接
    int initMysqlConnection()
    {
        m_mysql =  mysql_init(NULL);
	    unsigned int timeout = 3000;
	    mysql_options(m_mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

	    if (mysql_real_connect(m_mysql, DBHOST, DBUSER, DBPASS, DBNAME, DBPORT, DBSOCK, DBPCNT) == NULL)
        {
            std::cout << "connect failed: " << mysql_error(m_mysql) << std::endl;
		    mysql_close(m_mysql);
		    mysql_library_end();
		    return -1;
	    }
    }

    //获取SQL语句
    virtual std::string getSqlStatment() const = 0;
    //获取SQL操作类型
    virtual std::string getOperationType() const = 0;
    //查询并保存结果
    void getQueryReuslt(std::string &result)
    {
        m_res = mysql_store_result(m_mysql);
		if(m_res == NULL)
		{
            std::cout << "mysql_store_result failed" << std::endl;
			return ;
		}

		m_row = mysql_fetch_row(m_res);
		int fieldcount = mysql_num_fields(m_res);
		int rows = mysql_num_rows(m_res);
		char buf[32];
        std::string queryResultBuf;
		int i, j;

		for(j = 0; j < rows; ++j)
		{
			for(i = 0; i < fieldcount; ++i)
			{
				//memset(buf, 0, sizeof(buf));
				if (m_row[i] != NULL && (strlen(m_row[i]) > 0))
				{
					//snprintf(buf, sizeof(buf), "%s", row[i]);
                    queryResultBuf = m_row[i];
					result += queryResultBuf + " ";
					std::cout << queryResultBuf << " ";
				}
			}
            result += "\n";
			std::cout << std::endl;
			m_row = mysql_fetch_row(m_res);
		}
		std::cout << "result: " << std::endl;
        std::cout << result;
    }

protected:
    std::string operationType;
    MYSQL_RES* m_res;
	MYSQL_ROW m_row;
	MYSQL* m_mysql;
};

#endif