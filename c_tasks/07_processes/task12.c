/*
Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри) и име на файл в текущата директория. Ако файлът не съществува, го създава. Програмата изпълнява командите последователно, по реда на подаването им. Ако първата команда завърши успешно, програмата добавя нейното име към съдържанието на файла, подаден като команден параметър. Ако командата завърши неуспешно, програмата уведомява потребителя чрез подходящо съобщение.
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
bool terminatedSuccessfully(int wstatus) {
	return (WIFEXITED(wstatus)) && (WEXITSTATUS(wstatus)==0);
}
int main(int argc, char* argv[]) {
	
	if(argc!=4) {
		errx(1,"Wrong number of arguments. Expected 3, got %d",argc-1);
	}	

	int fd=open(argv[3],O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);
	if(fd==-1) {
		err(2,"Cannot open file %s",argv[3]);
	}
	pid_t pid=fork();
	if(pid==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(3,"Could not fork");
	}
	if(pid==0) {
		//@child1
		if(execlp(argv[1],argv[1],(char*)NULL)==-1) {
			err(4,"Could not exec %s",argv[1]);
		}
	}
	int wstatus;
	if(wait(&wstatus)==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(5,"Could not wait for child 1");
	}
	if(terminatedSuccessfully(wstatus)) {
		int w=write(fd,argv[1],strlen(argv[1]));
		if(w==-1) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(8,"Could not write to file %s",argv[3]);
		}
	} else {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		errx(6,"Command %s was not executed successfully",argv[1]);
	}

	pid=fork();
	if(pid==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(3,"Could not fork");
	}
	if(pid==0) {
		//@child2
		if(execlp(argv[2],argv[2],(char*)NULL)==-1) {
			err(4,"Could not exec %s",argv[2]);
		}
	}
	if(wait(&wstatus)==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(5,"Could not wait for child 2");
	}
	if(terminatedSuccessfully(wstatus)) {
		int w=write(fd,argv[2],strlen(argv[2]));
		if(w==-1) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(7,"Could not write to file %s",argv[3]);
		}
	} else {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		errx(9,"Child 2 did not terminate successfully");
	}
	close(fd);
	exit(0);
}
