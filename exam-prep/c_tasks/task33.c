/*
We have 256MB RAM=256*1024*1024=268 697 600 bytes
Max nums count is 100 000 000= 4*100 000 000 = 400 000 000 bytes max
Which means that we can store half the file in RAM
*/
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
int cmp(const void* n1, const void* n2) {
	return ( *((uint32_t*)n1) - *((uint32_t*)n2) );
}
int createTmpFile(int suffix,char* name_buf) { //Returns a file descriptor used to read and write from/to the file
	int fname_size=sprintf(name_buf,"tmp_%d_%d",getpid(),suffix);
	name_buf[fname_size]='\0';
	return open(name_buf,O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
}
int main(int argc, char* argv[]) {
	if(argc!=2) {
		errx(1,"Wrong number of args. Expected 1, got %d",argc-1);
	}
	struct stat st;
	if(stat(argv[1],&st)==-1) {
		err(2,"Cannot stat %s",argv[1]);
	}
	if(st.st_size%sizeof(uint32_t)!=0) {
		errx(3,"%s has wrong size",argv[1]);
	}

	uint64_t file_len=st.st_size;
	uint64_t numbersCount=file_len/sizeof(uint32_t);
	uint64_t firstHalfNumbers=numbersCount/2;
	uint64_t secondHalfNumbers=numbersCount-firstHalfNumbers;

	uint32_t* data=(uint32_t*)malloc(firstHalfNumbers*sizeof(uint32_t));
	
	int fd=open(argv[1],O_RDONLY);
	if(fd==-1) {
		const int olderrno=errno;
		free(data);
		errno=olderrno;
		err(4,"Could not open file %s",argv[1]);
	}
	int readBytes=read(fd,data,firstHalfNumbers*sizeof(uint32_t));
	if(readBytes==-1) {
		const int olderrno=errno;
		free(data);
		close(fd);
		errno=olderrno;
		err(5,"Could not read from file %s",argv[1]);
	}
	qsort(data,firstHalfNumbers,sizeof(uint32_t),cmp);
	
	char temp_name1[128];
	int temp_fd1=createTmpFile(1,temp_name1);
	if(temp_fd1==-1) {
		const int olderrno=errno;
		free(data);
		close(fd);
		errno=olderrno;
		err(6,"Cannot create a tmp file(1)");
	}
	int writtenBytes=write(temp_fd1,data,firstHalfNumbers*sizeof(uint32_t));
	if(writtenBytes==-1) {
		const int olderrno=errno;
		free(data);
		close(fd);
		close(temp_fd1);
		errno=olderrno;
		err(7,"Could not write to tmp file(1)");
	}
	free(data);
	uint32_t* rest=(uint32_t*)malloc(secondHalfNumbers*sizeof(uint32_t));
	readBytes=read(fd,rest,secondHalfNumbers*sizeof(uint32_t));
	if(readBytes==-1) {
		const int olderrno=errno;
		free(rest);
		close(fd);
		close(temp_fd1);
		errno=olderrno;
		err(5,"Could not read from file %s",argv[1]);
	}
	close(fd);//We're finished with reading the file argv[1], so we can safely close this fd
	qsort(rest,secondHalfNumbers,sizeof(uint32_t),cmp);
	
	char temp_name2[128];
	int temp_fd2=createTmpFile(2,temp_name2);
	if(temp_fd2==-1) {
		const int olderrno=errno;
		free(rest);
		close(temp_fd1);
		errno=olderrno;
		err(6,"Cannot create a tmp file(2)");
	}
	writtenBytes=write(temp_fd2,rest,secondHalfNumbers*sizeof(uint32_t));
	if(writtenBytes==-1) {
		const int olderrno=errno;
		free(rest);
		close(temp_fd1);
		close(temp_fd2);
		errno=olderrno;
		err(7,"Could not write to tmp file(2)");
	}
	free(rest);

	//Time to merge the files
	const char* output_file="pesho.txt";
	int fd_o=open(output_file,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(fd_o==-1) {
		const int olderrno=errno;
		close(temp_fd1);
		close(temp_fd2);
		errno=olderrno;
		err(10,"Could not create an output file");
	}
	if(lseek(temp_fd1,0,SEEK_SET)==-1) {
		const int olderrno=errno;
		close(fd_o);
		close(temp_fd1);
		close(temp_fd2);
		errno=olderrno;
		err(8,"Could not lseek tmp file(1)");
	}
	if(lseek(temp_fd2,0,SEEK_SET)==-1) {
		const int olderrno=errno;
		close(fd_o);
		close(temp_fd1);
		close(temp_fd2);
		errno=olderrno;
		err(8,"Could not lseek tmp file(2)");
	}
	
	uint64_t counter1=0,counter2=0;
	uint32_t n1;
	uint32_t n2;
	
	while(counter1<firstHalfNumbers&&counter2<secondHalfNumbers) {
		if(read(temp_fd1,&n1,sizeof(n1))==-1) {
			const int olderrno=errno;
			close(fd_o);
			close(temp_fd1);
			close(temp_fd2);
			errno=olderrno;
			err(11,"Could not read from tmp file(1)");
		}
		if(read(temp_fd2,&n2,sizeof(n2))==-1) {
			const int olderrno=errno;
			close(fd_o);
			close(temp_fd1);
			close(temp_fd2);
			errno=olderrno;
			err(11,"Could not read from tmp file(1)");
		}
		if(n1<n2) {
			counter1++;
			write(fd_o,&n1,sizeof(n1));
			if(lseek(temp_fd2,-1*sizeof(uint32_t),SEEK_CUR)==-1) {
				const int olderrno=errno;
				close(fd_o);
				close(temp_fd1);
				close(temp_fd2);
				errno=olderrno;
				err(12,"Could not lseek tmp file(1)");
			}
		} else {
			counter2++;
			write(fd_o,&n2,sizeof(n2));
			if(lseek(temp_fd1,-1*sizeof(uint32_t),SEEK_CUR)==-1) {
				const int olderrno=errno;
				close(fd_o);
				close(temp_fd1);
				close(temp_fd2);
				errno=olderrno;
				err(12,"Could not lseek tmp file(2)");
			}
		}
	}
	while(counter1<firstHalfNumbers) {
		int readBytes=read(temp_fd1,&n1,sizeof(n1));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd_o);
			close(temp_fd1);
			close(temp_fd2);
			errno=olderrno;
			err(11,"Could not read from tmp file(1)");
		}

		counter1++;
		if(write(fd_o,&n1,sizeof(n1))!=sizeof(n1)) {
			const int olderrno=errno;
			close(fd_o);
			close(temp_fd1);
			close(temp_fd2);
			errno=olderrno;
			err(12,"Could not write to %s",output_file);
		}
	}
	while(counter2<secondHalfNumbers) {
		int readBytes=read(temp_fd2,&n2,sizeof(n2));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd_o);
			close(temp_fd1);
			close(temp_fd2);
			errno=olderrno;
			err(11,"Could not read from tmp file(1)");
		}
		counter2++;
		if(write(fd_o,&n2,sizeof(n2))!=sizeof(n2)) {
			const int olderrno=errno;
			close(fd_o);
			close(temp_fd1);
			close(temp_fd2);
			errno=olderrno;
			err(12,"Could not write to %s",output_file);

		}
	}
	
	close(fd_o);
	close(temp_fd1);
	close(temp_fd2);
	unlink(temp_name1);
	unlink(temp_name2);
	exit(0);
}
