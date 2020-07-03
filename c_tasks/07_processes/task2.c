#include <stdlib.h>
#include <unistd.h>
#include <err.h>
int main(int argc, char* argv[]) {

	if(argc!=2) {
		errx(1,"Expected 1 argument, got %d",argc-1);
	}
	if(execlp("ls","ls",argv[1],(char*)NULL)==-1) {
		err(2,"Could not execute ls");
	}
	exit(0);
}
