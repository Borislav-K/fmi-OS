#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
struct triple_t {
	uint16_t offset;
	uint8_t old;
	uint8_t new;
};
int main(int argc, char* argv[]) {
	
	if(argc!=4) {
		errx(1,"Wrong number of arguments. Got %d, expected 3",argc-1);
	}
	
	struct stat st1,st2;
	if(stat(argv[1],&st1)==-1) {
		err(2,"Cannot stat %s",argv[1]);
	}	
	if(stat(argv[2],&st2)==-1) {
		err(2,"Cannot stat %s",argv[2]);
	}
	if(st1.st_size!=st2.st_size) {
		errx(3,"Error: Input files differ in size!");
	}
	int fd1=open(argv[1],O_RDONLY);
	if(fd1==-1) {
		err(4,"Cannot open file %s",argv[1]);
	}
	int fd2=open(argv[2],O_RDONLY);
	if(fd2==-1) {
		const int olderrno=errno;
		close(fd1);
		errno=olderrno;
		err(4,"Cannot open file %s",argv[2]);
	}

	int fd3=open(argv[3],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd3==-1) {
		const int olderrno=errno;
		close(fd1);
		close(fd2);
		errno=olderrno;
		err(4,"Cannot open file %s",argv[3]);
	}
	uint8_t b1;
	uint8_t b2;
	uint16_t currentOffset=0;
	while(1) {
		int readBytes=read(fd1,&b1,sizeof(b1));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(5,"Cannot read from file %s",argv[1]);
		}
		if(readBytes==0) {
			break;//We're done
		}
		readBytes=read(fd2,&b2,sizeof(b2));
		if(readBytes!=1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(5,"Cannot read from file %s",argv[2]);
		}
		if(b1!=b2) {
			struct triple_t patchStr;
			patchStr.offset=currentOffset;
			patchStr.old=b1;
			patchStr.new=b2;
			if(write(fd3,&patchStr,sizeof(patchStr))!=(ssize_t)sizeof(patchStr)) {
				const int olderrno=errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno=olderrno;
				err(5,"Cannot write to file %s",argv[3]);
			}
		}
		currentOffset++;
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
