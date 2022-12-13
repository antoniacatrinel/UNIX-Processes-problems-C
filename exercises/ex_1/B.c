#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void process1(int fifo, int w)
{
	char s;
	int sum=0, number;
	while(read(fifo, &s, sizeof(char)) > 0)
	{
		// obtain integer
		number = s - '0';
		sum += number;	
	}
	
	if (write(w, &sum, sizeof(int)) < 0)
	{
		perror("Unable to send sum!");
		exit(4);	
	}
}

void process2(int r)
{
	int sum=0;
	if (read(r, &sum, sizeof(int)) < 0)
	{
		perror("Unable to read sum!");
		exit(5);	
	}
	int d;
	for (d=1; d <=sum; d++)
		if (sum%d == 0)
			printf("%d ", d);
	printf("\n");
}

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
        exit(2);            
	}
	int p1 = fork();
	if (p1<0)
	{
		perror("Unable to create process p1!");
		exit(2);
	}
	else
		if (p1 == 0)  // we are in p1
		{
			close(rw[0]);
			process1(fd_fifo, rw[1]);

			exit(0);	
		}
	
	int p2 = fork();
	if (p2<0)
	{
         perror("Unable to create process p2!");
         exit(3);
    }
    else
        if (p2 == 0)  // we are in p2
        {
             close(rw[1]);
			 close(fd_fifo);
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
