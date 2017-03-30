/*

2/26/2017
Pei-Tzu Chen

For the client part, the procedure is almost the same:
. Ask for a shared memory with the same memory key and memorize the returned shared memory ID.
. Attach this shared memory to the client's address space.
. Use the memory.
. Detach all shared memory segments, if necessary.
. Exit.

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
	int count;
	char line[BUFSIZ];
	char *s;

	// *****************************************************************************************
    // create ouput file "secrets.out"
    // *****************************************************************************************

	FILE *fp;
	fp = fopen("secrets.out", "w");

	// *****************************************************************************************
    // ask for shared memory with the same key
    // *****************************************************************************************

	key = getuid();
	shmid = shmget(key, 1024, 0666);
	if(shmid == -1){
    	perror("shared memory not located successfully");
    	exit(EXIT_FAILURE);
    }

    // *****************************************************************************************
    // attach to address space
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
    	
        // wait for new input
        if(*shm_ptr != (int)NULL){
    		
            // wait for the next input
            if(*shm_ptr != '*'){
    			
    			count = 0;
                // reset line into all zeros
    			memset(line, 0, sizeof(line));

                // copy every word into line
                for(s = shm_ptr; *s != '$'; ++s){
    				line[count] = *s;
    				count++;
    			}

                // if "quit" is received
                if (strncmp(line, "quit", 4) == 0) {
                    *shm_ptr = '*';
                    status = 0;
                    break;
                }

    			*shm_ptr = '*';

    			fp = fopen("secrets.out", "a");

    			if(fp != NULL){
                    fputs("This line: ", fp);
                    fputs(line, fp);
                    fputs("contains ", fp);
    				fprintf(fp, "%d", --count);
                    fputs(" words\n\n", fp);
    				fclose(fp);
    			}
			}
    	}
	}

	// *****************************************************************************************
    // dettatch shared memory
    // *****************************************************************************************
	if (shmdt(shm_ptr) == -1) {
		perror("shmdt failed: segment wasn't detached from data space");
		exit(EXIT_FAILURE);
	}

}