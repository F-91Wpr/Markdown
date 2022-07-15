## 安装

1. 解压

2. 修改配置文件`elasticsearch.yml`

    ```yml
    # 集群名称，同一集群名称必须相同
    cluster.name: my-es
    
    # 节点名称
    node.name: node-1
    
    # 关掉 bootstrap 自检程序
    bootstrap.memory_lock: false
    
    # 网络部分  
    # 允许任意 ip 访问
    network.host: 0.0.0.0
    # 数据服务端口 
    http.port: 9200 
    # 集群间通信端口
    transport.tcp.port: 9301
    
    # 自发现配置：新节点向集群报到的主机名
    # 集群的”介绍人”节点
    discovery.seed_hosts: ["hadoop102:9301", "hadoop103:9301"]
    # 默认候选master节点
    cluster.initial_master_nodes: ["node-1", "node-2", "node-3"]
    # 集群检测的超时时间和次数 
    discovery.zen.fd.ping_timeout: 1m
    discovery.zen.fd.ping_retries: 5
    ```

    - 修改yml配置的注意事项:
        
        - 每行必须顶格，不能有空格
        - ":"后面必须有一个空格

    - 教学环境启动优化，修改配置文件`jvm.options`

        ```
        -Xms512m
        -Xmx512m
        ```

3. 分发 es7，修改节点名`node.name`

4. 启动测试：http://hadoop102:9200/_cat/nodes?v

## 群起脚本`ess.sh`

```shell
#!/bin/bash 

ES_HOME=/opt/module/es7

if [ $# -lt 1 ]
then
	echo "USAGE:es.sh {start|stop}"
	exit
fi	

case $1  in
"start") 
  #启动ES		
  for i in hadoop102 hadoop103 hadoop104
  do
    	ssh $i  "nohup ${ES_HOME}/bin/elasticsearch >/dev/null 2>&1 &"
  done
;;
"stop") 
  #停止ES
  for i in hadoop102 hadoop103 hadoop104
  do
      ssh $i "ps -ef|grep $ES_HOME |grep -v grep|awk '{print \$2}'|xargs kill" >/dev/null 2>&1
  done
 
;;
*)
	echo "USAGE:es.sh {start|stop}"
	exit
;;
esac
```

## 启动前的准备

1. 问题：max file descriptors [4096] for elasticsearch process likely too low, increase to at least [65536] elasticsearch

    修改系统允许 Elasticsearch 打开的最大文件数需要修改成65536。

    ```shell
    sudo vim /etc/security/limits.conf

    #在文件最后添加如下内容:
    * soft nofile 65536
    * hard nofile 131072
    * soft nproc 2048
    * hard nproc 65536
    ```
    分发配置文件。

2. 问题：max virtual memory areas vm.max_map_count [65530] likely too low, increase to at least [262144]

    修改一个进程可以拥有的虚拟内存区域的数量。

    ```shell
    sudo vim /etc/sysctl.conf

    #在文件最后添加如下内容
    vm.max_map_count=262144
    ```
    分发配置文件。

3. 问题：max number of threads [1024] for user [judy2] likely too low, increase to at least [4096]  （CentOS7.x  不用改）

    修改允许最大线程数为 4096。

    ```shell
    sudo vim /etc/security/limits.d/20-nproc.conf

    #修改如下内容
	* soft nproc 4096
    ```
    分发配置文件

4. 重启 linux 使配置生效。
