# MAL-Kafisp

## 内置函数

1. 关键字：`Evaluator`

    * apply
    * begin
    * define
    * define-macro
    * eval
    * lambda
    * let
    * set
    * quote
    * quasiquote
    * splice-unquote
    * unquote

2. 内置函数：`Core`

    传统函数：

    * \+
    * \-
    * \*
    * /
    * \>=
    * <=
    * \>
    * <
    * atom
    * atom?
    * buildin?
    * car
    * cdr
    * cons
    * empty?
    * eq
    * false?
    * lambda?
    * list
    * list?
    * macro?
    * pair?
    * string?
    * true?

    字符串相关：

    * read
    * read-string
    * read-file
    * join-string
    * print
    * print-string
    * newline
    * translate-from-string
    * translate-to-string
    * write-file

    计时器：

    * time

3. 全局变量：`Interface`

    * false
    * load-file
    * not
    * println
    * true

## 匿名函数递归

由于对匿名函数进行包装从而达到能够调用自身目的的实现方式并不优雅，所以在MAL中新增关键字`apply-self`，该关键字将变量传递给最近的环境中正在执行的函数（实际上，满足该要求的只有lambda表达式，包括define的“具名”匿名函数）并且调用，例如：

```scheme
((lambda (x all) (if (empty? x) all (apply-self (cdr x) (+ all (car x))))) (list 1 2 3 4 5) 0)
```

由于每一个匿名函数都会创建新的环境，所以`apply-self`关键字将后面的参数传递给这个lambda本身。若是多个lambda过程嵌套，只会将参数传递给最近的lambda函数体。

我们只需要在每个环境中记录创建自己的是哪个Lambda，在调用`apply-self`的时候调用即可。包括`Macro`。

## 变长参数

例如：

```scheme
(define-macro my-cond (& x)
    (if (empty? x) 
        ''()
        `(if ~(car (car x))
            ~(car (cdr (car x)))
            (my-cond ~@(cdr x))
        )
    )
)

(my-cond
    (false 1)
    (true 5)
    (false 3)
    (true 2)
)
```

`&`操作符将剩余的所有参数以表的形式赋值给`&`后的唯一一个（也必须有）变量。

注意，`& name`中的`name`变量可能是空表。`&`前的变量不受影响。

## 内存管理

该lisp版本除了会对数据本身进行修改的操作以外，任何操作都不拷贝任何数值，而是引用。

会进行复制的操作有：

	* `splice-unquote`：由于将列表嵌入会改变列表结构，所以复制。

所有对象由智能指针进行管理，当引用数降到0时完全销毁对象。

注意，能够创建对象的场合只有四个：

1. `Reader::read`返回的，新创建的值。

2. `Core`中的部分数值计算函数新创建的值。

3. `splice-quote`为了防止列表修改，复制的列表。

4. 某些宏对变量进行包装，比如`QUOTE`，`BEGIN`等等。

能够持有变量的对象只有：

1. `Environment`

所以我们只需要保证`Environment`存在即可。剩余的所有指针持有全部采用`shared_ptr`即可。

`Environment`是否应该死亡应该受到`LambdaType`的约束。

## 错误处理

内置的`(try A (catch x B))`不仅可以接受`throw`返回的参数，也能接受解释器报错。

## 环境

例如以下scheme代码：

```scheme
(define (create-accumulator) 
    (define value 0)
    (define (accumulator delta) 
        (set! value (+ value delta))
        value
    )
    accumulator
)

(define a (create-accumulator))
(define b (create-accumulator))

(a 1) 		->		1
(a 3)		->		4
(b 2)		->		2
```

`define`关键字和`let`关键字一样，也创造了一个小型环境。

```scheme
(define create-accumulator (lambda ()
    (define value 0)
    (define (accumulator delta) 
        (define value (+ value delta))
        value
    )
    accumulator
)

(define a (create-accumulator))
(define b (create-accumulator))

(a 1) 		->		1
(a 3)		->		4
(b 2)		->		2

```

当一个Lambda对象被创建的时候，他的运行环境就已经和他当前所在的Environment所绑定了。不论从何处运行Lambda，都应该继承原来的环境而运行。

## define 和 define-macro 的使用方式

```scheme
(define name exps)
(define-macro name (args) exps)
```

`exps`可以是多句表达式，但是把最后一个表达式的值赋值给`name`。
`define-macro`相当于执行了一个Lambda。
