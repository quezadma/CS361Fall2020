#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

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

	//creating buffer and file descriptor to store data from input file
	char buffer[4096];
	int fd = 0;
	
	//creating data structure to store file names and their corresponding file descriptors
	int maxFiles = 8;
	int numFiles = 0;
	char fileNames[maxFiles][13];
	int fds[maxFiles];
	bool flag = false;
	
	
	//Iterate through each entry in map file
	for (int i = 0; i < numberOfEntries; i++){
		
		//Read entry
		read(mapFD, entryBuffer, 16);
		
		//store file name string and cluster offset #
		strncpy(fileName, entryBuffer, 12);
		offset = (int)entryBuffer[12];
		
		//Check if file has been opened. If so, set flag 				
		for (int j = 0; j < numFiles; j++)
		{
			if (strcmp(fileNames[i], fileName) == 0) flag = true;
			
		}
		
		//Opens file only if flag is set to false (this entry's file has not been opened yet)
		if (flag == false)
		{
			//open file at current entry or create it if it does not exist
			fd = open(fileName, O_WRONLY | O_CREAT, 0666);
			if (fd == -1) {
				printf("Opening of file %s failed: %s\n", fileName, strerror(errno));
				exit(1);
			}
			
			//Resize file name and file descriptor arrays if they're full
			if (numFiles == maxFiles)
			{
				maxFiles *= 2;
				char temp[maxFiles][13];
				int temp2[maxFiles];
				
				for (int x = 0; x < numFiles; x++)
				{
					memcpy(temp[x], fileNames[x], 13);
					temp2[x] = fds[x];
				}
				
				char fileNames[maxFiles][13];
				int fds[maxFiles];
				
				fds[0] += 0; //Useless code to avoid extraneous "fds set but not used" compiler warning even though fds is used.
				
				for (int y = 0; y < numFiles; y++)
				{
					memcpy(fileNames[y], temp[y], 13);
					fds[y] = temp2[y];
				}
				
				
			}
			
			//add file to list of fileNames
			strncpy(fileNames[numFiles], fileName, 12);
			fds[numFiles] = fd;
			numFiles++;
		}
		
		//reset flag for next entry
		flag = false;

		//Seek correct positions in input and output files
		lseek(fd, offset * CLUSTER_SIZE, SEEK_SET);
		lseek(inputFD, i * CLUSTER_SIZE, SEEK_SET);
		
		//Read from input file into buffer
		read(inputFD, buffer, CLUSTER_SIZE);
		
		
		//Write from buffer into output file
		write(fd, buffer, CLUSTER_SIZE);
		
	}
		 
	
	
	//Close all files
	for (int i = 0; i < numFiles; i++)
	{
		close(fds[i]);
	}
	
	close(mapFD);
	close(inputFD);
	
	

}
