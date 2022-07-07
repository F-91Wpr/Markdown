cp /opt/software/MySQL/mysql-connector-java-5.1.48.jar /opt/module/hive-3.1.2/lib/

4.2 Hive元数据配置到MySQL
4.2.1 拷贝驱动
将MySQL的JDBC驱动拷贝到Hive的lib目录下
[atguigu@hadoop102 lib]$ cp /opt/software/mysql-connector-java-5.1.27-bin.jar /opt/module/hive/lib/
4.2.2 配置Metastore到MySQL
在$HIVE_HOME/conf目录下新建hive-site.xml文件
[atguigu@hadoop102 conf]$ vim hive-site.xml
添加如下内容
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<configuration>
    <property>
        <name>javax.jdo.option.ConnectionURL</name>
        <value>jdbc:mysql://hadoop102:3306/metastore?useSSL=false&amp;useUnicode=true&amp;characterEncoding=UTF-8</value>
    </property>

    <property>
        <name>javax.jdo.option.ConnectionDriverName</name>
        <value>com.mysql.jdbc.Driver</value>
    </property>

    <property>
        <name>javax.jdo.option.ConnectionUserName</name>
        <value>root</value>
    </property>

    <property>
        <name>javax.jdo.option.ConnectionPassword</name>
        <value>000000</value>
    </property>

    <property>
        <name>hive.metastore.warehouse.dir</name>
        <value>/user/hive/warehouse</value>
    </property>

    <property>
        <name>hive.metastore.schema.verification</name>
        <value>false</value>
    </property>

    <property>
    <name>hive.server2.thrift.port</name>
    <value>10000</value>
    </property>

    <property>
        <name>hive.server2.thrift.bind.host</name>
        <value>hadoop102</value>
    </property>

    <property>
        <name>hive.metastore.event.db.notification.api.auth</name>
        <value>false</value>
    </property>
    
    <property>
        <name>hive.cli.print.header</name>
        <value>true</value>
    </property>

    <property>
        <name>hive.cli.print.current.db</name>
        <value>true</value>
    </property>
</configuration>
4.3 启动Hive
4.3.1 初始化元数据库
1）登陆MySQL
[atguigu@hadoop102 conf]$ mysql -uroot -p000000
2）新建Hive元数据库
mysql> create database metastore;

schematool -initSchema -dbType mysql -verbose
3）修改元数据库字符集
Hive元数据库的字符集默认为Latin1，由于其不支持中文字符，故若建表语句中包含中文注释，会出现乱码现象。如需解决乱码问题，须做以下修改。
修改Hive元数据库中存储注释的字段的字符集为utf-8
（1）字段注释
mysql> alter table COLUMNS_V2 modify column COMMENT varchar(256) character set utf8;
（2）表注释
mysql> alter table TABLE_PARAMS modify column PARAM_VALUE mediumtext character set utf8;
4）退出mysql
mysql> quit;
4.3.2 启动Hive客户端
1）启动Hive客户端
[atguigu@hadoop102 hive]$ bin/hive
2）查看一下数据库
hive (default)> show databases;
OK
database_name
default




###



6.1.1 Hive环境搭建
1）Hive引擎简介
	Hive引擎包括：默认MR、tez、spark
