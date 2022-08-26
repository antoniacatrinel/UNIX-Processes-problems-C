#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

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
		perror("Uanble to create pipe!");
		exit(2);
	}
	
	if (mkfifo("fifo", 0600) < 0)
	{
		perror("Uanble to create fifo!");
		exit(3);
	}
	int fd_fifo = open("fifo", O_WRONLY);
	if(fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(4);
	}
	
	int p1 = fork();
	if (p1 < 0)
	{
		perror("Unable to create process p1!");
		exit(5);
	}
	if (p1 == 0)
	{
		close(rw[0]);
		char string[300]="", c;
		int k=-1;
		while(fscanf(fd, "%c", &c) == 1)
		{
			string[++k] = c;
		}
		string[++k] = 0; // terminator
		int n;
		printf("n=");
		scanf("%d", &n);
		
		if (n >= k)
		{
			perror("We dont have a nth character!");
			exit(6);
		}
		
		if (write(rw[1], &string[n+1], sizeof(char)) < 0)
		{
			perror("Unable to send nth character!");
			exit(7);
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
		char s;
		if (read(rw[0], &s, sizeof(char)) < 0)
		{
			perror("Unable to read nth character!");
			exit(8);
		}
		char type[10];
		if (s >= '0' && s <= '9')
			strcpy(type, "digit");
		else
			if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z'))
				strcpy(type, "letter");
			else
				strcpy(type, "other");
		
		printf("Nature of nth character: %s.\n", type);
		if (write(fd_fifo, &s, sizeof(char))< 0)
		{
			perror("Unable to send nth character via fifo!");
			exit(10);
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
