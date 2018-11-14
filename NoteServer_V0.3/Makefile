CFLAG = -Iinclude
LDLIBS = -L/usr/lib64/mysql -lmysqlclient
TARGET = server/noteServer client/noteClient

all: ${TARGET}

server/noteServer: server/noteServer.o server/sqlExecuteResultFactory.o
	g++ -o $@ $^ ${CFLAG} ${LDLIBS}
client/noteClient: client/noteClient.o 
	g++ -o $@ $^ ${CFLAG} ${LDLIBS}

server/noteServer.o: server/noteServer.cpp
	g++ -c -o $@ $^ ${CFLAG} 
server/sqlExecuteResultFactory.o: server/sqlExecuteResultFactory.cpp
	g++ -c -o $@ $^ ${CFLAG} 
client/noteClient.o: client/noteClient.cpp  include/noteHeader.h include/noteClient.h
	g++ -c -o $@ $< ${CFLAG}
clean:
	rm -f ${TARGET} */*.o *.o