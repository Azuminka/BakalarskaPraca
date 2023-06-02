#define _GNU_SOURCE //Povolenie GNU rozsireni
#include <unistd.h> // access()
#include <stdlib.h> //printf(), perror(), sprintf()
#include <stdio.h>  //printf(), perror(), sprintf()

int main(int argc, char *argv[]) {
    if (argc != 3) {
        //Kontrola, ci bol zadany spravny pocet argumentov
        //Program ocakava dva argumenty: nazov menneho priestoru a trasu 
        printf("Použitie: %s [sieťový_menný_priestor] [sieťová_trasa]\n", argv[0]);
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


