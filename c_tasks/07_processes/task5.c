#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <stdio.h>
void periodicPrint(const char* string) {
	
	for(int i=1;i<=5;i++) {
		write(1,string,strlen(string));
		sleep(1);
	}
}
void printNum(int num) {
	char output[16];
	int n=sprintf(output,"%d\n",num);
	write(1,output,n);
}
int main() {


	pid_t pid=fork();
	if(pid==-1) {
		err(1,"Could not fork()");
	}
	if(pid==0) {
		//@child
		periodicPrint("child\n");
		exit(0);
	}
	//@parent
	int status;
	wait(&status);
	periodicPrint("parent\n");
	exit(0);
}
