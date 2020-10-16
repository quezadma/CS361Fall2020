
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "common.h"
#include "classify.h"

//Project 3
//Max Forbang and Martin Quezada
//This work abides by the JMU Honor Code.

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
        printf("Error opening file \"%s\" for reading: %s\n", argv[1], strerror(errno));
        return 1;
    }
    
	
	
	classification_fd = open("classification", O_RDWR | O_CREAT, 0666);
    // Iterate through all the clusters, reading their contents 
    // into cluster_data
    cluster_number = 0;
    while ((bytes_read = read(input_fd, &cluster_data, CLUSTER_SIZE)) > 0) {
        assert(bytes_read == CLUSTER_SIZE);
        classification = TYPE_UNCLASSIFIED;
        
		//JPG CODE
		if(has_jpg_header(cluster_data) != 0) {
			if(has_jpg_footer(cluster_data) != 0){
				classification = TYPE_IS_JPG | TYPE_JPG_HEADER | TYPE_JPG_FOOTER; //JHF
			}
			else {
				classification = TYPE_IS_JPG | TYPE_JPG_HEADER;		//JHEADER
			}
		}
        else if(has_jpg_footer(cluster_data) != 0) 
        {
            classification = TYPE_IS_JPG | TYPE_JPG_FOOTER;		//JFOOTER
            
        }
        else if(has_jpg_body(cluster_data) != 0) {
            classification = TYPE_IS_JPG; 		//JONLY
        }


        //HTML CODE
        if(has_html_header(cluster_data) != 0) {
			if(has_html_footer(cluster_data) != 0){
				classification = TYPE_IS_HTML | TYPE_HTML_HEADER | TYPE_HTML_FOOTER; //HHF
			}
			else {
				classification = TYPE_IS_HTML | TYPE_HTML_HEADER;		//HHEADER
			}
		}
        else if(has_html_footer(cluster_data) != 0) 
        {
            classification = TYPE_IS_HTML | TYPE_HTML_FOOTER;		//HFOOTER
            
        }
        else if(has_html_body(cluster_data) != 0) {
            classification = TYPE_IS_HTML; 		//HONLY
        }
			
        
        //printf("Processing cluster %06d, classification type: %d\n", cluster_number, classification); // This line needs to be removed in your final submission
        write(classification_fd, &classification, 1);
        cluster_number++;
    }
    
    close(input_fd);

    // Try opening the classification file for reading, exit with appropriate
    // error message if open fails 
	classification_fd = open(CLASSIFICATION_FILE, O_RDONLY);
		 
    if (classification_fd < 0) {
        printf("Error opening file \"%s\": %s\n", "classificiation", strerror(errno));
        return 1;
    }
    
	lseek(classification_fd, 0, SEEK_SET);	//Seek to beginning of classification file						
	int map_fd = open("map", O_WRONLY | O_CREAT, 0666);		//Create map file
	char hName[13]; char jName[13];		//Buffer for file namespace
	int hOffset = 0; int jOffset = 0;	//Offsets for different file types
	int jFileNumber = 1; int hFileNumber = 1; //Number of file being written, starts at 1
	
    // Iterate over each cluster, reading in the cluster type attributes
    while ((bytes_read = read(classification_fd, &cluster_type, 1)) > 0) {
			
		if ((cluster_type & TYPE_IS_JPG) == 1) {

			if((cluster_type & TYPE_JPG_HEADER) == 2) 	//JHF or JHEADER
			{
				snprintf(jName, 13, "File%04d.jpg", jFileNumber);
				jFileNumber++;
				jOffset = 0;
				writeMapEntry(map_fd, jName, &jOffset);
			}	
			else {
				writeMapEntry(map_fd, jName, &jOffset);		//JFOOTER or JONLY
			}
		}
		else if ((cluster_type & TYPE_IS_HTML) == 8) {
				
			if((cluster_type & TYPE_HTML_HEADER) == 16) 	//HHF or HHEADER
			{
				snprintf(hName, 13, "File%04d.htm", hFileNumber);
				hFileNumber++;
				hOffset = 0;
				
				writeMapEntry(map_fd, hName, &hOffset);
			}	
			else {
				writeMapEntry(map_fd, hName, &hOffset);		//HFOOTER or HONLY
			}
		}
			
    }
	
    close(classification_fd);
	
    return 0;
};

//Write a map entry
	void writeMapEntry(int mapFD, char* name, int* offset) {
			write(mapFD, name, 12);
			write(mapFD, offset, 4);
			*offset += 1;
	}



