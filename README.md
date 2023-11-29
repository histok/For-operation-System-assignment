# _OS_lab_one_

## 目的
---
通过在云服务器上搭建进程和线程，学生可以将理论知识转化为实际应用，加深对操作系统内部工作原理的理解，并提高编程和问题解决能力。这种实践有助于培养他们成为更有竞争力的软件工程师或系统管理员。

---

## 实验内容

详见指导书
## 实验思想

旨在帮助学生深入了解操作系统的内部工作原理和进程管理，励学生实际编写操作系统相关的代码，以提高编程技能，教授其如何进行并发编程，处理竞争条件和协同工作。

---
##
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


# _OS_lab_two_

## 目的

学习进程通信与内存管理的目的在于有效地组织和控制计算机系统中的资源，以实现多任务处理和多进程协同工作。

## 实验内容

详见指导书

## 实验思想

学习进程通信与内存管理的目的在于实现多任务处理和多进程协同工作，通过协作、数据传输和资源共享提高系统性能。内存管理旨在有效利用内存、防止内存泄漏、避免内存碎片，确保进程隔离与保护。实验思想包括实现基本通信机制、多进程协同工作，动态内存分配与释放，模拟内存碎片和异常处理，通过理论学习与实践培养解决实际问题的能力。

## Part1 IPC

---

### 软中断通信


补充  `inter_handler` ,通过信号量的触发来使得进程被杀死。
```
	if(signum == SIGALRM ||signum ==SIGQUIT||signum== SIGUSR1||signum==SIGUSR2){
		flag =1;
	}
	else{
	printf("[info] error \n");
	}
```

waiting的补充；
```
void waiting(int i){
	while(!flag){
		sleep(1);
	}
	printf("Child process%d is killed by parent!!\n",i);
	flag=0;
}
```

父进程
```
			int time=0;
			alarm(5);
			while(1){
				signal(SIGALRM, SIG_IGN);
				signal(SIGQUIT, inter_handler);
				printf("[info] time passed: %d\n",++time);
				if(flag||timeout){

					kill(pid1,SIGUSR1);
					kill(pid2,SIGUSR2);
					wait(NULL);
					wait(NULL);

					printf("[info] Parent process is killed!!\n");
					exit(0);
				}
				if(time==4){
					timeout =1;
				}
				sleep(1);
			}
```

子进程
```
			signal(SIGUSR2, inter_handler);
			signal(SIGQUIT,SIG_IGN);
			signal(SIGALRM,SIG_IGN);
			waiting(对应的进程数);
```

运行程序，由于 `alarm(5)`, 因此 5s 后父进程收到软中断信号 `SIGALRM`, 父进程向子进程发送信号，子进程接收信号后退出，父进程随之退出，输出如下:
![2-1-1](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test1/pic/2-1-2.jpg)

但是如果在时间内键盘按下 `Ctrl + \`， 向父进程发出信号
`SIGQUIT`, 则父进程会退出，但是子进程不会打印。
经过查阅, `Ctrl + \`终端会向所有现在运行中的前台进程发送 `SIGQUIT`, 因此子进程直接退出，不会执行其剩下的语句
。在子进程里加上
```
    signal(SIGQUIT, SIG_IGN);
```
使其忽略 `SIGQUIT` 信号后问题解决.
![2-1-2](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test1/pic/2-1-3.jpg)>

---

### 管道通信

根据所给代码补充完在父(子)进程中读写的部分代码
```cpp
        lockf(fd[1],1,0);
        for (int i = 0; i < (BUFFER_SIZE - 1) / 2; ++i)
        {
            write(fd[WRITE_END], c1, 1);
        }
        lockf(fd[1],0,0);         
        sleep(2); 

        // **************************

        lockf(fd[1],1,0);                 
        for (int i = 0; i < (BUFFER_SIZE - 1) / 2; ++i)
        {
            write(fd[WRITE_END], c2, 1);
        }
        lockf(fd[1],0,0); 
        sleep(2);     

        // *****************************

        wait(NULL);
        lockf(fd[0],1,0);  
        ssize_t size = read(fd[READ_END], InPipe, BUFFER_SIZE - 1);
        if(size > 0)
            InPipe[size] = '\0';
        else if(size == 0)
            printf("quit\n");
        else
            printf("error\n");
        lockf(fd[0],0,0);  
        printf("%s\n",InPipe);    
