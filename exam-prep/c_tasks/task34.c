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
	uint8_t len;
	uint8_t res;
};
bool startsWithCaps(const char* str) {
	return str[0]>='A' && str[0]<='Z';
}
int main(int argc, char* argv[]) {
	
	if(argc!=5) {
		errx(1,"Wrong number of arguments.Expected 4, got %d",argc-1);
	}
	
	struct stat st;
	if(stat(argv[2],&st)==-1) {
		err(2,"Cannot stat %s",argv[2]);
	}
	if(st.st_size%(sizeof(uint32_t)+sizeof(uint8_t)*2)!=0) {
		errx(3,"Wrong file format for %s",argv[2]);
	}

	int f1dat=open(argv[1],O_RDONLY);
	if(f1dat==-1) {
		err(4,"Could not open file %s",argv[1]);
	}
	int f1idx=open(argv[2],O_RDONLY);
	if(f1idx==-1) {
		const int olderrno=errno;
		close(f1dat);
		errno=olderrno;
		err(4,"Could not open file %s",argv[2]);
	}

	int f2dat=open(argv[3],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(f2dat==-1) {
		const int olderrno=errno;
		close(f1dat);
		close(f1idx);
		errno=olderrno;
		err(4,"Could not open file %s",argv[3]);
	}
	int f2idx=open(argv[4],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(f2idx==-1) {
		const int olderrno=errno;
		close(f1dat);
		close(f1idx);
		close(f2dat);
		unlink(argv[3]);
		errno=olderrno;
		err(4,"Could not open file %s",argv[4]);
	}

	struct triple_t t1;
	struct triple_t t2;
	uint16_t current_pos=0;
	while(1) {
		int readBytes=read(f1idx,&t1,sizeof(t1));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(f1dat);
			close(f1idx);
			close(f2dat);
			close(f2idx);
			unlink(argv[3]);
			unlink(argv[4]);
			errno=olderrno;
			err(5,"Could not read from file %s",argv[2]);
		}
		if(readBytes==0) {
			break;//We're done
		}	
		t2.len=t1.len;
		t2.res=t1.res;
		t2.offset=current_pos;
		current_pos+=t1.len;

		if(lseek(f1dat,t1.offset,SEEK_SET)==-1) {
			const int olderrno=errno;
			close(f1dat);
			close(f1idx);
			close(f2dat);
			close(f2idx);
			unlink(argv[3]);
			unlink(argv[4]);
			errno=olderrno;
			err(6,"Could not lseek to position %d in file %s",t1.offset,argv[1]);
		}
		char* buf=(char*)malloc((t1.len+1)*sizeof(char));
		buf[t1.len]='\0';
		if(read(f1dat,buf,t1.len*sizeof(char))!=(ssize_t)t1.len) {
			const int olderrno=errno;
			close(f1dat);
			close(f1idx);
			close(f2dat);
			close(f2idx);
			unlink(argv[3]);
			unlink(argv[4]);
			free(buf);
			errno=olderrno;
			err(7,"Could not read from file %s",argv[1]);
		}
		if(!startsWithCaps(buf)) {
			continue;
		}

		if(write(f2dat,buf,strlen(buf))!=(ssize_t)strlen(buf)) {
			const int olderrno=errno;
			close(f1dat);
			close(f1idx);
			close(f2dat);
			close(f2idx);
			unlink(argv[3]);
			unlink(argv[4]);
			free(buf);
			errno=olderrno;
			err(8,"Could not write to file %s",argv[4]);
		}
		free(buf);		
		if(write(f2idx,&t2,sizeof(t2))!=(ssize_t)sizeof(t2)) {
			const int olderrno=errno;
			close(f1dat);
			close(f1idx);
			close(f2dat);
			close(f2idx);
			unlink(argv[3]);
			unlink(argv[4]);
			errno=olderrno;
			err(7,"Could not write to file %s",argv[4]);
		}
	}

	close(f1dat);
	close(f1idx);
	close(f2dat);
	close(f2idx);
	exit(0);
}

