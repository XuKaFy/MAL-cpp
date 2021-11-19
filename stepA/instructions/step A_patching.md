# MAL-XLisp

## 匿名函数递归

由于对匿名函数进行包装从而达到能够调用自身目的的实现方式并不优雅，所以在MAL中新增关键字`apply-self`，该关键字将变量传递给最近的环境中正在执行的函数（实际上，满足该要求的只有lambda表达式，包括define的“具名”匿名函数）并且调用，例如：

```scheme
((lambda (x all) (if (empty? x) all (apply-self (cdr x) (+ all (car x))))) (list 1 2 3 4 5) 0)
```

由于每一个匿名函数都会创建新的环境，所以`apply-self`关键字将后面的参数传递给这个lambda本身。若是多个lambda过程嵌套，只会将参数传递给最近的lambda函数体。

我们只需要在每个环境中记录创建自己的是哪个Lambda，在调用`apply-self`的时候调用即可。包括`Macro`。

## 向量

原本内置的宏系统并不能展开形如(cond ('() 1) ('t 2))这样的表达式，原因是lisp本身的语法并没有平级展开的能力。

例如，你不能用传统的方式写出这样一个函数：

```scheme
(grasp 'a1 'a2 ...)
```

而`grasp`可以对不定长的`('a1 'a2 ...)`进行操作。在传统lisp下，只能写作：

```scheme
(grasp '(a1 a2 ...))
```

所以我们增加一种数据类型`vector`：这种数据类型基于`std::vector<AbstractType*>`，并且可以帮助函数平级展开参数，例如：

```scheme
(define grasp [x] (cdr x))
(grasp 'a1 'a2 ...)		-> 		(a2 ...)
```

对于需要固定参数的情况，可以写作：

```scheme
(define grasp [must-grasp remain] (cons must-grasp (cdr (cdr remain))))
(grasp 'a1 'a2 ...)		-> 		(a1 a3 ...)
```

默认最后一个参数接受列表的所有剩余项，如果不希望接受剩余项，则应该使用内置列表`(a1 a2 ... an)`。

## 内存管理

该lisp版本除了会对数据本身进行修改的操作以外，任何操作都不拷贝任何数值，而是引用。

会进行复制的操作有：

	* `splice-unquote`：由于将列表嵌入会改变列表结构，所以复制。

所有对象由智能指针进行管理，当引用数降到0时完全销毁对象。

## 错误处理

内置的`(try A (catch x B))`不仅仅可以接受`throw`返回的参数，也能接受解释器报错。

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

## 面向对象
