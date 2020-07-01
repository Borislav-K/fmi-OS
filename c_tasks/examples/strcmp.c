//Сравнява всички подадени аргументи с pesho и извежда резултата на stdout
#include <unistd.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char* argv[]) {
	char buffer[1024];
	for(int i=1;i<argc;i++) {
		int result=strcmp(argv[i],"pesho");
		int output_size;
		if(result==0) {
			output_size=sprintf(buffer,"%s is equal to pesho\n",argv[i]);
		} else if(result<0) {
			output_size=sprintf(buffer,"%s is lexicographically less than pesho\n",argv[i]);
		} else {
			output_size=sprintf(buffer,"%s is lexicographically greater than pesho\n",argv[i]);
		}
		write(1,buffer,output_size);
	}
}
