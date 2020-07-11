//65536*2=131 072 bytes MAX for input.bin
//256*1024=262 144 bytes of RAM
//It means that we can load the whole file into the memory
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
	uint16_t n1=*((uint16_t*)a);
	uint16_t n2=*((uint16_t*)b);

	if(n1>n2) {
		return 1;
	}
	if(n1<n2) {
		return -1;
	}
	return 0;
}
int main(int argc, char* argv[]) {
	if(argc!=3) {
		errx(1,"Wrong number of arguments. Got %d, expected 2",argc-1);
	}

	struct stat st;
	if(stat(argv[1],&st)==-1) {
		err(2,"Cannot stat file %s",argv[1]);
	}
	if(st.st_size%sizeof(uint16_t)!=0) {
		err(3,"File %s must consist of uint16_ts",argv[1]);
	}

	int input=open(argv[1],O_RDONLY);
	if(input==-1) {
		err(4,"Cannot open input file %s",argv[1]);
	}
	size_t count=st.st_size/sizeof(uint16_t);
	uint16_t* data=(uint16_t*)malloc(count*sizeof(uint16_t));
	int rBytes=read(input,data,count*sizeof(uint16_t));
	if(rBytes==-1) {
		const int olderrno=errno;
		close(input);
		free(data);
		errno=olderrno;
		err(5,"Cannot read from file %s",argv[1]);
	}
	if(rBytes!=(ssize_t)(count*sizeof(uint16_t))) {
		close(input);
		free(data);
		errx(6,"Error. Input file was corrupted");
	}
	close(input);//No longer needed
	qsort(data,count,sizeof(uint16_t),cmp);
	
	int output=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(output==-1) {
		free(data);
		err(7,"Cannot open/create file %s",argv[2]);
	}
	int wBytes=write(output,data,count*sizeof(uint16_t));
	if(wBytes==-1) {
		const int olderrno=errno;
		close(output);
		free(data);
		errno=olderrno;
		err(8,"Cannot write to file %s",argv[2]);
	}
	if(wBytes!=(ssize_t)(count*sizeof(uint16_t))) {
		close(output);
		free(data);
		errx(9,"Could not write the whole data to the output file");
	}
	free(data);
	close(output);
	exit(0);
}
