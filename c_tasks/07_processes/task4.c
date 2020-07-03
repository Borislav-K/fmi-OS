#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
int main() {
	
	const int PRINT_TIMES=10;
	pid_t pid=fork();
	if(pid==-1) {
		err(1,"Could not fork");
	}
	if(pid==0) {
		//@child
		for(int i=1;i<=PRINT_TIMES;i++) {
			write(1,"CHILD\n",sizeof("CHILD\n"));
			sleep(1);
		}
	} else {
		//@parent
		for(int i=1;i<=PRINT_TIMES;i++) {
			write(1,"PARENT\n",sizeof("PARENT\n"));
			sleep(1);
		}
	}
	exit(0);
}
