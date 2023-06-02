#define _GNU_SOURCE //Povolenie GNU rozsirenie
#include <stdlib.h> //EXIT_FAILURE
#include <stdio.h>  //sprintf(), printf()
#include <unistd.h> //access()

int main(int argc, char *argv[]) {
    //Kontrola, ci bol zadany spravny pocet argumentov
    //Program ocakava dva argumenty: nazov prveho menneho priestoru a nazov druheho menneho priestoru
    if (argc != 3) {
        printf("Použitie: %s [sieťový_menný_priestor1] [sieťový_menný_priestor2]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Definujeme cesty k suborom, ktoré reprezentuju sietove menne priestory
    char path1[256];
    char path2[256];

    sprintf(path1, "/var/run/netns/%s", argv[1]);
    sprintf(path2, "/var/run/netns/%s", argv[2]);
    //Kontrola, ci sietove menne priestory existuju
    if(access(path1, F_OK) != 0) {
        printf("Sieťový menný priestor %s neexistuje\n", argv[1]);
        return EXIT_FAILURE;
    }

    if(access(path2, F_OK) != 0) {
        printf("Sieťový menný priestor %s neexistuje\n", argv[2]);
        return EXIT_FAILURE;
    }

    char cmd[256];
    int status;

    // Odstránenie veth paru
    sprintf(cmd, "ip netns exec %s ip link delete veth1", argv[1]);
    status = system(cmd);
    if (status != 0) {
        printf("Chyba pri odstraňovaní veth1 z sieťového menného priestoru %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    sprintf(cmd, "ip netns exec %s ip link delete veth2", argv[2]);
    status = system(cmd);
    if (status != 0) {
        printf("Chyba pri odstraňovaní veth2 z sieťového menného priestoru %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    return 0;
}
