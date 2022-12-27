在刷牛客网时，遇到一类机试（华为机试）是采用[ACM](https://so.csdn.net/so/search?q=ACM&spm=1001.2101.3001.7020)模式下的算法，这样针对不同的输入输出就需要有一个大致的了解。

牛客网题目链接：[https://ac.nowcoder.com/acm/contest/5657#question](https://ac.nowcoder.com/acm/contest/5657#question)

首先对与Java的输入，要用[Scanner](https://so.csdn.net/so/search?q=Scanner&spm=1001.2101.3001.7020)：

```java
Scanner in = new Scanner(System.in);

```

读取字符或行：

```java
in.hasNext()
in.hasNextLine()

```

使用的方法总结：

**采用has xxxx的话，后面也要用next xxxx。比如前面用hasNextLine，那么后面要用 nextLine 来处理输入。**

具体区别参考：[笔试在线判题系统输入输出（剑指Offer类和传统ACM的OJ模式）](https://blog.csdn.net/weixin_44052055/article/details/120054907?spm=1001.2014.3001.5501)

以下是针对各种输入输出条件下举例实现的Java代码：

### 1\. 多组空格分割的两个整数 （无行数，结束字符限制）

**题目描述**：

```java
输入包括两个正整数a,b(1 <= a, b <= 10^9),输入数据包括多组。
输出a+b的结果

```

**输入示例**：

```java
1 5
10 20

```

输出示例

```java
6
30

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        while (in.hasNext()) {
            int a = in.nextInt();
            int b = in.nextInt();
            System.out.println(a + b);
        }
    }
}

```

### 2\. 第一行组数接空格分割的两个正整数 （行数限制）

**题目描述**：

```java
输入第一行包括一个数据组数t(1 <= t <= 100)
接下来每行包括两个正整数a,b(1 <= a, b <= 10^9)
输出a+b的结果

```

**输入示例**：

```java
2
1 5
10 20

```

**输出示例**：

```java
6
30

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int num = in.nextInt();
        for(int i = 0; i < num; i++) { // 注意 while 处理多个 case
            int a = in.nextInt();
            int b = in.nextInt();
            System.out.println(a + b);
        }
    }
}

```

### 3\. 空行分割的两个正整数，0 0 结束 （结束符限制）

**题目描述**：

```java
输入包括两个正整数a,b(1 <= a, b <= 10^9),输入数据有多组, 如果输入为0 0则结束输入
输出a+b的结果

```

**输入示例**：

```java
1 5
10 20
0 0

```

**输出示例**：

```java
6
30

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);

        while (in.hasNext()) {
            int a = in.nextInt();
            int b = in.nextInt();
            if(a ==0 && b == 0) break;
            System.out.println(a + b);
        }
    }
}

```

### 4\. 每行第一个为个数后带空格分割整数，0结束 （结束符限制,每行有个数限制）

**题目描述**：

```java
输入数据包括多组。
每组数据一行,每行的第一个整数为整数的个数n(1 <= n <= 100), n为0的时候结束输入。
接下来n个正整数,即需要求和的每个正整数。

每组数据输出求和的结果

```

**输入示例**：

```java
4 1 2 3 4
5 1 2 3 4 5
0

```

**输出示例**：

```java
10
15

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);

        while (in.hasNext()) {
            int n = in.nextInt();
            if(n == 0) break;
            int sum = 0;
            for (int i = 0; i < n; i++) {
                sum += in.nextInt();
            }
            System.out.println(sum);
        }
    }
}

```

### 5\. 第一行组数接第一个个数接空格分开的整数 （行数限制,每行有个数限制）

**题目描述**：

```java
输入的第一行包括一个正整数t(1 <= t <= 100), 表示数据组数。
接下来t行, 每行一组数据。
每行的第一个整数为整数的个数n(1 <= n <= 100)。
接下来n个正整数, 即需要求和的每个正整数。

每组数据输出求和的结果

```

**输入示例**：

```java
2
4 1 2 3 4
5 1 2 3 4 5

```

**输出示例**：

```java
10
15

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {

        Scanner in = new Scanner(System.in);

        int num = in.nextInt();
        for (int i = 0; i < num; i++){
            int n = in.nextInt();
            int sum = 0;
            for(int j = 0; j < n; j++){
                sum += in.nextInt();
            }
            System.out.println(sum);
        }
    }
}

```

### 6\. 每行第一个为个数后带空格分割整数 (无结束限制,每行有个数限制)

**题目描述**：

```java
输入数据有多组, 每行表示一组输入数据。
每行的第一个整数为整数的个数n(1 <= n <= 100)。
接下来n个正整数, 即需要求和的每个正整数。

每组数据输出求和的结果

```

**输入示例**：

```java
4 1 2 3 4
5 1 2 3 4 5

```

**输出示例**：

```java
10
15

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        while (in.hasNextInt()) {
            int n = in.nextInt();
            int sum = 0;
            while (n > 0) {
                sum += in.nextInt();
                n--;
            }
            System.out.println(sum);
        }
    }
}

```

### 7\. 多组空格分隔的正整数 (无结束限制,每行无个数限制,需要当作字符串处理)

**题目描述**：

```java
输入数据有多组, 每行表示一组输入数据。
每行不定有n个整数，空格隔开。(1 <= n <= 100)。

每组数据输出求和的结果

```

**输入示例**：

```java
1 2 3
4 5
0 0 0 0 0

```

**输出示例**：

```java
6
9
0

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        while (in.hasNextLine()) {
            String[] s = in.nextLine().split(" ");
            int sum = 0;
            for (int i = 0; i < s.length; i++) {
                sum += Integer.parseInt(s[i]);
            }
            System.out.println(sum);
        }
    }
}

```

```java
// 扩展：第一行组数接空格分开的多个整数 (有行数限制,每行无个数限制,需要当作字符串处理)
// 需要注意的是：读取行数后，要加一行in.nextLine();再读下面的行。
// eg: 每组数据输出求和的结果
// 3
// 1 2 3
// 4 5
// 0 0 0 0 0

```

```java
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int num = in.nextInt();
        in.nextLine();  // 注意：跳过行数所在行
        for (int i = 0; i < num; i++ ){
            String[] s = in.nextLine().split(" ");
            int sum = 0;
            for(int j = 0; j < s.length; j++){
                sum += Integer.parseInt(s[j]);  // 将字符转成整数类型
            }
            System.out.println(sum);
        }
    }
}

```

### 8\. 第一行个数第二行字符串

**题目描述**：

```java
输入有两行，第一行n
第二行是n个空格隔开的字符串

输出一行排序后的字符串，空格隔开，无结尾空格

```

**输入示例**：

```java
5
c d a bb e

```

**输出示例**：

```java
a bb c d e

```

```java
import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        in.nextLine();
        while (in.hasNext()) {
            String[] s = in.nextLine().split(" ");
            Arrays.sort(s);
            for (int i = 0; i < s.length; i++) {
                System.out.print(s[i] + " ");
            }

        }
    }
}

```

### 9\. 多行空格分开的字符串

**题目描述**：

```java
多个测试用例，每个测试用例一行。
每行通过空格隔开，有n个字符，n＜100

对于每组测试用例，输出一行排序过的字符串，每个字符串通过空格隔开

```

**输入示例**：

```java
a c bb
f dddd
nowcoder

```

**输出示例**：

```java
a bb c
dddd f
nowcoder

```

```java
import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        while (in.hasNextLine()) {
            String[] s = in.nextLine().split(" ");
            Arrays.sort(s);
            for (String c:s) {
                System.out.print(c + " ");
            }
            System.out.println();
        }
    }
}

```

### 10\. 多行逗号分开的字符串 （逗号和空格的区别就是最后一个字符输完的时候",“还是” " ）

**题目描述**：

```java
多个测试用例，每个测试用例一行。
每行通过,隔开，有n个字符，n＜100

对于每组用例输出一行排序后的字符串，用','隔开，无结尾空格

```

**输入示例**：

```java
a,c,bb
f,dddd
nowcoder

```

**输出示例**：

```java
a,bb,c
dddd,f
nowcoder

```

```java
import java.util.*;
public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        while (in.hasNextLine()) {
            String[] s = in.nextLine().split(",");
            Arrays.sort(s);
            int l = s.length;
            for (int i = 0; i < l - 1; i++) {
                System.out.print(s[i] + ",");
            }
            System.out.println(s[l-1]);
        }
    }
}

```

以上就是遇到的各种输入输出的一个总结，后面再遇到新的情况会及时更新。

一个整体的代码如下：

```java
import java.util.Arrays;
import java.util.Scanner;

public class InOutTest {
    public static void main(String[] args){
        // 1. 多组空格分割的两个整数
        // eg：每行输入两个数求和
        // 1 5
        // 10 20
//        Scanner in = new Scanner(System.in);
//        while (in.hasNext()){ // while处理多个case
//            int a = in.nextInt();
//            int b = in.nextInt();
//            System.out.println(a + b);
//        }

        // 2. 第一行组数接空格分割的两个正整数 （行数限制）
        // eg: 输出a+b的结果
        // 2
        // 1 5
        // 10 20
//        Scanner in = new Scanner(System.in);
//        int num = in.nextInt();
//        for (int i =0; i < num; i++){
//            int a = in.nextInt();
//            int b = in.nextInt();
//            System.out.println(a + b);
//        }

        // 3. 空行分割的两个正整数，0 0 结束 （结束符限制）
        // eg: 输出a+b的结果
        // 1 5
        // 10 20
        // 0 0
//        Scanner in = new Scanner(System.in);
//        while (in.hasNext()){
//            int a = in.nextInt();
//            int b = in.nextInt();
//            if(a == 0 && b == 0) break;
//            System.out.println(a + b);
//        }

        // 4. 每行第一个为个数后带空格分割整数，0结束 （结束符限制,每行有个数限制）
        // 输入数据包括多组。
        // 每组数据一行,每行的第一个整数为整数的个数n(1 <= n <= 100), n为0的时候结束输入。
        // 接下来n个正整数,即需要求和的每个正整数。
        // eg: 每组数据输出求和的结果
        // 4 1 2 3 4
        // 5 1 2 3 4 5
        // 0
//        Scanner in = new Scanner(System.in);
//        while (in.hasNext()){
//            int n = in.nextInt();
//            if (n == 0) break;
//            int sum = 0;
//            for (int i = 0; i < n; i++){
//                sum += in.nextInt();
//            }
//            System.out.println(sum);
//        }

        // 5. 第一行组数接第一个个数接空格分开的整数 （行数限制,每行有个数限制）
        // 输入的第一行包括一个正整数t(1 <= t <= 100), 表示数据组数。
        // 接下来t行, 每行一组数据。
        // 每行的第一个整数为整数的个数n(1 <= n <= 100)。
        // 接下来n个正整数, 即需要求和的每个正整数。
        // eg: 每组数据输出求和的结果
        // 2
        // 4 1 2 3 4
        // 5 1 2 3 4 5
//        Scanner in = new Scanner(System.in);
//        int num = in.nextInt();
//        for (int i = 0; i < num; i++){
//            int n = in.nextInt();
//            int sum = 0;
//            for(int j = 0; j < n; j++){
//                sum += in.nextInt();
//            }
//            System.out.println(sum);
//        }
        // 6. 每行第一个为个数后带空格分割整数 (无结束限制,每行有个数限制)
        // 输入数据有多组, 每行表示一组输入数据。
        // 每行的第一个整数为整数的个数n(1 <= n <= 100)。
        // 接下来n个正整数, 即需要求和的每个正整数。
        // eg: 每组数据输出求和的结果
        // 4 1 2 3 4
        // 5 1 2 3 4 5
//        Scanner in = new Scanner(System.in);
//        while (in.hasNextInt()){
//            int num = in.nextInt();
//            int sum = 0;
//            for(int i = 0; i < num; i++){
//                sum += in.nextInt();
//            }
//            System.out.println(sum);
//        }

        // 7. 多组空格分隔的正整数 (无结束限制,每行无个数限制,需要当作字符串处理)
        // 输入数据有多组, 每行表示一组输入数据。
        // 每行不定有n个整数，空格隔开。(1 <= n <= 100)。
        // eg: 每组数据输出求和的结果
        // 1 2 3
        // 4 5
        // 0 0 0 0 0 0
//        Scanner in = new Scanner(System.in);
//        while (in.hasNextLine()){   // 直接读一行字符串
//            String[] s = in.nextLine().split(" ");
//            int sum = 0;
//            for(int i = 0; i < s.length; i++){
//                sum += Integer.parseInt(s[i]);
//            }
//            System.out.println(sum);
//        }

        // 扩展：第一行组数接空格分开的多个整数 (有行数限制,每行无个数限制,需要当作字符串处理)
        // eg: 每组数据输出求和的结果
        // 3
        // 1 2 3
        // 4 5
        // 0 0 0 0 0
        Scanner in = new Scanner(System.in);
        int num = in.nextInt();
        in.nextLine();  // 跳过行数所在行
        for (int i = 0; i < num; i++ ){
            String[] s = in.nextLine().split(" ");
            int sum = 0;
            for(int j = 0; j < s.length; j++){
                sum += Integer.parseInt(s[j]);
            }
            System.out.println(sum);
        }

        // 8. 第一行个数第二行字符串
        // 输入有两行，第一行n
        // 第二行是n个空格隔开的字符串
        // eg: 输出一行排序后的字符串，空格隔开，无结尾空格
        // 5
        // c d a bb e
//        Scanner in = new Scanner(System.in);
//        int num = in.nextInt();
//        in.nextLine();
//        while (in.hasNext()){
//            String[] s = in.nextLine().split(" ");
//            Arrays.sort(s);
//            for (int i = 0; i < s.length; i++){
//                System.out.print(s[i] + " ");
//            }
//        }

        // 9. 多行空格分开的字符串
        // 多个测试用例，每个测试用例一行。
        // 每行通过空格隔开，有n个字符，n＜100
        // eg: 对于每组测试用例，输出一行排序过的字符串，每个字符串通过空格隔开
        // a c bb
        // f dddd
        // nowcoder
//        Scanner in = new Scanner(System.in);
//        while (in.hasNextLine()){
//            String[] s = in.nextLine().split(" ");
//            Arrays.sort(s);
//            for (String c:s){
//                System.out.print(c + " ");
//            }
//            System.out.println();
//        }

        // 10. 多行逗号分开的字符串 （逗号和空格的区别就是最后一个字符输完的时候","还是" " ）
        // 多个测试用例，每个测试用例一行。
        // 每行通过,隔开，有n个字符，n＜100
        // eg: 对于每组用例输出一行排序后的字符串，用','隔开，无结尾空格
        // a,c,bb
        // f,dddd
        // nowcoder
//        Scanner in = new Scanner(System.in);
//        while (in.hasNextLine()) {
//            String[] s = in.nextLine().split(",");
//            Arrays.sort(s);
//            int l = s.length;
//            for (int i = 0; i < l - 1; i++) {
//                System.out.print(s[i] + ",");
//            }
//            System.out.println(s[l-1]);
//        }

    }

}

```

在做笔试题的时候，遇到了一道输入一个Java代码，输出for循环的嵌套层数问题，这种需要循环读入字符串，直到遇到"}"。
![在这里插入图片描述](https://img-blog.csdnimg.cn/ba1afd5e243d4a84a5eaaff9b717eea0.png)

```java
String[] codes = new String[10000];
int index = 0;
String s = null;
while (!(s = in.nextLine()).equals("}")) {
    codes[index++] = s;
}

```

定义了一个数组，把每行字符串存到数组里面，这里也可以用动态数组。

注意的是，这里只需要定义字符串s，判断`s = in.nextLine()).equals("}")`即可。同理，遇到空行结束什么的也可以这样处理。