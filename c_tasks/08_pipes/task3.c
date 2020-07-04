/*
T3 - Напишете програма на C, която демонстрира употребата на dup/dup2 между parent и child процеси. Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, където той да може да се чете от stdin. Child процесът да изпълняваwc -c.
*/
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
	
	if(argc!=2) {
		errx(1,"Wrong number of arguments. Expected 1, got %d",argc-1);
	}

	int pipefd[2];
	if(pipe(pipefd)==-1) {
		err(2,"Could not pipe");
	}
	
	const pid_t pid=fork();
	if(pid==-1) {
		err(3,"Could not fork");
	}
	if(pid==0) {
		//@Child
		close(pipefd[1]);
		if(dup2(pipefd[0],0)==-1) {
			err(4,"Cannot dup2 in child");
		}
		if(execlp("wc","wc","-c",(char*)NULL)==-1) {
			err(5,"Cannot execlp wc");
		}
	}
	//@Parent
	close(pipefd[0]);
	write(pipefd[1],argv[1],strlen(argv[1]));
	close(pipefd[1]);
	wait(NULL);//Wait for child to terminate -- for clearer prompt(not mandatory)
	exit(0);
}
