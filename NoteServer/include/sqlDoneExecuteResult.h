#ifndef _SQL_DONE_EXECUTE_RESULT_H_
#define _SQL_DONE_EXECUTE_RESULT_H_
#include "sqlExecuteResult.h"

//done->update
class CSqlDoneExecuteResult:public CSqlExecuteResult
{
public:
    virtual ~CSqlDoneExecuteResult(){}

    virtual void assginParameter(const std::vector<std::string>& vecQueryElement)
    {
        if(vecQueryElement.size() != 2)
        {
            std::cout << "vecQueryElement does not meet the condition!" << std::endl;
            return ;
        }

        //m_dateStr = vecQueryElement[1].substr(0, 10);
        m_info_seq = vecQueryElement[1];
    }

    virtual int executeAndGetResult(std::string &result)
    {
        if(initMysqlConnection() < 0)
        {
            std::cout << "initMysqlConnection error" << std::endl;
            return -1;
        }

        int ret = mysql_query(m_mysql, getSqlStatment().c_str());
        if(ret == 0)//success
        {
            result = "mysql done operation success\n";
            std::cout << result << std::endl;
        }
        else
        {
            result = "mysql done operation failed\n";
            std::cout << result << std::endl;
            return -1;
        }        
    }

private:
    virtual std::string getOperationType() const
    {
        return std::string("Done");
    }

    virtual std::string getSqlStatment() const
    {
        return std::string("update noteinfo set complete_flag = 1 where info_seq = " + m_info_seq);
    }

private:
    std::string m_dateStr;
    std::string m_info_seq;
};

#endif