#define _GNU_SOURCE     //GNU extension 
#include <sched.h>      //clone()
#include <unistd.h>     //getpid()
#include <stdlib.h>     //malloc(), free(), EXIT_FAILURE
#include <stdio.h>      //sprintf(), perror(), printf()
#include <sys/types.h>  //pid_t
#include <sys/wait.h>   //waitpid() and macros associated with it
#include <fcntl.h> 
#include <sys/stat.h>   //mkdir()


#define STACK_SIZE (1024 * 1024) // Size of stack for child process

//A function that runs in a new process created by clone()
static int child_func(void *arg) {
    char cmd[256];
    mkdir("/var/run/netns",0755);
    //Preparing a command to create a new network namespace
    sprintf(cmd, "touch /var/run/netns/%d &&mount --bind /proc/%d/ns/net /var/run/netns/%d",getpid(),getpid(),getpid());
    
    //Execution of the command
    int status = system(cmd);
    
    //Check if the command was executed successfully
    if(status == -1){
        perror("Failure: system call");
        return EXIT_FAILURE;
    }
    
    return 0;
}
int main(int argc, char *argv[]) {
    pid_t pid;

    //Allocation of memory for the child process stack
    char *stack = malloc(STACK_SIZE);

    //Checking whether the memory was allocated correctly
     if(stack == NULL){
        perror("Failure: memory stack allocation");
        return EXIT_FAILURE;
    }

    //Creating a child process that has its own network namespace
    pid = clone(child_func,stack+(1024 * 1024),CLONE_NEWNET | SIGCHLD,argv[1]);

    //Checking whether the process was created successfully
    if (pid == -1){
        perror("Failure: clone function");
        return EXIT_FAILURE;
    }
   
    int status;

    //Waiting for the child process to finish and catch the return value
    if(waitpid(pid, &status, 0) == -1){
	    perror("Failure: waitpid");
	    return EXIT_FAILURE;
    }
      

    //Checking whether the process ended normally
    if (WIFEXITED(status)) {
        //Catch the termination status of the child process
        const int es = WEXITSTATUS(status);
        //If it ended 0 => success, otherwise => error
        if (es != 0) {
            printf("Failure: the process ended with status %d\n", es);
            return EXIT_FAILURE;
        }
    } else {
        printf("Failure: the process did not end normally\n");
        return EXIT_FAILURE;
    }
    //Freeing the memory allocated for the child process stack
    free(stack);
    return 0;
}

