#define _GNU_SOURCE     //GNU extension
#include <stdlib.h>     //EXIT_FAILURE
#include <stdio.h>      //sprintf(), perror(), printf()
#include <unistd.h>     // access()

int main(int argc, char *argv[]) {
    char cmd[256];
    //Check whether the correct number of arguments was entered
    //The program expects two arguments: namespace name a network interface
    if (argc != 3) {
        printf("Run: %s [network_namespace] [network_interface]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Defines paths to files that represent network namespaces
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Namespace doesn't exist.");
        return EXIT_FAILURE;
    }
    //Preparing the command to move the network interface to the namespace
    sprintf(cmd, "ip link set %s netns %s  ",argv[2],argv[1]);

    int status;

    //Running the command + checking whether the network interface is found
    if((status= system(cmd)) == 256){
        printf("Failure: The network interface was not found\n");
        return -1;
    }
    //Checking whether the command was executed successfully
    if(status == -1){
        perror("Failure: system call");
        return EXIT_FAILURE;
    }
    return 0;
}
