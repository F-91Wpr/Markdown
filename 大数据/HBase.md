[HOME](https://hbase.apache.org/)

## 准备

1. hdfs
2. [zookeeper](zookeeper.md)

## 安装 HBase

1. 解压

2. 配置

    1. 环境变量

        ```shell
        sudo vim /etc/profile.d/my_env.sh

        #HBASE_HOME
        export HBASE_HOME=/opt/module/hbase-2.0.5
        export PATH=$PATH:$HBASE_HOME/bin

        source /etc/profile.d/my_env.sh
        ```

    2. 配置文件`/opt/module/hbase-2.0.5/conf`

        1. `hbase-env.sh`:

            ```shell
            export HBASE_MANAGES_ZK=false
            ```

        2. `hbase-site.xml`:
            ```xml
            <configuration>
                <property>
                    <name>hbase.rootdir</name>
                    <value>hdfs://hadoop102:8020/hbase</value>
                </property>

                <property>
                    <name>hbase.cluster.distributed</name>
                    <value>true</value>
                </property>

                <property>
                    <name>hbase.zookeeper.quorum</name>
                    <value>hadoop102,hadoop103,hadoop104</value>
                </property>
            </configuration>
            ```

        3. `regionservers`:
        
            ```text
            hadoop102
            hadoop103
            hadoop104
            ```

3. 删除冲突 jar 包

    ```shell
    rm /opt/module/hbase-2.0.5/lib/slf4j-log4j12-1.7.25.jar
    ```

4. 分发

    ```shell
    sudo xsync /etc/profile.d/my_env.sh 
    xsync /opt/module/hbase-2.0.5
    ```

5. 启动

    - 启动命令：```start-hbase.sh```
    - 停止命令：```stop-hbase.sh```
    - HBase 管理页面：http://hadoop102:16010
    - 注意：如果集群之间的节点时间不同步，会导致regionserver无法启动，抛出`ClockOutOfSyncException`异常。

6. 高可用

    1. 关闭 HBase 集群（如果没有开启则跳过此步）
    
        ```shell
        stop-hbase.sh
        ```

    2. 在 conf 目录下创建`backup-masters`文件
    
        ```shell
        touch conf/backup-masters
        ```
    
    3. 在`backup-masters`文件中配置高可用 HMaster 节点
    
        ```shell
        echo hadoop103 > conf/backup-masters
        ```

    4. 分发 conf
    
        ```shell
        xsync conf
        ```

    5. 重启 hbase ,查看管理页面：http://hadooo102:16010 

## HBase 操作

1. HBase shell
    ```
    hbase shell
    ```

2. HBase API

    1. maven 依赖

        ```xml
        <dependencies>
            <dependency>
                <groupId>org.apache.hbase</groupId>
                <artifactId>hbase-server</artifactId>
                <version>2.0.5</version>
                <exclusions>
                    <exclusion>
                        <groupId>org.glassfish</groupId>
                        <artifactId>javax.el</artifactId>
                    </exclusion>
                </exclusions>
            </dependency>
            <dependency>
                <groupId>org.apache.hbase</groupId>
                <artifactId>hbase-client</artifactId>
                <version>2.0.5</version>
            </dependency>
            <dependency>
                <groupId>org.glassfish</groupId>
                <artifactId>javax.el</artifactId>
                <version>3.0.1-b06</version>
            </dependency>
        </dependencies>
        ```
    
    2. 待施工

## 原理

RegionServer <--> zk <--> Master
-->BlockCeche
-->WAL          -->     HDFS
-->MemStore
-->StoreFile    -->     HDFS

写流程
读流程
RegionServer                        

## 调参

预分区
Row_key 设计
优化

## 整合 Phoenix

### 安装

1. 解压

2. 添加环境变量

    ```shell
    #PHOENIX_HOME
    export PHOENIX_HOME=/opt/module/phoenix
    export PHOENIX_CLASSPATH=$PHOENIX_HOME
    export PATH=$PATH:$PHOENIX_HOME/bin
    ```

3. 将 server 包拷贝到各个 HBase 节点的`$HBASE_HOME/lib`

    ```shell
    cp $PHOENIX_HOME/phoenix-5.0.0-HBase-2.0-server.jar $HBASE_HOME/lib
    xsync $HBASE_HOME/lib/phoenix-5.0.0-HBase-2.0-server.jar
    ```

4. 启动

    ```shell
    sqlline.py hadoop102,hadoop103,hadoop104:2181
    ```

### 操作

1. shell

    1. 开启 Phoenix 命名空间的自动映射

        将以下 property 添加到`$PHOENIX_HOME/bin/hbase-site.xml`和所有`$HBASE_HOME/conf/hbase-site.xml`

        ```xml
        <property>
            <name>phoenix.schema.isNamespaceMappingEnabled</name>
            <value>true</value>
        </property>
        ```

   2. 在phoenix中，表名等会自动转换为大写，若要小写，使用双引号，如"us_population"。
   3. For signed types(TINYINT, SMALLINT, INTEGER and BIGINT), Phoenix will flip the first bit so that negative values will sort before positive values. 

2. JDBC

    1. 胖客户端

        1. maven 依赖

            ```xml
            <dependencies>
                <dependency>
                    <groupId>org.apache.phoenix</groupId>
                    <artifactId>phoenix-core</artifactId>
                    <version>5.0.0-HBase-2.0</version>
                    <exclusions>
                        <exclusion>
                            <groupId>org.glassfish</groupId>
                            <artifactId>javax.el</artifactId>
                        </exclusion>
                        <exclusion>
                            <groupId>org.apache.hadoop</groupId>
                            <artifactId>hadoop-common</artifactId>
                        </exclusion>
                    </exclusions>
                </dependency>

                <dependency>
                    <groupId>org.glassfish</groupId>
                    <artifactId>javax.el</artifactId>
                    <version>3.0.1-b06</version>
                </dependency>

                <dependency>
                    <groupId>org.apache.hadoop</groupId>
                    <artifactId>hadoop-common</artifactId>
                    <version>2.8.4</version>
                </dependency>
            </dependencies>
            ```

    2. 瘦客户端

        1. 启动 query server

            ```shell
            queryserver.py start
            ```

        2. maven 依赖

            ```xml
            <dependencies>
                <!-- https://mvnrepository.com/artifact/org.apache.phoenix/phoenix-queryserver-client -->
                <dependency>
                    <groupId>org.apache.phoenix</groupId>
                    <artifactId>phoenix-queryserver-client</artifactId>
                    <version>5.0.0-HBase-2.0</version>
                </dependency>
            </dependencies>
            ```

### 二级索引
    
添加如下配置到 HBase 的 Regionserver 节点的 `hbase-site.xml`。
    ```xml
    <!-- phoenix regionserver 配置参数-->
    <property>
        <name>hbase.regionserver.wal.codec</name>
        <value>org.apache.hadoop.hbase.regionserver.wal.IndexedWALEditCodec</value>
    </property>
    ```

1. 全局索引

    ```SQL
    CREATE INDEX my_index ON my_table (my_col);
    ```

2. 包含索引

    ```SQL
    CREATE INDEX my_index ON my_table (v1) INCLUDE (v2);
    ```

3. 本地索引

    ```SQL
    CREATE LOCAL INDEX my_index ON my_table (my_column);
    ```

## EOF