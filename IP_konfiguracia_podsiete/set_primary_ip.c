#define _GNU_SOURCE     //Povolenie GNU rozsireni 
#include <unistd.h>     //access() 
#include <sys/wait.h>   //WEXITSTATUS
#include <stdlib.h>     //EXIT_FAILURE, EXIT_SUCCESS
#include <stdio.h>       //sprintf(), perror(), printf()

int main(int argc, char *argv[]) {
    if (argc != 4) {
        //Kontrola, ci bol zadany spravny pocet argumentov
        //Program ocakava dva argumenty: nazov menneho priestoru, IP adresu a masku 
        printf("Použitie: %s [sieťový_menný_priestor] [IP_adresa] [maska_podsiete]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Definujeme cesty k suborom, ktore reprezentuju sietove menne priestory
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Sieťový menný priestor neexistuje");
        return EXIT_FAILURE;
    }

    char cmd[256];
    int ret;

     // Odstránenie existujúcej IP adresy
    sprintf(cmd, "ip netns exec %s ip addr flush dev eth0", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri odstraňovaní existujúcej IP adresy");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Odstraňovanie existujúcej IP adresy zlyhalo, príkaz skončil s chybou %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // Nastavenie IP adresy a podsiete
    sprintf(cmd, "ip netns exec %s ip addr add %s/%s dev eth0", argv[1], argv[2], argv[3]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri nastavovaní IP adresy a podsiete");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Nastavovanie IP adresy a podsiete zlyhalo, príkaz skončil s chybou %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // Zapnutie sieťového zariadenia
    sprintf(cmd, "ip netns exec %s ip link set eth0 up", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri zapínaní sieťového zariadenia");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Zapínanie sieťového zariadenia zlyhalo, príkaz skončil s chybou %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    return 0;
}
