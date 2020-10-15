/* project3.c */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "common.h"
#include "classify.h"

int main(int argc, char *argv[])
{
    int input_fd;
    int classification_fd;
    int cluster_number;
    int bytes_read;
    unsigned char classification, cluster_type;
    char cluster_data[CLUSTER_SIZE];
    
    // We only accept running with one command line argumet: the name of the
    // data file
    if (argc != 2) {
        printf("Usage: %s data_file\n", argv[0]);
        return 1;
    }
    
    // Try opening the file for reading, exit with appropriate error message
    // if open fails
    input_fd = open(argv[1], O_RDONLY);
    if (input_fd < 0) {
        printf("AAAError opening file \"%s\" for reading: %s\n", argv[1], strerror(errno));
        return 1;
    }
    
	
	
	classification_fd = open("classification", O_RDWR | O_CREAT, 0666);
    // Iterate through all the clusters, reading their contents 
    // into cluster_data
    cluster_number = 0;
    while ((bytes_read = read(input_fd, &cluster_data, CLUSTER_SIZE)) > 0) {
        assert(bytes_read == CLUSTER_SIZE);
        classification = TYPE_UNCLASSIFIED;
        
		
		if(has_jpg_header(cluster_data) != 0) {
			if(has_jpg_footer(cluster_data) != 0){
				classification = TYPE_IS_JPG | TYPE_JPG_HEADER | TYPE_JPG_FOOTER;
			}
			
			classification = TYPE_IS_JPG | TYPE_JPG_HEADER;
			
		}
		
		
		
		
			
				
        /*
            In this loop, you need to implement the functionality of the
            classifier. Each cluster needs to be examined using the functions
            provided in classify.c. Then for each cluster, the attributes
            need to be written to the classification file.
        */
        
        printf("Processing cluster %06d\n", cluster_number); // This line needs to be removed in your final submission
        write(classification_fd, &classification, 1);
        cluster_number++;
    }
    
    close(input_fd);

    // Try opening the classification file for reading, exit with appropriate
    // error message if open fails 
	
	
     // Instead of opening this file here, you may elect to open it before the classifier loop in read/write mode
	 
    if (classification_fd < 0) {
        printf("BBBError opening file \"%s\": %s\n", "classificiation", strerror(errno));
        return 1;
    }
    
	
	lseek(classification_fd, 0, SEEK_SET);	
	int map_fd = open("map", O_WRONLY | O_CREAT, 0666);	
	printf("test");

	
	char hName[12]; char jName[12];		//Buffer for file namespace
	int hOffset = 0; int jOffset = 0;	//Offsets for different file types
	
    // Iterate over each cluster, reading in the cluster type attributes
    while ((bytes_read = read(classification_fd, &cluster_type, 1)) > 0) {
        
		//if (classification & TYPE_JPG_HEADER == 1)
			write(map_fd, "File0001.jpg", 12);
			write(map_fd, &jOffset, 4);
		
		
		/*
            In this loop, you need to implement the functionality of the
            mapper. For each cluster, a map entry needs to be created and
            written to the map file.
        */
    }
    
    close(classification_fd);

    return 0;
};
