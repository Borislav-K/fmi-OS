#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
/*
	cat argv[1] | sort
	   child     parent

*/
int main(int argc, char* argv[]) {

	if(argc!=2) {
		errx(1,"Wrong number of args. Got %d, expected 1",argc-1);
	}
	int pipefd[2];
	if(pipe(pipefd)==-1) {
		err(2,"Could not create pipe");
	}
	const pid_t pid=fork();
	if(pid==-1) {
		const int olderrno=errno;
		close(pipefd[0]);
		close(pipefd[1]);
		errno=olderrno;
		err(3,"Could not fork");	
	}
	if(pid==0) {
		//@child
		close(pipefd[0]);
		if(dup2(pipefd[1],1)==-1) {
			const int olderrno=errno;
			close(pipefd[1]);
			errno=olderrno;
			err(4,"Could not dup2 in child");
		}
		if(execlp("cat","cat",argv[1],(char*)NULL)==-1) {
			const int olderrno=errno;
			close(pipefd[1]);
			errno=olderrno;
			err(5,"Could not execute cat");
		}
	}
	//@Parent
	close(pipefd[1]);
	if(dup2(pipefd[0],0)==-1) {
		err(4,"Could not dup2 in parent");
	}
	if(execlp("sort","sort",(char*)NULL)==-1) {
		const int olderrno=errno;
		close(pipefd[0]);
		errno=olderrno;
		err(5,"Could not execute sort");
	}
	
	exit(0);
}