```
得到结果
![2-2-1](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test2/pic/2-2-1.jpg)
在父进程中，如果去掉lockf函数
```
    // lockf(fd[0],1,0); 
    // .............
    // lockf(fd[0],0,0);  
```

则管道读写的互斥和同步出现问题，输出结果如下:
![2-2-2](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test2/pic/2-2-2.jpg)

---

## Part2 内存分配与回收

这是一个简单的内存管理系统，用C语言编写。该系统实现了基本的内存分配和释放功能，支持不同的内存分配算法（First Fit、Best Fit、Worst Fit）。

### 宏定义

- `PROCESS_NAME_LEN`: 进程名称的最大长度
- `MIN_SLICE`: 最小内存块大小
- `DEFAULT_MEM_SIZE`: 默认内存大小
- `DEFAULT_MEM_START`: 默认内存起始地址
- `MA_FF`, `MA_BF`, `MA_WF`: 内存分配算法的宏定义

### 全局变量

- `mem_size`: 总内存大小
- `ma_algorithm`: 当前内存分配算法
- `pid`: 进程ID
- `flag`: 标志，用于判断是否已设置过内存大小

### 数据结构

#### free_block_type

表示空闲内存块的结构体，包括大小、起始地址和指向下一块空闲内存的指针。

#### allocated_block

表示已分配内存块的结构体，包括进程ID、大小、起始地址、进程名称和指向下一块已分配内存的指针。

### 函数

- `init_free_block(mem_size)`: 初始化空闲内存块链表
- `display_menu()`: 显示用户菜单
- `set_mem_size()`: 设置内存大小
- `set_algorithm()`: 设置内存分配算法
- `rearrange(algorithm)`: 根据算法重新排列内存块链表
- `rearrange_FF()`, `rearrange_BF()`, `rearrange_WF()`: 不同算法的具体实现
- `new_process()`: 创建新进程并分配内存
- `mem_compact(sliceSize)`: 压缩内存空间
- `allocate_mem(ab)`: 分配内存给进程
- `kill_process()`: 终止进程并释放内存
- `find_process(pid)`: 根据进程ID查找进程
- `free_mem(ab)`: 释放已分配的内存
- `dispose(free_ab)`: 释放已分配的进程
- `display_mem_usage()`: 显示内存使用情况

### 主函数

包含一个无限循环，根据用户选择执行相应的功能。

```c
int main() {
    // ...
    while(1) {
        display_menu();
        // ...
    }
}
```

---

### 运行展示

菜单界面
![2-3-1](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-1.jpg)

假如没有设置内存，会默认为为`1024`
![2-3-2](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-2.jpg)

设置内存为1000
![2-3-3](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-3.jpg)

内存设置好后就无法再更改
![2-3-4](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-4.jpg)

没有设置上限，一般为分配内存上限
![2-3-5](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-5.jpg)
![2-3-6](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-6.jpg)

创建一个进程内存大小为100
![2-3-8](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-8.jpg)

创建一个进程内存大小为800
![2-3-9](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-9.jpg)

目前内存布局如下
![2-3-10](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-10.jpg)

之后设置内存为950，重新分配内存将100到200分配给2号，再将800-900分配给4号，同时设置算法为BF
![2-3-15](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-15.jpg)

分配一个大小的10的进程，内存布局如下，可以看到采用了 BF 算法，选择了最小的块, 即后面的那个块去分配这 10 的大小
![2-3-16](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-16.jpg)

设置算法为FF
![2-3-17](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-17.jpg)

重新分配一个大小为10的进程，可以看到根据 FF 算法，内存分配在了最前面的块里
![2-3-18](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-18.jpg)

设置算法为WF
![2-3-19](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-19.jpg)

再分配一个大小为10的进程，可以看到根据 FF 算法，内存分配在了最大面的块里
![2-3-20](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-20.jpg)

终止进程2，查看内存布局如下，可以看到进程2的内存已经被全部释放
![2-3-21](https://github.com/histok/For-operation-System-assignment/blob/main/second%20opera/test3/pic/2-3-21.jpg)

---

## Part 3 遇到的问题

在管道程序中，由于未添加 `sleep(1)`, 因此 5s 后父进程收到软中断信号 `SIGALRM`, 父进程向子进程发送信号，子进程接收信号后退出，父进程随之退出，未得到子进程被中断的输出

但父子进程正常退出，之后经过了解，发现需要 `sleep` ，
基于子函数一定的程序执行时间。
因此在函数里加上
```c
    sleep(1)
