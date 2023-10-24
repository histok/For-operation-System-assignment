# _OS_lab_one_
## 步骤一
### 在运行1-1所给代码后得到以下结果：

![初步实验](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test1/pic/1.jpg)

图中可以看出，可以看出父进程和子进程是并发执行的，在父进程中 `fork()` 的返回值是子进程的pid，而在子进程中 `fork()` 的返回值是0。 
而linux中 `wait ( NULL)` 的作用是等待任何子进程结束。 如果 `wait ( NULL)` 被调用，那么父进程会阻塞，直到任何子进程结束并返回状态。因此可以得到结论：程序中父进程被阻塞直到子进程返回结束返回状态。

---

### 去除 `wait(NULL)` 后，结果如同预料。为父进程先得出结果，子进程之后得出结果：

![除去wait后的结果](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test1/pic/2.jpg)

关于为什么是父进程先得出结果：个人认为是由于父进程在进程调度中拥有更高的优先级，因此能够获取更多的资源，因此能够更快的输出。

---

### 之后，在代码中添加如下程序
```
{
  global =0;//设置为全局变量
  global++;//应用于父进程
  global--;//应用于子进程
}
```

运行程序并输出打印的值：
![添加全局变量后](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test1/pic/3.jpg)

从图中可以得到：输出的value的值并不一样，但其地址相同其原因在于输出地址为各个进程的相对地址空间，原因在于 **当调用fork(),当控制转移到内核中的fork代码后，内核开始做： 1.分配新的内存块和内核数据结构给子进程。 2.将父进程部分数据结构内容拷贝至子进程。 3.将子进程添加到系统进程列表。 4.fork返回开始调度器，调度。** 在这个过程中，`global` 的数据结构被拷贝过去，因此相对地址相同。

---

### 在return前添加对于golbal的各种操作：
```
{
  value=value+;
  printf("brefore return value =%d, *value =%d",value,&value）；
}
```

得到如下结果：
 ![return前添加程序](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test1/pic/4.jpg)

 更验证了之前的说法。

---

之后将程序分割，用 `system`和 `exce` 族另外调用文件。 

### system

