#define _GNU_SOURCE //GNU extension
#include <stdlib.h> //EXIT_FAILURE
#include <stdio.h>  //printf(), perror(), sprintf()
#include <unistd.h> // access()

int main(int argc, char *argv[]) {
    if (argc != 2) {
        //Check whether the correct number of arguments was entered
        //The program expects argument: namespace name 
        printf("Run: %s [network_namespace]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Defines paths to files that represent network namespaces
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if(access(path, F_OK) != 0) {
        printf("Namespace %s doesn't exist.\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    char cmd[256];
    int status;

    // Create a command to display routing tables in a network namespace
    sprintf(cmd, "ip netns exec %s ip route show", argv[1]);
    status = system(cmd);
    // Command success check
    if (status != 0) {
        printf("Error executing 'ip route show' command in network namespace %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return 0;
}




