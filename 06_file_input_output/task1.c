//Да се копира съдържанието на argv[1] в argv[2]
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>
int main(int argc, char* argv[]) {
	
	if(argc!=3) {
		errx(1,"Wrong num of arguments. Expected %d, got %d",2,argc-1);	
	}
	int fd1;
	int fd2;
	
	if ((fd1=open(argv[1],O_RDONLY))==-1) {
		err(2,"File %s failed to open in read only mode",argv[1]);
	}

	if ((fd2=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))==-1) {
		const int olderrno=errno;
		close(fd1);
		errno=olderrno;
		err(2,"File %s failed to open in write only mode",argv[2]);
	}
	char buf;
	while(1) {
		int bytes=read(fd1,&buf,1);
		if(bytes==0) {
			break;
		}
		if(bytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			errno=olderrno;
			err(3,"Cannot read from file %s",argv[1]);
		}
		bytes=write(fd2,&buf,sizeof(buf));
		if(bytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			errno=olderrno;
			err(4,"Cannot write to file %s",argv[1]);
		}
	}
	close(fd1);
	close(fd2);

}
