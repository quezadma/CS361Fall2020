#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

//Project: 1
//Max Forbang and Martin Quezada
//This work abides by the JMU Honor Code.

#define CLUSTER_SIZE 4096

int main(int argc, char *argv[]) {
	

	//check for command line arguments
	if(argc != 3){
		printf("Not enough arguments being passed.");
		exit(-1);
	}
	
	
	//Opening input file
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		printf("Opening of input file %s failed: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	
	fd = open(argv[2], O_RDONLY);
	if (fd == -1) {
                printf("Opening of map file %s failed: %s\n", argv[2], strerror(errno));
                exit(1);
        }

	


}
