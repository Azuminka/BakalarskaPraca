#define _GNU_SOURCE //GNU extension
#include <stdlib.h> //EXIT_FAILURE
#include <stdio.h>  //sprintf(), printf()
#include <unistd.h> //access()

int main(int argc, char *argv[]) {
    //Check whether the correct number of arguments was entered
    //The program expects two arguments: the name of the first namespace and the name of the second namespace
    if (argc != 3) {
        printf("Run: %s [network_namespace1] [network_namespace2]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Defines paths to files that represent network namespaces
    char path1[256];
    char path2[256];

    sprintf(path1, "/var/run/netns/%s", argv[1]);
    sprintf(path2, "/var/run/netns/%s", argv[2]);
    //Check whether network namespaces exist
    if(access(path1, F_OK) != 0) {
        printf("Namespace %s doesn't exist.\n", argv[1]);
        return EXIT_FAILURE;
    }

    if(access(path2, F_OK) != 0) {
        printf("Namespace %s doesn't exist.\n", argv[2]);
        return EXIT_FAILURE;
    }

    char cmd[256];
    int status;

    // Create veth pair
    sprintf(cmd, "ip link add veth1 type veth peer name veth2");
    status = system(cmd);
    if (status != 0) {
        printf("Error while creating veth pair\n");
        return EXIT_FAILURE;
    }

    // Assigning one end to a network namespace 1
    sprintf(cmd, "ip link set veth1 netns %s", argv[1]);
    status = system(cmd);
    if (status != 0) {
        printf("Error assigning veth1 to network namespace %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Assigning the other end to the network namespace 2.
    sprintf(cmd, "ip link set veth2 netns %s", argv[2]);
    status = system(cmd);
    if (status != 0) {
        printf("Error assigning veth2 to network namespace %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    // Activating veth1 in network namespace 1
    sprintf(cmd, "ip netns exec %s ip link set veth1 up", argv[1]);
    status = system(cmd);
    if (status != 0) {
        printf("Error enabling veth1 in network namespace %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Activating veth2 in network namespace 2
    sprintf(cmd, "ip netns exec %s ip link set veth2 up", argv[2]);
    status = system(cmd);
    if (status != 0) {
        printf("Error enabling veth2 in network namespace %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    return 0;
}
