//Реализирайте команда cp, работеща с два аргумента, подадени като входни параметри.
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
int main(int argc, char* argv[]) {
	if(argc!=3) {
		errx(1,"Wrong number of arguments. Expected 2, got %d",argc-1);
	}

	int fd1;
	int fd2;
	if((fd1=open(argv[1],O_RDONLY))==-1) {
		err(2,"Cannot open file %s",argv[0]);
	}
	if((fd2=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR))==-1) {
		const int olderrno=errno;
		close(fd1);
		errno=olderrno;
		err(2,"Cannot open file %s",argv[1]);
	}
	while(1) {
		char buf[1024];
		int readBytes=read(fd1,buf,sizeof(buf));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			errno=olderrno;
			err(3,"Cannot read from file %s",argv[1]);
		}
		if(readBytes==0) {
			break; //We're done
		}
		int writtenBytes=write(fd2, buf,readBytes);
		if(writtenBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			errno=olderrno;
			err(4,"Cannot write to file %s",argv[2]);
		}
	}

	close(fd1);
	close(fd2);
	exit(0);
}
