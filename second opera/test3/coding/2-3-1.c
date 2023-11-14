#define PROCESS_NAME_LEN 32
#define MIN_SLICE 10
#define DEFAULT_MEM_SIZE 1024
#define DEFAULT_MEM_START 0

#define MA_FF 1
#define MA_BF 2
#define MA_WF 3

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
int mem_size = DEFAULT_MEM_SIZE;
int ma_algorithm =MA_FF;
static int pid =0;
int flag =0;

struct free_block_type{
	int size;
	int start_addr;
	struct free_block_type *next;
};
struct free_block_type *free_block;

struct allocated_block{
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	struct allocated_block *next;
};
struct allocated_block *allocated_block_head=NULL;
struct free_block_type* init_free_block(int mem_size);
void display_menu();
int set_mem_size();
void set_algorithm();
void rearrange(int algorithm);
void rearrange_FF();
void rearrange_BF();
void rearrange_WF();
int new_process();
void mem_compact(int sliceSize);
int allocate_mem(struct allocated_block *ab);
void kill_process();
struct allocated_block* find_process(int pid);
int free_mem(struct allocated_block *ab);
int dispose(struct allocated_block *free_ab);
int display_mem_usage();
int main(){
	char choice;
	free_block=init_free_block(mem_size);
	while(1){
		display_menu();
		fflush(stdin);
		choice=getchar();
		switch(choice){
			case '1':set_mem_size();break;
			case '2':set_algorithm();flag=1;break;
			case '3':new_process();flag=1;break;
			case '4':kill_process();flag=1;break;
			case '5':display_mem_usage();flag=1;break;
			case '0':exit(0);//do_exit()
			default:break;
		}
	}
}
struct free_block_type* init_free_block(int mem_size){
	struct free_block_type *fb;
	fb=(struct free_block_type *)malloc(sizeof(struct free_block_type));
	if(fb==NULL){
		printf("No mem\n");
		return NULL;
	}
	fb->size =mem_size;
	fb->start_addr =DEFAULT_MEM_START;
	fb->next=NULL;
	return fb;
}

void display_menu(){
	printf("\n");
	printf("1 - Set memory size(default =%d)\n",DEFAULT_MEM_SIZE);
	printf("2 - Select memory allocation algorithm\n");
	printf("3 - New process \n");
	printf("4 - Terminate a process \n");
	printf("5 - Display memory usage \n");
	printf("0 - Exit\n");
}

int set_mem_size(){
	int size;
	if(flag!=0){
		printf("Cannot set memory size again\n");
		return 0;
	}
	printf("Total memory size =");
	scanf("%d",&size);
	if(size>0){
		mem_size = size;
		free_block->size =mem_size;
	}
	flag=1;
	return 1;
}

void set_algorithm(){
	int algorithm;
	printf("\t1 - First Fit\n");
	printf("\t2 - Best Fit \n");
	printf("\t3 - Worst Fit\n");
	scanf("%d", &algorithm);
	if(algorithm>=1&&algorithm<=3)
		ma_algorithm=algorithm;
	rearrange(ma_algorithm);
}

void rearrange(int algorithm){
	switch(algorithm){
		case MA_FF: rearrange_FF();break;
		case MA_BF: rearrange_BF();break;
		case MA_WF: rearrange_WF();break;
	}
}

void rearrange_FF(){
	struct free_block_type *head =free_block;
	int free_block_count=0;
	while(head!=NULL){
		free_block_count++;
		head = head->next;
	}
	int i=0,j=0;
	for(i;i<free_block_count-1;i++){
		struct free_block_type *node =free_block;
		struct free_block_type *pre =free_block;
		j=0;
		for(j;j<free_block_count -1 -i;j++){
			if(node->start_addr>node->next->start_addr){
				if(j==0){
					free_block= node->next;
					struct free_block_type *temp =node->next;
					node ->next =temp->next;
					temp->next =node;
				}
				else{
					struct free_block_type *temp =node->next;
					pre->next =temp;
					node->next=temp->next;
					temp->next =node;
				}
			}
			else{
				pre =node;
				node=node->next;
			}
		}
	}
}

void rearrange_BF(){
	struct free_block_type *head =free_block;
	int free_block_count =0;
	while(head!=NULL){
		free_block_count++;
		head=head->next;
	}
	int i=0,j=0;
	for(i;i<free_block_count -1;i++){
		struct free_block_type *node =free_block;
		struct free_block_type *pre =free_block;
		j=0;
		for(j;j<free_block_count -1 -j;j++){
			if(node->size>node->next->size){
				if(j==0){
					free_block=node->next;
					struct free_block_type *temp =node->next;
					node->next=temp->next;
					temp->next =node;
				}
				else{
					struct free_block_type *temp =node->next;
					pre->next =temp;
					node->next =temp->next;
					temp->next =node;
				}
			}
			else{
				pre =node;
				node =node->next;
			}
		}
	}
}

void rearrange_WF(){
	struct free_block_type *head =free_block;
	int free_block_count =0;
	while(head!=NULL){
		free_block_count++;
		head =head->next;
	}
	int i=0,j=0;
	for(i;i<free_block_count -1; i++){
		struct free_block_type *node =free_block;
		struct free_block_type *pre =free_block;
		j=0;
		for(j;j<free_block_count -1-i;j++){
			if(node->size<node->next->size){
				if(j==0){
					free_block =node ->next;
					struct free_block_type *temp =node->next;
					node->next =temp->next;
					temp->next =node;
				}
				else{
					struct free_block_type *temp=node->next;
					pre->next =temp;
					node->next=node;
					temp->next =node;
				}
			}
			else{
				pre =node;
				node =node->next;
			}
		}
	}
}

