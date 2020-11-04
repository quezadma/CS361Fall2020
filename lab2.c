//Lab 2
//Max Forbang and Martin Quezada
//This work abides by the JMU Honor Code.

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <mqueue.h>

struct msg {
    char magic[8];
    uint32_t id;
    char now[200];    
    char data[36];
};

int main (int argc, char *argv[])
{
	
	
	char correctMagic[8] = "cs361lab";
	int magicCorrect = 1;		//set to 1 if magic array is correct, set to 0 if not
    char *name = argv[1];		//name of queue to receive messages from specified in the command line
    struct msg *m;				//struct to store message from queue
	struct mq_attr attr;		//struct to store attributes of queue
	
    mqd_t mqd = mq_open(name, O_RDONLY, 0600, NULL); 	//opening queue

	//exit if there's an error opening queue
    if(mqd < 0)
    {
        printf("MQD UNSUCCESSFUL: %s", strerror(errno));
        exit(1);
    }
   
    
	//exit if there's an error getting attributes
    if(mq_getattr(mqd, &attr) == -1)
    {
        printf("Error with attr: %s", strerror(errno));
    }
	
	//create memory message
    char *buffer = calloc(attr.mq_msgsize, 1);
    ssize_t bytes;

	//receive message, exit if failure
    if( (bytes = mq_receive(mqd, buffer, attr.mq_msgsize, NULL)) != -1)
    {
        m = (struct msg *) buffer;
    }
    else
    {
        printf("Problem receiving message: %s", strerror(errno));
        exit(1);
    }

	//print queue attributes
    printf("Queue %s:\n\tFlags:\t%ld\n\tMax messages:\t%ld\n\tMax size:\t%ld\n\tCurrent messages:\t%ld\n\n", name, attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
    
	//check if magic array is correct
	for(int i = 0; i < 8; i++) {
		if (m->magic[i] != correctMagic[i])
		{
			magicCorrect = 0;
			break;
		}
	}
	
	
	//If magicCorrect flag hasn't been set to 0, then output message contents. Otherwise, print output for an invalid message.
    if(magicCorrect == 1) {
       printf("Receieved message (%ld bytes):\n\tMagic: %s\n\tID: %d\n\tTime: %s\n\tData: %s", bytes, "cs361lab", m->id, m->now, m->data);
    }
    else
    {
        //output for invalid message
       printf("Receieved message (%ld bytes):\n\tInvalid message", bytes);
    }

    free(buffer);
    mq_close(mqd);
}

