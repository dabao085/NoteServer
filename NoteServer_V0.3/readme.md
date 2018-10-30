###1.这是一个简易的备忘录小程序,包括:
1) 网络编程和服务器设计  
2) 多进程和多线程(后续版本里会体现)  
3) 设计模式的使用
4) IO复用,epoll
5) 其他特性(比如reactor模式等)
6) 具体编译和使用方法见各版本的子目录  


注意:该小程序目前支持在本地网络上运行。  

###2.使用方法  
1) server   
./noteServer  
2) client  
./noteClient [add] [get] [list] [delete] [done]
Example:
./noteClient add 2018-07-10_9:00:00  2018-07-10_10:00:00 看书 #向Note程序添加一条事项，默认为未完成，如果时间结束时间比当前时间早，则返回一条错误信息
./noteClient get 2018-07-10_9:00:00 2018-07-10_10:00:00 #返回该时间段所有事项，并打印这些事项
./noteClient list 2018-07-10 #打印当天所有事项
./noteClient delete 2018-07-10 1 #删除当天编号1的事项
./noteClient done 2018-07-10 1 #设置当天编号1的事项为完成

###3.编译方法
1) make clean
2) make
会在当前目录下生成noteClient和noteServer

###4.数据库配置
1) 修改noteMysql.h中的相关配置

===========================================================
###1.this is a simple & small memo program, while includes:
1) network program and server design
2) multi process and multi thread(to be continued...)
3) the usage of design pattern(to be continued...)
4) IO multiplexing
5) other new features(to be continued...)
6) more details are introduced in coresponding directories

###2.usage
1) server 
./noteServer 
2) client 

./noteClient [add] [get] [list] [delete] [done]
Example:
./noteClient add 2018-07-10_9:00:00  2018-07-10_10:00:00 reading #向Note程序添加一条事项，默认为未完成，如果时间结束时间比当前时间早，则返回一条错误信息
./noteClient get 2018-07-10_9:00:00 2018-07-10_10:00:00 #返回该时间段所有事项，并打印这些事项
./noteClient list 2018-07-10 #打印当天所有事项
./noteClient delete 2018-07-10 1 #删除当天编号1的事项
./noteClient done 2018-07-10 1 #设置当天编号1的事项为完成

###3.compile
1) make clean
2) make
It will generate noteClient and noteServer under current directory

###4.configure datebase
1) modify the configurations in noteMysql.h 