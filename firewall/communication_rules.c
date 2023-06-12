#define _GNU_SOURCE     // GNU extension 
#include <unistd.h>     // access() 
#include <stdlib.h>     // EXIT_FAILURE
#include <stdio.h>      // printf(), sprintf(), perror()

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
    if (access(path, F_OK) == -1) {
        perror("Namespace doesn't exist.");
        return EXIT_FAILURE;
    }

    char cmd[256];
    int ret;

    // Setting the INPUT DROP rule for the firewall
    sprintf(cmd, "ip netns exec %s iptables -P INPUT DROP", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error setting INPUT DROP rule");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Setting the INPUT DROP rule failed, the command ended with an error %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // Nastavenie pravidla OUTPUT ACCEPT pre firewall
    sprintf(cmd, "ip netns exec %s iptables -P OUTPUT ACCEPT", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error setting the OUTPUT ACCEPT rule");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Setting the OUTPUT ACCEPT rule failed, the command ended with an error %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // Setting the FORWARD DROP rule for the firewall
    sprintf(cmd, "ip netns exec %s iptables -P FORWARD DROP", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Error setting the FORWARD DROP rule");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Setting the FORWARD DROP rule failed, the command ended with an error %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    return 0;
}
