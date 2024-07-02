# 简易`Mini-Lisp` 解释器
这是北京大学2024年春季学期课程《软件设计实践》课程大作业：用C++实现一个简易的Mini-Lisp解释器。

语言基本指导规范：[Mini-Lisp 编程语言规范](https://pku-software.github.io/mini-lisp-spec/)

2024/6/6：完成第一次面测版本，此时能通过所有RJSJ_TEST宏测试用例。

## 程序启动方法

通过命令行/cmd/终端启动，启动指令：`./mini-lisp (filename)`

### REPL模式

不传入任何命令行参数，则进入REPL模式，可以输入符合基本指导规范或拓展特性的任意Mini-lisp表达式，程序会自动进行求值并输出相应返回结果。

在执行`exit`命令、`error`命令或输入`Ctrl+Z`前，程序会反复进行“读取-求值-打印”的循环。

### 文件模式

命令行参数传入有且仅有一个Mini-Lisp源代码文件`filename`，程序会自动读取`filename`文件并执行文件内的源代码。此时程序**不会**即时打印返回结果，而是只在执行到`filename`文件内需要输出的语句时才进行打印。

## 拓展特性

### 多行输入

支持将一个表达式拆成多行输入。换行后进行新一个参数的输入。

当处于REPL模式时，当上一行的表达式输入还未结束时，解释器的提示符会显示`... `，表示输入尚未结束。

当处于文件模式时，支持输入文件同一表达式内部换行。

### 自动缩进

当处于REPL模式时，输入多行表达式时，解释器会自动按照输入表达式的结构进行缩进。

缩进按照代码嵌套的层级进行，与c++等语言的缩进方式一致。

例示（同时展现多行输入功能）：

```scheme
>>> (define
...   (f
...     x)
...   (* x
...     x
...     )
...   )
()
>>> (f 2)
4
```

### 注释

支持单行注释和块注释（多行注释），其中：

-   单行注释以本行第一个`;`开头，直到行末结束。在REPL模式下或文件模式下均可以使用。

例示：

```scheme
>>> (define (f x)      ; 此处换行，解释器继续等待输入
...    (* x x))
()
>>> (f 2)
4
```

-   块注释以`#|`开头，以`|#`结尾。在REPL模式下或文件模式下均可以使用。*（真的有人会在REPL模式下写块注释吗？）*

        注意：块注释内支持再使用单行注释，但不支持嵌套使用（即不支持块注释内再使用块注释）。

例示：

```scheme
>>> #|这是一个多行注释例
>>> 定义f(x) = x^2|#
>>> (define (f x) (* x x))
()
>>> (f 3)
9
```

### 更多的内置过程

#### 特殊过程

-   `read-line`
    <br>

    用法：<br>
    `(read-line)`
    <br>

    从标准输入读取一行表达式，并返回表达式的`eval`结果。
    ```scheme
    >>> (read-line)
    IN: (+ 1 2)
    3
    ```
    <br>

-   `load-file`
    <br>

    用法：<br>
    `(load-file filename)`
    <br>

    读取`filename`文件并按文件模式执行。*```（主要用于REPL模式下调试）```*



#### 更多数学函数

-   `max`
    <br>

    用法：<br>
    `(max x1 x2 ...)`<br>
    `(max list)`
    <br>

    前者接收一系列数类型参数，返回这一系列数的最大值。<br>

    `(max 1 2 3 4 5) => 5`
    <br>

    后者接收一个全数列表参数，返回该列表中的最大值。<br>
    `(max '(1 2 3 4 5)) => 5`
    <br>

-   `min`
    <br>

    用法：<br>
    `(min x1 x2 ...)`<br>
    `(min list)`
    <br>

    与`max`类似，接收一系列数类型参数或一个全数列表，返回这一系列数的最小值。

#### 字符串操作

-   `number->string`
    <br>

    用法：<br>
    `(number->string x)`
    <br>

    将数字`x`转换为字符串，并返回。如果x不是数字，则返回`#f`<br>
    `(number->string 123) => "123"`<br>
    `(number->string 12.45) => "12.450000"`<br>
    `(number->string "123") => #f`
    <br>

-   `string->number`
    <br>

    用法：<br>
    `(string->number str)`
    <br>

    将字符串`str`转换为数字，并返回。如果`str`不能转化为合法的数字，则返回`#f`<br>
    `(string->number "123") => 123`<br>
    `(string->number "12.45") => 12.450000`<br>
    `(string->number "123a") => #f`<br>
    `(string->number 12.45) => #f`
    <br>

-   `make-string`
    <br>

    用法：<br>
    `(make-string n char)`<br>
    `(make-string n)`
    <br>

    创建一个长度为`n`的字符串，列表中的每个字符都是字符`char`，当`char`缺省时默认为一个空格。如果`n`小于0或者非整数，则报错。<br>
    `(make-string 5) => "(五个空格)"`<br>
    `(make-string 5 "a") => "aaaaa"`
    <br>

-   `string-ref`
    <br>

    用法：<br>
    `(string-ref str i)`
    <br>

    返回字符串`str`中索引为`i`的单字符构成的字符串。如果`i`超出字符串长度或非整数，则报错。<br>
    `(string-ref "abcdefg" 3) => "d"`
    <br>

-   `string-length`
    <br>

    用法：<br>
    `(string-length str)`
    <br>

    返回字符串`str`的长度。如果`str`不是字符串，则报错。<br>
    `(string-length "abcdefg") => 7`
    <br>

-   `string-copy`
    <br>

    用法：<br>
    `(string-copy str)`
    <br>

    返回字符串`str`的拷贝。如果`str`不是字符串，则报错。<br>
    <br>

-   `substring`
    <br>

    用法：<br>
    `(substring str pos num)`<br>
    `(substring str pos)`
    <br>

    返回字符串`str`中从索引`pos`开始的`num`个字符构成的字符串。如果`str`不为字符串，则报错。如果`pos`超出字符串长度或非整数，则报错。如果`num`为负数或非整数，则报错。如果`num`为0，则返回空字符串。如果`num`缺省，或`pos+num`超出字符串长度，则返回从`pos`开始的剩余字符串。<br>
    `(substring "abcdefg" 3) => "defg"`<br>
    `(substring "abcdefg" 3 2) => "de"`
    <br>

-   `string-append`
    <br>

    用法：<br>
    `(string-append str1 str2 ...)`
    <br>

    返回字符串`str1`、`str2`、...的连接结果。如果某一个参数不是字符串，则报错。<br>
    `(string-append "abc" "def" "ghi") => "abcdefghi"`

### 有理数类

本解释器实现了简单有理数类及其基本运算：

#### 有理数的构造

-   `rational-set`
    <br>

    用法：<br>
    `(rational-set x y)`<br>
    `(rational-set x)`
    <br>

    对于第一种构造函数，x和y均为整数（若传入不是整数则将其向下取整），返回以x为分子，y为分母的既约分数有理数。<br>
    `(rational-set 5 10) => 1/2`<br>
    `(rational-set 5) => 5`
    <br>

    对于第二种构造函数，x为浮点数，返回值为将x化为既约分数的有理数。<br>
    `(rational-set 1.5) => 3/2`<br>
    `(rational-set 3) => 3`<br>
    **注意：使用第二种构造函数时，若浮点数x的小数位数过多，会返回精度有限的有理数！**
    例：<br>
    `(rational-set 0.14281428) => 3570357/25000000`<br>
    `(rational-set 0.142814281428) => 142814281/1000000000`，但实际应为`35703570357/250000000000`
    <br>

#### 有理数的表示

有理数类的数将以`(n/d)`的形式表示，其中`n`为分子，`d`为分母。

#### 有理数的运算

有理数类支持基本指导规范和拓展算术运算库的所有运算，如加减乘除取余取整等。例如：<br>
`(+ 1 0.5 (rational-set 2 3)) => 2.166667`<br>
`(- (rational-set 1 4)) => -0.250000`<br>
`(max 0.1 0.2 (rational-set 2 3)) => 0.666667`

除此之外，解释器还实现了以下运算：

-   `rational+`
    <br>

    用法：<br>
    `(rational+ rational1 rational2 ...)`
    <br>

    返回有理数`rational1`、`rational2`、...的和的既约分数形式。如果某一个参数不是数，则报错。<br>
    `(rational+ (rational-set 2 3) (rational-set 4 5)) => 22/15`<br>
    `(rational+ (rational-set 2 3) 1) => 5/3`
    <br>

-   `rational-`
    <br>

    用法：<br>
    `(rational- rational1 rational2)`<br>
    `(rational- rational2)`
    <br>

    rational1缺省时为0。返回有理数`rational1`、`rational2`的差的既约分数形式。如果某一个参数不是数，则报错。<br>
    `(rational- (rational-set 2 3) (rational-set 4 5)) => -2/15`<br>
    `(rational- (rational-set 2 3)) => -2/3`
    <br>

-   `rational*`
    <br>

    用法：<br>
    `(rational* rational1 rational2 ...)`
    <br>

    返回有理数`rational1`、`rational2`、...的积的既约分数形式。如果某一个参数不是数，则报错。<br>
    `(rational* (rational-set 2 3) (rational-set 4 5)) => 8/15`<br>
    `(rational* (rational-set 2 3) 2) => 4/3`

-   `rational/`
    <br>

    用法：<br>
    `(rational/ rational1 rational2)`<br>
    `(rational/ rational2)`
    <br>

    rational1缺省时为1。返回有理数`rational1`、`rational2`的商的既约分数形式。如果某一个参数不是数，则报错。<br>
    `(rational/ (rational-set 2 3) (rational-set 4 5)) => 5/6`<br>
    `(rational/ (rational-set 2 3)) => 3/2`
    <br>

-   `rational-abs`
    <br>

    用法：<br>
    `(rational-abs rational)`
    <br>

    返回有理数`rational`的绝对值的既约分数形式。如果`rational`不是数，则报错。<br>
    `(rational-abs (rational-set 2 -3)) => 2/3`<br>
    `(rational-abs -0.5) => 1/2`
    <br>

-   `rational-equal`
    <br>

    用法：<br>
    `(rational-equal rational1 rational2)`
    <br>

    以比较分子分母的方式判断有理数`rational1`、`rational2`是否相等。如果相等则返回`#t`，否则返回`#f`，如果某一个参数不是数，则报错。<br>
    `(rational-equal (rational-set 2 3) (rational-set 4 6)) => #t`<br>
    `(rational-equal (rational-set 2 3) (rational-set 4 5)) => #f`

### 矩阵类

本解释器实现了简单矩阵类及其基本运算：

#### 矩阵的构造

-   `matrix-set`
    <br>

    用法：<br>
    `(matrix-set list1 list2 ...)`
    <br>

    返回由行向量列表list1、list2、...构成的矩阵。如果某一个参数不是列表或矩阵形状不匹配，则报错。<br>
    ```scheme
    (matrix-set `(1 2 3) `(4 5 6) `(7 8 9)) =>
    [
    1.000000 2.000000 3.000000 
    4.000000 5.000000 6.000000 
    ]
    (matrix-set `(,(+ 1 2) 7) '(3 4)) =>    ;使用,获得(+ 1 2)求值后的结果作为矩阵元素
    [
    3.000000 7.000000
    3.000000 4.000000
    ]
    (matrix-set `(1 2 3) `(5)) => Error

    ```
    `matrix-set`也支持通过已有行向量列表构造矩阵：
    ```scheme
    >>> (define x (list 1 2 3))
    ()
    >>> (define y (list 4 5 6))
    ()
    >>> (matrix-set x y)
    [
    1.000000 2.000000 3.000000
    4.000000 5.000000 6.000000
    ]
    ```
-   `I`
    <br>

    用法：<br>
    `(I n)`
    <br>
    返回大小为n*n的单位矩阵。如果n不是正整数，则报错。<br>
    ```scheme
    >>> (I 3)
    [
    1.000000 0.000000 0.000000
    0.000000 1.000000 0.000000
    0.000000 0.000000 1.000000
    ]
    ```


#### 矩阵的表示

矩阵类以`[row1 row2 ...]`的形式表示，其中`row1`、`row2`、...为行向量。

#### 矩阵的运算

矩阵类重载了运算符`+`、`-`、`*`，可以直接按照如如下方式（类似于基本指导规范）进行运算。

-   `+`
    <br>

    用法：<br>
    `(+ matrix1 matrix2 ...)`
    <br>

    返回若干个矩阵的和的矩阵。如果某一个参数不是矩阵或者矩阵形状存在不匹配则报错。
    <br>

-   `-`
    <br>

    用法：<br>
    `(- matrix1 matrix2)`<br>
    `(- matrix2)`
    <br>

    返回矩阵`matrix1`、`matrix2`的差的矩阵。如果某一个参数不是矩阵或者矩阵形状存在不匹配则报错。如果matrix1缺省则为0（全零矩阵）。
    <br>

-   `*`
    <br>

    用法：<br>
    `(* l1 l2 ...)`
    <br>

    返回若干个矩阵或标量的积的矩阵（按照参数顺序从左往后进行矩阵乘或者数乘运算）。如果某一个参数不是矩阵或数或者矩阵形状存在不匹配则报错。<br>
    ```scheme
    >>> (define x (matrix-set `(1 2 3) `(4 5 6)))   ;之后的矩阵x都用这个定义
    [
    1.000000 2.000000 3.000000
    4.000000 5.000000 6.000000
    ]
    >>> (define z (matrix-set `(4) `(5) `(6)))
    [
    4.000000
    5.000000
    6.000000
    ]
    >>> (* x 3 z)
    [
    96.000000
    231.000000
    ]
    ```

除此以外，矩阵类还支持以下几种运算：

-   `@` *纯矩阵乘*
    <br>

    用法：<br>
    `(@ matrix1 matrix2)`
    <br>

    返回矩阵`matrix1`、`matrix2`的矩阵乘的矩阵。如果某一个参数不是矩阵或者矩阵形状存在不匹配则报错。
    <br>

-   `T` *矩阵转置*
    <br>

    用法：<br>
    `(T matrix)`
    <br>

    返回矩阵`matrix`的转置矩阵。如果参数不是矩阵则报错。
    <br>
    ```scheme
    >>> (T x)
    [
    1.000000 4.000000
    2.000000 5.000000
    3.000000 6.000000
    ]
    ```
-   'multiply' *矩阵对应元素乘*
    <br>

    用法：<br>
    `(multiply matrix1 matrix2)`
    <br>

    返回矩阵`matrix1`、`matrix2`对应元素相乘的矩阵。如果某一个参数不是矩阵或者矩阵形状存在不匹配则报错。
    <br>

-   `det` *矩阵行列式*

    用法：<br>
    `(det matrix)`
    <br>

    返回矩阵`matrix`的行列式。如果参数不是方阵则报错。
    ```scheme
    >>> (define k (matrix-set `(1 2) `(4 5)))       ;之后的k都用这个定义
    ()
    >>> (det k)
    -3
    ```

-   `inverse` *矩阵求逆*
    <br>

    用法：<br>
    `(inverse matrix)`
    <br>

    返回矩阵`matrix`的逆矩阵。如果参数不是方阵或者矩阵不可逆则报错。
    ```scheme
    >>> (inverse k)
    [
    -1.666667 0.666667
    1.333333 -0.333333
    ]
    ```

-   `rank` *矩阵秩*
    <br>

    用法：<br>
    `(rank matrix)`
    <br>

    返回矩阵`matrix`的秩。如果参数不是矩阵则报错。
    ```scheme
    >>> (rank k)
    2
    ```

-   `trace` *矩阵迹*
    <br>

    用法：<br>
    `(trace matrix)`
    <br>

    返回矩阵`matrix`的迹。如果参数不是方阵则报错。
    ```scheme
    >>> (trace k)
    6
    ```
-   `upper-triangle` *矩阵上三角化*
    <br>

    用法：<br>
    `(upper-triangle matrix)`
    <br>

    返回矩阵`matrix`的一种上三角化矩阵。如果参数不是矩阵则报错。
    ```scheme
    >>> (define m (matrix-set `(1 2 3) `(4 5 6) `(7 8 10)))
    ()
    >>> (upper-triangle m)
    [
    1.000000 2.000000 3.000000
    0.000000 -3.000000 -6.000000
    0.000000 0.000000 1.000000
    ]
    ```








    

    





