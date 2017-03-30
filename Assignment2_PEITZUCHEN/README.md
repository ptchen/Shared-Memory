Two simple C programs that communicate with each other via shared memory. 

>> The first program receiver.c  runs in an infinite loop receiving alpha numeric strings as input from the user, one line at a time. 
After reading one line from the standard input, this program sends this information to the other program only if the line contains the secret code "C00L". 
The sharing of data between the two processes should take place via shared memory. 

>> The second program processor.c creates an output file secrets.out and waits for user input to be sent by the receiver program. 
As soon as one line is received from the receiver, it counts the number of digits in that line and dumps the digit count along with the original line in the secrets.out file. 
This program also runs in an infinite loop.



**Tested both programs on MAC terminal, two windows simultaneously
// compile (on MAC terminal)
gcc receiver.c and gets a.out
gcc processor.c -o b.out and gets b.out
// execute
> run receiver.c(a.out) first then processor.c(b.out) and make sure
the shared memory created
> wait for the next prompt line to input new sentences (if "COOL" is
detected, might be a bit slow)
> enter "quit" to quit both receiver.c and processor.c
> cat secrets.c to check that both c files work fine and correctly