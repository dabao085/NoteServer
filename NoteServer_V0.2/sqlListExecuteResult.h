#ifndef _SQL_LIST_EXECUTE_RESULT_H_
#define _SQL_LIST_EXECUTE_RESULT_H_

#include "sqlExecuteResult.h"

//list->select
class CSqlListExecuteResult:public CSqlExecuteResult
{
public:
    virtual ~CSqlListExecuteResult(){}

    virtual void assginParameter(std::vector<std::string>& vecQueryElement)
    {
        if(vecQueryElement.size() != 2)
        {
            std::cout << "vecQueryElement is not meet the condition!" << std::endl;
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
        getQueryReuslt(result);
        
        if(ret = 1)//success
        {
            std::cout << result << std::endl;
        }
        else
        {
            std::cout << result << std::endl;
            return -1;
        }          
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
private:
    std::string m_dateStr;
};


#endif
