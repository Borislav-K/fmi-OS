//TODO fix this task - does not work properly
/*
 Напишете програма, която приема като аргументи имена
на две команди, и след това прочита редове от STDIN.
Четните редове се пращат на STDIN на първата команда,
а нечетните - на STDIN на втората. Програмата излиза,
когато и двете команди излязат.
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char* argv[]) {

	if(argc!=3) {
		errx(1,"Wrong number of arguments. Expected 2, got %d",argc-1);
	}
	
	int pipe1fd[2];//Used to send even lines to first child
	if(pipe(pipe1fd)==-1) {
		err(2,"Cannot create pipe1");
	}
	int pipe2fd[2];//Used to send uneven lines to second child
	if(pipe(pipe2fd)==-1) {
		err(2,"Cannot create pipe2");
	}
	const pid_t pid1=fork();
	if(pid1==-1) {
		err(3,"Could not fork(cmd1)");
	}
	if(pid1==0) {
		//@Child1
		close(pipe2fd[0]);
		close(pipe2fd[1]);
		close(pipe1fd[1]);
		dup2(pipe1fd[0],0);
		if(execlp(argv[1],argv[1],(char*)NULL)==-1) {
			err(4,"Could not execute %s",argv[1]);
		}
	}
	//@Parent
	close(pipe1fd[0]);
	const pid_t pid2=fork();
	if(pid2==-1) {
		err(3,"Could not fork(cmd2)");
	}
	if(pid2==0) {
		//@Child2
		close(pipe1fd[1]);
		close(pipe2fd[1]);
		dup2(pipe2fd[0],0);
		if(execlp(argv[2],argv[2],(char*)NULL)==-1) {
			err(4,"Could not execute %s",argv[1]);
		}
	}
	//@Parent
	char buf;
	bool even_line=true;
	while(read(0,&buf,sizeof(buf))>0) {
		if(buf=='\n') {
			even_line=!even_line;
			//continue; --> Uncomment in order not to send the \n to the commands
		}
		if(even_line&&write(pipe1fd[1],&buf,sizeof(buf)!=1)) {
			err(5,"Could not write to the pipe(1)");
		}
		if(!even_line&&write(pipe2fd[1],&buf,sizeof(buf)!=1)) {
			err(5,"Could not write to the pipe(2)");
		}
	}
	close(pipe1fd[1]);
	close(pipe1fd[2]);
	if(wait(NULL)==-1) {
		err(6,"Could not wait for child(1)");
	}
	if(wait(NULL)==-1) {
		err(6,"Could not wait for child(2)");
	}
	
	exit(0);
}
