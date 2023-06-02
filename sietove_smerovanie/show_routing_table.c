#define _GNU_SOURCE //Povolenie GNU rozsireni 
#include <stdlib.h> //EXIT_FAILURE
#include <stdio.h>  //printf(), perror(), sprintf()
#include <unistd.h> // access()

int main(int argc, char *argv[]) {
    if (argc != 2) {
        //Kontrola, ci bol zadany spravny pocet argumentov
        //Program ocakava dva argumenty: nazov menneho priestoru a trasu 
        printf("Použitie: %s [sieťový_menný_priestor]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Definujeme cesty k suborom, ktoré reprezentuju sietove menne priestory
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if(access(path, F_OK) != 0) {
        printf("Sieťový menný priestor %s neexistuje\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    char cmd[256];
    int status;

    // Vytvorenie príkazu pre zobrazenie smerovacích tabuliek v sieťovom mennom priestore
    sprintf(cmd, "ip netns exec %s ip route show", argv[1]);
    status = system(cmd);
    // Kontrola uspesnosti prikazu
    if (status != 0) {
        printf("Chyba pri vykonávaní príkazu 'ip route show' v sieťovom mennom priestore %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return 0;
}




