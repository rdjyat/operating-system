#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <semaphore.h> 
#include <sched.h> 

sem_t* mySem = NULL;
int tickets = 1000;

void *sell_ticket(){
    int i = 50;
    while(i--){
		sem_wait(mySem);
        printf("当前票数为 %d\n",tickets);
        int temp = tickets;
        sched_yield();
        temp = temp - 1;
        sched_yield();        
        tickets = temp;
        sem_post(mySem);
         }
         
}
void *return_ticket(){
    int i = 50;
    while(i--){
		sem_wait(mySem);
        printf("当前票数为 %d\n",tickets);
        int temp = tickets;
        sched_yield();
        temp = temp + 1;
        sched_yield();        
        tickets = temp;
        sem_post(mySem);
        }
       
     
}
int main(){
    pthread_t p1,p2;
    mySem = sem_open("Ticket", O_CREAT, 0666, 1);
    pthread_create(&p1,NULL,sell_ticket,NULL);//创建卖票线程
    pthread_create(&p2,NULL,return_ticket,NULL);//创建售票线程
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    sem_close(mySem);
    sem_unlink("Ticket");
    printf("最终票数为 %d.\n",tickets);
    return 0;
}
