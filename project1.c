#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

//Project 1
//Max Forbang and Martin Quezada
//This work abides by the JMU Honor Code.

#define CLUSTER_SIZE 4096

int main(int argc, char *argv[]) {

	//check for command line arguments
	if(argc != 3){
		printf("Incorrect number of arguments being passed.");
		exit(-1);
	}
		
	//Opening input file
	int inputFD = open(argv[1], O_RDONLY);
	if (inputFD == -1) {
		printf("Opening of input file %s failed: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	//Opening map file
	int mapFD = open(argv[2], O_RDONLY);
	if (mapFD == -1) {
		printf("Opening of map file %s failed: %s\n", argv[2], strerror(errno));
		exit(1);
	}
	
	//creating entry buffer, file name array, and cluster offset #
	char entryBuffer[16];
	char fileName[13]; //size of file name (12) + 1 for null character
	int offset;
	
	//creating variables to determine size of input file
	int numberOfEntries = lseek(mapFD, 0, SEEK_END) / 16;
	lseek(mapFD, 0, SEEK_SET);

	//creating buffer to store actual data from input file
	char buffer[4096];
	
	//Iterate through each entry in map file
	for (int i = 0; i < numberOfEntries; i++){
		
		//Read entry
		read(mapFD, entryBuffer, 16);
		
		//store file name string and cluster offset #
		strncpy(fileName, entryBuffer, 12);
		offset = (int)entryBuffer[12];
		
		//open file at current entry or create it if it does not exist
		int fd = open(fileName, O_WRONLY | O_CREAT, 0666);
		if (fd == -1) {
			printf("Opening of file %s failed: %s\n", fileName, strerror(errno));
            exit(1);
        }


		//Seek correct positions in input and output files
		lseek(fd, offset * CLUSTER_SIZE, SEEK_SET);
		lseek(inputFD, i * CLUSTER_SIZE, SEEK_SET);
		
		//Read from input file into buffer
		read(inputFD, buffer, CLUSTER_SIZE);
		
		
		//Write from buffer into output file
		write(fd, buffer, CLUSTER_SIZE);
		
		close(fd);
		
		//test printf to see if variables have the right values from map file
		printf("%s: %d\n", fileName, offset);
	}
		 

	close(mapFD);
	close(inputFD);
	

}
