
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	int p2,p3,p4,p5;
	while ((p2=fork())==-1); //p1 create p2
	if (!p2) {

		while ((p4=fork())==-1); //p2 create p4
		if (!p4) {
			while(1) {
				printf("I am p4,with ID[%d],	my father's ID[%d].\n",getpid(),getppid());
				sleep(1);
			}
    		}


    		while ((p5=fork())==-1); //p2 create p5
   		if (!p5) {
			while(1) {
				printf("I am p5,with ID[%d],	my father's ID[%d].\n",getpid(),getppid());
				sleep(1);
			}
    		}

		for(int i = 0;i<5;i++){
			printf("I am p2,with ID[%d],	my father's ID[%d].\n",getpid(),getppid());
			sleep(1);
			
			
		}
		exit(0);
	}	


	while ((p3=fork())==-1); //p1 create p3	
	if(!p3){
		while(1) {
			printf("I am p3,with ID[%d],	my father's ID[%d].\n",getpid(),getppid());
			sleep(1);
		}
	}

	while(1) {
		printf("I am p1,with ID[%d],	my father's ID[%d].\n",getpid(),getppid());
		sleep(1);
	}
		
	return 0;
}
