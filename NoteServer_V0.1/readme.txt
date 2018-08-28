备忘录程序
1.由服务端和客户端组成
2.客户端传递参数，服务端返回查询结果
3.客户端录入数据，服务端写入数据库
ADD:有一个守护程序(脚本)，定时发送邮件提醒未完成事项，若当天仍有事项未完成，晚上8点批量发送一次至邮箱。

4.客户端:NoteClient
NoteClient write 时间段[2018-07-10 9:00 2018-07-10 10:00] "Things todo"	//Note:if there is no data specified, then default date is today; if the first date parameter contains the date, then the second parameter's default date is same as the first parameter's.
5.服务端:NoteServer
服务端读入请求，决定写入数据库还是读取数据库信息。
ADD:酌情考虑开发Windows版本的客户端
ADD:酌情考虑开发Linux图形界面客户端

6.开发周期(两周2018-07-10 - 2018-07-24)

7.工作安排
7/10:
1)确定客户端参数格式
能写入(add),能读取(get),能根据日期列出TODOlist(list),能删除(delete)错误的事项,能将已完成的事情单次或者批量提交(done),服务器更新数据库，打上已完成标志。
NoteClient [add] [get] [list] [delete] [done]
Example:
NoteClient add 2018-07-10 9:00 - 2018-07-10 10:00 看书 #向Note程序添加一条事项，默认为未完成，如果时间结束时间比当前时间早，则返回一条错误信息

NoteClient get 2018-07-10 9:00 - 2018-07-10 10:00 #返回该时间段所有事项，并打印这些事项
#NoteClient get 2018-07-10 1 #打印当天编号1的事项，功能与list有重复

NoteClient list 2018-07-10 #打印当天所有事项

NoteClient delete 2018-07-10 1 #删除当天编号1的事项

NoteClient done 2018-07-10 1 #设置当天编号1的事项为完成

2)服务端架构设计
当前选用最简单的fork模型，便于后期扩展。
3)数据库结构设计
由于备忘录程序功能简单，故只设置一张表NoteInfo
CREATE TABLE `noteinfo` (
  `info_seq` int(10) NOT NULL COMMENT '主键id',
  `start_time` timestamp COLLATE utf8mb4_bin NOT NULL COMMENT '开始时间',
  `end_time` timestamp COLLATE utf8mb4_bin NOT NULL COMMENT '结束时间',
  `issue_content` varchar(255) COLLATE utf8mb4_bin NOT NULL COMMENT '事项内容',
  `complete_flag` tinyint(4) COLLATE utf8mb4_bin NOT NULL COMMENT '完成情况，1表示已完成，0表示未完成',
  `serial_num_of_the_day` varchar(10) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '当天事项的编号',
  `state` varchar(3) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '状态：FOA 有效 FOX 无效',
  PRIMARY KEY (`info_seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin COMMENT='备忘录信息表';

无法建表:
set @@session.explicit_defaults_for_timestamp=on;

TODO:
设计数据库的时候遇到了几个问题
1)mysql中，日期格式有哪几种，各种格式之间的区别。
timestamp占用空间较小，但是时间只能表示到2038年。分组数据库里两种(timestamp和datetime)同时存在，无法作为参考
2)建表语句中需要注意的事情。

7/19:
7/20:
There are several functions should be defined:
1.deal with mysql connection(including query and fetch result).
2.return value to client.
7/21
There are still some problems need to resolve:
1)The format of time should transform from string like 2018-07-21_09:00:00 to 2018-07-21 09:00:00