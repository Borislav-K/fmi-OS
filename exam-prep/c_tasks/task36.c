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
void printNum(const uint16_t line_num) {
	char buf[64];
	int n=sprintf(buf,"\t%d ",line_num);
	write(1,buf,n);
}
void cat(const char* filename,uint16_t* line_num) {
	int fd=0;
	if(strcmp(filename,"-")!=0) {
		fd=open(filename,O_RDONLY);
		if(fd==-1) {
			err(1,"Cannot open file %s for reading",filename);
		}
	}
	const char* inputSource= (strcmp(filename,"-")==0) ? "STDIN" : filename;
	
	char c;
	bool shouldPrintNum=true;
	if((*line_num)>0) {//If lines are going to be numbered
		while(1) {
			ssize_t readBytes=read(fd,&c,sizeof(c));
			if(readBytes==-1) {
				const int olderrno=errno;
				close(fd);
				errno=olderrno;
				err(2,"Cannot read from %s",inputSource);
			}
			if(readBytes==0) {
				break;//We're done
			}
			if(shouldPrintNum) {
				printNum(*line_num);
				(*line_num)++;
				shouldPrintNum=false;
			}
			write(1,&c,sizeof(c));
			if(c=='\n') {
				shouldPrintNum=true;
			}
		}
	
	} else {//Ordinary cat
		while(1) {
			ssize_t readBytes=read(fd,&c,sizeof(c));
			if(readBytes==-1) {
				const int olderrno=errno;
				close(fd);
				errno=olderrno;
				err(2,"Cannot read from %s",inputSource);
			}
			if(readBytes==0) {
				break;
				//We're done
			}
			write(1,&c,sizeof(c));
		}
	}	

	
	if(fd!=0) {
		close(fd);
	}
}
int main(int argc, char* argv[]) {
	
	uint8_t currentArg=1;
	uint16_t currentLineNumber=0;
	if(argc>1&&strcmp(argv[1],"-n")==0) {
		currentLineNumber++;//If it stays at 0, lines will not be numbered
		currentArg++;
	}
	
	while(currentArg<argc) {
		cat(argv[currentArg],&currentLineNumber);
		currentArg++;
	}
	exit(0);
}
