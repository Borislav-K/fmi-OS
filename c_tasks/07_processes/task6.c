#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>
int main(int argc, char* argv[]) {
	
	if(argc!=2) {
		errx(1,"Wrong number of arguments, expected 1 got %d",argc-1);
	}

	pid_t pid=fork();
	if(pid==-1) {
		err(2,"Could not fork");
	}
	if(pid==0) {
		//child
		if(execlp(argv[1],argv[1],(char*)NULL)==-1) {
			err(3,"Cannot execute %s",argv[1]);
		}
	}
	//parent
	int wstatus;
	pid_t pid1=wait(&wstatus);
	if(pid1==-1) {
		err(4,"Could not wait for child with pid %d",pid);
	}
	if(!WIFEXITED(wstatus)) {
		err(5,"Child did not terminate normally");
	}
	if((WEXITSTATUS(wstatus))==0) {
		char output_buffer[256];
		int output_length=sprintf(output_buffer,"%s\n",argv[1]);
		write(1,output_buffer,output_length);
		exit(0);
	}
	errx(6,"Child did not terminate with exit status 0");
}
