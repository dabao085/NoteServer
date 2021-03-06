#ifndef _SQL_DELETE_EXECUTE_RESULT_H_
#define _SQL_DELETE_EXECUTE_RESULT_H_
#include "sqlExecuteResult.h"

//delete->delete
class CSqlDeleteExecuteResult:public CSqlExecuteResult
{
public:
    virtual ~CSqlDeleteExecuteResult(){}

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
            result = "mysql delete operation success\n";
            std::cout << result << std::endl;
        }
        else
        {
            result = "mysql delete operation failed\n";
            std::cout << result << std::endl;
            return -1;
        }        
    }
    
private:
    virtual std::string getOperationType() const
    {
        return std::string("Delete");
    }

    virtual std::string getSqlStatment() const
    {
        return std::string("delete from noteinfo where info_seq = " + m_info_seq);
    }
    
private:
    std::string m_dateStr;
    std::string m_info_seq;
};

#endif
