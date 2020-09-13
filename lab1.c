#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

//Lab 1
//Max Forbang and Martin Quezada
//This work abides by the JMU Honor Code.

int main(int argc, char *argv[]) {
	
	//Opening file
	int fd = open(argv[0], O_RDONLY);
	if (fd == -1) {
		printf("Opening of file %s failed: %s\n", argv[0], strerror(errno));
		exit(1);
	}
	
	//Declaring different size buffers
	char charBuffer;
	int intBuffer; 
	long longBuffer;
	char stringBuffer[20];
	
	//Declaring current position and bytes read for error checking
	ssize_t bytes_read;
	int pos;
	
	//Item 1
	pos = lseek(fd, 32, SEEK_SET);
	bytes_read = read(fd, &intBuffer, sizeof(int));
	if(bytes_read == -1) {
		printf("Error reading from %s: %s\n", argv[0], strerror(errno));
		exit(1);
	}
	printf("Item 1:\n\tDecimal: %d\n\tHex: 0x%08x\n", intBuffer, intBuffer);
	
	
	//Item 2
	pos = lseek(fd, 300, SEEK_SET);
	bytes_read = read(fd, &charBuffer, sizeof(char));
	if(bytes_read == -1) {
		printf("Error reading from %s: %s\n", argv[0], strerror(errno));
		exit(1);
	}
	printf("Item 2:\n\tDecimal: %d\n\tHex: 0x%02x\n", charBuffer, charBuffer);
	
	
	//Item 3
	pos = lseek(fd, 123, SEEK_CUR);
	bytes_read = read(fd, &intBuffer, sizeof(int));
	if(bytes_read == -1) {
		printf("Error reading from %s: %s\n", argv[0], strerror(errno));
		exit(1);
	}
	printf("Item 3:\n\tDecimal: %d\n\tHex: 0x%04x\n", intBuffer, intBuffer);
	
	
	//Item 4
	pos = lseek(fd, 512, SEEK_SET);
	bytes_read = read(fd, &intBuffer, sizeof(int));
	if(bytes_read == -1) {
		printf("Error reading from %s: %s\n", argv[0], strerror(errno));
		exit(1);
	}
	printf("Item 4:\n\tDecimal: %d\n\tHex: 0x%08x\n", intBuffer, intBuffer);
	
	
	//Item 5
	//pos = lseek(fd, 1234, SEEK_SET);
	//bytes_read = read(fd, stringBuffer, sizeof(stringBuffer));
	//if(bytes_read == -1) {
	//	printf("Error reading from %s: %s\n", argv[0], strerror(errno));
	//	exit(1);
	//}

	//printf("Item 5:\n\t%.*s", (int)bytes_read, stringBuffer);

	        //Item 6
        //pos = lseek(fd, 2573, SEEK_SET);
        //bytes_read = read(fd, &intBuffer, sizeof(int));
        //if(bytes_read == -1) {
        //        printf("Error reading from %s: %s\n", argv[0], strerror(errno));
        //        exit(1);
        //}

        //printf("Item 6:\n");

        //printf("\tMember 1:\n\t\tDecimal: %d\tHex: 0x%08x\n", intBuffer, intBuffer);

        //pos = lseek(fd, 2577, SEEK_SET);
        //bytes_read = read(fd, &charBuffer, sizeof(char));

        //printf("\tMember 2:\n\t\tDecimal: %d\tHex: 0x%02x\n", charBuffer, charBuffer);

        //pos = lseek(fd, 2579, SEEK_SET);
        //bytes_read = read(fd, &intBuffer, sizeof(int));
        //printf("\tMember 3:\n\t\tDecimal: %d\tHex: 0x%04x\n", intBuffer, intBuffer);

        //pos = lseek(fd, 2581, SEEK_SET);
        //bytes_read = read(fd, stringBuffer, sizeof(stringBuffer));
        //printf("\tMember 4:\n\t\t%s",  stringBuffer);

        //Item 7
        pos = lseek(fd, 0xbbb, SEEK_SET);
        bytes_read = read(fd, &intBuffer, sizeof(int));
        if(bytes_read == -1){
                printf("Error reading from %s: %s\n", argv[0], strerror(errno));
                exit(1);
        }
        printf("Item 7:\n\tDecimal: %d\n\tHex: 0x%016x\n", intBuffer, intBuffer);


        //Member 8
        pos = lseek(fd, -100, SEEK_END);
        bytes_read = read(fd, &intBuffer, sizeof(int));
        if(bytes_read == -1){
                printf("Error reading from %s: %s\n", argv[0], strerror(errno));
                exit(1);
        }
        printf("Item 8:\n\tDecimal: %d\n\tHex: 0x%08x\n", intBuffer, intBuffer);
	

	exit(0);


