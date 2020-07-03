
#include <err.h>
#include <unistd.h>
int main() {
	if(execlp("date","date",(char*)NULL)==-1) {
		err(1,"Could not execute date");
	}
	/*
		execl("/bin/date","date",(char*)NULL);
		is also possible(execlp searches in PATH if there are no '\'s in the name of the executable

	*/
}
