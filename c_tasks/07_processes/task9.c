/*
    9. Да се напише програма на C, която която създава файл в текущата директория и генерира два процесa, 
    които записват низовете foo и bar в създадения файл.
    Програмата не гарантира последователното записване на низове.(Version 9.1)
    Променете програмата така, че да записва низовете последователно, като първия е foo.(Version 9.2)
*/
#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
char* trimStr(const char* str,int to) {
	int str_len=strlen(str);
	if(to>=str_len) {
		return (char*)NULL;
	}
	char* result=malloc((to+2)*sizeof(char));
	for(int i=0;i<=to;i++) {
		result[i]=str[i];
	}
	result[to+1]='\0';
	return result;
}
int main() {
	char filename_buf[64];
	int filename_size=sprintf(filename_buf,"created-by-%d",getpid());
	char* filename=trimStr(filename_buf,filename_size-1);
	int fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd==-1) {
		err(2,"Cannot open file %s",filename);
	}
	pid_t pid=fork();
	if(pid==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(3,"Could not fork");
	}
	if(pid==0) {
		//At child-foo
		int writtenBytes=write(fd,"foo",3);
		if(writtenBytes!=3) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(4,"Could not write \"foo\" to %s",filename);
		}
		close(fd);
		exit(0);
	}
	//At parent
	wait(NULL); // Wait for child-foo to terminate - comment this line to remove go to version 9.1
	pid=fork();
	if(pid==-1) {
		const int olderrno=errno;
		close(fd);
		errno=olderrno;
		err(3,"Could not fork");
	}
	if(pid==0) {
		//At child-bar
		int writtenBytes=write(fd,"bar",3);
		if(writtenBytes!=3) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(4,"Could not write \"bar\" to %s",filename);
		}
		close(fd);
		exit(0);
	}
	close(fd);
	exit(0);
}
