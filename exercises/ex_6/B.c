#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char** argv)
{
	int fd_fifo = open("fifo", O_RDONLY);
	if (fd_fifo < 0)
	{
		perror("Unable to open fifo!");
		exit(1);
	}
	int A=0, E=0, I=0, O=0, U=0;	
	if(read(fd_fifo, &A, sizeof(int)) < 0)
    {
        perror("Unable to read frequency via fifo!");
        exit(12);
    }
    if(read(fd_fifo, &E, sizeof(int)) < 0)
    {
        perror("Unable to read frequency via fifo!");
        exit(13);
    }
    if(read(fd_fifo, &I, sizeof(int)) < 0)
    {            
	    perror("Unable to read frequency via fifo!");
        exit(14);
    }
    if(read(fd_fifo, &O, sizeof(int)) < 0)
    {
        perror("Unable to read frequency via fifo!");
        exit(15);
    }
    if(read(fd_fifo, &U, sizeof(int)) < 0)
    {
        perror("Unable to read frequency via fifo!");
        exit(16);
    }
	printf("Frequency of A: %d.\n", A);
	printf("Frequency of E: %d.\n", E);
	printf("Frequency of I: %d.\n", I);
	printf("Frequency of O: %d.\n", O);
	printf("Frequency of U: %d.\n", U);	
	close(fd_fifo);
	unlink("fifo");
	return 0;
}
