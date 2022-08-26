#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	char* fifo_name = argv[1];
	int fd_fifo = open(fifo_name, O_RDONLY);
	if (fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(1);
	}
	int rw[2];
	if (pipe(rw) < 0)
	{
		perror("Unable to create pipe!");
		exit(3);
	}

	int p1= fork();
	if (p1 < 0)
 	{
		perror("Unable to create process p1!");
		exit(2);
	}
	if (p1 == 0)
	{
		close(rw[0]);
		int d=0, sum=0;
		while(read(fd_fifo, &d, sizeof(int)) > 0)
			sum += d;
		if (write(rw[1], &sum, sizeof(int)) < 0)
		{
			perror("Unable to send sum via pipe!");
			exit(4);
		}
		close(rw[1]);
		exit(0);
	}
	
	int p2 = fork();
	if (p2 < 0)
	{
		perror("Unable to create process p2!");
		exit(5);
	}
	if (p2 == 0)
	{
		close(rw[1]);
		int s=0;
		if (read(rw[0], &s, sizeof(int)) < 0)
		{
			perror("Unable to read sum from pipe!");
			exit(6);
		}
		printf("Sum is %d.\n", s);
		close(rw[0]);
		exit(0);
	}
	close(rw[0]);
	close(rw[1]);
	close(fd_fifo);
	unlink(fifo_name);
	wait(0);
	wait(0);
	return 0;
}
