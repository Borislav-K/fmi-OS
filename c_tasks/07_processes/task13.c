/*
Да се напише програма на C, която получава като командни параметри две команди (без параметри). Изпълнява ги едновременно и извежда на стандартния изход номера на процеса на първата завършила успешно. Ако нито една не завърши успешно извежда -1.
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
bool terminatedSuccessfully(int wstatus) {
	return (WIFEXITED(wstatus)) && (WEXITSTATUS(wstatus)==0);
}
int main(int argc, char* argv[]) {

	if(argc!=3) {
		errx(1,"Wrong number of arguments. Expected 2, got %d",argc-1);
	}
	pid_t pid1=fork();
	if(pid1==-1) {
		err(2,"Could not fork");
	}
	if(pid1==0) {
		//@Child1
		if(execlp(argv[1],argv[1],(char*)NULL)==-1) {
			err(3,"Could not execute %s",argv[1]);
		}
	}
	//@Parent
	pid_t pid2=fork();
	if(pid2==-1) {
		err(2,"Could not fork");
	}
	if(pid2==0) {
		//@Child2
		if(execlp(argv[2],argv[2],(char*)NULL)==-1) {
			err(3,"Could not execute %s",argv[2]);
		}
	}
	//@Parent
	char output_buf[128];
	int output_size;
	int wstatus;
	int firstPid;

	bool winnerAnnounced=false;
	if((firstPid=wait(&wstatus))==-1) {
		err(4,"Could not wait for the first child");
	}
	if(terminatedSuccessfully(wstatus)) {
		output_size=sprintf(output_buf,"Child with PID %d terminated successfully first\n",firstPid);
		write(1,output_buf,output_size);
		winnerAnnounced=true;
	} 
	int secondPid;
	if((secondPid=wait(&wstatus))==-1) {//Wait for second child in any case (for a clearer prompt)
		err(4,"Could not wait for the second child");
	}
	if(winnerAnnounced) {
		exit(0);
	}
	if(terminatedSuccessfully(wstatus)) {//Winner not announced yet
		output_size=sprintf(output_buf,"Child with PID %d terminated successfully first\n",secondPid);
		write(1,output_buf,output_size);
		exit(0);
	}
	write(1,"-1\n",3);
	exit(5);
}
