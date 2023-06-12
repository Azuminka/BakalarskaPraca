#define _GNU_SOURCE     //GNU extension
#include <unistd.h>     // access()
#include <stdlib.h>     //printf(), perror(), sprintf()
#include <stdio.h>      //printf(), perror(), sprintf()
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
    int ret;

    // Checks if the namespace exists
    sprintf(cmd, "/var/run/netns/%s", argv[1]);
    if (access(cmd, F_OK) == -1) {
        perror("Namespace doesn't exist.");
        return EXIT_FAILURE;
    }

    // Will try to remove the route
    sprintf(cmd, "ip netns exec %s ip route del %s", argv[1], argv[2]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error deleting route");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Failed to remove the route, the command ended with an error code:  %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    printf("Route successfully deleted\n");
    return EXIT_SUCCESS;
}


