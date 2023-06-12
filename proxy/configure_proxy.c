#define _GNU_SOURCE     // GNU extension 
#include <stdlib.h>     // EXIT_FAILURE
#include <stdio.h>      // printf(), sprintf(), perror()
#include <unistd.h>     // access() 
#include <string.h>     // strtok()
#include <arpa/inet.h>  // inet_pton()
#include <ctype.h>      // isdigit()
#include <sys/socket.h> // socket(), connect()

// Check if the port value is within the valid range
int valid_port(int port) {
    if (port >= 1 && port <= 65535) {
        return 1;
    }
    return 0;
}

// Checking if the port is open
int is_port_open(char *ip, int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    // Creating a socket for a TCP connection
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error creating socket \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // IP address conversion from standard format to network format
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0) {
        printf("\n Invalid address / Address not supported \n");
        return -1;
    }

    // Trying to connect to the specified IP address and port
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nThe port is not open or the connection failed\n");
        return 0;
    }

    // Terminate the connection and return a value that means the port is open
    close(sockfd);
    return 1;
}

int main(int argc, char *argv[]) {
    int netns_fd;
    int pid;
    //Check whether the correct number of arguments was entered
    //The program expects two arguments: namespace name a proxy server address
    if (argc != 3) {
        printf("Run: %s [network_namespace] [proxy_server]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Defines paths to files that represent network namespaces
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Namespace doesn't exist.");
        return EXIT_FAILURE;
    }

    // Opening the namespace
    netns_fd = open(path,O_RDONLY);
    if (netns_fd == -1){
       perror("Could not open namespace");
       exit(EXIT_FAILURE);
    }

    // Setting the active namespace
    if (setns(netns_fd, 0) == -1){
        perror("setns");
	    exit(EXIT_FAILURE);
    }

    // We parse the IP address and port from the proxy server address
    char ip_address[256];
    int port;
    sscanf(argv[2], "http://%255[^:]:%d/", ip_address, &port);

    // Checking that the IP address and port are in valid formats
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0) {
        printf("Invalid IP address\n");
        return EXIT_FAILURE;
    }
    
    // Checking whether the port is valid
    if (!valid_port(port)) {
        printf("Invalid port\n");
        return EXIT_FAILURE;
    }

    // Check if the port is open
    if (!is_port_open(ip_address, port)) {
        printf("The port is not open or it is not possible to establish a connection with the IP address\n");
        return EXIT_FAILURE;
	}
	    char cmd[256];

    sprintf(cmd, "ip netns exec %s bash -c 'echo \"http_proxy=%s\" >> /etc/environment'", argv[1], argv[2]);

    // Executing the command through system() and checking the return value
    int result = system(cmd);
    if (result == -1) {
        perror("Error executing the command\n");
        return EXIT_FAILURE;
    } else if (result != 0) {
        printf("The command was not executed successfully\n");
        return EXIT_FAILURE;
    }

    return 0;
}

