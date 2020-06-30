/*
    Реализирайте команда swap, разменяща съдържанието на два файла, подадени като
    входни параметри. Приемаме, че двата файла имат еднакъв брой символи. Може да
    модифицирате решението, да работи и когато нямат еднакъв брой символи.
*/
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

	if(argc!=3) {
		errx(1,"Wrong number of arguments. Expected %d got %d",2,argc-1);
	}
	int fd1;
	int fd2;
	int fd3;

	if((fd1=open(argv[1],O_RDWR))==-1) {
		err(2,"Cannot open file %s",argv[1]);
	}
	if((fd2=open(argv[2],O_RDWR))==-1) {
		const int olderrno=errno;
		close(fd1);
		errno=olderrno;
		err(2,"Cannot open file %s",argv[1]);
	}
	if((fd3=open("my-tmp-file",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1) {
		const int olderrno=errno;
		close(fd1);
		close(fd2);
		errno=olderrno;
		err(3,"Cannot create temporary file");
	}



	char buf[1024];
	
	//copy argv[1]-->tmpfile
	while(1) {
		int rbytes=read(fd1,&buf,sizeof(buf));
		if(rbytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(4,"Cannot read from file %s",argv[1]);
		}
		if(rbytes==0) {
			break;//Finished reading
		}
		int wbytes=write(fd3,&buf,rbytes);
		if(wbytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(5,"Cannot write to temp file");
		}
	}
	if(lseek(fd1,0,SEEK_SET)==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(6,"Cannot lseek %s",argv[1]);
	}

	//copy argv[2]-->argv[1]
	while(1) {
		int rbytes=read(fd2,&buf,sizeof(buf));
		if(rbytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(7,"Cannot read from file %s",argv[2]);
		}
		if(rbytes==0) {
			break;//finished reading
		}
		int wbytes=write(fd1,&buf,rbytes);
		if(wbytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(8,"Cannot write to file %s",argv[1]);
		}
	}
	if(lseek(fd3,0,SEEK_SET)==-1) {
		const int olderrno=errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno=olderrno;
		err(6,"Cannot lseek tmp file");
	}
	if(lseek(fd2,0,SEEK_SET)==-1) {
		const int olderrno=errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno=olderrno;
		err(6,"Cannot lseek file %s",argv[2]);
	}

	//copy tmp-->argv[2]
	while(1) {
		int rbytes=read(fd3,&buf,sizeof(buf));
		if(rbytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(9,"Cannot read from temp file");
		}
		if(rbytes==0) {
			break;//Finished reading
		}
		int wbytes=write(fd2,&buf,rbytes);
		if(wbytes==-1) {
			const int olderrno=errno;
			close(fd1);
			close(fd2);
			close(fd3);
			errno=olderrno;
			err(10,"Cannot write to file %s",argv[2]);
		}
	}
	
	close(fd1);
	close(fd2);
	close(fd3);

	unlink("my-tmp-file");
}
