
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

union semun{
  /*定义联合体*/
    int val;
    struct semid_ds*buf;
    unsigned short *array;
};


/*函数声明*/
int init_sem(int sem_id,int init_value);///*信号量初始化函数*/
int del_sem(int sem_id);		/*删除信号量函数*/
int wait(int sem_id);			/*wait函数*/
int signal(int sem_id);		/*signal函数*/



int init_sem(int sem_id,int init_value){
	/*信号量初始化函数*/
	union semun sem_union;
	sem_union.val = init_value;  //init_value为初始值
	if(semctl(sem_id,0,SETVAL,sem_union) == -1){  //调用semctl函数为信号量赋初值
		perror("Initialize semaphore");
		return -1;
	}
	return 0;
}

int del_sem(int sem_id){
	/*删除信号量函数*/
	union semun sem_union;
	if(semctl(sem_id,0,IPC_RMID,sem_union) == -1){
		perror("Delete semaphore");
		return -1;
	}
	return 0;
}

int wait(int sem_id){
	/*P操作函数*/
	struct sembuf sem_b;		
	sem_b.sem_num = 0;		//信号量编号，这里单个信号的编号应该为0
	sem_b.sem_op = -1;		//信号量操作，取值为-1表示P操作
	sem_b.sem_flg = SEM_UNDO;	//在进程没释放信号量而退出时，系统自动释放该进程中未释放的信号量
	
	if(semop(sem_id, &sem_b, 1) == -1){	//进行P操作
		perror("P operation");
		return -1;
	}
	return 0;
}

int signal(int sem_id){
	/*V操作函数*/
	struct sembuf sem_b;
	sem_b.sem_num = 0;		//信号量编号，这里单个信号的编号应该为0
	sem_b.sem_op = 1;		//信号量操作，取值为+1表示V操作
	sem_b.sem_flg = SEM_UNDO;	//在进程没释放信号量而退出时，系统自动释放该进程中未释放的信号量
	
	if(semop(sem_id, &sem_b, 1) == -1){	//进行V操作
		perror("V operation");
		return -1;
	}
	return 0;
}


/**************************************
*************main start*******************
******************************************/

int main(){
	pid_t p1,p2,p3,p4;
	int s1,s2,s3;
	/*调用semget函数，创建三个信号量*/
	s1 = semget(ftok(".",'a'),1,0666|IPC_CREAT);
	init_sem(s1,0);

	s2 = semget(ftok(".",'b'),1,0666|IPC_CREAT);
	init_sem(s2,0);

	s3 = semget(ftok(".",'c'),1,0666|IPC_CREAT);
	init_sem(s3,0);
	

	/*调用fork函数，创建四个进程*/
	p1 = fork();

	if(!p1){	//子进程p1
		printf("\nI'm the process p1 with PID=%d\n",getpid());
		//signal(s1);
		exit(0);
	}


	p2 = fork();
	if(!p2){	//子进程p2
		wait(s1);
		printf("I'm the process p2 with PID=%d\n",getpid());
		signal(s2);
		signal(s1);
		sleep(1);
		exit(0);
	}

	p3 = fork();
	if(!p3){	//子进程p3
		wait(s1);
		printf("I'm the process p3 with PID=%d\n",getpid());
		signal(s3);
		signal(s1);
		sleep(1);
		exit(0);
	}

	p4 = fork();
	if(!p4){	//子进程p4
		//wait(s2);
		//wait(s3);
		printf("I'm the process p4 with PID=%d\n",getpid());
		signal(s2);
		signal(s3);

		//del_sem(s1);
		//del_sem(s2);
		//del_sem(s3);
		exit(0);
	}
	exit(0);
}
