#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
int main(void) {
	int p2,p3,p4,p5;
	while ((p2=fork())==-1); //create p2
	if (!p2) {
		printf("I am p2,with ID[%d],	my father is p1 with ID[%d].\n",getpid(),getppid());

		while ((p4=fork())==-1); //create p4
		if (!p4) {
			printf("I am p4,with ID[%d],	my father is p2 with ID[%d].\n",getpid(),getppid());
      			exit(0);
    		}
    		else wait(0);	//p2 gets waiting


    		while ((p5=fork())==-1); //create p5
   		if (!p5) {
			printf("I am p5,with ID[%d],	my father is p2 with ID[%d].\n",getpid(),getppid());
     			exit(0);
    		}
    		else wait(0);	//p2 gets waiting
		exit(0);
	}	
  	else wait(0);	//p1 gets waiting


	while ((p3=fork())==-1); //create p3	
	if(!p3){
		printf("I am p3,with ID[%d],	my father is p1 with ID[%d].\n",getpid(),getppid());
		exit(0);
	}
	else wait(0);	//p1 gets waiting
		
	return 0;
}
