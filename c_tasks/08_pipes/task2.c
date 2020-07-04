/*
T2 - Напишете програма на C, която демонстрира комуникация през pipe между parent и child процеси. Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, който да го отпечатва на стандартния изход.
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
	
	int fd[2];
	if(pipe(fd)==-1) {
		err(2,"Could not create pipe");
	}
	
	const pid_t pid=fork();
	if(pid==-1) {
		const int olderrno=errno;
		close(fd[0]);
		close(fd[1]);	
		errno=olderrno;
		err(3,"Could not fork");
	}
	if(pid==0) {
		//@Child
		char buf[512];
		close(fd[1]);
		int readBytes=read(fd[0],buf,sizeof(buf));
		if(readBytes==-1) {
			err(4,"Could not read from pipe");
		}
		close(fd[0]);
		write(1,buf,readBytes);
		exit(0);
	}
	//@Parent
	close(fd[0]);
	int writtenBytes=write(fd[1],argv[1],strlen(argv[1]));
	if(writtenBytes==-1) {
		const int olderrno=errno;
		close(fd[1]);
		errno=olderrno;
		err(5,"Could not write to pipe");
	}

	close(fd[1]);
	if(wait(NULL)==-1) {//for clear prompt
		err(6,"Could not wait for child");
	}
	write(1,"\n",1);
	exit(0);
}
