#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

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