int new_process(){
	struct allocated_block *ab;
	int size;
	int ret;
	ab= (struct allocated_block *)malloc(sizeof(struct allocated_block));
	if(!ab)
		exit(-5);
	ab->next =NULL;
	pid++;
	sprintf(ab->process_name, "PROCESS-%02d",pid);
	ab->pid =pid;
	printf("Memory for %s:",ab->process_name);
	scanf("%d",&size);
	if(size>0)
		ab->size=size;
	ret =allocate_mem(ab);
	
	if((ret==1)&&(allocated_block_head ==NULL)){
		allocated_block_head =ab;
		return 1;
	}
	else if(ret ==1){
		ab->next=allocated_block_head;
		allocated_block_head=ab;
		return 2;
	}
	else if(ret ==-1){
		printf("Allocation fail\n");
		free(ab);
		return -1;
	}
	return 3;
}
void mem_compact(int sliceSize){
	struct allocated_block *ab =allocated_block_head;
	free_block->start_addr =0;
	free_block->size =sliceSize;
	free_block->next =NULL;
	while(ab!=NULL){
		ab->start_addr =sliceSize;
		sliceSize +=ab->size;
		ab=ab->next;
	}
}
int allocate_mem(struct allocated_block *ab){
	struct free_block_type *fbt, *pre;
	int request_size =ab->size;
	fbt =pre=free_block;
	int totalSize =0;
	if(fbt==NULL)return -1;
	while(fbt!=NULL){
		totalSize +=fbt->size;
		if(fbt->size>=request_size){
			if(fbt->size-request_size >=MIN_SLICE){
				ab->start_addr =fbt->start_addr;
				fbt->start_addr =fbt->start_addr +request_size;
				fbt->size =fbt->size - request_size;
			}
			else{
				ab->size =fbt->size;
				ab->start_addr =fbt->start_addr;
				if(fbt==free_block)free_block =fbt->next;
				else pre->next =fbt->next;
			}
			return 1;
		}
		else{
			pre =fbt;
			fbt=fbt->next;
		}
	}
	if(totalSize >= request_size){
		mem_compact(totalSize);
		if(totalSize -request_size>=MIN_SLICE){
			free_block->size =free_block->size-request_size;
			ab->start_addr =free_block->size;
		}
		else{
			free_block->size =0;
			ab->start_addr =free_block->start_addr;
		}
		return 1;
	}
	else return -1;
}
struct allocated_block* find_process(int pid){
	struct allocated_block *ab;
	ab = allocated_block_head;
	while(ab!=NULL){
		if(ab->pid ==pid)return ab;
		ab = ab->next;
	}
	printf("can't find process%d",pid);
	return NULL;
}
void kill_process(){
	struct allocated_block *ab;
	int pid;
	printf("Kill Process, pid=");
	scanf("%d",&pid);
	ab=find_process(pid);
	if(ab!=NULL){
		free_mem(ab);
		dispose(ab);
	}
}
int free_mem(struct allocated_block *ab){
	int algorithm =ma_algorithm;
	struct free_block_type *fbt, *pre, *work;
	fbt=(struct free_block_type*)malloc(sizeof(struct free_block_type));
	if(!fbt)return -1;
	fbt->size =ab->size;
	fbt->start_addr =ab->start_addr;
	work=free_block;
	if(work ==NULL){
		free_block =work;
		work->next =NULL;
	}
	else{
		while(work->next !=NULL)work =work->next;
		work->next =fbt;
		fbt->next =NULL;
	}
	rearrange_FF();
	pre =free_block;
	if(pre!=NULL){
		work =pre->next;
		while(work!=NULL){
			work=pre->next;
			if(pre->start_addr + pre->size ==work->start_addr){
				pre->next =work->next;
				pre->size+=work->size;
				free(work);
			}
			else pre = pre->next;
			if(pre !=NULL) work =pre->next;
			else work =NULL;
		}
	}
	rearrange(algorithm);
	return 1;
}

int dispose(struct allocated_block *free_ab){
	struct allocated_block *pre,*ab;
	if(free_ab == allocated_block_head){
		allocated_block_head=allocated_block_head->next;
		free(free_ab);
		return 1;
	}
	pre =allocated_block_head;
	ab =allocated_block_head->next;
	while(ab!=free_ab){pre=ab; ab=ab->next;}
	pre->next=ab->next;
	free(ab);
	return 2;
}

int display_mem_usage(){
	struct free_block_type *fbt =free_block;
	struct allocated_block *ab=allocated_block_head;
	if(fbt==NULL)return -1;
	printf("--------------------------\n");
	printf("Free Memory:\n");
	printf("%20s %20s\n","     start_addr","     fbt->size");
	while(fbt!=NULL){
		printf("%20d %20d\n",fbt->start_addr,fbt->size);
		fbt=fbt->next;
	}
	printf("\nUsed Memory:\n");
	printf("%10s %20s %10s %10s\n","PID","ProcessName","start_addr","size");
	while(ab!=NULL){
		printf("%10d %20s %10d %10d\n",ab->pid,ab->process_name,ab->start_addr,ab->size);
		ab=ab->next;
	}
	printf("---------------------------\n");
	return 0;
}
