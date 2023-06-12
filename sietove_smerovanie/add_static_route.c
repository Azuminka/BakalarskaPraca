#define _GNU_SOURCE     //GNU extension
#include <unistd.h>     //access()
#include <stdlib.h>     //EXIT_FAILURE
#include <stdio.h>      //sprintf(), perror(), printf()
#include <arpa/inet.h>  //inet_aton()
#include <string.h>     // strtok()

// Checking whether the mask value is within the valid range
int valid_mask(int mask) {
    if (mask >= 0 && mask <= 32) {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        //Check whether the correct number of arguments was entered
        //The program expects two arguments: namespace name and route
        printf("Run: %s [network_namespace] [route]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Defines paths to files that represent network namespaces
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if(access(path, F_OK) != 0) {
        printf("Namespace %s doesn't exist.\n", argv[1]);
        return EXIT_FAILURE;
    }
    // IP address and mask separation
    char *ip_address = strtok(argv[2], "/");
    char *mask_str = strtok(NULL, "/");
    
    // Checking that the IP address and mask are in valid formats
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0) {
        printf("Invalid IP address\n");
        return EXIT_FAILURE;
    }
    int mask = atoi(mask_str);
    if (!valid_mask(mask)) {
        printf("Invalid subnet mask\n");
        return EXIT_FAILURE;
    }

    char cmd[256];
    int status;

    // Create command to add static route
    sprintf(cmd, "ip netns exec %s ip route add %s dev eth0", argv[1], argv[2]);
    status = system(cmd);
    // Check the return status of the command
    if (status != 0) {
        printf("Error adding route %s in network namespace %s\n", argv[2], argv[1]);
        return EXIT_FAILURE;
    }

    return 0;
}






