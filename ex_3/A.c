#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	char* file_name = argv[1];
	FILE* fd = fopen(file_name, "r");
	if (fd < 0)
	{
		perror("Unable to open file!");
		exit(1);
	}	

	int rw[2];
	if (pipe(rw) < 0)
	{
		perror("Unable to create pipe!");
		exit(4);
	}
  
	if (mkfifo("Myfifo", 0600) < 0)
	{
		perror("Unable to create fifo!");
		exit(6);
	}
	int fd_fifo = open("Myfifo", O_WRONLY);
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
		int numbers[250], nr, k=-1;
		while (fscanf(fd, "%d", &nr) == 1)
		{
			numbers[++k]=nr;
		}
		int n=0;
		printf("n=");
		scanf("%d", &n);
		int i;
		for(i=0; i<n;i++)
		{
			if (write(rw[1], &numbers[i], sizeof(int))<0)
			{
				perror("Unable to send numbers via pipe to p2!");
				exit(5);
			}
		}
		close(rw[1]);
		exit(0);
	}
	
	int p2 = fork();
	if (p2 < 0)
	{
		perror("Unable to create process p2!");
		exit(6);
	}
	if (p2 == 0)
	{
		close(rw[1]);
		int nr, sum=0;
		while(read(rw[0], &nr, sizeof(int)) > 0)
		{
			if (nr % 2 == 1)
				sum += nr;	
		}
		if (write(fd_fifo, &sum, sizeof(int)) < 0)
		{
			perror("Unable to send sum via fifo!");
			exit(8);
		}
		close(rw[0]);
		exit(0);
	}
	close(rw[0]);
	close(rw[1]);
	fclose(fd);
	close(fd_fifo);
	wait(0);
	wait(0);
	return 0;
}
