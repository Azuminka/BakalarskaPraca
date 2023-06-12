#define _GNU_SOURCE     //GNU extension
#include <unistd.h>     //access() 
#include <sys/wait.h>   //WEXITSTATUS
#include <stdlib.h>     //EXIT_FAILURE, EXIT_SUCCESS
#include <stdio.h>      //sprintf(), perror(), printf()
#include <arpa/inet.h>  // inet_pton()
#include <string.h>     //atoi()

int main(int argc, char *argv[]) {
    if (argc != 4) {
        //Check whether the correct number of arguments was entered
        //The program expects three arguments: namespace name, IP address a mask 
        printf("Run: %s [network_namespace] [IP_address] [subnet_mask]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Defines paths to files that represent network namespaces
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Namespace doesn't exist.\n");
        return EXIT_FAILURE;
    }

    // Check if the mask is valid
    int mask = atoi(argv[3]);   
    if (mask <= 0 || mask >= 32) {
        printf("Invalid subnet mask\n");
        return EXIT_FAILURE;
    }

    // Checking whether the IP address is in a valid format
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, argv[2], &(sa.sin_addr)) == 0) {
        printf("Invalid IP address\n");
        return EXIT_FAILURE;
    }
    
    
    char cmd[256];
    int ret;

     // Removing an existing IP address
    sprintf(cmd, "ip netns exec %s ip addr flush dev eth0", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error while deleting existing IP address");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Failed to delete an existing IP address, the command ended with an error %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // IP address and subnet settings
    sprintf(cmd, "ip netns exec %s ip addr add %s/%s dev eth0", argv[1], argv[2], argv[3]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error while setting IP address and subnet");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Setting the IP address and subnet failed, the command ended with an error %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // Turn on the network device
    sprintf(cmd, "ip netns exec %s ip link set eth0 up", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error turning on network device");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Failed to turn on network device, command ended with error %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    return 0;
}
