/*
Да се напише програма на C, която изпълнява последователно подадените ѝ като параметри команди, като реализира следната функционалност постъпково:
main cmd1 ... cmdN Изпълнява всяка от командите в отделен дъщерен процес.
... при което се запазва броя на изпълнените команди, които са дали грешка и броя на завършилите успешно.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
int main(int argc, char* argv[]) {
	
	uint8_t successful_cmd=0;
	
	for(int i=1;i<argc;i++) {
		pid_t pid=fork();
		if(pid==-1) {
			err(1,"Could not fork");
		}
		if(pid==0) {
			//@child #i
			if(execlp(argv[i],argv[i],(char*)NULL)==-1) {
				err(2,"Could not execute %s",argv[i]);
			}
		}
		//At parent
		int wstatus;
		if(wait(&wstatus)==-1) {
			warn("Could not wait for child #%d, counting it for a failure",i);
		}
		if((WIFEXITED(wstatus)) && (WEXITSTATUS(wstatus)==0)) {
			successful_cmd++;
		}
	}
	char output_buf[256];
	int output_size=sprintf(output_buf,"Successful cmds: %d, Failed cmds: %d\n",successful_cmd,argc-1-successful_cmd);
	write(1,output_buf,output_size);
	exit(0);
}

