## 
### Flink 是什么

Apache Flink 是一个框架和分布式处理引擎，用于在**无界**和**有界**数据流上进行有状态的计算。
Flink 能**部署应用到任意地方**运行，并能以**内存速度**和**任意规模**进行计算。

[解析](https://flink.apache.org/zh/flink-architecture.html#leverage-in-memory-performance)

### Flink 特性

- 结果的准确性。完美解决了乱序数据对结果正确性的影响。

- 精确一次（exactly-once）：Flink 的 checkpoint 和故障恢复算法保证了故障发生后应用状态的一致性。

- 高吞吐和低延迟。每秒处理数百万个事件，毫秒级延迟。

- 可以连接到最常用的存储系统，如Apache Kafka、Apache Cassandra、Elasticsearch、JDBC、Kinesis和（分布式）文件系统，如HDFS和S3。

- [高可用。](https://flink.apache.org/zh/flink-operations.html)

- Flink能够更方便地升级、迁移、暂停、恢复应用服务（save point）。

### 分层 API

Flink 为流式/批式处理应用程序的开发提供了不同级别的抽象。

![图 1](https://cdn.jsdelivr.net/gh/Z-404/imageHost/2022/07/mdi_20220730_1659182979452.png)

- Flink API 最底层的抽象为有状态实时流处理。
    - 抽象实现： Process Function，被 Flink 框架集成到了 DataStream API 中。
    - 它允许用户在应用程序中自由地处理来自单流或多流的事件（数据），并提供具有全局一致性和容错保障的状态。
    - 用户可以在此层抽象中注册事件时间（event time）和处理时间（processing time）回调方法。

- Flink API 第二层抽象是 Core APIs。
    - 其中包含 DataStream API（应用于有界/无界数据流场景）和 DataSet API（应用于有界数据集场景）两部分。
    - Core APIs 提供的流式 API（Fluent API）为数据处理提供了通用的模块组件，例如各种形式的用户自定义转换（transformations）、联接（joins）、聚合（aggregations）、窗口（windows）和状态（state）操作等。

Process Function 这类底层抽象和 DataStream API 的相互集成使得用户可以选择使用更底层的抽象 API 来实现自己的需求。DataSet API 还额外提供了一些原语，比如循环/迭代（loop/iteration）操作。

- Flink API 第三层抽象是 Table API。
    - Table API 是以表（Table）为中心的声明式编程（DSL）API，例如在流式数据场景下，它可以表示一张正在动态改变的表。
    - Table API 遵循（扩展）关系模型：即表拥有 schema（类似于关系型数据库中的 schema）
    - Table API 也提供了类似于关系模型中的操作，比如 select、project、join、group-by 和 aggregate 等。
    - Table API 程序是以声明的方式定义应执行的逻辑操作，而不是确切地指定程序应该执行的代码。
    - 尽管 Table API 使用起来很简洁并且可以由各种类型的用户自定义函数扩展功能，但还是比 Core API 的表达能力差。此外，Table API 程序在执行之前还会使用优化器中的优化规则对用户编写的表达式进行优化。

表和 DataStream/DataSet 可以进行无缝切换，Flink 允许用户在编写应用程序时将 Table API 与 DataStream/DataSet API 混合使用。

- Flink API 最顶层抽象是 SQL。
    - 这层抽象在语义和程序表达式上都类似于 Table API，但是其程序实现都是 SQL 查询表达式。SQL 抽象与 Table API 抽象之间的关联是非常紧密的，并且 SQL 查询语句可以在 Table API 中定义的表上执行。

### （待做）Flink VS Spark




## （待完成）部署



## 架构

### 整体架构

![图 1](https://cdn.jsdelivr.net/gh/Z-404/imageHost/2022/08/mdi_20220812_1660310165288.png)  

![图 2](https://cdn.jsdelivr.net/gh/Z-404/imageHost/2022/07/mdi_20220731_1659233313165.png)  

### JobManager
JobManager 协调 Flink 应用程序的分布式执行：它决定何时调度下一个 task（或一组 task）、对完成的 task 或执行失败做出反应、协调 checkpoint、并且协调从失败中恢复等等。这个进程由三个不同的组件组成：

1. Dispatcher

    Dispatcher 提供了一个 REST 接口，用来提交 Flink 应用程序执行，并**为每个提交的作业启动一个新的 JobMaster**。它还运行 Flink WebUI 用来提供作业执行信息。

2. JobMaster -- Job

    JobMaster 负责管理单个JobGraph的执行。Flink 集群中可以同时运行多个作业，每个作业都有自己的 JobMaster。

3. ResourceManager -- Task slots

    ResourceManager 负责 Flink 集群中的资源提供、回收、分配 - **管理 task slots**，这是 Flink 集群中资源调度的单位。Flink 为不同的环境和资源提供者（例如 YARN、Kubernetes 和 standalone 部署）实现了对应的 ResourceManager。在 standalone 设置中，ResourceManager 只能分配可用 TaskManager 的 slots，而不能自行启动新的 TaskManager。

#### JobManager 的数据结构

JobManager --> ExecutionGraph
JobVertex --> ExecutionVertex
IntermediateDataSet --> IntermediateResult 和 IntermediateResultPartition

<details>
<summary>细节</summary>
在作业执行期间，JobManager 会持续跟踪各个 task，决定何时调度下一个或一组 task，处理已完成的 task 或执行失败的情况。

JobManager 会接收到一个 JobGraph - 由多个算子顶点 ( JobVertex ) 组成的数据流图，以及中间结果数据 ( IntermediateDataSet )。每个算子都有自己的可配置属性，比如并行度和运行的代码。除此之外，JobGraph 还包含算子代码执行所必须的依赖库。

JobManager 会将 JobGraph 转换成 ExecutionGraph 。可以将 ExecutionGraph 理解为并行版本的 JobGraph，对于每一个顶点 JobVertex，它的每个并行子 task 都有一个 ExecutionVertex 。一个并行度为 100 的算子会有 1 个 JobVertex 和 100 个 ExecutionVertex。ExecutionVertex 会跟踪子 task 的执行状态。 同一个 JobVertex 的所有 ExecutionVertex 都通过 ExecutionJobVertex 来持有，并跟踪整个算子的运行状态。ExecutionGraph 除了这些顶点，还包含中间数据结果和分片情况 IntermediateResult 和 IntermediateResultPartition 。前者跟踪中间结果的状态，后者跟踪每个分片的状态。
</details>

![JobManager 的数据结构](https://cdn.jsdelivr.net/gh/Z-404/imageHost/2022/08/mdi_20220812_1660308873765.png)  

<details>
<summary>ExecutionGraph 的作业状态</summary>
每个 ExecutionGraph 都有一个与之相关的作业状态信息，用来描述当前的作业执行状态。

Flink 作业刚开始会处于 created 状态，然后切换到 running 状态，当所有任务都执行完之后会切换到 finished 状态。如果遇到失败的话，作业首先切换到 failing 状态以便取消所有正在运行的 task。如果所有 job 节点都到达最终状态并且 job 无法重启， 那么 job 进入 failed 状态。如果作业可以重启，那么就会进入到 restarting 状态，当作业彻底重启之后会进入到 created 状态。

如果用户取消了 job 话，它会进入到 cancelling 状态，并取消所有正在运行的 task。当所有正在运行的 task 进入到最终状态的时候，job 进入 cancelled 状态。

Finished、canceled 和 failed 会导致全局的终结状态，并且触发作业的清理。跟这些状态不同，suspended 状态只是一个局部的终结。局部的终结意味着作业的执行已经被对应的 JobManager 终结，但是集群中另外的 JobManager 依然可以从高可用存储里获取作业信息并重启。因此一个处于 suspended 状态的作业不会被彻底清理掉。
</details>

![ExecutionGraph 的作业状态](https://cdn.jsdelivr.net/gh/Z-404/imageHost/2022/08/mdi_20220812_1660309101626.png)  

### TaskManager
https://nightlies.apache.org/flink/flink-docs-master/zh/docs/concepts/flink-architecture/#taskmanagers

slot -- 线程
slots -- Parallelism（并行度）

对于分布式执行，Flink 将算子的 subtasks 链接成 tasks。每个 task 由一个线程执行。将算子链接成 task 是个有用的优化：它减少线程间切换、缓冲的开销，并且减少延迟的同时增加整体吞吐量。[链行为配置](https://nightlies.apache.org/flink/flink-docs-master/zh/docs/dev/datastream/operators/overview/#%e7%ae%97%e5%ad%90%e9%93%be%e5%92%8c%e8%b5%84%e6%ba%90%e7%bb%84)。

Flink 经常并发执行连续的 task。

slot 共享有两个主要优点：

- Flink 集群所需的 task slot 和作业中使用的最大并行度恰好一样。无需计算程序总共包含多少个 task（具有不同并行度）。

- 容易获得更好的资源利用。

### 作业提交流程

### 一些概念

1. 数据流图（Dataflow Graph）

    1. Source Operator

    2. Transformation Operator

        1. Map

        2. KeyBy().window().apply()

    3. Sink Operator

2. 并行度（Parallelism）

3. 算子链（Operator Chain）

4. 执行图（ExecutionGraph）

5. 任务（Tasks）和任务槽（Task Slots）


## API

### DataStream API

1. env
2. source
3. transformation
4. sink

### Watermark
水位线：
- 事件时间语义下的时钟
- 单调递增
- 基于数据中的时间戳生成
- 在处理乱序流时设置延迟
    - 一个水位线Watermark(t)，表示在当前流中事件时间已经达到了时间戳t, 这代表t之前的所有数据都到齐了，之后流中不会出现时间戳t’ ≤ t的数据

```java
// 乱序流水位线生成
steam.assignTimestampsAndWatermarks(
        WatermarkStrategy.<Event>forBoundedOutOfOrderness(Duration.ofSeconds(2))
                .withTimestampAssigner((data, timestamp) -> data.timestamp)
);
```

### window
Flink 中窗口是动态创建———当有落在这个窗口区间范围的数据达到时，才创建对应的窗口。另外，这里我们认为到达窗口结束时间时，窗口就触发计算并关闭，事实上“窗口关闭”和“触发计算”两个行为也可以分开。

分类：

1. 按照驱动类型分类

    - 时间窗口(Time Window)

        - 事件时间窗口
        - 处理时间窗口

    - 计数窗口(Count Window)

2. 按照窗口分配数据的规则分类

    - 滚动窗口(Tumbling Window)

        - size

    - 滑动窗口(Sliding Window)

        - slide

    - 会话窗口(Sesion Window)

        - 基于时间 gap

    - 全局窗口(Global Window)

        - 需自定义 Trigger

### Process Function

按 key 分区处理函数(KeyedProcessFunction)
    Timer 和 TimerService

### State

### SQL / Table API

## 容错机制

### Check Point

### Save Point

###  Exactly-once
```java
SingleOutputStreamOperator<Tuple2<String, Long>> aggregate = stream
        .keyBy(event -> event.user)
        .window(TumblingEventTimeWindows.of(Time.seconds(10L)))
        .allowedLateness(Time.seconds(2L))  // 事件时间窗口的延迟计算时间
        .sideOutputLateData(new OutputTag<Event>("lateData") {
        })
```

## 复杂事件处理(CEP)
https://nightlies.apache.org/flink/flink-docs-release-1.15/zh/docs/libs/cep/