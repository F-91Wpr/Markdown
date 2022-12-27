### 1\. 数组排序

#### 1\. [一维数组](https://so.csdn.net/so/search?q=%E4%B8%80%E7%BB%B4%E6%95%B0%E7%BB%84&spm=1001.2101.3001.7020)

**直接调用sort方法：**

```java
int[] num = new int[n];
Arrays.sort(num);

```

**自定义排序：**

```java
Integer[] = new Integer[n];
Arrays.sort(minNum, new Comparator<String>() {
@Override
	public int compare(Integer e1, Integer e2) {
	    return e1.compareTo(e2);
	}
});

```

**注**：在自定义排序方法的时候，需要注意，**不能对基本类型的数组进行自定义排序**，如int，byte，char

所以，举例子用的Integer。

**lambda表达式排序：**

```java
Arrays.sort(minNum, ((o1, o2) -> (o1).compareTo(o2)));

```

#### 2\. 二维数组

```java
int[][] arr = new int[m][n];

```

1.  **实现Comparator接口 ：匿名内部类实现**

```java
Arrays.sort(arr, new Comparator<int[]>() {    // 匿名内部类
	@Override
	public int compare(int[] e1, int[] e2) {
		// 如果第一列元素相等，则比较第二列元素
		if (e1[0] == e2[0]) return e1[1] - e2[1];   // e1[1]-e2[1]表示对于第二列元素进行升序排序
		return e1[0] - e2[0];                     // e1[0]-e2[0]表示对于第一列元素进行升序排序
	}
});

```

2.  **lambda表达式实现**

```java
// 仅按第一列元素升序排序
Arrays.sort(arr, (e1, e2) -> (e1[0] - e2[0]));   // "->"lambda表达式符号，其将参数与实现隔开。

// 先按第一列元素升序排序，如果第一列相等再按第二列元素升序；
Arrays.sort(arr, (e1, e2)->(e1[0] == e2[0] ? (e1[1] - e2[1]):(e1[0] - e2[0])));

```

### 2\. ArrayList排序

**直接调用sort方法：**

```java
ArrayList<Integer> list = new ArrayList<>();
Collections.sort(list);  // Java7，list并没有sort方法

```

**自定义排序方法：**

```java
List<Integer> list = new ArrayList<Integer>();
Collections.sort(list, new Comparator<Integer>() {
    @Override
    public int compare(Integer o1, Integer o2) {
    // 可以比较对象中任意属性，这里是Integer类型，直接是它本身进行比较
        return o1.compareTo(o2);
    }
});

```

**Java8新特性：lambda表达式：**

```java
Collections.sort(list, (e1 , e2) -> (e1.compareTo(e2)));

```

需要去重的话：

1.  可以用list去重
2.  用TreeSet直接去重并排序，只是获取元素的时候仍需转为list

### 3\. HashMap排序

Java中HashMap是一种用于存储“键”和“值”信息对的[数据结构](https://so.csdn.net/so/search?q=%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84&spm=1001.2101.3001.7020)。不同于Array、ArrayList和LinkedLists，它不会维持插入元素的顺序。因此对HashMap排序的掌握很重要。

HashMap的排序分为按key和按Value进行排序。

HashMap排序有两个步骤：

1.  **通过map.entrySet()方法获取键值对;并将键值对传递给链表list**；

    ```java
    List<Map.Entry<String,Integer>> list = new ArrayList<>(map.entrySet());

    ```

2.  **自定义比较器来使用Collections.sort()方法排序链表**。

    ```java
     Collections.sort(list, new Comparator<Map.Entry<String,Integer>>() {
         public int compare(Map.Entry<String, Integer> e1,
                Map. Entry<String, Integer> e2) {
             return e1.getValue().compareTo(e2.getValue());
         }
     });

    ```

**示例：**

**单关键字排序：**

```java
Map<Integer, Integer> map = new HashMap<>();
List<Map.Entry<Integer,Integer>> list = new ArrayList<>(map.entrySet());
// 直接使用lambda表达式排序
list.sort((o1, o2) -> o2.getValue() - o1.getValue());

```

**多关键字排序：**

```java
Map<Integer, int[]> map = new HashMap<>();
// Map按value排序，先将map转为list,再排序list(按value值进行排序)
List<Map.Entry<Integer, int[]>> list = new ArrayList<Map.Entry<Integer, int[]>>(map.entrySet());

// 通过比较器来实现排序
Collections.sort(list, new Comparator<Map.Entry<Integer, int[]>>() {
    @Override
    public int compare(Map.Entry<Integer, int[]> o1, Map.Entry<Integer, int[]> o2) {
        // 降序排序
        int re = o1.getValue()[0] - o2.getValue()[0];
        if (re != 0) {
            return re;
        }
        re = o1.getValue()[1] - o2.getValue()[1];
        if (re != 0) {
            return re;
        }
        return 0;
    }
});
// 直接使用lambda表达式排序
list.sort((o1, o2) -> (o1.getValue()[0]==o2.getValue()[0]?(o1.getValue()[1]-o2.getValue()[1]):(o1.getValue()[0]-o2.getValue()[0])));

```

以下再按valaue和key排序总结一下，是重写[compare](https://so.csdn.net/so/search?q=compare&spm=1001.2101.3001.7020)方法的一个介绍：

1.  **对value降序**

```java
public int compare(Map.Entry<Integer,Integer>e1,Map.Entry<Integer,Integer>e2){
    return e2.getValue().compareTo(e1.getValue());
}

```

2.  **对值value升序排序**

```java
public int compare(Map.Entry<Integer,Integer>e1,Map.Entry<Integer,Integer>e2){
    return e1.getValue().compareTo(e2.getValue());
}

```

3.  **对键KEY降序排列**

```java
public int compare(Map.Entry<Integer,Integer>e1,Map.Entry<Integer,Integer>e2){
    return e2.getKey().compareTo(e1.getKey());
}

```

4.  **对键KEY升序排列**

```java
public int compare(Map.Entry<Integer,Integer>e1,Map.Entry<Integer,Integer>e2){
    return e1.getKey().compareTo(e2.getKey());
}

```

5.  **优先按值value降序、其次按键key升序排序**

```java
public int compare(Map.Entry<String,Integer>e1,Map.Entry<String,Integer>e2){
    int re = e2.getValue().compareTo(e1.getValue());
    if(re!=0){return re;}
    else{return e1.getKey().compareTo(e2.getKey());}
}

```

6.  **优先按键key降序、其次按值value升序排序**

```java
public int compare(Map.Entry<String,Integer>e1,Map.Entry<String,Integer>e2){
    int re = e2.getKey().compareTo(e1.getKey());
    if(re!=0){return re;}
    else{return e1.getValue().compareTo(e2.getValue());}
}

```

**总的来说HashMap的排序，就是先加入list，转而对list进行排序。这时候可以选择重写匿名内部类，也可以使用lambda表达式。**

也可以转为TreeMap进行排序，同List。

最后是获取排序后的HashMap：

1.  **从list取值再赋给Map.Entry<Integer, Integer>**

```java
for (int i = 0; i < list.size(); i++) {
    Map.Entry<Integer, Integer> entry = list.get(i);
    if (i != list.size() - 1) {
        System.out.print(entry.getKey() + ",");
    } else {
        System.out.println(entry.getKey());
    }
}

```

2.  **直接从list取**

```java
for (Map.Entry<Integer, Integer> entry : list) {
    System.out.print(entry.getKey());
}
```