#define _GNU_SOURCE     //GNU extension
#include <sched.h>      //setns()
#include <unistd.h>     //access(), fork()
#include <fcntl.h>      //O_RDONLY
#include <sys/wait.h>   //waitpid()
#include <stdlib.h>     //EXIT_FAILURE, EXIT_SUCCESS 
#include <stdio.h>      //printf(), perror()

int main(int argc, char *argv[]) {
    //Check whether the correct number of arguments was entered
    //The program expects an argument: namespace name
    if (argc != 2) {
        printf("Run: %s [network_namespace]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Defines paths to files that represent network namespaces
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    //Check whether network namespaces exist
    if(access(path, F_OK) != 0) {
        printf("Namespace %s doesn't exist.\n", argv[1]);
        return EXIT_FAILURE;
    }

    pid_t pid;
    int fd;
    char cmd[256];

    // Creating two processes
    pid = fork();
    if (pid == -1) {
        perror("Failed to create process");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // detsky proces - prepojenie na sietovy menny priestor
        sprintf(cmd, "/var/run/netns/%s", argv[1]);
        fd = open(cmd, O_RDONLY);
        if (fd == -1) {
            perror("Could not open namespace");
            return EXIT_FAILURE;
        }

        if (setns(fd, CLONE_NEWNET) == -1) {
            perror("Failed to switch to namespace");
            return EXIT_FAILURE;
        }

        // The child process is performing some network activity
        system("ping -c 3 8.8.8.8");
    } else {
        // The parent process waits for the child process to complete
        waitpid(pid, NULL, 0);
    }

    return EXIT_SUCCESS;
}

