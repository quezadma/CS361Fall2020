/* project4.c

Team: 24
Names: Max Forbang and Martin Quezada 
Honor code statement: This work complies with the JMU Honor Code.

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

#include "common.h"
#include "classify.h"
#ifdef GRADING // do not delete this or the next two lines
#include "gradelib.h"
#endif


#define NUM_PROCESSES 5

// This is the recommended struct for sending a message about
// a cluster's type through the pipe. You need not use this,
// but I would recommend that you do. 
struct msg {
    int msg_cluster_number;  // The cluster number from the input file
    unsigned char msg_cluster_type;  // The type of the above cluster
                                     // as determined by the classifier
};

int main(int argc, char *argv[])
{
	

	char cluster_data[CLUSTER_SIZE];
	unsigned char classification;
    int input_fd;
    int classification_fd;
    pid_t pid;
    int pipefd[2];
    struct msg message;
    int start_cluster; // When a child process is created, this variable must
                       // contain the first cluster in the input file it
                       // needs to classify
    int clusters_to_process; // This variable must contain the number of
                             // clusters a child process needs to classify

    // The user must supply a data file to be used as input
    if (argc != 2) {
        printf("Usage: %s data_file\n", argv[0]);
        return 1;
    }

    // Open classification file for writing. Create file if it does not
    // exist. Exit with error if open() fails.
    classification_fd = open(CLASSIFICATION_FILE, O_WRONLY | O_CREAT, 0600);
    if (classification_fd < 0) {
        printf("Error creating file \"%s\": %s\n", CLASSIFICATION_FILE, strerror(errno));
        return 1;
    }


    // Try opening the file for reading, exit with appropriate error message
    // if open fails
    input_fd = open(argv[1], O_RDONLY);
    if (input_fd < 0) {
        printf("Error opening file \"%s\" for reading: %s\n", argv[1], strerror(errno));
        return 1;
    }



    // Create the pipe here. Exit with error if this fails.
    if( pipe(pipefd) < 0)
    {
        printf("Error creating pipe: %s\n", strerror(errno));
        return 1;
    }

	//getting number of clusters from size of the file
    int fileSize, numClusters;
	
	fileSize = lseek(input_fd, 0, SEEK_END);
	lseek(input_fd, 0, SEEK_SET);
	
	numClusters = fileSize / CLUSTER_SIZE;

    

    //creating variables to use for children logic
	int clusters_per_child = numClusters / NUM_PROCESSES;   //find how many clusters should be distributed to each child
    int extraClusters = numClusters % NUM_PROCESSES;		//if clusters aren't evenly divisible, find how many are left over
    start_cluster = 0;

    // The pipe must be created at this point
#ifdef GRADING // do not delete this or you will lose points
    test_pipefd(pipefd, 0);
#endif


    // Fork NUM_PROCESS number of child processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
		
		//calculate start cluster before generating child
		if (extraClusters >= i) {
			start_cluster = i*clusters_per_child + i;
		}
		else if (i != 0) {
			start_cluster = extraClusters*(clusters_per_child + 1) + (i-extraClusters)*clusters_per_child;
		}
		
		//calculate number of clusters to calculate based on whether there are extra clusters or not
		if(extraClusters > i) {
				clusters_to_process = clusters_per_child + 1;       
			} else {
				clusters_to_process = clusters_per_child;
			}
			
        pid = fork();
        // Exit if fork() fails.
        if (pid == -1)
            exit(1);
        // This is the place to write the code for the child processes
        else if (pid == 0) {
			printf("%d\n", start_cluster);
			printf("%d\n", clusters_to_process);

			// In this else if block, you need to implement the entire logic
            // for the child processes to be aware of which clusters
            // they need to process, classify them, and create a message
            // for each cluster to be written to the pipe.
			

            // At this point, the child must know its start cluster and
            // the number of clusters to process.
#ifdef GRADING // do not delete this or you will lose points
            printf("Child process %d\n\tStart cluster: %d\n\tClusters to process: %d\n",
              getpid(), start_cluster, clusters_to_process);
#endif

            close(pipefd[0]);
            // At this point the pipe must be fully set up for the child
            // This code must be executed before you start iterating over the input
            // file and before you generate and write messages.
#ifdef GRADING // do not delete this or you will lose points
            test_pipefd(pipefd, getpid());
#endif
			
			

            // Implement the main loop for the child process below this line
			
			
			lseek(input_fd, start_cluster * CLUSTER_SIZE, SEEK_SET);

            for(int j = 0; j < clusters_to_process; j++)
            {
				read(input_fd, &cluster_data, CLUSTER_SIZE);
				classification = TYPE_UNCLASSIFIED;
        
		
				//Code from project 3
				
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
				
				
				//assign message correct attributes and then write message to pipe
				struct msg currentMsg;				
				currentMsg.msg_cluster_number = start_cluster + j;        
				currentMsg.msg_cluster_type = classification;
                write(pipefd[1], &currentMsg, sizeof(message));
            }
            
            close(pipefd[1]);
           
            exit(0); // This line needs to be the last one for the child
                     // process code. Do not delete this!
        }

    }

    // All the code for the parent's handling of the messages and
    // creating the classification file needs to go in the block below

	
	//close(pipefd[1]);
	
    // At this point, the pipe must be fully set up for the parent
#ifdef GRADING // do not delete this or you will lose points
    test_pipefd(pipefd, 0);
#endif

    // Read one message from the pipe at a time
    while (read(pipefd[0], &message, sizeof(message)) > 0) {
        // In this loop, you need to implement the processing of
        // each message sent by a child process. Based on the content,
        // a proper entry in the classification file needs to be written.
		
		lseek(classification_fd, message.msg_cluster_number, SEEK_SET);
		write(classification_fd, &message.msg_cluster_type, 1);
		
    }
	close(pipefd[0]);
    close(classification_fd); // close the file descriptor for the
                              // classification file
    return 0;
};

