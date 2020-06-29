// Реализирайте команда wc, с един аргумент подаден като входен параметър
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
int main(int argc, char* argv[]) {

	if(argc!=2) {
		errx(1,"Wrong number of arguments. Got %d want %d",argc-1,1);
	}

	int lines=0;
	int chars=0;
	int words=0;

	int fd;
	if((fd=open(argv[1],O_RDONLY))==-1) {
		err(2,"Cannot open file %s for reading",argv[1]);
	}

	char c;
	while(1) {
		int bytes=read(fd,&c,sizeof(c));
		if(bytes==0) {
			break;
		}
		if(bytes==-1) {
			const int olderrno=errno;
			close(fd);
			errno=olderrno;
			err(3,"Cannot read from file %s",argv[1]);
		}
		chars=chars+1;
		if(c=='\n') {
			lines=lines+1;
			words=words+1;
		} else if (c==' ') {
			words=words+1;
		}
	}
	printf(" %d %d %d %s\n",lines,words,chars,argv[1]);
	close(fd);
}
