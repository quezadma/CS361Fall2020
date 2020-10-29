/* project4.c

Team: 
Names: 
Honor code statement: 

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
    if( pipe(fd_pipe) < 0)
    {
        printf("Error creating pipe");
        exit(1);
    }

    //method of getting the number of clusters there are
    int counter =0;
    while ((bytes_read = read(input_fd, &cluster_data, CLUSTER_SIZE)) > 0) 
    {    
        counter++;
    }


    //block to see if theres an even number of clusters for procecesses
    //can use the flag in an if statement when sending messages to add an 
    //extra cluster to a process if needed
    int num = counter %5;
    bool even = true;
    if(num != 0)
    {
        even = false;
    }

    //write info child needs to read. still needs to get the info from input file.
    //Loop may need to go somewhere else
    for(int i =0; i < NUM_PROCESSES; i++)
    {
        write(fd_pipe[1], &msg, sizeof(msg));
        wait(10);
    } 


    // The pipe must be created at this point
#ifdef GRADING // do not delete this or you will lose points
    test_pipefd(pipefd, 0);
#endif


    // Fork NUM_PROCESS number of child processes
    for (int i = 0; i < NUM_PROCESSES; i++) {

        pid = fork();
        // Exit if fork() fails.
        if (pid == -1)
            exit(1);
        // This is the place to write the code for the child processes
        else if (pid == 0) {

            // In this else if block, you need to implement the entire logic
            // for the child processes to be aware of which clusters
            // they need to process, classify them, and create a message
            // for each cluster to be written to the pipe.


            //not sure if this is right
            //read info sent from parent and then close reading pipe-AQ
            read(fd_pipe[0], &msg, sizeof(msg));
            start_cluster = msg.msg_cluster_number;
            //clusters_to_process = msg.msg_cluster_type;
            close(fd_pipe[0]);
            
            

            // At this point, the child must know its start cluster and
            // the number of clusters to process.
#ifdef GRADING // do not delete this or you will lose points
            printf("Child process %d\n\tStart cluster: %d\n\tClusters to process: %d\n",
              getpid(), start_cluster, clusters_to_process);
#endif


            // At this point the pipe must be fully set up for the child
            // This code must be executed before you start iterating over the input
            // file and before you generate and write messages.
#ifdef GRADING // do not delete this or you will lose points
            test_pipefd(pipefd, getpid());
#endif


            // Implement the main loop for the child process below this line
            write(fd_pipe[1]),
            exit(0); // This line needs to be the last one for the child
                     // process code. Do not delete this!
        }

    }

    // All the code for the parent's handling of the messages and
    // creating the classification file needs to go in the block below


    // At this point, the pipe must be fully set up for the parent
#ifdef GRADING // do not delete this or you will lose points
    test_pipefd(pipefd, 0);
#endif

    // Read one message from the pipe at a time
    while (read(pipefd[0], &message, sizeof(message)) > 0) {
        // In this loop, you need to implement the processing of
        // each message sent by a child process. Based on the content,
        // a proper entry in the classification file needs to be written.
    }

    close(classification_fd); // close the file descriptor for the
                              // classification file
    return 0;
};
