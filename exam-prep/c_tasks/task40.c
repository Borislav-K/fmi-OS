//-c <num> --> 2 args
//-c <num-num> --> 2 args
//-d <str> -f (<num>|<num-num>) --> 4 args
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
bool isDigit(const char c) {
	return c>='1'&&c<='9';
}
void validateArgs(int argc, char* argv[]) {
	if(argc!=3&&argc!=5) {
		errx(1,"Error. Arguments can be either 2 or 4, got %d",argc-1);
	}
	if(strcmp(argv[1],"-c")==0) {
		if(strlen(argv[2])==1) {
			if(!isDigit(argv[2][0])) {
				errx(1,"Error. When using option -c the next argument is either a digit or <num>-<num>");
			}
		} else if(strlen(argv[2])==3) {
			if(!isDigit(argv[2][0]) || !isDigit(argv[2][2]) || argv[2][0]>argv[2][2] || (argv[2][1]!='-')) {
				errx(1,"Error. When using option -c the next argument is either a digit or <num1>-<num2>, where num1<=num2");
			}
		} else {
			errx(1,"Error. When using option -c the second argument can have length of 1 or 3, got %d",(int)strlen(argv[2]));
		}
	} else if (strcmp(argv[1],"-d")==0) {
		if(argc!=5) {
			errx(1,"Error. If the first argument is -d, the args need to be 4, got %d",argc-1);
		}
		if(strcmp(argv[3],"-f")!=0) {
			errx(1,"Error. If the first argument is -d, the third needs to be -f, got %s",argv[3]);
		}
		if(strlen(argv[4])==1) {
			if(!isDigit(argv[4][0])) {
				errx(1,"Error. If the option is -d, the fourth argument needs to be a digit or num-num. %s is not a valid argument",argv[4]);
			}
		} else if(strlen(argv[4])==3) {
			if(!isDigit(argv[4][0]) || !isDigit(argv[4][2]) || argv[4][0]>argv[4][2] || (argv[4][1]!='-')) {
				errx(1,"Error. If the option is -d, the fourth argument needs to be a digit or num1-num2(num1<=num2). %s is not a valid argument",argv[4]);
			}
		} else {
			errx(1,"Error. If the option is -d, the fourth argument needs to have length of either 1 or 3, got %d",(int)strlen(argv[4]));
		}
	} else {
		errx(1,"Error. First argument has to be either -c or -d, got %s",argv[1]);
	}
}
void optionC(const char* selectedBytes) {
	uint8_t minByte;
	uint8_t maxByte;
	if(strlen(selectedBytes)==1) {
		minByte=selectedBytes[0]-'0';
		maxByte=minByte;
	} else {
		minByte=selectedBytes[0]-'0';
		maxByte=selectedBytes[2]-'0';
	}
	char c;
	uint8_t currentByte=0;
	while(1) {
		currentByte++;
		int rBytes=read(0,&c,sizeof(c));
		if(rBytes==-1) {
			err(2,"Cannot read from STDIN");
		}
		if(rBytes==0) {
			break;//We're done
		}
		if(c=='\n') {
			currentByte=0;
			write(1,&c,sizeof(c));//Always print the newlines
			continue;
		}
		if(currentByte<minByte || currentByte >maxByte) {
			continue;
		}
		write(1,&c,sizeof(c));
	}
	
}
void optionD(const char delim, const char* fields) {
	
	uint8_t minField;
	uint8_t maxField;
	if(strlen(fields)==1) {
		minField=fields[0]-'0';
		maxField=minField;
	} else {
		minField=fields[0]-'0';
		maxField=fields[2]-'0';
	}
	char c;
	uint8_t currentField=1;
	bool shouldWriteChar;
	while(1) {
		shouldWriteChar= currentField>=minField&&currentField<=maxField;
		int rBytes=read(0,&c,sizeof(c));
		if(rBytes==-1) {
			err(2,"Cannot read from STDIN");
		}
		if(rBytes==0) {
			break;//We're done
		}
		if(c=='\n') {
			currentField=1;
			write(1,&c,sizeof(c)); //Always print the newlines
			continue;
		}
		if(c==delim) {
			currentField++;
			shouldWriteChar=currentField>=minField&&currentField<=maxField&&(maxField-minField)>0;//Delims are also written if there are more fields
		}
		if(shouldWriteChar) {
			write(1,&c,sizeof(c));
		}
	}
}
int main(int argc, char* argv[]) {

	validateArgs(argc,argv);
	if(strcmp(argv[1],"-c")==0) {
		optionC(argv[2]);
	} else if(strcmp(argv[1],"-d")==0) {
		optionD(argv[2][0],argv[4]);
	} else {
		write(1,"WTF",3);//Not possible, args are validated
	}
	
	exit(0);
}
