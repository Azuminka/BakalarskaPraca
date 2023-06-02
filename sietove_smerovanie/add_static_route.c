#define _GNU_SOURCE     //Povolenie GNU rozsirenie
#include <unistd.h>     //access()
#include <stdlib.h>     //EXIT_FAILURE
#include <stdio.h>      //sprintf(), perror(), printf()
#include <arpa/inet.h>  //inet_aton()
#include <string.h>     // funkciu strtok()

// Kontrola, ci je hodnota masky v platnom rozsahu
int valid_mask(int mask) {
    if (mask >= 0 && mask <= 32) {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        //Kontrola, ci bol zadany spravny pocet argumentov
        //Program ocakava dva argumenty: nazov menneho priestoru a trasu 
        printf("Použitie: %s [sieťový_menný_priestor] [sieťová_trasa]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Definujeme cesty k suborom, ktore reprezentuju sietove menne priestory
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if(access(path, F_OK) != 0) {
        printf("Sieťový menný priestor %s neexistuje\n", argv[1]);
        return EXIT_FAILURE;
    }
    // Oddelenie IP adresy a masky
    char *ip_address = strtok(argv[2], "/");
    char *mask_str = strtok(NULL, "/");
    
    // Kontrola, či sú IP adresa a maska v platných formátoch
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0) {
        printf("Neplatná IP adresa\n");
        return EXIT_FAILURE;
    }
    int mask = atoi(mask_str);
    if (!valid_mask(mask)) {
        printf("Neplatná maska podsiete\n");
        return EXIT_FAILURE;
    }

    char cmd[256];
    int status;

    // Vytvorenie prikazu pre pridanie statickej trasy
    sprintf(cmd, "ip netns exec %s ip route add %s dev eth0", argv[1], argv[2]);
    status = system(cmd);
    // Kontrola navratoveho statusu prikazu
    if (status != 0) {
        printf("Chyba pri pridávaní trasy %s v sieťovom mennom priestore %s\n", argv[2], argv[1]);
        return EXIT_FAILURE;
    }

    return 0;
}






