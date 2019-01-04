#ifndef _SQL_LIST_EXECUTE_RESULT_H_
#define _SQL_LIST_EXECUTE_RESULT_H_

#include "sqlExecuteResult.h"

//list->select
class CSqlListExecuteResult:public CSqlExecuteResult
{
public:
    virtual ~CSqlListExecuteResult(){}

    virtual void assginParameter(const std::vector<std::string>& vecQueryElement)
    {
        if(vecQueryElement.size() != 2)
        {
            std::cout << "vecQueryElement does not meet the condition!" << std::endl;
            return ;
        }

        m_dateStr = vecQueryElement[1].substr(0, 10);
    }
    
    virtual int executeAndGetResult(std::string &result)
    {
        if(initMysqlConnection() < 0)
        {
            std::cout << "initMysqlConnection error" << std::endl;
            return -1;
        }

        std::cout << "getSqlStatment: " << getSqlStatment().c_str() << std::endl;
        int ret = mysql_query(m_mysql, getSqlStatment().c_str());
        if(ret != 0)
        {
            std::cerr << "msyql_query failed!" << std::endl;
            return -1;
        }
        getQueryReuslt(result);
        
        // if(ret = 1)//success
        // {
        //     std::cout << result << std::endl;
        // }
        // else
        // {
        //     std::cout << result << std::endl;
        //     return -1;
        // }   
    }

private:
    virtual std::string getOperationType() const
    {
        return std::string("List");
    }

    virtual std::string getSqlStatment() const
    {
        return std::string("select info_seq, start_time, end_time, issue_content, complete_flag, serial_num_of_the_day, state from noteinfo where start_time like \'" + m_dateStr + "%%\'");
    }

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

private:
    std::string m_dateStr;
    MYSQL_RES* m_res;
	MYSQL_ROW m_row;
};


#endif
