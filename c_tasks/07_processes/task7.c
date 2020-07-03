/*
    7. Да се напише програма на С, която получава като параметри три команди (без параметри), 
    изпълнява ги последователно, като изчаква края на всяка и извежда на стандартния изход 
    номера на завършилия процес, както и неговия код на завършване.
*/
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
void exec_and_wait(const char* cmd) {
	pid_t pid=fork();
	if(pid==-1) {
		err(1,"Cannot fork");
	}
	if(pid==0) {
		//@child
		if(execlp(cmd,cmd,(char*)NULL)==-1) {
			err(2,"Cannot execute %s",cmd);
		}
	}
	//at parent
	int wstatus;
	pid=wait(&wstatus);
	if(pid==-1) {
		err(3,"Could not wait for child with pid %d",pid);
	}
	if(!WIFEXITED(wstatus)) {
		err(4,"Child with pid %d did not terminate properly",pid);
	}
	//At this point child exited successfully
	char output_buf[256];
	int output_size=sprintf(output_buf,"Child with pid %d exited with code %d\n",pid,WEXITSTATUS(wstatus));
	write(1,output_buf,output_size);
	
}
int main(int argc, char* argv[]) {

	if(argc!=4) {
		errx(1,"Wrong number of arguments. Expected 3, got %d",argc-1);
	}
	exec_and_wait(argv[1]);
	exec_and_wait(argv[2]);
	exec_and_wait(argv[3]);
	exit(0);
}