```
后问题解决


---

## Part 4 分析和思考

学习进程通信与内存管理时，可以思考如何优化系统性能、确保安全性、提高资源利用效率，并将这些概念应用于实际场景。同时，考虑故障排除和调试方法，关注未来发展趋势，以更深刻理解这些概念，并为系统设计和优化做出明智决策。

---

## Part 5 Score

自评分: 28


# _OS_lab_three_

## 文件系统

### 构思

Ext2的磁盘布局使用了组块的结构, 并且采用了多级索引使得单文件的大小得到了很大的提升.

![3-2-3](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/str.png)

设计和实现基于文件空间的Ext2文件系统时，需要关注以下几个方面：

1. **超级块（Superblock）：**
   - 定义文件系统的基本参数，如块大小、inode 大小、块总数等。
   - 存储文件系统的状态信息，例如最后一次挂载的时间、上次检查的时间等。

2. **块组描述符表（Block Group Descriptor Table）：**
   - 存储关于每个块组的信息，如空闲块和inode 的数量、块位图和inode 位图的起始块号等。

3. **块位图和inode 位图：**
   - 用于跟踪块和inode 的分配情况，记录哪些块和inode 已经被使用，哪些是空闲的。

4. **inode（Index Node）表：**
   - 存储文件和目录的元数据，包括文件大小、所有者、权限、链接数等信息。

5. **目录项：**
   - 用于存储目录中文件和子目录的关联关系，其中包含文件名和对应的inode 号。

6. **数据块的管理：**
   - 确定如何分配和管理数据块，包括直接块、间接块、双间接块等。

7. **文件系统的布局：**
   - 确定文件系统中各个部分的物理布局，包括超级块、块组描述符表、数据块等的位置。

解决方案如下:
使用 `fseek` + `fwrite/fread` 模拟磁盘的读取和写入以及定位，并且制造接口提供给用户、

在本次实验中，我们进行了简化处理，具体包括以下几点：
1. 我们仅设立了一个组（Group），也就是说只有一个组描述符。
2. 系统中仅包含一个用户，因此与用户相关的实现在本实验中可以不予考虑。

简化后的文件系统布局如下:

![3-2-2](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/loc.png)

<br/>

---

### ***Analysis***

简化后的文件系统结构很简单, 其中关键在于从文件名到实际数据块的映射, 大体上是这样一个顺序:

```shell
  File/Dir_Name ------> Inode ------> DataBlock(s)
```

在此实验中，每个Inode可以映射到文件或目录。它们之间的区别主要体现在以下几个方面：

### 文件的Inode：
- 直接指向所占用的所有数据块。
- `i_blocks`字段的前六位数据为直接索引，即直接指向数据块。
- 第七位数据为一个一级间接索引，第八位数据为一个二级间接索引。
- 数据块中存储的是文件的内容。

### 目录的Inode：
- 同样指向数据块，但存储的是许多`dir_entry`，即目录项。
- 对于每个目录项，如果`file_type`为`FileType::DIR`，则形成一个树形结构。
- 如果是文件，则成为一个叶子节点，指向其文件实际内容所在的数据块。

![3-1-1](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-1.jpg)

![3-1-2](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-2.jpg)

---

---

### ***Run***

![3-1-3](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-1.jpg)
![3-1-4](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-1.jpg)
![3-1-5](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-1.jpg)
![3-1-6](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-1.jpg)
![3-1-7](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-1.jpg)
![3-1-8](https://github.com/histok/For-operation-System-assignment/blob/main/third%20opera/pic/3-1-1.jpg)


## Part 3 遇到的问题

在init.o中出现复定义，经过检测后发现原因在于源文件中没有加入关于路径的定义导致

---

## Part 4 分析和思考

在写程序时要注意设计以及功能模块的划分，对于每一个功能要考虑各种情况，保证程序的健壮性.

---

## Part 5 自评分

`Score: ` 27
