#include "sqlExecuteResultFactory.h"
#include "sqlExecuteResult.h"

CSqlExecuteResult* CSqlExecuteResultFactory::create(const std::string &operationType)
{
    // switch(operationType)
    // {
    //     case ADD_OPERATION:
    //         return new CSqlAddExecuteResult();
    //         break;
    //     case DEL_OPERATION:
    //         return new CSqlDeleteExecuteResult();
    //         break;
    //     case DONE_OPERATION:
    //         return new CSqlDoneExecuteResult();
    //         break;
    //     case GET_OPERATION:
    //         return new CSqlListExecuteResult();
    //         break;
    //     default:
    //         break;
    // }
    if(operationType == "add")
    {
        return new CSqlAddExecuteResult();
    }
    else if(operationType == "delete")
    {
        return new CSqlDeleteExecuteResult();
    }
    else if(operationType == "done")
    {
        return new CSqlDoneExecuteResult();
    }
    else if(operationType == "list")
    {
        return new CSqlListExecuteResult();
    }
}