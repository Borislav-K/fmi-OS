/* find . -type f -printf "%T@ %p\n" \
		| sort -nk 1 \
		| tail -n 1 \
		| cut -d ' ' -f 2 
*/
/* 
 1.Extract dir name from args
 find | sort | tail | cut
    pipeA   pipeB  pipeC
     1|0    1|0    1|0
*/
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argc, char** argv) {
	if(argc!=2) {
		errx(1,"Wrong number of arguments");
	}

	char* dir_name=argv[1];
	
	int a[2];
	if(pipe(a)<0) {
		err(2,"Cannot pipe");
	}
	pid_t pid1=fork();
	if(pid1<0) {
		err(3,"Cannot fork");
	}
	if(pid1==0) {
	//child-find
		close(a[0]);
		dup2(a[1],1);
		if(execlp("find","find",dir_name,"-type","f","-printf","%T@ %p\n",(char*)NULL)<0) {
			err(5,"Cannot execute find cmd");
		}	
	}
	close(a[1]);
	
	int b[2];
	if(pipe(b)<0) {
		err(2,"Cannot pipe");
	}
	pid_t pid2=fork();
	if(pid2<0) {
		err(3,"Cannot fork");
	}
	if(pid2==0) {
	//child - sort
		close(b[0]);
		if(dup2(a[0],0)<0) {
			err(4,"Cannot dup");
		}
		if(dup2(b[1],1)<0) {
			err(4,"Cannot dup");
		}
		if(execlp("sort","sort","-nk","1",(char*)NULL)<0) {
			err(5,"Cannot execute sort cmd");
		}
	}
	
	close(b[1]);
	int c[2];
	if(pipe(c)<0) {
		err(2,"Cannot pipe");
	}
	pid_t pid3=fork();
	if(pid3<0) {
		err(3,"Cannot fork");
	}
	if(pid3==0) {
	//child - tail
		if(dup2(b[0],0)<0) {
			err(4,"Cannot dup");
		}
		if(dup2(c[1],1)<0) {
			err(4,"Cannot dup");
		}
		close(c[0]);
		if(execlp("tail","tail","-n1",(char*)NULL)<0) {
			err(5,"Cannot execute tail cmd");
		}
	}
	close(c[1]);
	if(dup2(c[0],0)<0) {
		err(4,"Cannot dup");
	}
	if(execlp("cut","cut","-d ","-f2",(char*)NULL)<0) {
		err(5,"Cannot execute cut cmd");
	}
}
