//Project 3
//Max Forbang and Martin Quezada
//This work abides by the JMU Honor Code.

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
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
}

int main (int argc, char *argv[])
{
    struct msg m;
    mqd_t mqd = open("/cs361lab2", O_CREAT | O_RONLY, 0600, NULL);

    if(mqd < 0)
    {
        fprintf("MQD UNSUCCESSFUL", strerror(errno));
        exit(1);
    }
   
        struct mq_attr attr;
    char *buffer = calloc (attr.mq_msgsize, 1);

    if((mq_receive (mqd, buffer, attr.mq_msgsize, NULL)) ! = -1)
    {
        struct msg *m = (struct msg *) buffer;
        //retrieve message fields here
    }


    //still need to find out where we get the arguments for the Queue output
    fprintf("Queue %s:\n\t Flags:\t%s\n\tMax messagesses:\t%d\n\tMax size:\t%d\n\tCurrent messages:%\td");
    
    
    //still need to get arguments from struct
    fprintf("Receieved message (%d bytes):\n\tMagic: %c\n\tID: %d\n\tTime: %s\n\tData: %s");
    
    
    
    //output for invalid message
    fprintf("Receieved message (%d bytes):\n\tInvalid message");

    
    mq_close(mqd);
}

