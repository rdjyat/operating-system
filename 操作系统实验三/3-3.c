#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>

char buf[10] = {0};

sem_t empty;	//信号量1，缓存区中空闲单元的个数
sem_t datas;	//信号量2，缓存区中非空单元的个数

void *producer(void *arg){
	for (int i = 0; i < 10;) {
	sem_wait(&empty);	//判断是否有空闲单元可供输入
	scanf("%c",&buf[i]);
	i++;
	i = i%10;			
	sem_post(&datas);	//输入一个数据，非空单元数+1
	}
	return NULL;
}

void *consumer(void *arg){
	for (int i = 0; i < 10;) {
	sem_wait(&datas);	
	printf("%c",buf[i]);
	sleep(1);			
	i++;
	i=i%10;
	sem_post(&empty);	//输出一个数据，空闲单元数+1
	}
	return NULL;
}

int main(int argc, char *argv[]){	
	sem_init(&empty, 0, 10);//初始化信号量
	sem_init(&datas, 0, 0);

	pthread_t p1, p2;
	pthread_create(&p1, NULL, producer, NULL);	//创建线程
	pthread_create(&p2, NULL, consumer, NULL);

	pthread_join(p1, NULL);//线程等待
	pthread_join(p2, NULL);

	sem_destroy(&empty);  //销毁线程
	sem_destroy(&datas);

	return 0;
}
