#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

void printUsage(char* name) {
	errx(1,"Error. Usage: %s <digit> <cmd> [args...]",name);
}
void validateArgs(int argc, char* argv[]) {
	if(argc<3) {
		printUsage(argv[0]);
	}
	if(strlen(argv[1])!=1 || argv[1][0]<'1' || argv[1][0]>'9') {
		printUsage(argv[0]);
	}		
}
char** fetchArgs(int argc, char* argv[]) {//The arguments that are going to be passed to execvp
	const size_t argsCount=argc-2;
	char** args=(char**)malloc((argsCount+1)*sizeof(char*));
	for(size_t i=0;i<argsCount;i++) {
		args[i]=(char*)malloc((strlen(argv[i+2])+1)*sizeof(char));
		strcpy(args[i],argv[i+2]);
	}
	args[argsCount]=(char*)NULL;
	return args;
}
void freeArgs(int argCount,char** args) {
	for(int i=0;i<argCount;i++) {
		free(args[i]);
	}
	free(args);
}
struct execResult_t {
	time_t start_time;
	time_t finish_time;
	uint8_t exit_code;
};
bool stopConditionSatisfied(struct execResult_t result, uint8_t time_limit) {
	uint8_t executionTime=(uint8_t)(result.finish_time-result.start_time);
	return ((executionTime<time_limit) && (result.exit_code!=0));
}
int main(int argc, char* argv[]) {
	validateArgs(argc,argv);

 	uint8_t time_limit=argv[1][0]-'0';
	char** cmd_args=fetchArgs(argc,argv);
	
	int fd=open("run.log",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd==-1) {
		freeArgs(argc-1,cmd_args);
		err(2,"Cannot open a fd to run.log");
	}

	struct execResult_t result;
	bool stopConditionSatisfiedLastTime=false;
	while(1) {
		if((result.start_time=time(NULL))==-1) {
			const int olderrno=errno;
			close(fd);
			freeArgs(argc-1,cmd_args);
			errno=olderrno;
			err(3,"Cannot get time");
		}
		const pid_t pid=fork();
		if(pid==-1) {
			const int olderrno=errno;
			close(fd);
			freeArgs(argc-1,cmd_args);
			errno=olderrno;
			err(4,"Cannot fork");
		}
		if(pid==0) {
			//@child
			if(execvp(cmd_args[0],cmd_args)==-1) {
				const int olderrno=errno;
				close(fd);
				freeArgs(argc-1,cmd_args);
				errno=olderrno;
				err(5,"Cannot execute %s",cmd_args[0]);
			}
		}
		//@Parent
		int wstatus;
		if(wait(&wstatus)==-1) {
			const int olderrno=errno;
			close(fd);
			freeArgs(argc-1,cmd_args);
			errno=olderrno;
			err(6,"Cannot wait for child");
		}
		if((result.finish_time=time(NULL))==-1) {
			const int olderrno=errno;
			close(fd);
			freeArgs(argc-1,cmd_args);
			errno=olderrno;
			err(3,"Cannot get time");
		}
		result.exit_code=0;
		if(WIFSIGNALED(wstatus)) {
			result.exit_code=129;
		} else if(WIFEXITED(wstatus)) {
			result.exit_code=WEXITSTATUS(wstatus);
		}
		
		char output_buf[128];	
		int n=sprintf(output_buf,"%lu %lu %u\n",result.start_time,result.finish_time,result.exit_code);
		if(write(fd,output_buf,n)!=n) {
			const int olderrno=errno;
			close(fd);
			freeArgs(argc-1,cmd_args);
			errno=olderrno;
			err(6,"Could not write to the log file");
		}
		bool stopCond=stopConditionSatisfied(result,time_limit);
		if(stopCond&&stopConditionSatisfiedLastTime) {
			break;//We're done
		}
		stopConditionSatisfiedLastTime=stopCond;
	}
	close(fd);
	freeArgs(argc-1,cmd_args);
	exit(0);
}
