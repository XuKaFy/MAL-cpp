第九步需要实现：

	(try A (catch x B))：尝试执行A的值，如果报错，就catch值x，并且执行B

	以下将会延缓实现
	(apply-self ...)：用于在匿名函数中调用自身

		如何实现apply-self呢？我们认为自身表示最靠近内层的非buildin的lambda函数。比如说：

		((lambda (x all) (if (empty? x) all (apply-self (cdr x) (+ all (car x))))) (list 1 2 3 4 5) 0)

		这里的(apply-self ...)就是指代该lambda本身。

	这里是和并发相关的部分，延缓实现
	(atom ...)：创建一个原子
	(atom? ...)：是否是原子
	(deref ...)：读取原子的值
	(reset! ...)：修改原子的值
	(swap! atom fun additional-args...)：调用(fun (deref atom) additional-args)并且覆盖atom的值
