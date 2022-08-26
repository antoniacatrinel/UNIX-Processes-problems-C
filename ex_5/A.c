#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	char* fifo_name = argv[1];
	int n;
	printf("n=");
	scanf("%d", &n);
	if (mkfifo(fifo_name, 0600) < 0)
	{
		perror("Unable to create fifo!");
		exit(1);
	}
	int fd_fifo = open(fifo_name, O_WRONLY);
	if (fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(2);
	}
	int d;
	for(d=1; d <= n; d++)
		if(n%d == 0)
			if (write(fd_fifo, &d, sizeof(int)) < 0)
			{
				perror("Unable to send divisor!");
				exit(3);
			}
	close(fd_fifo);
	return 0;
}

