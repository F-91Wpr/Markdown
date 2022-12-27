# 从零搭建Hadoop集群

1. 安装虚拟机，安装过程中需要更改的配置如下
   - 改分区——自动分区即可
   - 关闭kdump
   - 更改网络配置——直接可以在安装过程中讲网络改为静态地址，hostname配好
   - 添加新的root密码：1

2. 添加常用hosts

   ```bash
   vim /etc/hosts
   ```

   ```bash
   # 添加如下内容
   192.168.10.100 hadoop100
   192.168.10.101 hadoop101
   192.168.10.102 hadoop102
   192.168.10.103 hadoop103
   192.168.10.104 hadoop104
   192.168.10.105 hadoop105
   ```

3. 关闭防火墙

   ```bash
   systemctl stop firewalld
   systemctl disable firewalld
   ```

3. 安装常用的包

   ```bash
   yum install -y epel-release
   yum install -y vim net-tools psmisc nc rsync lrzsz ntp libzstd openssl-static tree iotop git
   ```

4. 新建一个atguigu用户，并配置使其能sudo

   ```bash
   # 新建用户
   useradd atguigu
   passwd atguigu
   ```

   ```bash
   # 配置sudo权限
   visudo
   ```

   ```bash
   # 在配置中添加
   atguigu ALL=(ALL)       NOPASSWD:ALL
   ```

5. 准备将来安装集群的父目录

   ```bash
   # 新建两个用来安装集群目录
   mkdir /opt/software /opt/module
   ```

   ```bash
   # 将刚刚创建的目录所有者改为atguigu
   sudo chown -R atguigu:atguigu /opt/software /opt/module
   ```

