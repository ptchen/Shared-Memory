/*

2/26/2017
Pei-Tzu Chen

For a server, it should be started before any client. The server should perform the following tasks:
. Ask for a shared memory with a memory key and memorize the returned shared memory ID. This is performed by system call shmget().
. Attach this shared memory to the server's address space with system call shmat().
. Initialize the shared memory, if necessary.
. Do something and wait for all clients' completion.
. Detach the shared memory with system call shmdt().
. Remove the shared memory with system call shmctl().

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/shm.h>
#include <sys/ipc.h> // for interprocess communication (IPC): messages, semaphores and shared memory

int main(){

	key_t key;
    int shmid;
    char *shm_ptr;
    int status = 1;
    char *input = (char*) malloc(sizeof(char) * BUFSIZ);
    char *keyword = "COOL";
    char *s;

    // *****************************************************************************************
    // ask for a shared memory with memory key 
    // *****************************************************************************************

    key = getuid();
    shmid = shmget(key, 1024, IPC_CREAT | 0666); 
    if(shmid == -1){
    	perror("shared memory not created successfully");
    	exit(EXIT_FAILURE);
    }

    // *****************************************************************************************
    // attach the shared memory to server's address with shmat()
    // *****************************************************************************************

    shm_ptr = shmat(shmid, NULL, 0);
    if((int)shm_ptr == -1){
    	perror("segment not attached successfully");
    	exit(EXIT_FAILURE);
    }

    // *****************************************************************************************
    // run infinite loop
    // *****************************************************************************************

    while(status){
    	
        // prompt user input
        printf("Enter an alpha numeric string: ");
    	fgets(input, BUFSIZ, stdin);

        // add $ to end of the string, indicating the end of the input
		s = shm_ptr;
		s += strlen(input);
		*s = '$';

        // *****************************************************************************************
        // "COOL" found, copy input to shared memory and wait for completion
        // *****************************************************************************************
    	if(strstr(input, keyword) != NULL){
    		memcpy(shm_ptr, input, strlen(input));

            // processor.c would add '*' at the end of the line to indicate reading process complete
            while (*shm_ptr != '*'){
                sleep(1);
            }
    	}

        // *****************************************************************************************
        // quit program, copy 'quit' to shared memory and exit infinite loop
        // *****************************************************************************************
        else if (strncmp(input, "quit", 4) == 0) {
            memcpy(shm_ptr, input, strlen(input));
            status = 0;    
        }

        // *****************************************************************************************
        // 'COOL' not found, keep prompting user input
        // *****************************************************************************************
        else{
            continue;
        }		
    }

    // *****************************************************************************************
    // dettatch shared memory
    // *****************************************************************************************
    
	if (shmdt(shm_ptr) == -1) {
		perror("shmdt failed: segment wasn't dettached from data space");
		exit(EXIT_FAILURE);
	}

	// *****************************************************************************************
    // remove shared memory
    // *****************************************************************************************

	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("shmctl failed: ipc didn't close");
		exit(EXIT_FAILURE);
	}
}
