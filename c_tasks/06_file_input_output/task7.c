//Реализирайте команда cp, работеща с произволен брой подадени входни параметри.
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
void copyFile(int fd0, const char* dst) {//Relies that fd0 is already open
	int fd=open(dst,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd==-1) {
		warn("Cannot open file %s",dst);
		return;
	}
	char buf[1024];
	while(1) {
		int readBytes=read(fd0,buf,sizeof(buf));
		if(readBytes==-1) {
			warn("Cannot read from SRC file");
			close(fd);
			return;
		}
		if(readBytes==0) {
			close(fd);
			return;//Transfer complete
		}
		int writeBytes=write(fd,buf,readBytes);
		if(writeBytes==-1) {
			warn("Cannot write to file %s",dst);
			close(fd);
			return;
		}
	}
}
int main(int argc, char* argv[]) {
	if(argc<=2) {
		errx(1,"%s requires at least 2 arguments",argv[0]);
	}
	struct stat st;

	int fd0=open(argv[1],O_RDONLY);
	if(fd0==-1) {
		err(2,"Cannot open file %s for reading", argv[1]);	
	}
	for(int i=2;i<argc;i++) {
		int lseekCode=lseek(fd0,0,SEEK_SET);
		if(lseekCode==-1) {
			const int olderrno=errno;
			close(fd0);
			errno=olderrno;
			err(3,"Cannot lseek file %s",argv[1]);
		}
		if((stat(argv[i],&st)==-1) || (S_ISREG(st.st_mode))) {
			//Then the file is a regular file, or does not exist ==> Copy argv[0] into argv[i]
			copyFile(fd0,argv[i]);
		} else if(S_ISDIR(st.st_mode)) {
			//Then the file is a directory ==> Create a copy named argv[i]/argv[0] there
			char dest_buf[256];
			int nfn_size=sprintf(dest_buf,"%s/%s",argv[i],argv[1]);
			char* dst_name=malloc(nfn_size+1);
			memcpy(dst_name,dest_buf,nfn_size);
			dst_name[nfn_size]='\0';
			copyFile(fd0,dst_name);
			free(dst_name);
		}
		 else {
			warnx("File %s is not a valid file",argv[i]);
		}
	}
	close(fd0);
	exit(0);
}
