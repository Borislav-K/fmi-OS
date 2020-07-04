/*
T1 - Да се напише програма на C, която приема аргумент - име на файл. Програмата да:

записва във файла 'fo'
създава child процес, който записва 'bar\n'
parent-а, след като изчака child процеса, записва 'o\n'
*/
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if(argc!=2) {
		errx(1,"Wrong number of arguments. Expected 1, got %d",argc-1);
	}

	int fd=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd==-1) {
		err(2,"Could not open file %s",argv[1]);
	}
	if(write(fd,"fo",2)!=2) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(3,"Could not write to file %s",argv[1]);
	}

	pid_t pid=fork();
	if(pid==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(4,"Could not fork");
	}
	if(pid==0) {
		//@Child
		if(write(fd,"bar\n",4)!=4) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(5,"Could not write to %s",argv[1]);
		}
		exit(0);
	}

	//Parent
	int wstatus;
	if(wait(&wstatus)==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(6,"Could not wait for child");
	}
	if(!WIFEXITED(wstatus) || (WEXITSTATUS(wstatus)!=0) ) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		errx(7,"Child did not terminate normally");
	}
	if(write(fd,"o\n",2)!=2) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(8,"Could not write to file %s",argv[1]);
	}
	
	close(fd);
	exit(0);

}
