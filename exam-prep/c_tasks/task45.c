#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
//cut -d: -f7 /etc/passwd | sort | uniq -c | sort -nk1
//         ch1            A  ch2 B   ch3   C  parent
int main() {
	
	int pipeA[2];
	if(pipe(pipeA)==-1) {
		err(1,"Cannot create a pipe");
	}
	const pid_t pid1=fork();
	if(pid1==-1) {
		err(2,"Cannot fork");
	}
	if(pid1==0) {
		//@ch1
		close(pipeA[0]);
		if(dup2(pipeA[1],1)==-1) {
			err(3,"Cannot dup2");
		}
		if(execlp("cut","cut","-d:","-f7","/etc/passwd",(char*)NULL)==-1) {
			err(4,"Cannot execlp");
		}
	}
	//Parent
	close(pipeA[1]);
	
	int pipeB[2];
	if(pipe(pipeB)==-1) {
		err(1,"Cannot create a pipe");
	}
	const pid_t pid2=fork();
	if(pid2==-1) {
		err(2,"Cannot fork");
	}	
	if(pid2==0) {
		//@ch2
		close(pipeB[0]);
		if(dup2(pipeA[0],0)==-1) {
			err(3,"Cannot dup2");
		}
		if(dup2(pipeB[1],1)==-1) {
			err(3,"Cannot dup2");
		}
		if(execlp("sort","sort",(char*)NULL)==-1) {
			err(4,"Cannot execlp");
		}
	}
	//Parent
	close(pipeB[1]);
	close(pipeA[0]);
	
	int pipeC[2];
	if(pipe(pipeC)==-1) {
		err(1,"Cannot create a pipe");
	}
	const pid_t pid3=fork();
	if(pid3==-1) {
		err(2,"Cannot fork");
	}
	if(pid3==0) {
		//@ch3
		close(pipeC[0]);
		if(dup2(pipeB[0],0)==-1) {
			err(3,"Cannot dup2");
		}
		if(dup2(pipeC[1],1)==-1) {
			err(3,"Cannot dup2");
		}
		if(execlp("uniq","uniq","-c",(char*)NULL)==-1) {
			err(4,"Cannot execlp");
		}
	}
	//Parent
	close(pipeC[1]);
	close(pipeB[0]);

	if(dup2(pipeC[0],0)==-1) {
		err(3,"Cannot dup2");
	}
	if(execlp("sort","sort","-nk1",(char*)NULL)==-1) {
		err(4,"Cannot execlp");
	}
	close(pipeC[0]);//Unreachable
	exit(0); //Unreachable

}
