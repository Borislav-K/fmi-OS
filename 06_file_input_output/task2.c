/*
    Реализирайте команда head без опции (т.е. винаги да извежда
    на стандартния изход само първите 10 реда от съдържанието на
    файл подаден като първи параматър)
*/
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>	

int main(int argc, char* argv[]) {

	if(argc!=2) {
		errx(1,"Usage: %s <file>",argv[0]);
	}

	int fd;
	if ((fd=open(argv[1],O_RDONLY))==-1) {
		err(2,"Error: Cannot open file %s",argv[1]);
	}
	char c;
	
	int cur_line=1;
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
		write(1,&c,sizeof(c));
		if(c=='\n') {
			cur_line=cur_line+1;
		}
		if(cur_line==11) {
			break;
		}

	}
	close(fd);
	exit(0);
}