Hive on Spark：Hive既作为存储元数据又负责SQL的解析优化，语法是HQL语法，执行引擎变成了Spark，Spark负责采用RDD执行。
Spark on Hive : Hive只作为存储元数据，Spark负责SQL解析优化，语法是Spark SQL语法，Spark负责采用RDD执行。
2）Hive on Spark配置
（1）兼容性说明
注意：官网下载的Hive3.1.2和Spark3.0.0默认是不兼容的。因为Hive3.1.2支持的Spark版本是2.4.5，所以需要我们重新编译Hive3.1.2版本。
编译步骤：官网下载Hive3.1.2源码，修改pom文件中引用的Spark版本为3.0.0，如果编译通过，直接打包获取jar包。如果报错，就根据提示，修改相关方法，直到不报错，打包获取jar包。
（2）在Hive所在节点部署Spark
如果之前已经部署了Spark，则该步骤可以跳过
○1Spark官网下载jar包地址：
http://spark.apache.org/downloads.html
○2上传并解压解压spark-3.0.0-bin-hadoop3.2.tgz
[atguigu@hadoop102 software]$ tar -zxvf spark-3.0.0-bin-hadoop3.2.tgz -C /opt/module/
[atguigu@hadoop102 software]$ mv /opt/module/spark-3.0.0-bin-hadoop3.2 /opt/module/spark
（3）配置SPARK_HOME环境变量
[atguigu@hadoop102 software]$ sudo vim /etc/profile.d/my_env.sh
添加如下内容
# SPARK_HOME
export SPARK_HOME=/opt/module/spark
export PATH=$PATH:$SPARK_HOME/bin
source 使其生效
[atguigu@hadoop102 software]$ source /etc/profile.d/my_env.sh
（4）在hive中创建spark配置文件
[atguigu@hadoop102 software]$ vim /opt/module/hive/conf/spark-defaults.conf
添加如下内容（在执行任务时，会根据如下参数执行）
spark.master                               yarn
spark.eventLog.enabled                   true
spark.eventLog.dir                        hdfs://hadoop102:8020/spark-history
spark.executor.memory                    1g
spark.driver.memory					   1g
在HDFS创建如下路径，用于存储历史日志
[atguigu@hadoop102 software]$ hadoop fs -mkdir /spark-history
（5）向HDFS上传Spark纯净版jar包
	说明1：由于Spark3.0.0非纯净版默认支持的是hive2.3.7版本，直接使用会和安装的Hive3.1.2出现兼容性问题。所以采用Spark纯净版jar包，不包含hadoop和hive相关依赖，避免冲突。
	说明2：Hive任务最终由Spark来执行，Spark任务资源分配由Yarn来调度，该任务有可能被分配到集群的任何一个节点。所以需要将Spark的依赖上传到HDFS集群路径，这样集群中任何一个节点都能获取到。
○1上传并解压spark-3.0.0-bin-without-hadoop.tgz
[atguigu@hadoop102 software]$ tar -zxvf /opt/software/spark-3.0.0-bin-without-hadoop.tgz
○2上传Spark纯净版jar包到HDFS
[atguigu@hadoop102 software]$ hadoop fs -mkdir /spark-jars

[atguigu@hadoop102 software]$ hadoop fs -put spark-3.0.0-bin-without-hadoop/jars/* /spark-jars
（6）修改hive-site.xml文件
[atguigu@hadoop102 ~]$ vim /opt/module/hive/conf/hive-site.xml
添加如下内容
<!--Spark依赖位置（注意：端口号8020必须和namenode的端口号一致）-->
<property>
    <name>spark.yarn.jars</name>
    <value>hdfs://hadoop102:8020/spark-jars/*</value>
</property>
  
<!--Hive执行引擎-->
<property>
    <name>hive.execution.engine</name>
    <value>spark</value>
</property>
3）Hive on Spark测试
（1）启动hive客户端
[atguigu@hadoop102 hive]$ bin/hive
（2）创建一张测试表
hive (default)> create table student(id int, name string);
（3）通过insert测试效果
hive (default)> insert into table student values(1,'abc');
若结果如下，则说明配置成功
 
6.1.2 Yarn环境配置
1）增加ApplicationMaster资源比例
容量调度器对每个资源队列中同时运行的Application Master占用的资源进行了限制，该限制通过yarn.scheduler.capacity.maximum-am-resource-percent参数实现，其默认值是0.1，表示每个资源队列上Application Master最多可使用的资源为该队列总资源的10%，目的是防止大部分资源都被Application Master占用，而导致Map/Reduce Task无法执行。
生产环境该参数可使用默认值。但学习环境，集群资源总数很少，如果只分配10%的资源给Application Master，则可能出现，同一时刻只能运行一个Job的情况，因为一个Application Master使用的资源就可能已经达到10%的上限了。故此处可将该值适当调大。
（1）在hadoop102的/opt/module/hadoop-3.1.3/etc/hadoop/capacity-scheduler.xml文件中修改如下参数值
[atguigu@hadoop102 hadoop]$ vim capacity-scheduler.xml

<property>
    <name>yarn.scheduler.capacity.maximum-am-resource-percent</name>
    <value>0.8</value>
</property
（2）分发capacity-scheduler.xml配置文件
[atguigu@hadoop102 hadoop]$ xsync capacity-scheduler.xml
（3）关闭正在运行的任务，重新启动yarn集群
[atguigu@hadoop103 hadoop-3.1.3]$ sbin/stop-yarn.sh
[atguigu@hadoop103 hadoop-3.1.3]$ sbin/start-yarn.sh
