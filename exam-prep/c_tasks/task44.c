#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

const size_t MAX_CMD_LEN=64;
void printPrompt() {
	write(1,"Enter command: ",15);
}
char* readCmd() {
	char* cmdName=(char*)malloc((MAX_CMD_LEN+1)*sizeof(char));
	size_t currentChar=0;
	char c;
	while(1) {
		int rBytes=read(0,&c,sizeof(c));
		if(rBytes==-1) {
			free(cmdName);
			err(1,"Cannot read from STDIN");
		}
		if(rBytes==0 || c==' ' || c=='\t' || c=='\n') {
			cmdName[currentChar]='\0';
			return cmdName;
		}
		cmdName[currentChar++]=c;
		if(currentChar==64) {
			free(cmdName);
			errx(2,"Error. Maximum cmd length is %d",(int)MAX_CMD_LEN);
		}
	}
}
int main() {
	while(1) {
		printPrompt();
		char* cmd=readCmd();
		const pid_t pid=fork();
		if(pid==-1) {
			free(cmd);
			err(3,"Cannot fork");
		}		
		if(pid==0) {
			//@child
			if(execlp(cmd,cmd,(char*)NULL)==-1) {
				err(4,"Cannot execute %s",cmd);
			}
		}
		if(wait(NULL)==-1) {
			free(cmd);
			err(5,"Could not wait for child");
		}
		free(cmd);
	}
	exit(0);
}
