#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv)
{
	char file_path[25];
	int rw[2];
	if (pipe(rw) < 0)
	{
		perror("Unable to create pipe!");
		exit(3);
	}
	if(mkfifo("fifo", 0600) < 0)
	{
		perror("Unable to create fifo!");
		exit(10);
	}
	int fd_fifo = open("fifo", O_WRONLY);
	if(fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(11);
	}
	int p1 = fork();
	if (p1 < 0)
	{
		perror("Unable to create process p1!");
		exit(1);
	}	
	if (p1 == 0)
	{
		close(rw[0]);
		printf("File path=");
		scanf("%24s", file_path);
		int len = strlen(file_path);
		if (write(rw[1], &len, sizeof(int)) < 0)
		{
			perror("Unable to send length of path!");
			exit(5);
		}
		if (write(rw[1], file_path, sizeof(char) * len) < 0)
		{
			perror("Unable to send file path via pipe!");
			exit(4);
		}
		close(rw[1]);
		exit(0);
	}
	
	int p2 = fork();
	if (p2 < 0)
	{
		perror("Unable to create process p2!");
		exit(2);
	}
	if (p2 == 0)
	{
		int len;
		close(rw[1]);
		if (read(rw[0], &len, sizeof(int)) < 0)
		{
			perror("Unable to read size of path!");
			exit(6);
		}
		if (len <= 0)
		{
			perror("Unsuccessful read from pipe!");
			exit(7);
		}
		char* file = (char*)malloc(sizeof(char)	* (len + 1));
		if (read(rw[0], file, sizeof(char) * len) < 0)
		{
			perror("Unable to read file path form pipe!");
			exit(8);
		}
		file[len] = 0; // terminator
		
		int A=0, E=0, I=0, O=0, U=0;
		FILE* fd = fopen(file, "r");
		if (fd< 0)
		{
			perror("Unable to open file!");
			exit(9);
		}
		char c;
		while((c = fgetc(fd)) != EOF)
		{
			if (c == 'A')
				A++;
			if (c == 'E')
				E++;
			if (c == 'I')
				I++;
			if (c == 'O')
				O++;
			if (c == 'U')
				U++;
		}
		if(write(fd_fifo, &A, sizeof(int)) < 0)
		{
			perror("Unable to send frequency via fifo!");
			exit(12);	
		}
		if(write(fd_fifo, &E, sizeof(int)) < 0)
		{
    		perror("Unable to send frequency via fifo!");
     		exit(13);
		}
		if(write(fd_fifo, &I, sizeof(int)) < 0)
		{
     		perror("Unable to send frequency via fifo!");
     		exit(14);
		}
		if(write(fd_fifo, &O, sizeof(int)) < 0)
		{
     		perror("Unable to send frequency via fifo!");
     		exit(15);
		}	
		if(write(fd_fifo, &U, sizeof(int)) < 0)
		{
     		perror("Unable to send frequency via fifo!");
     		exit(16);
		}
		close(rw[0]);
		fclose(fd);
		free(file);
		exit(0);
	}
	close(rw[0]);
	close(rw[1]);
	close(fd_fifo);
	wait(0);
	wait(0);
	return 0;
}
