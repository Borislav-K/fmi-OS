//Koпирайте файл /etc/passwd в текущата ви работна директория и променете разделителят на копирания файл от ":", на "?"

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
int main() {
	
	int fd1=open("/etc/passwd",O_RDONLY);
	if(fd1==-1) {
		err(1,"Cannot open /etc/passwd for reading");
	}

	int fd2=open("copy_passwd.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd2==-1) {
		const int olderrno=errno;
		close(fd1);
		errno=olderrno;
		err(2,"Cannot open/create file copy_passwd.txt in current directory");
	}
	
	char buf;
	while(true) {
		int readBytes=read(fd1,&buf,sizeof(buf));
		if(readBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			errno=olderrno;
			err(3,"Cannot read from /etc/passwd");
		}
		if(readBytes==0) {
			break;//We're done
		}
		if(buf==':') {
			buf='?';
		}
		int writtenBytes=write(fd2,&buf,sizeof(buf));
		if(writtenBytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			errno=olderrno;
			err(4,"Cannot write to file copy_passwd.txt");
		}
	}
	close(fd1);
	close(fd2);
	exit(0);
}
