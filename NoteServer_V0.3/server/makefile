#deps = noteHeader.h

#objs = noteClient.o noteServer.o
all: noteServer

#stringUtil.o: stringUtil.cpp stringUtil.h
#       g++ -c stringUtil.cpp

#noteClient.o: noteClient.cpp noteClient.h
#       g++ -c noteClient.cpp

#noteServer.o: noteServer.cpp noteServer.h
#       g++ -c noteServer.cpp

#noteClient: noteClient.cpp noteClient.h
#        g++ -g -o noteClient noteClient.cpp -L/usr/lib64/mysql -lmysqlclient

noteServer: noteServer.cpp stringUtil.cpp sqlExecuteResultFactory.cpp
        g++ -g -o noteServer noteServer.cpp stringUtil.cpp sqlExecuteResultFactory.cpp -I../include -L/usr/lib64/mysql -lmysqlclient

clean:
        rm noteServer