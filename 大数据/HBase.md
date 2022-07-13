[HOME](https://hbase.apache.org/)

## 准备

1. hdfs
2. zookeeper

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
    - HBase管理页面：http://hadoop102:16010
    - 注意：如果集群之间的节点时间不同步，会导致regionserver无法启动，抛出ClockOutOfSyncException异常。

6. 高可用

    1. 关闭HBase集群（如果没有开启则跳过此步）
    
        ```shell
        stop-hbase.sh
        ```

    2. 在 conf 目录下创建`backup-masters`文件
    
        ```shell
        touch conf/backup-masters
        ```
    
    3. 在`backup-masters`文件中配置高可用HMaster节点
    
        ```shell
        echo hadoop103 > conf/backup-masters
        ```

    4. 分发conf
    
        ```shell
        xsync conf
        ```

    5. 重启hbase,查看管理页面：http://hadooo102:16010 

## HBase Shell 操作

```
hbase shell
```
