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

    // Odstránenie veth paru
    sprintf(cmd, "ip netns exec %s ip link delete veth1", argv[1]);
    status = system(cmd);
    if (status != 0) {
        printf("Error removing veth1 from network namespace %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    sprintf(cmd, "ip netns exec %s ip link delete veth2", argv[2]);
    status = system(cmd);
    if (status != 0) {
        printf("Error removing veth2 from network namespace%s\n", argv[2]);
        return EXIT_FAILURE;
    }

    return 0;
}
