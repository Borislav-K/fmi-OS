/* Напишете програма, която приема точно 2 аргумента. Първият може да бъде само --min, --max или --print (вижте man 3 strcmp). Вторият аргумент е двоичен файл, в който има записани цели неотрицателни двубайтови числа (uint16_t - вижте man stdint.h). Ако първият аргумент е:

--min - програмата отпечатва кое е най-малкото число в двоичния файл.
--max - програмата отпечатва кое е най-голямото число в двоичния файл.
--print - програмата отпечатва на нов ред всяко число.
Използвайте двоичния файл binary/dump или генерирайте сами такъв (правилно подравнен).
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
int main(int argc, char* argv[]) {
	
	if(argc!=3) {
		errx(1,"Expected 2 arguments, got %d",argc-1);
	}
	if( strcmp(argv[1],"--min")!=0 && strcmp(argv[1],"--max")!=0 && strcmp(argv[1],"--print")!=0) {
		errx(2,"Error, first argument must be --min, --max, or --print");
	}
	
	struct stat st;
	if( (stat(argv[2],&st))<0) {
		err(3,"Cannot stat file %s",argv[2]);
	}
	if(st.st_size%sizeof(uint16_t)!=0) {
		err(4,"File %s has wrong format",argv[2]);
	}
	if(st.st_size==0) {
		err(4,"File %s is empty",argv[2]);
	}
	int fd=open(argv[2],O_RDONLY);
	if(fd==-1) {
		err(5,"Cannot open file %s",argv[2]);
	}
	if(strcmp(argv[1],"--min")==0) {
		uint16_t min_num=INT16_MAX;
		uint16_t buf;
		while(1) {
			int readBytes=read(fd,&buf,sizeof(buf));
			if(readBytes==-1) {
				const int olderrno=errno;
				close(fd);
				errno=olderrno;
				err(6,"Cannot read from file %s",argv[2]);
			}
			if(readBytes==0) {
				break; //We're done
			}
			if(buf<min_num) {
				min_num=buf;
			}
		}
		char output[128];
		int output_size=sprintf(output,"Minimum number: %d\n",min_num);
		write(1,&output,output_size);
	} else if(strcmp(argv[1],"--max")==0) {
		uint16_t max_num=INT16_MIN;
		uint16_t buf;
		while(1) {
			int readBytes=read(fd,&buf,sizeof(buf));
			if(readBytes==-1) {
				const int olderrno=errno;
				close(fd);
				errno=olderrno;
				err(6,"Cannot read from file %s",argv[2]);
			}
			if(readBytes==0) {
				break;//We're done
			}
			if(buf>max_num) {
				max_num=buf;
			}
		}
		char output[128];
		int output_size=sprintf(output,"Maximum number: %d\n",max_num);
		write(1,&output,output_size);
	} else {
		uint16_t buf;
		char output[128];
		while(1) {
			int readBytes=read(fd,&buf,sizeof(buf));
			if(readBytes==-1) {
				const int olderrno=errno;
				close(fd);
				errno=olderrno;
				err(6,"Cannot read from file %s",argv[2]);
			}
			if(readBytes==0) {
				break; //We're done
			}
			int output_size=sprintf(output,"%d\n",buf);
			write(1,&output,output_size);
		}
	}
	close(fd);
	exit(0);
}
