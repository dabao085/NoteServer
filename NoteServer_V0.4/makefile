#deps = noteHeader.h 

#objs = noteClient.o noteServer.o 

noteClient: noteClient.o
    g++ -g -o noteClient noteClient.c -L/usr/lib64/mysql -lmysqlclient

noteServer: noteServer.o
    g++ -g -o noteServer noteServer.c -L/usr/lib64/mysql -lmysqlclient

noteClient.o: noteClient.c noteClient.h
    g++ -c noteClient.c

noteServer.o: noteServer.c noteServer.h
    g++ -c noteServer.c

clean:
    rm *.o