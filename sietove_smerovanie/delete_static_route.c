#define _GNU_SOURCE //Povolenie GNU rozsirenie
#include <unistd.h> // access()
#include <stdlib.h> //printf(), perror(), sprintf()
#include <stdio.h>  //printf(), perror(), sprintf()
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
    int ret;

    // Skontroluje, či sieťový menný priestor existuje
    sprintf(cmd, "/var/run/netns/%s", argv[1]);
    if (access(cmd, F_OK) == -1) {
        perror("Sieťový menný priestor neexistuje");
        return EXIT_FAILURE;
    }

    // Pokúsi sa odstrániť trasu
    sprintf(cmd, "ip netns exec %s ip route del %s", argv[1], argv[2]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri odstraňovaní trasy");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Odstraňovanie trasy zlyhalo, príkaz skončil chybovým kódom:  %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    printf("Trasa úspešne odstránená\n");
    return EXIT_SUCCESS;
}


