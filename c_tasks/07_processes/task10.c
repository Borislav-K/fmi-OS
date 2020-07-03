/*
Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри). Изпълнява първата. Ако тя е завършила успешно изпълнява втората. Ако не, завършва с код 42.
*/
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char* argv[]) {
	if(argc!=3) {
		errx(1,"Wrong number of arguments. Expected 2, got %d",argc-1);
	}
	
	pid_t pid=fork();
	if(pid==-1) {
		err(2,"Could not fork");
	}
	if(pid==0) {
		//At child1
		if(execlp(argv[1],argv[1],(char*)NULL)==-1) {
			err(3,"Could not execute %s",argv[1]);
		}
	}
	//Parent
	int wstatus;
	pid_t _pid=wait(&wstatus);
	if(pid==-1) {
		err(42,"Could not wait for child to finish execution");
	}
	if(!WIFEXITED(wstatus)) {
		errx(42,"Child did not terminate successfully");
	}
	if(WEXITSTATUS(wstatus)!=0) {
		errx(42,"Child did not exit with status code 0");
	}
	pid=fork();
	if(pid==-1) {
		err(2,"Could not fork");
	}
	if(pid==0) {
		//At child2
		if(execlp(argv[2],argv[2],(char*)NULL)==-1) {
			err(3,"Could not execute %s",argv[2]);
		}	
	}
	_pid=wait(&wstatus);
	if(_pid==-1) {
		err(4,"Could not wait for second child");
	}
	if(!WIFEXITED(wstatus)) {
		errx(5,"Child 2 did not terminate successfully");
	}
	if(WEXITSTATUS(wstatus)!=0) {
		errx(6,"Child 2 did not terminate with status code 0");
	}
	exit(0);
}
