# _OS_lab_one_
## 步骤一：process
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

之后将程序分割，用 `system`和 `exec` 族另外调用文件。 

### system

![system](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test1/pic/7.jpg)

通过 `system()` 中输出的pid与父进程中输出的pid不同可以得出，其作用相当于调用了一次 `fork()` 函数，新创建了一个线程，因此父进程能够运行。在本程序中就是父进程创建了子进程，子进程又创建了子子进程，子子进程去执行 `b.out` 。


### exec

![exec](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test1/pic/8.jpg)

与 `system` 不同，`exec`是将原进程替换为他创建的进程。 因此后面父进程不再执行，并且子进程输出的pid与父进程相同。

---

## 步骤二：thread

---

创建两个线程，并在其中对全局变量 `sumnum` 执行 100000 次不同操作

![未修改](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test2/pic/3.jpg)

但结果并不如同想象中的一样。函数中的输出语句并未执行，且 `sumnum` 的值也并不为0.

于是猜测是 `thread` 中的线程返回特定状态后，主函数得到信号，执行并结束程序。在这个过程中，另一个线程并未完成其任务，而输出语句则可能是存储在缓存区，等待输出期间被主函数的输出刷新，而没有成功输出。

因此使用 `pthread_join(id1, NULL)，pthread_join(id2, NULL)`对两个线程进行阻塞等待，但结果不尽人意。
 `sumnum` 的绝对值反而变得更大。

![第一次修改](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test2/pic/5.jpg)

同时也作出另一种修改，将主进程挂起，结果出现了没有接收信号，导致原线程被挂起的情况。

![第一次修改2](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test2/pic/4.jpg)

这时候意识到 `sumnum` 对于两个线程来说是临界资源，需要加锁，使其在进入临界区时发生互斥，保证只有一个线程访问。
修改后重新执行程序，得到了正确的结果。

![第二次修改](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test2/pic/7.jpg)

---

将 `system` 代码放在线程中运行，执行thread5.c，然后里面的 `system` 执行 b.out ，可以得到如下结果：

![修改上一次1](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test2/pic/8.jpg)

这个程序的执行流程如下：首先，父进程创建了一个子进程。在子进程中，它创建了一个线程并执行了外部命令 b.out 。在 b.out 中，会输出进程的ID，但这个ID与子进程的ID并不相同。这是因为在执行调用后，子进程创建了一个新的子进程，然后将控制权切换到这个新的子进程，而hello_world.exe在这新的子进程中执行。

这个过程导致了两个不同的进程ID：
子进程的ID，这是在父进程中获取的，用于标识子进程的起始点。

b.out 执行的新子进程的ID，这是b.out中输出的，因为 `system` 调用实际上在一个新的子进程中执行外部命令。

所以会看到两个不同的进程ID，一个是原始子进程的ID，另一个是b.out中新子进程的ID。这说明了在 `system` 调用中执行外部命令会创建一个新进程，并且进程ID会有所不同。

---

将 `execlp` 代码放在线程中运行，执行thread52.c，然后里面的 `system` 执行 b.out ，可以得到如下结果：

![修改上一次2](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test2/pic/9.jpg)

在子进程中，线程执行函数来启动外部命令, 创建一个新的进程。在这个新进程中输出了进程ID，但在此过程中，原始子进程的执行被暂时中断了，因为`execlp`函数执行了进程切换。

由于进程切换，原始子进程中的代码暂停执行，包括输出进程ID的部分。所以不会看到原始子进程中的两次打印进程ID的输出，因为这些代码在进程切换期间未执行。

---

## 步骤三：自旋锁

补充实验代码后得到如下实验结果：

![enter image description here](https://github.com/histok/For-operation-System-assignment/blob/main/first%20opera/test3/pic/1.jpg)

其原因为：两个线程在竞争中使用自旋锁来逐次增加一个共享变量。自旋锁确保同时只有一个线程能够修改这个变量。因为两个线程各自增加5000次，所以最终共享变量的值是10000。
```
{
  spinlock_lock(lock);//线程尝试获取自旋锁，如果锁已经被其他线程占用，那么它会进入自旋等待，直到锁被释放。
  shared_value++;//一旦线程获得了自旋锁，它会增加 shared_value 的值。
  spinlock_unlock(lock);//线程释放自旋锁，以便其他线程可以获得它。
}
```

---


## 遇到问题

在整个实验过程中，我遇到了一个问题，导致程序无法正常输出结果，最终只能通过重启虚拟机来解决。经过后续的调查和查询，我初步确定了问题的原因，即主进程被挂起（suspended）导致无法输出。

问题的关键是主进程中的 wait(NULL) 这一部分。在多线程或多进程的程序中，wait(NULL) 通常用于等待子进程的退出，并回收子进程的资源。然而，如果没有子进程，或者子进程在主进程之前结束，wait(NULL) 可能会导致主进程被挂起，等待一个不存在的子进程。

这种情况会导致程序似乎没有响应，无法输出结果。为解决这个问题，可以在主进程中使用适当的同步机制，以确保主进程不会被挂起，或者使用合适的等待机制来等待线程的完成。这将确保程序能够正常输出结果，而不会被无关的子进程阻塞。

鉴定为：常见的并发编程问题。

## 结论

通过实验，实验展示了互斥锁的重要性，它可以保护共享资源，避免数据混乱。这是操作系统学习中的一个关键概念，并且深入了解了多线程文件读写的原理，以及互斥锁如何确保数据的一致性，对于理解并发编程很有帮助。
