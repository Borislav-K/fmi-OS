/*
    8. Да се напише програма на С, която получава като параметър име на файл. 
    Създава детински процес, който записва стринга foobar във файла (ако не съществува, го създава, 
    в противен случай го занулява), след което процеса родител прочита записаното във файла съдържание 
    и го извежда на стандартния изход, добавяйки по един интервал между всеки два символа.
*/
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
	
	if(argc!=2) {
		errx(1,"Wrong number of arguments. Expected 1, got %d",argc-1);
	}
	
	pid_t pid=fork();
	if(pid==-1) {
		err(2,"Could not fork");
	}
	if(pid==0) {
		//At child
		int fd=open(argv[1],O_CREAT|O_TRUNC|O_RDWR,S_IRUSR|S_IWUSR);
		if(fd==-1) {
			err(3,"Could not open file %s",argv[1]);
		}
		int writtenBytes=write(fd,"foobar",6);
		if(writtenBytes==-1) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(4,"Cannot write to file %s",argv[1]);
		}
		close(fd);
		exit(0);	
	}
	//at parent
	int wstatus;
	wait(&wstatus);//In order to have clear prompt
	int fd=open(argv[1],O_RDONLY);
	if(fd==-1) {
		err(5,"Cannot open file %s for reading",argv[1]);
	}
	while(true) {
		char buf[2];
		int readBytes=read(fd,buf,sizeof(buf));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(5,"Cannot read from file %s",argv[1]);
		}
		if(readBytes==0) {
			break;//Whole file is read
		}
		//The other options are 1 or 2 bytes
		if(readBytes==2) {
			write(1,buf,sizeof(buf));
			write(1," ",1);
			continue;
		}
		//At this point, we're sure that 1 byte is read only and that this is the last byte
		write(1,buf,1);
	}
	write(1,"\n",1); // For a clearer prompt
	close(fd);
	exit(0);
}
