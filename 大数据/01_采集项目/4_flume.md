- source
  - taildir:实时监控文件，支持断点续传和多目录
  - avro：Flume之间
  - nc：接受网络端口
  - exec：不支持断点续传
  - spooling：监控文件夹
  - kafka source

- channel
  - file
  - memory
  - kafka channel

- sink
  - avro
  - hdfs sink
  - kafka sink
  
## Flume 配置

1. 定义组件
2. 配置sources
3. 配置channels
4. 配置sinks
5. 组装

### 日志采集 Flume 配置（hadoop2、hadoop3）

1. Flume 配置文件

    ```shell
    vim /opt/module/flume-1.9.0/job/file_to_kafka.conf
    ```

    ```shell
    #定义组件
    a1.sources = r1
    a1.channels = c1

    #配置sources
    a1.sources.r1.type = TAILDIR
    a1.sources.r1.filegroups = f1
    a1.sources.r1.filegroups.f1 = /opt/module/applog/log/app.*
    a1.sources.r1.positionFile = /opt/module/flume-1.9.0/taildir_position.json
    ##拦截器interceptors
    a1.sources.r1.interceptors =  i1
    a1.sources.r1.interceptors.i1.type = com.example.gmall.flume.interceptor.ETLInterceptor$Builder

    #配置channels
    a1.channels.c1.type = org.apache.flume.channel.kafka.KafkaChannel
    a1.channels.c1.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092
    a1.channels.c1.kafka.topic = topic_log
    a1.channels.c1.parseAsFlumeEvent = false

    #组装
    a1.sources.r1.channels = c1
    ```

2. 编写 Flume 拦截器

    1. 实现 Interceptor 重写四个方法，静态内部类Build

    2. 打包上传 hadoop102 `/opt/module/flume-1.9.0/lib`目录下。

    3. 编辑配置文件`job/file_to_kafka.conf`中拦截器 type （全类名）。

3. 测试命令：

    ```shell
    #启动hadoop2的日志采集Flume
    bin/flume-ng agent -n a1 -c conf/ -f job/file_to_kafka.conf -Dflume.root.logger=info,console

    #启动一个 Kafka 的 Console-Consumer
    kafka-console-consumer.sh --bootstrap-server hadoop102:9092 --topic topic_log

    #生成模拟数据
    lg.sh
    ```

4. 日志采集 Flume 启停脚本`f1.sh`

    ```shell
    #!/bin/bash

    case $1 in
    "start"){
            for host in hadoop102 hadoop103
            do
                    echo " --------启动 $host 采集flume-------"
                    ssh $host "nohup /opt/module/flume-1.9.0/bin/flume-ng agent -n a1 -c /opt/module/flume-1.9.0/conf/ -f /opt/module/flume-1.9.0/job/file_to_kafka.conf >/dev/null 2>&1 &"
            done
    };; 
    "stop"){
            for host in hadoop102 hadoop103
            do
                    echo " --------停止 $host 采集flume-------"
                    ssh $host "ps -ef | grep file_to_kafka | grep -v grep |awk  '{print \$2}' | xargs -n1 kill -9 "
            done

    };;
    esac
    ```

5. 补充：集群日志生成脚本`lg.sh`

    ```shell
    #!/bin/bash
    for i in hadoop102 hadoop103; do
        echo "========== $i =========="
        ssh $i "cd /opt/module/applog/; java -jar gmall2020-mock-log-2021-10-10.jar >/dev/null 2>&1 &"
    done 
    ```

### 日志消费 Flume 配置（hadoop4）

1. Flume 配置文件

    ```shell
    vim /opt/module/flume-1.9.0/job/kafka_to_hdfs_log.conf
    ```

    ```shell
    #定义组件
    a1.sources=r1
    a1.channels=c1
    a1.sinks=k1

    #配置source1
    a1.sources.r1.type = org.apache.flume.source.kafka.KafkaSource
    a1.sources.r1.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092
    a1.sources.r1.kafka.topics = topic_log
    a1.sources.r1.kafka.consumer.group.id = topic_log
    a1.sources.r1.batchSize = 5000
    a1.sources.r1.batchDurationMillis = 2000
    a1.sources.r1.interceptors = i1
    a1.sources.r1.interceptors.i1.type = com.example.gmall.flume.interceptor.TimestampInterceptor$Builder
    
    #配置channel
    a1.channels.c1.type = file
    a1.channels.c1.checkpointDir = /opt/module/flume-1.9.0/checkpoint/behavior1
    a1.channels.c1.dataDirs = /opt/module/flume-1.9.0/data/behavior1
    a1.channels.c1.maxFileSize = 2146435071
    a1.channels.c1.capacity = 1000000
    a1.channels.c1.keep-alive = 6

    #配置sink
    a1.sinks.k1.type = hdfs
    a1.sinks.k1.hdfs.path = /origin_data/gmall/log/topic_log/%Y-%m-%d
    a1.sinks.k1.hdfs.filePrefix = log
    a1.sinks.k1.hdfs.round = false


    a1.sinks.k1.hdfs.rollInterval = 10
    a1.sinks.k1.hdfs.rollSize = 134217728
    a1.sinks.k1.hdfs.rollCount = 0

    #控制输出文件类型
    a1.sinks.k1.hdfs.fileType = CompressedStream
    a1.sinks.k1.hdfs.codeC = gzip

    #组装 
    a1.sources.r1.channels = c1
    a1.sinks.k1.channel = c1
    ```

