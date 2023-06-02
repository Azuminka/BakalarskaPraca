#define _GNU_SOURCE     // Povolenie GNU rozsirenie 
#include <stdlib.h>     // EXIT_FAILURE
#include <stdio.h>      // printf(), sprintf(), perror()
#include <unistd.h>     // access() 
#include <string.h>     // strtok()
#include <arpa/inet.h>  // inet_pton()
#include <ctype.h>      // isdigit()
#include <sys/socket.h> // socket(), connect()

// Kontrola, ci je hodnota portu v platnom rozsahu
int valid_port(int port) {
    if (port >= 1 && port <= 65535) {
        return 1;
    }
    return 0;
}

// Overenie, ci je port otvoreny
int is_port_open(char *ip, int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    // Vytvorenie socketu pre TCP spojenie
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Chyba pri vytvoreni socketu \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Konverzia IP adresy zo standardného formatu na sietovy format
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0) {
        printf("\n Neplatná adresa / Adresa nie je podporovaná \n");
        return -1;
    }

    // Pokus o pripojenie na zadanu IP adresu a port
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nPort nie je otvorený alebo sa nepodarilo nadviazať spojenie\n");
        return 0;
    }

    // Ukoncenie spojenia a vratenie hodnoty, ktora znamena, ze port je otvoreny
    close(sockfd);
    return 1;
}

int main(int argc, char *argv[]) {
    int netns_fd;
    int pid;
    //Kontrola, ci bol zadany spravny pocet argumentov
    //Program ocakava dva argumenty: nazov menneho priestoru a adresu proxy servera 
    if (argc != 3) {
        printf("Použitie: %s [sieťový_menný_priestor] [proxy_server]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Definujeme cesty k suborom, ktore reprezentuju sietove menne priestory
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Sieťový menný priestor neexistuje");
        return EXIT_FAILURE;
    }

    // Otvorenie menneho priestoru
    netns_fd = open(path,O_RDONLY);
    if (netns_fd == -1){
       perror("Nepodarilo sa otvoriť menný priestor");
       exit(EXIT_FAILURE);
    }

    // Nastavenie aktívneho menneho priestoru
    if (setns(netns_fd, 0) == -1){
        perror("setns");
	    exit(EXIT_FAILURE);
    }

    // Vyparsujeme IP adresu a port z proxy servera
    char ip_address[256];
    int port;
    sscanf(argv[2], "http://%255[^:]:%d/", ip_address, &port);

    // Kontrola, či sú IP adresa a port v platných formátoch
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0) {
        printf("Neplatná IP adresa\n");
        return EXIT_FAILURE;
    }
    
    // Kontrola, ci je port platny
    if (!valid_port(port)) {
        printf("Neplatný port\n");
        return EXIT_FAILURE;
    }

    // Kontrola, ci je port otvoreny
    if (!is_port_open(ip_address, port)) {
        printf("Port nie je otvorený alebo nie je možné naviazať spojenie s IP adresou\n");
        return EXIT_FAILURE;
	}
	    char cmd[256];

    sprintf(cmd, "ip netns exec %s bash -c 'echo \"http_proxy=%s\" >> /etc/environment'", argv[1], argv[2]);

    // Vykonanie príkazu cez system() a kontrola návratovej hodnoty
    int result = system(cmd);
    if (result == -1) {
        perror("Chyba pri vykonávaní príkazu");
        return EXIT_FAILURE;
    } else if (result != 0) {
        printf("Príkaz sa nevykonal úspešne\n");
        return EXIT_FAILURE;
    }

    return 0;
}

