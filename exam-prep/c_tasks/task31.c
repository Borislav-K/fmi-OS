#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
void update_counts(uint64_t counts[256],char buf[],int buf_size,uint64_t overflows[256]) {
	for(int i=0;i<buf_size;i++) {
		counts[(uint64_t)buf[i]]++;
		if(counts[(uint64_t)buf[i]]==UINT64_MAX) {
			counts[(uint64_t)buf[i]]=0;
			overflows[(uint64_t)buf[i]]++;
		}
	}
}
void singleOverflow(int fd, char c, char* file_name, uint64_t times) {
	for(uint64_t i=1;i<=times;i++) {
		int writtenBytes=write(fd,&c,sizeof(c));
		if(writtenBytes!=1) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
				err(4,"Cannot write to file %s",file_name);
		}
	}
}
void writeN(int fd, char c, char* file_name,uint64_t times,uint64_t overflows) {
	while(overflows>0) {
		overflows--;
		singleOverflow(fd,c,file_name,UINT64_MAX);
	}
	singleOverflow(fd,c,file_name,times);
}
void nullify(uint64_t arr[], int size) {
	for(int i=0;i<size;i++) {
		arr[i]=0;
	}
}
int main(int argc, char* argv[]) {
	
	if(argc!=2) {
		errx(1,"Wrong number of arguments. Expected 1, got %d",argc-1);
	}

	int fd=open(argv[1],O_RDWR);
	if(fd==-1) {
		err(2,"Cannot open file %s",argv[1]);
	}
	
	uint64_t counts[256];
	uint64_t overflows[256];
	nullify(counts,256);
	nullify(overflows,256);
	char buf[1024];
	while(1) {
		int readBytes=read(fd,&buf,sizeof(buf));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(3,"Cannot read from file %s",argv[1]);
		}
		if(readBytes==0) {
			break;//We're done
		}
		update_counts(counts,buf,readBytes,overflows);
	}
	if(lseek(fd,SEEK_SET,0)==-1) {
		err(4,"Cannot lseek %s",argv[1]);
	}
	for(int i=0;i<256;i++) {
		writeN(fd,(char)i,argv[1],counts[i],overflows[i]);
	}
	close(fd);
	exit(0);
}
