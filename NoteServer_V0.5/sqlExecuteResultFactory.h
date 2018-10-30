#ifndef _SQL_EXECUTE_RESULT_FACTORY_H_
#define _SQL_EXECUTE_RESULT_FACTORY_H_

#include "sqlExecuteResult.h"
#include "sqlAddExecuteResult.h"
#include "sqlListExecuteResult.h"
#include "sqlDeleteExecuteResult.h"
#include "sqlDoneExecuteResult.h"


//单例模式的工厂
class CSqlExecuteResultFactory
{
public:
    enum
    {
        ADD_OPERATION = 0,
        DEL_OPERATION = 1,
        DONE_OPERATION = 2,
        GET_OPERATION = 3
    };

public:
    static CSqlExecuteResult* create(const std::string &operationType);
};

#endif