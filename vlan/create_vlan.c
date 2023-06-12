#define _GNU_SOURCE //GNU extension
#include <stdlib.h> //EXIT_FAILURE
#include <stdio.h>  //sprintf(), perror(), printf()
#include <string.h> //atoi()
#include <unistd.h>    // access()

int main(int argc, char *argv[]) {
    if (argc != 3) {
        //Check whether the correct number of arguments was entered
        //The program expects two arguments: namespace name and VLAN id
        printf("Run: %s [network_namespace] [VLAN ID]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Conversion of VLAN ID from string to integer
    int vlan_id = atoi(argv[2]);   

    if (vlan_id < 1 || vlan_id > 4094) {
        printf("VLAN ID must be in range 1 - 4094.\n");
        return EXIT_FAILURE;
    }

    // Check if the network namespace exists
    char netns_path[256];
    sprintf(netns_path, "/var/run/netns/%s", argv[1]);
    if (access(netns_path, F_OK) == -1) {
        printf("The network namespace does not exist.\n");
        return EXIT_FAILURE;
    }

    char cmd[256];
    char buf[256];
    FILE *fp;

    // Check if a network interface with the given VLAN ID already exists
    sprintf(cmd, "ip link show | grep 'vlan%s@'", argv[2]);
    if ((fp = popen(cmd, "r")) == NULL) {
        printf("An error occurred while running the command.\n");
        return EXIT_FAILURE;
    }

    // If we get any output, it means that the VLAN ID already exists
    if (fgets(buf, sizeof(buf), fp) != NULL) {
        printf("VLAN ID already exists.\n");
        pclose(fp);
        return EXIT_FAILURE;
    }
    pclose(fp);

    // Create VLAN interface in the network namespace
    sprintf(cmd, "ip netns exec %s ip link add link eth0 name eth0.%s type vlan id %s", argv[1], argv[2], argv[2]);
    if (system(cmd) != 0) {
        printf("Error while creating VLAN interface\n");
        return EXIT_FAILURE;
    }

    // VLAN interface setting to UP
    sprintf(cmd, "ip netns exec %s ip link set eth0.%s up", argv[1], argv[2]);
    if (system(cmd) != 0) {
        printf("Error setting VLAN interface to UP\n");
        return EXIT_FAILURE;
    }

    return 0;
}
