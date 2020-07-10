//find argv[1] -type f -printf "%T@ %p\n" | sort -nk1 | tail -n1 | cut -d " " -f2
//				chFind						  chSort     chTail        parent
//										pipeA  		pipeB		pipeC
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
int main(int argc, char* argv[]) {
	if(argc!=2) {
		errx(1,"Wrong number of arguments. Expected 1, got %d",argc-1);
	}
	int pipeA[2];
	if(pipe(pipeA)==-1) {
		err(2,"Cannot pipe(1)");
	}
	const pid_t pid1=fork();
	if(pid1==-1) {
		err(3,"Cannot fork(1)");
	}
	if(pid1==0) {
		//@chFind
		close(pipeA[0]);//Close unused read end
		dup2(pipeA[1],1);
		if(execlp("find","find",argv[1],"-type","f","-printf","%T@ %p\n",(char*)NULL)==-1) {
			err(4,"Cannot execute find");
		}
	}
	close(pipeA[1]);//Close unused write end
	int pipeB[2];
	if(pipe(pipeB)==-1) {
		err(2,"Cannot pipe(2)");
	}
	const pid_t pid2=fork();
	if(pid2==-1) {
		err(3,"Cannot fork(2)");
	}
	if(pid2==0) {
		//@chSort
		close(pipeB[0]);//Close unused read end
		dup2(pipeA[0],0);
		dup2(pipeB[1],1);
		if(execlp("sort","sort","-nk","1",(char*)NULL)==-1) {
			err(4,"Cannot execute sort");
		}
	}
	close(pipeB[1]);
	close(pipeA[0]);
	int pipeC[2];
	if(pipe(pipeC)==-1) {
		err(2,"Cannot pipe(3)");
	}
	const pid_t pid3=fork();
	if(pid3==-1) {
		err(3,"Cannot fork(3)");	
	}
	if(pid3==0) {
		//@chTail
		close(pipeC[0]);
		dup2(pipeB[0],0);
		dup2(pipeC[1],1);
		if(execlp("tail","tail","-n1",(char*)NULL)==-1) {
			err(4,"Cannot execute tail");
		}
	}
	close(pipeB[0]);
	close(pipeC[1]);
	dup2(pipeC[0],0);
	if(execlp("cut","cut","-d"," ","-f2",(char*)NULL)==-1) {
		err(4,"Cannot execute cut");
	}
	exit(0);//Unreachable
}

