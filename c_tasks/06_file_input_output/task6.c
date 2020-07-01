//Реализирайте команда cat, работеща с произволен брой подадени входни параметри.
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
int main(int argc, char* argv[]) {
	if(argc==1) {
		errx(1,"%s requires at least 1 argument",argv[0]);
	}
	char buf[1024];
	for(int i=1;i<argc;i++) {
		int fd=open(argv[i],O_RDONLY);
		if(fd==-1) {
			warn("Cannot open file %s",argv[i]);
			close(fd);
			continue;
		}
		while(1) {
			int readBytes=read(fd,&buf,sizeof(buf));
			if(readBytes==-1) {
				warn("Cannot read from file %s",argv[i]);
				break;
			}
			if(readBytes==0) {
				break; //We're done
			}
			write(1,&buf,readBytes);
		}
		close(fd);
	}
	exit(0);
}
