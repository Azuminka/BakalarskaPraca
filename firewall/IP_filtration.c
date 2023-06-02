#define _GNU_SOURCE     // Povolenie GNU rozsirenie 
#include <unistd.h>     // access() 
#include <stdlib.h>     // EXIT_FAILURE
#include <stdio.h>      // printf(), sprintf(), perror()
#include <arpa/inet.h>  // inet_pton()

int main(int argc, char *argv[]) {
    //Kontrola, ci bol zadany spravny pocet argumentov
    //Program ocakava dva argumenty: nazov menneho priestoru, IP adresa 
    if (argc != 3) {
        printf("Použitie: %s [sieťový_menný_priestor] [IP_adresa]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Definujeme cesty k suborom, ktore reprezentuju sietove menne priestory
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Sieťový menný priestor neexistuje");
        return EXIT_FAILURE;
    }

    // Kontrola, či je IP adresa vo validnom formáte
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, argv[2], &(sa.sin_addr)) == 0) {
        printf("Neplatná IP adresa\n");
        return EXIT_FAILURE;
    }

    char cmd[256];
    int ret;
    
    // Nastavenie pravidiel pre firewall v sieťovom mennom priestore
    sprintf(cmd, "ip netns exec %s iptables -A OUTPUT -d %s -p icmp --icmp-type echo-request -j DROP", argv[1], argv[2]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri nastavovaní pravidiel pre firewall");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Nastavovanie pravidiel pre firewall zlyhalo, príkaz skončil s chybou %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    return 0;
}