2. 编写 Flume 拦截器

    1. 实现 Interceptor 重写四个方法，静态内部类Build

    2. 打包上传 hadoop104 `/opt/module/flume-1.9.0/lib`目录下。

    3. 编辑配置文件`job/kafka_to_hdfs_log.conf`中拦截器 type （全类名）。

3. 测试命令：

    ```shell
    #启动日志采集Flume
    f1.sh start

    #启动一个 hadoop104 的日志消费 Flume
    bin/flume-ng agent -n a1 -c conf/ -f job/kafka_to_hdfs_log.conf -Dflume.root.logger=info,console

    #生成模拟数据
    lg.sh
    ```

    观察 HDFS 是否出现数据。

4. 日志消费 FLume 启停脚本`f2.sh`

    ```shell
    #!/bin/bash

    case $1 in
    "start")
            echo " --------启动 hadoop104 日志消费 flume -------"
            ssh hadoop104 "nohup /opt/module/flume-1.9.0/bin/flume-ng agent -n a1 -c /opt/module/flume-1.9.0/conf -f /opt/module/flume-1.9.0/job/kafka_to_hdfs_log.conf >/dev/null 2>&1 &"
    ;;
    "stop")

            echo " --------停止 hadoop104 日志消费 flume -------"
            ssh hadoop104 "ps -ef | grep kafka_to_hdfs_log | grep -v grep |awk '{print \$2}' | xargs -n1 kill"
    ;;
    esac
    ```

### 业务消费 Flume 配置（hadoop4）

1. Flume 配置文件

    ```shell
    vim /opt/module/flume-1.9.0/job/kafka_to_hdfs_db.conf
    ```

    ```shell
    a1.sources = r1
    a1.channels = c1
    a1.sinks = k1

    a1.sources.r1.type = org.apache.flume.source.kafka.KafkaSource
    a1.sources.r1.batchSize = 5000
    a1.sources.r1.batchDurationMillis = 2000
    a1.sources.r1.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092
    a1.sources.r1.kafka.topics = topic_db
    a1.sources.r1.kafka.consumer.group.id = topic_db
    a1.sources.r1.setTopicHeader = true
    a1.sources.r1.topicHeader = topic
    a1.sources.r1.interceptors = i1
    a1.sources.r1.interceptors.i1.type = com.example.gmall.flume.interceptor.TimestampAndTableNameInterceptor$Builder

    a1.channels.c1.type = file
    a1.channels.c1.checkpointDir = /opt/module/flume-1.9.0/checkpoint/behavior2
    a1.channels.c1.dataDirs = /opt/module/flume-1.9.0/data/behavior2/
    a1.channels.c1.maxFileSize = 2146435071
    a1.channels.c1.capacity = 1000000
    a1.channels.c1.keep-alive = 6

    ## sink1
    a1.sinks.k1.type = hdfs
    a1.sinks.k1.hdfs.path = /origin_data/gmall/db/%{tableName}_inc/%Y-%m-%d
    a1.sinks.k1.hdfs.filePrefix = db
    a1.sinks.k1.hdfs.round = false


    a1.sinks.k1.hdfs.rollInterval = 10
    a1.sinks.k1.hdfs.rollSize = 134217728
    a1.sinks.k1.hdfs.rollCount = 0


    a1.sinks.k1.hdfs.fileType = CompressedStream
    a1.sinks.k1.hdfs.codeC = gzip

    ## 拼装
    a1.sources.r1.channels = c1
    a1.sinks.k1.channel= c1
    ```

2. 编写 Flume 拦截器

    1. 实现 Interceptor 重写四个方法，静态内部类Build

    2. 打包上传 hadoop104 `/opt/module/flume-1.9.0/lib`目录下。

    3. 编辑配置文件`job/kafka_to_hdfs_db.conf`中拦截器 type （全类名）。

3. 测试命令：

    ```shell
    #启动一个 hadoop104 的业务消费 Flume
    /opt/module/flume-1.9.0/bin/flume-ng agent -n a1 -c conf/ -f job/kafka_to_hdfs_db.conf -Dflume.root.logger=info,console

    #生成模拟数据
    cd /opt/module/dblog | java -jar gmall2020-mock-db-2021-11-14.jar

    ```

    观察 HDFS 是否出现数据。

4. 业务消费 FLume 启停脚本`f3.sh`

    ```shell
    #!/bin/bash

    case $1 in
    "start")
            echo " --------启动 hadoop104 业务消费 flume -------"
            ssh hadoop104 "nohup /opt/module/flume-1.9.0/bin/flume-ng agent -n a1 -c /opt/module/flume-1.9.0/conf -f /opt/module/flume-1.9.0/job/kafka_to_hdfs_db.conf >/dev/null 2>&1 &"
    ;;
    "stop")

            echo " --------停止 hadoop104 业务消费 flume -------"
            ssh hadoop104 "ps -ef | grep kafka_to_hdfs_db | grep -v grep |awk '{print \$2}' | xargs -n1 kill"
    ;;
    esac
    ```