6. (选做）做一个修改克隆以后虚拟机IP和Hostname的脚本

   ```bash
   #!/bin/bash
   
   #改IP
   sed -i "/IPADDR/s/.*/IPADDR=\"192.168.10.$1\"/" /etc/sysconfig/network-scripts/ifcfg-ens33
   
   #改Hostname
   hostnamectl --static set-hostname hadoop$1
   
   #重启网络
   systemctl restart network
   ```

8. 验证虚拟机配置正确

   - 权限

     ```bash
     # 登录atguigu
     sudo ls
     # 如果无法执行，检查visudo
     ```

     ```bash
     cd /opt/module
     touch abc
     rm abc
     # 如果报错，检查文件夹的所有者是不是atguigu
     ```

   - 网络

     ```bash
     ping www.baidu.com
     # 如果网络不通，查看网卡配置文件
     ```

     ```bash
     ping hadoop102
     ping hadoop103
     ping hadoop104
     # 如果ping不通，查看/etc/hosts
     ```

# 从这里关机，并克隆虚拟机，然后用atguigu用户登录

1. 先在102搭建JDK环境

   - 解压tar包

     ```bash
     tar -zxf /opt/software/jdk-8u212-linux-x64.tar.gz -C /opt/module
     ```

   - 配置环境变量

     ```bash
     sudo vim /etc/profile.d/my_env.sh
     ```

     ```bash
     # JAVA_HOME
     export JAVA_HOME=/opt/module/jdk1.8.0_212
     export PATH=$PATH:$JAVA_HOME/bin
     ```

   - 关闭旧的shell窗口，重新连接新的会话，让环境变量生效

     ```bash
     # 测试Java环境变量是否生效
     java -version
     ```

     看到如下内容

     ```bash
     java version "1.8.0_212"
     Java(TM) SE Runtime Environment (build 1.8.0_212-b10)
     Java HotSpot(TM) 64-Bit Server VM (build 25.212-b10, mixed mode)
     ```

     说明Java环境部署完成

2. 在102搭建Hadoop环境

   - 解压tar包

     ```bash
     tar -zxf /opt/software/hadoop-3.1.3.tar.gz -C /opt/module
     ```

   - 配置环境变量

     ```bash
     sudo vim /etc/profile.d/my_env.sh
     ```

     ```bash
     # HADOOP_HOME
     export HADOOP_HOME=/opt/module/hadoop-3.1.3
     export PATH=$PATH:$HADOOP_HOME/bin
     export PATH=$PATH:$HADOOP_HOME/sbin
     ```

   - 关闭旧的shell窗口，重新连接新的会话，让环境变量生效

     ```bash
     # 测试Hadoop环境是否生效
     hadoop version
     ```

     看到如下内容

     ```
     Hadoop 3.1.3
     Source code repository https://gitbox.apache.org/repos/asf/hadoop.git -r ba631c436b806728f8ec2f54ab1e289526c90579
     Compiled by ztang on 2019-09-12T02:47Z
     Compiled with protoc 2.5.0
     From source with checksum ec785077c385118ac91aadde5ec9799
     This command was run using /opt/module/hadoop-3.1.3/share/hadoop/common/hadoop-common-3.1.3.jar
     ```

     说明Hadoop环境部署完成

3. （选做）Hadoop本地测试模式

   - 新建一个input文件夹，准备一个输入文件

     ```bash
     cd $HADOOP_HOME
     mkdir input
     vim input
     ```

     内容如下

     ```
     abc abc
     bcd bcd
     cat
     dog
     ```

   - 执行Hadoop的官方范例程序

     ```bash
     hadoop jar share/hadoop/mapreduce/hadoop-mapreduce-examples-3.1.3.jar wordcount input output
     ```

     注意：输出目录output必须不存在

4. 编写集群同步脚本

   ```bash
   vim /home/atguigu/xsync
   ```

   内容如下

   ```bash
   #!/bin/bash
   
   #1. 判断参数个数
   if [ $# -lt 1 ]
   then
       echo Not Enough Arguement!
       exit;
   fi
   
   #2. 遍历集群所有机器
   for host in hadoop102 hadoop103 hadoop104
   do
       echo ====================  $host  ====================
       #3. 遍历所有目录，挨个发送
   
       for file in $@
       do
           #4. 判断文件是否存在
           if [ -e $file ]
               then
                   #5. 获取父目录
                   pdir=$(cd -P $(dirname $file); pwd)
   
                   #6. 获取当前文件的名称
                   fname=$(basename $file)
                   ssh $host "mkdir -p $pdir"
                   rsync -av $pdir/$fname $host:$pdir
               else
                   echo $file does not exists!
           fi
       done
   done
   ```

   给脚本增加执行权限，并拷贝到/bin

   ```bash
   chmod +x /home/atguigu/xsync
   sudo cp /home/atguigu/xsync /bin
   ```

   将脚本同步到集群所有机器

   ```bash
   sudo xsync /bin/xsync
   ```

5. 配置免密登录

   ```bash
   # 在三台机器生成，生成密钥对
   ssh-keygen -t rsa   # 三次回车
   ```

   ```bash
   # 将公钥发送到所有节点
   ssh-copy-id hadoop102
   ssh-copy-id hadoop103
   ssh-copy-id hadoop104
   ```

   root用户可以配置一份

6. 将jdk，Hadoop和环境变量文件同步

   ```bash
   xsync /opt/module/jdk1.8.0_212 /opt/module/hadoop-3.1.3
   sudo xsync /etc/profile.d/my_env.sh
   ```

7. 集群规划

   | Hadoop102   | Hadoop103      | Hadoop104         |
   | ----------- | -------------- | ----------------- |
   | Namenode    | ResourceMnager | SecondaryNamenode |
   | Datanode    | Datanode       | Datanode          |
   | Nodemanager | Nodemanager    | Nodemanager       |

8. 修改Hadoop的配置文件

   配置文件的位置在$HADOOP_HOME/etc/hadoop目录下

   - core-site.xml

     ```xml
     <?xml version="1.0" encoding="UTF-8"?>
     <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
     
     <configuration>
       <!-- 指定NameNode的地址 -->
       <property>
         <name>fs.defaultFS</name>
         <value>hdfs://hadoop102:8020</value>
       </property>
     
       <!-- 指定hadoop数据的存储目录 -->
       <property>
         <name>hadoop.tmp.dir</name>
         <value>/opt/module/hadoop-3.1.3/data</value>
       </property>
     
       <!-- 配置HDFS网页登录使用的静态用户为atguigu -->
       <property>
         <name>hadoop.http.staticuser.user</name>
         <value>atguigu</value>
       </property>
     </configuration>
     ```

   - hdfs-site.xml

     ```xml
     <?xml version="1.0" encoding="UTF-8"?>
     <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
     
     <configuration>
       <!-- nn web端访问地址-->
       <property>
         <name>dfs.namenode.http-address</name>
         <value>hadoop102:9870</value>
       </property>
       <!-- 2nn web端访问地址-->
       <property>
         <name>dfs.namenode.secondary.http-address</name>
         <value>hadoop104:9868</value>
       </property>
     </configuration>
     ```

   - yarn-site.xml

     ```xml
     <?xml version="1.0" encoding="UTF-8"?>
     <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
     
     <configuration>
       <!-- 指定MR走shuffle -->
       <property>
         <name>yarn.nodemanager.aux-services</name>
         <value>mapreduce_shuffle</value>
       </property>
     
       <!-- 指定ResourceManager的地址-->
       <property>
         <name>yarn.resourcemanager.hostname</name>
         <value>hadoop103</value>
       </property>
     
       <!-- 环境变量的继承 -->
       <property>
         <name>yarn.nodemanager.env-whitelist</name>
         <value>JAVA_HOME,HADOOP_COMMON_HOME,HADOOP_HDFS_HOME,HADOOP_CONF_DIR,CLASSPATH_PREPEND_DISTCACHE,HADOOP_YARN_HOME,HADOOP_MAPRED_HOME</value>
       </property>
     </configuration>
     ```

   - mapred-site.xml

     ```xml
     <?xml version="1.0" encoding="UTF-8"?>
     <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
     
     <configuration>
       <!-- 指定MapReduce程序运行在Yarn上 -->
       <property>
         <name>mapreduce.framework.name</name>
         <value>yarn</value>
       </property>
     </configuration>
     ```

   - workers

     先把原来的localhost删掉，添加以下内容

     ```
     hadoop102
     hadoop103
     hadoop104
     ```

   - 都修改并保存以后，同步配置文件

     ```bash
     xsync /opt/module/hadoop-3.1.3/etc/hadoop
     ```

9. 初始化并启动集群

   - 初始化Namenode

     ```bash
     # 在102执行
     hdfs namenode -format
     ```

   - 启动集群

     ```bash
     # 102启动HDFS
     start-dfs.sh
     ```

     ```bash
     # 103启动YARN
     start-yarn.sh
     ```

     启动中如果报类似错误，是正常现象

     ```bash
     hadoop104: WARNING: /opt/module/hadoop-3.1.3/logs does not exist. Creating.
     ```

   - 停止集群

     ```bash
     # 102执行
     stop-dfs.sh
     
     #103执行
     stop-yarn.sh
     ```

   - 启动完成后，可以访问HTTP页面观察集群

     [namenode]: http://hadoop102:9870
     [resourcemanager]: http://hadoop103:8088

10. 集群模式测试wordcount

   ```bash
   # 将HDFS的LICENSE.txt文件下载
   sz /opt/module/hadoop-3.1.3/LICENSE.txt
   ```

   打开Namenode页面，最右边utilities菜单里面的Browse the filesystem标签点开，新建目录并上传文件

   然后提交wordcount

   ```bash
   hadoop jar /opt/module/hadoop-3.1.3/share/hadoop/mapreduce/hadoop-mapreduce-examples-3.1.3.jar wordcount /input /output
   ```

# 集群运行时除了问题怎么办

1. NN或其他服务启动失败，首先我们先要定位错误

   - 第一步 先确定是哪个节点哪个服务出了问题
   - 第二部 通过查看日志找到问题原因
     - 日志文件一般位于软件安装目录下的logs文件夹
     - 打开其中log结尾的文件
     - 先看每一行日志的级别，日志级别分4级
       - INFO：正常运行的信息
       - WARN：警告，如果WARN的下一行不是error或fatal，就暂时可以不管
       - ERROR：错误，说明框架出了较为严重的错误，需要修复
       - FATAL：框架出了致命错误，必须修复
     - 找到ERROR或FATAL，用原因去百度

2. 如果MR运行出错，去YARN页面查看日志

   Application页面找到任务->点击history链接->分别查看Application Master的log、Map和Reduce的log
---

11. 配置历史服务器和日志聚集

    - mapred-site.xml

      ```xml
        <!-- 历史服务器端地址 -->
        <property>
          <name>mapreduce.jobhistory.address</name>
          <value>hadoop102:10020</value>
        </property>
      
        <!-- 历史服务器web端地址 -->
        <property>
          <name>mapreduce.jobhistory.webapp.address</name>
          <value>hadoop102:19888</value>
        </property>
      ```

    - yarn-site.xml

      ```xml
        <!-- 开启日志聚集功能 -->
        <property>
          <name>yarn.log-aggregation-enable</name>
          <value>true</value>
        </property>
        <!-- 设置日志聚集服务器地址 -->
        <property>  
          <name>yarn.log.server.url</name>  
          <value>http://hadoop102:19888/jobhistory/logs</value>
        </property>
        <!-- 设置日志保留时间为7天 -->
        <property>
          <name>yarn.log-aggregation.retain-seconds</name>
          <value>604800</value>
        </property>
      ```

    - 同步配置文件

      ```bash
      xsync /opt/module/hadoop-3.1.3/etc
      ```

12. 重启YARN并启动历史服务器

    ```bash
    # hadoop103上执行
    stop-yarn.sh
    start-yarn.sh
    ```

    ```bash
    # hadoop102上执行
    mapred --daemon start historyserver
    ```

    这时候重跑mapreduce任务

13. 如果集群除了问题实在解决不了，可以进行如下尝试

    - 停止集群

      ```bash
      # 在三台节点上执行
      killall -9 java
      ```

    - 删除数据文件

      ```bash
      # 在三台节点上执行
      rm -rf /opt/module/hadoop-3.1.3/data /opt/module/hadoop-3.1.3/logs
      ```

    - 从第9步重新初始化集群