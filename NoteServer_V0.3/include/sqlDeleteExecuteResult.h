#ifndef _SQL_DELETE_EXECUTE_RESULT_H_
#define _SQL_DELETE_EXECUTE_RESULT_H_

//delete->delete
class CSqlDeleteExecuteResult:public CSqlExecuteResult
{
public:
    virtual ~CSqlDeleteExecuteResult(){}

    virtual void assginParameter(std::vector<std::string>& vecQueryElement)
    {
        if(vecQueryElement.size() != 3)
        {
            std::cout << "vecQueryElement is not meet the condition!" << std::endl;
            return ;
        }

        m_dateStr = vecQueryElement[1].substr(0, 10);
        m_info_seq = vecQueryElement[2];
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
        return std::string("delete from noteinfo where start_time like \'" + m_dateStr + "%%\' and info_seq = " + m_info_seq);
    }
    
private:
    std::string m_dateStr;
    std::string m_info_seq;
};

#endif
