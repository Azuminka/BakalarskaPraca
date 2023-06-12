#define _GNU_SOURCE     // GNU extension 
#include <unistd.h>     // access() 
#include <stdlib.h>     // EXIT_FAILURE
#include <stdio.h>      // printf(), sprintf(), perror()
#include <arpa/inet.h>  // inet_pton()

int main(int argc, char *argv[]) {
    //Check whether the correct number of arguments was entered
    //The program expects two arguments: namespace name, IP address 
    if (argc != 3) {
        printf("Run: %s [network_namespace] [IP_address]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Defines paths to files that represent network namespaces
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Namespace doesn't exist.");
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
    
    // Setting firewall rules in the network namespace
    sprintf(cmd, "ip netns exec %s iptables -A OUTPUT -d %s -p icmp --icmp-type echo-request -j DROP", argv[1], argv[2]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error setting firewall rules");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Setting firewall rules failed, command ended with error %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    return 0;
}
