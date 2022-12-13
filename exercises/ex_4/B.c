#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	int fd = open("fifo", O_RDONLY);
	if (fd < 0)
	{
		perror("Uanble to open fifo!");
		exit(1);
	}
	char c;
	if (read(fd, &c, sizeof(char)) < 0)
	{
		perror("Uanble to read nth character from fifo!");
		exit(2);
	}
	printf("ASCII code of nth character is: %d.\n", c);
	close(fd);
	unlink("fifo");	
	return 0;
}
