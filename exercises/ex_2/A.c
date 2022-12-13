#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

int cmmmc(int a, int b)
{
	int result = a;
	while(a % b != 0)
	{
		a = a + result;
	}
	return a;
}

int main(int argc, char** argv)
{
	char* fifo_name = argv[1];
	if (mkfifo(fifo_name, 0600) < 0)
	{
		perror("Unable to create fifo!");
		exit(1);
	}	

	int a = 0, b = 0;
	printf("a=");
	scanf("%d", &a);
	printf("b=");
	scanf("%d", &b);
	int c = cmmmc(a, b);
	int fd_fifo = open(fifo_name, O_WRONLY);
	if (fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(2);
	}
	if (write(fd_fifo, &c, sizeof(int)) < 0)
	{
		perror("Unable to write cmmmc!");
		exit(1);
	}
	close(fd_fifo);
	return 0;
}
