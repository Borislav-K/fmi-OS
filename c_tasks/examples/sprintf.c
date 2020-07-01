#include <unistd.h>
#include <math.h>
#include <stdio.h>
int main() {

	char buf[1024];
	int output_size=sprintf(buf,"pi=%0.5f\n",4*atan(1.0));
	write(1,buf,output_size);
	output_size=sprintf(buf,"I am inserting a string here: %s\n","pesho");
	write(1,buf,output_size);
}
