//input.bin has 4194304*4=16 777 216 bytes at most
//We have 9MB=9*1024*1024=9 437 184 bytes of RAM
//Which means that we can split the file in 2 and sort each half, then merge them (same as task33.c)
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

int cmp(const void* a, const void* b) {
	uint32_t num1= *((uint32_t*)a);
	uint32_t num2= *((uint32_t*)b);

	return num1-num2;
}
int generateTmpFile(int suffix,char* name) {
	int n=sprintf(name,"tmp-%d-%d",getpid(),suffix);
	name[n]='\0';
	return open(name,O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
}
int main(int argc, char* argv[]) {
	
	if(argc!=3) {
		errx(1,"Wrong number of args. Got %d, expected 2",argc-1);
	}
	int input=open(argv[1],O_RDONLY);
	if(input==-1) {
		err(2,"Cannot open file %s for reading",argv[1]);
	}	
	struct stat st;
	if(stat(argv[1],&st)==-1) {
		const int olderrno=errno;
		close(input);
		errno=olderrno;
		err(3,"Cannot stat file %s",argv[1]);
	}
	uint32_t nums_count=st.st_size/sizeof(uint32_t);
	if(nums_count>4194304) {
		close(input);
		errx(4,"File %s exceeds the maximum of 4194304 uint32_ts",argv[1]);
	}
	uint32_t firstHalf=nums_count/2;
	uint32_t secondHalf=nums_count-firstHalf;
	
	uint32_t* data=(uint32_t*)malloc(firstHalf*sizeof(uint32_t));
	if(data==NULL) {
		close(input);
		errx(42,"Not enough memory");
	}
	int rBytes=read(input,data,firstHalf*sizeof(uint32_t));
	if(rBytes==-1) {
		const int olderrno=errno;
		close(input);
		free(data);
		errno=olderrno;
		err(4,"Cannot read from file %s",argv[1]);
	}	
	qsort(data,firstHalf,sizeof(uint32_t),cmp);
	char tmp1Name[64];
	int tmp1=generateTmpFile(1,tmp1Name);
	if(tmp1==-1) {
		const int olderrno=errno;
		close(input);
		free(data);
		errno=olderrno;
		err(5,"Cannot create tmp file(1)");
	}
	int wBytes=write(tmp1,data,firstHalf*sizeof(uint32_t));
	if(wBytes==-1) {
		const int olderrno=errno;
		close(input);
		close(tmp1);
		free(data);
		unlink(tmp1Name);
		errno=olderrno;
		err(6,"Cannot write to tmp file(2)");
	}
	free(data);
	data=(uint32_t*)malloc(secondHalf*sizeof(uint32_t));
	if(data==NULL) {
		close(input);
		close(tmp1);
		unlink(tmp1Name);
		err(42,"Not enough memory");
	}
	rBytes=read(input,data,secondHalf*sizeof(uint32_t));
	if(rBytes==-1) {
		const int olderrno=errno;
		close(input);
		close(tmp1);
		free(data);
		unlink(tmp1Name);
		errno=olderrno;
		err(4,"Cannot read from file %s",argv[1]);
	}
	close(input);//We're done with the input file
	qsort(data,secondHalf,sizeof(uint32_t),cmp);
	
	char tmp2Name[64];
	int tmp2=generateTmpFile(2,tmp2Name);
	if(tmp2==-1) {
		const int olderrno=errno;
		close(tmp1);
		free(data);
		unlink(tmp1Name);
		errno=olderrno;
		err(5,"Cannot create tmp file(2)");
	}
	wBytes=write(tmp2,data,secondHalf*sizeof(uint32_t));
	if(wBytes==-1) {
		const int olderrno=errno;
		close(tmp1);
		close(tmp2);
		free(data);
		unlink(tmp1Name);
		unlink(tmp2Name);
		errno=olderrno;
		err(6,"Cannot write to tmp file(2)");
	}
	free(data);
	
	//Time to merge
	if(lseek(tmp1,0,SEEK_SET)==-1) {
		const int olderrno=errno;
		close(tmp1);
		close(tmp2);
		unlink(tmp1Name);
		unlink(tmp2Name);
		errno=olderrno;
		err(7,"Cannot lseek %s",tmp1Name);
	}
	if(lseek(tmp2,0,SEEK_SET)==-1) {
		const int olderrno=errno;
		close(tmp1);
		close(tmp2);
		unlink(tmp1Name);
		unlink(tmp2Name);
		errno=olderrno;
		err(7,"Cannot lseek %s",tmp2Name);
	}
	int output=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(output==-1) {
		const int olderrno=errno;
		close(tmp1);
		close(tmp2);
		unlink(tmp1Name);
		unlink(tmp2Name);
		errno=olderrno;
		err(8,"Cannot create/open file %s",argv[2]);
	}
	
	uint32_t pos1=0,pos2=0;
	uint32_t n1,n2;

	bool readt1=true,readt2=true;
	while(pos1<firstHalf&&pos2<secondHalf) {
		pos1++;
		pos2++;
		if(readt1) {
			int rBytes=read(tmp1,&n1,sizeof(n1)); 
			if(rBytes==-1) {
				const int olderrno=errno;
				close(tmp1);
				close(tmp2);
				close(output);
				unlink(tmp1Name);
				unlink(tmp2Name);
				errno=olderrno;
				err(9,"Cannot read from tmp file(1)");
			}
		}
		if(readt2) {
			rBytes=read(tmp2,&n2,sizeof(n2));
			if(rBytes==-1) {
				const int olderrno=errno;
				close(tmp1);
				close(tmp2);
				close(output);
				unlink(tmp1Name);
				unlink(tmp2Name);
				errno=olderrno;
				err(9,"Cannot read from tmp file(2)");		
			}
		}
		readt1=true;
		readt2=true;
		if(n1<n2) {
			readt2=false;
			pos2--;
			wBytes=write(output,&n1,sizeof(n1));
			if(wBytes==-1) {
				const int olderrno=errno;
				close(tmp1);
				close(tmp2);
				close(output);
				unlink(tmp1Name);
				unlink(tmp2Name);
				errno=olderrno;
				err(10,"Cannot write to file %s",argv[2]);
			}
		} else {
			readt1=false;
			pos1--;
			wBytes=write(output,&n2,sizeof(n2));
			if(wBytes==-1) {
				const int olderrno=errno;
				close(tmp1);
				close(tmp2);
				close(output);
				unlink(tmp1Name);
				unlink(tmp2Name);
				errno=olderrno;
				err(10,"Cannot write to file %s",argv[2]);
			}
		}
	}
	while(pos1<firstHalf) {
		pos1++;
		int rBytes=read(tmp1,&n1,sizeof(n1)); 
		if(rBytes==-1) {
			const int olderrno=errno;
			close(tmp1);
			close(tmp2);
			close(output);
			unlink(tmp1Name);
			unlink(tmp2Name);
			errno=olderrno;
			err(9,"Cannot read from tmp file(1)");
		}
		wBytes=write(output,&n1,sizeof(n1));
		if(wBytes==-1) {
			const int olderrno=errno;
			close(tmp1);
			close(tmp2);
			close(output);
			unlink(tmp1Name);
			unlink(tmp2Name);
			errno=olderrno;
			err(10,"Cannot write to file %s",argv[2]);
		}
	}
	while(pos2<secondHalf) {
		pos2++;
		int rBytes=read(tmp2,&n2,sizeof(n2)); 
		if(rBytes==-1) {
			const int olderrno=errno;
			close(tmp1);
			close(tmp2);
			close(output);
			unlink(tmp1Name);
			unlink(tmp2Name);
			errno=olderrno;
			err(9,"Cannot read from tmp file(2)");
		}
		wBytes=write(output,&n2,sizeof(n2));
		if(wBytes==-1) {
			const int olderrno=errno;
			close(tmp1);
			close(tmp2);
			close(output);
			unlink(tmp1Name);
			unlink(tmp2Name);
			errno=olderrno;
			err(10,"Cannot write to file %s",argv[2]);
		}
	}
	close(tmp1);
	close(tmp2);
	close(output);
	//unlink(tmp1Name);
	//unlink(tmp2Name);
	exit(0);
}
