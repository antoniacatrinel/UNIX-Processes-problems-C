#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void process1(int fifo, int w)
{
	int cmmmc = 0;
	if (read(fifo, &cmmmc, sizeof(int)) < 0)
	{
		perror("Unable to read cmmmc from fifo!");
		exit(4);
	}
	int d;
	for (d = 1; d <= cmmmc; d++)
		if (cmmmc%d == 0)
			if (write(w, &d, sizeof(int)) < 0)
			{
				perror("Unable to send divisor!");
				exit(5);
			}
}

void process2(int r)
{
	int div=0;
	while(read(r, &div, sizeof(int)) > 0)
	{
		printf("%d ", div);
	}
	printf("\n");
}

int main(int agrc, char** argv)
{
	char* fifo_name = argv[1];
	int rw[2];
	if(pipe(rw) < 0)
	{
		perror("Unable to create pipe!");
		exit(1);	
	}
	int fd_fifo = open(fifo_name, O_RDONLY);
	if (fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(7);
	}
	int p1 = fork();
	if (p1 < 0)
	{
		perror("Unable to create process p1!");
		exit(2);
	}
	if (p1 == 0)
	{
		close(rw[0]);
		process1(fd_fifo, rw[1]);
		close(rw[1]);
		exit(0);	
	}

	int p2 = fork();
	if (p2 < 0)
	{
		perror("Unable to create process p2!");
		exit(3);
	}
	if (p2 == 0)
	{
		close(rw[1]);
		process2(rw[0]);
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
