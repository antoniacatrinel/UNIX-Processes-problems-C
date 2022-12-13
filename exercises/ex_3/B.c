#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	int fd_fifo = open("Myfifo", O_RDONLY);
	if (fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(1);
	}
	int sum=0;
	if (read(fd_fifo, &sum, sizeof(int)) < 0)
	{
		perror("Unable to read sum!");
		exit(2);
	}
	printf("Sum is %d\n.", sum);
	close(fd_fifo);
	unlink("Myfifo");
	return 0;
}
