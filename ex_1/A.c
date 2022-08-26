#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	char* fifo_name=argv[1];
	char file_name[50]="";
	printf("File name=");
	scanf("%49s", file_name);
	
	FILE* fd = fopen(file_name, "r");
	
	if (fd < 0)
	{
		perror("Unable to open file!");
		exit(3);
	}

	if(mkfifo(fifo_name, 0600) < 0)
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
	
	char s;
	while((s = fgetc(fd)) != EOF)
	{
		if (s >= '0' && s <= '9')
			write(fd_fifo, &s, sizeof(char));	
	}
	fclose(fd);
	close(fd_fifo);
	return 0;
}
