#deps = noteHeader.h

#objs = noteClient.o noteServer.o
all: noteClient

#stringUtil.o: stringUtil.cpp stringUtil.h
#       g++ -c stringUtil.cpp

#noteClient.o: noteClient.cpp noteClient.h
#       g++ -c noteClient.cpp

#noteServer.o: noteServer.cpp noteServer.h
#       g++ -c noteServer.cpp

noteClient: noteClient.cpp
        g++ -g -o noteClient noteClient.cpp -I../include -L/usr/lib64/mysql -lmysqlclient

#noteServer: noteServer.cpp stringUtil.cpp noteServer.h stringUtil.h sqlExecuteResult.h sqlAddExecuteResult.h sqlListExecuteResult.h sqlDeleteExecuteResult.h sqlDoneExecuteResult.h sqlGetExecuteResult.h sqlExecuteResultFactory.h
#        g++ -g -o noteServer noteServer.cpp stringUtil.cpp sqlExecuteResultFactory.cpp -L/usr/lib64/mysql -lmysqlclient

clean:
        rm noteClient