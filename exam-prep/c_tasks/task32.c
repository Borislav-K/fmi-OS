#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
void verifySize(const char* path) {
	struct stat st;
	if(stat(path,&st)==-1) {
		err(1,"Could not stat %s",path);
	}
	if(st.st_size%sizeof(uint32_t)!=0) {
		errx(2,"Wrong file length for file %s",path);
	}
}
struct pair_t {
	uint32_t start;
	uint32_t len;
};
int main() {
	//File names are "f1", "f2", "f3"
	verifySize("f1");
	verifySize("f2");
	
	int fd1=open("f1",O_RDONLY);
	if(fd1==-1) {
		err(3,"Could not open f1");
	}
	int fd2=open("f2",O_RDONLY);
	if(fd2==-1) {
		const int olderrno=errno;
		close(fd1);
		errno=olderrno;
		err(3,"Could not open f2");
	}
	int fd3=open("f3",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd3==-1) {
		const int olderrno=errno;
		close(fd1);
		close(fd2);
		errno=olderrno;
		err(3,"Could not open f3");
	}
	
	struct pair_t buf;
	uint32_t num;
	while(1) {
		int rBytes=read(fd1,&buf,sizeof(buf));
		if(rBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(4,"Could not read from f1");
		}
		if(rBytes==0) {
			break;//We're done
		}
		if(lseek(fd2,SEEK_SET,buf.start*sizeof(uint32_t))==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(5,"Could not lseek to position %d in f2",buf.start);
		}
		while(buf.len>0) {
			buf.len--;
			int rBytes=read(fd2,&num,sizeof(num));
			if(rBytes==-1) {
				const int olderrno=errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno=olderrno;
				err(4,"Could not read from f1");
			}
			int wBytes=write(fd3,&num,sizeof(num));
			if(wBytes==-1) {
				const int olderrno=errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno=olderrno;
				err(6,"Could not write to f3");
			}
		}
	}
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
