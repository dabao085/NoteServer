#ifndef _SQL_ADD_EXECUTE_RESULT_H_
#define _SQL_ADD_EXECUTE_RESULT_H_

#include "sqlExecuteResult.h"

//Add->Insert
class CSqlAddExecuteResult:public CSqlExecuteResult
{
public:
    virtual ~CSqlAddExecuteResult(){}
    virtual void assginParameter(const std::vector<std::string>& vecQueryElement)
    {
        if(vecQueryElement.size() != 4)
        {
            std::cout << "vecQueryElement does not meet the condition!" << std::endl;
            return ;
        }

        m_startTime = vecQueryElement[1];
        m_endTime = vecQueryElement[2];
        m_content = vecQueryElement[3];
    }

    virtual int executeAndGetResult(std::string &result)
    {
        if(initMysqlConnection() < 0)
        {
            std::cout << "initMysqlConnection error" << std::endl;
            return -1;
        }

        int ret = mysql_query(m_mysql, getSqlStatment().c_str());
        if(ret = 1)//success
        {
            result = "mysql add operation success\n";
            std::cout << result << std::endl;
        }
        else
        {
            result = "mysql add operation failed\n";
            std::cout << result << std::endl;
            return -1;
        }        
    }

protected:
    virtual std::string getSqlStatment() const
    {
        return std::string("insert into noteinfo(start_time,end_time,issue_content,complete_flag,serial_num_of_the_day,state) values('" + m_startTime + "', '" + m_endTime + "', '" + m_content + "',0,0,'F0A')");
    }


    virtual std::string getOperationType() const
    {
        return std::string("Add");
    }

private:
    std::string m_startTime;
    std::string m_endTime;
    std::string m_content;
};

#endif