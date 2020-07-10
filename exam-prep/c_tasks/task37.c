#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
struct triple_t {
	uint16_t offset;
	uint8_t old;
	uint8_t new;
};
void cp_file(int fd1,int fd2,const char* fname1,const char* fname2) {
	char buf[512];
	while(1) {
		int readBytes=read(fd1,buf,sizeof(buf));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			unlink(fname2);
			errno=olderrno;
			err(5,"Cannot read from file %s",fname1);
		}
		if(readBytes==0) {
			break;//We're done
		}
		int writtenBytes=write(fd2,buf,readBytes);
		if(writtenBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			unlink(fname2);
			errno=olderrno;
			err(6,"Cannot write to file %s",fname2);
		}
	}
}
int main(int argc, char* argv[]) {

	if(argc!=4) {
		errx(1,"Wrong number of arguments.Got %d, expected 3",argc-1);
	}

	struct stat st;
	if(stat(argv[1],&st)==-1) {
		err(2,"Cannot stat file %s",argv[1]);
	}
	if(st.st_size%(sizeof(uint16_t)+2*sizeof(uint8_t))!=0) {
		errx(3,"File %s is corrupted - should consist of 4 byte blocks",argv[1]);
	}

	int f1bin=open(argv[2],O_RDONLY);
	if(f1bin==-1) {
		err(4,"Cannot open file %s",argv[2]);
	}
	int f2bin=open(argv[3],O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(f2bin==-1) {
		const int olderrno=errno;
		close(f1bin);
		errno=olderrno;
		err(4,"Cannot open/create file %s",argv[3]);
	}
	//First copy the contents of f1 into f2
	cp_file(f1bin,f2bin,argv[2],argv[3]);
	close(f1bin);//No longer needed
	//Now let's start patching
	int pfd=open(argv[1],O_RDONLY);
	if(pfd==-1) {
		const int olderrno=errno;
		close(f2bin);
		unlink(argv[3]);
		errno=olderrno;
		err(7,"Cannot open file %s",argv[1]);
	}
	struct triple_t p;
	while(1) {
		int readBytes=read(pfd,&p,sizeof(p));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(f2bin);
			close(pfd);
			unlink(argv[3]);
			errno=olderrno;
			err(8,"Cannot read from file %s",argv[1]);
		}
		if(readBytes==0) {
			break;//We're done
		}
		if(readBytes!=sizeof(p)) {
			close(f2bin);
			close(pfd);
			unlink(argv[3]);
			errx(9,"File %s is corrupted",argv[1]);
		}
		if(lseek(f2bin,p.offset,SEEK_SET)!=p.offset) {
			const int olderrno=errno;
			close(f2bin);
			close(pfd);
			unlink(argv[3]);
			errno=olderrno;
			err(10,"Cannot lseek to pos %d in file %s",p.offset,argv[3]);
		}
		uint8_t c;
		readBytes=read(f2bin,&c,sizeof(c));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(f2bin);
			close(pfd);
			unlink(argv[3]);
			errno=olderrno;
			err(11,"Cannot read from file %s",argv[3]);
		}
		if(c!=p.old) {
			close(f2bin);
			close(pfd);
			unlink(argv[3]);
			errx(12,"Inconsistency between patch file and %s",argv[3]);
		}
		if(lseek(f2bin,-1,SEEK_CUR)==-1) {
			const int olderrno=errno;
			close(f2bin);
			close(pfd);
			unlink(argv[3]);
			errno=olderrno;
			err(10,"Cannot lseek %s",argv[3]);
		}
		if(write(f2bin,&p.new,sizeof(p.new))!=(ssize_t)sizeof(p.new)) {
			const int olderrno=errno;
			close(f2bin);
			close(pfd);
			unlink(argv[3]);
			errno=olderrno;
			err(14,"Cannot write to file %s",argv[3]);
		}
	}
	close(f2bin);
	close(pfd);
	exit(0);
}
