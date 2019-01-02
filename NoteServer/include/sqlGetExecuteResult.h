#ifndef _SQL_GET_EXECUTE_RESULT_H_
#define _SQL_GET_EXECUTE_RESULT_H_

//get->select
// class CSqlGetExecuteResult:public CSqlExecuteResult
// {
// public:
//     virtual ~CSqlGetExecuteResult();

//     virtual void assginParameter(std::vector<std::string>& vecQueryElement)
//     {
//         if(vecQueryElement.size() != 2)
//         {
//             std::cerr << "vecQueryElement is not meet the condition!" << std::endl;
//             return ;
//         }

//         m_dateStr = vecQueryElement[1].substr(0, 10);
//     }

// private:
//     virtual std::string getOperationType() const
//     {
//         return std::string("Get");
//     }
// };

#endif