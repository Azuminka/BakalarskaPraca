#define _GNU_SOURCE     // Povolenie GNU rozsirenie 
#include <unistd.h>     // access() 
#include <stdlib.h>     // EXIT_FAILURE
#include <stdio.h>      // printf(), sprintf(), perror()

int main(int argc, char *argv[]) {
        //Kontrola, ci bol zadany spravny pocet argumentov
        //Program ocakava argument: nazov menneho priestoru 
    if (argc != 2) {
        printf("Použitie: %s [sieťový_menný_priestor]\n", argv[0]);
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

    // Nastavenie pravidla INPUT DROP pre firewall
    sprintf(cmd, "ip netns exec %s iptables -P INPUT DROP", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri nastavovaní pravidla INPUT DROP");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Nastavovanie pravidla INPUT DROP zlyhalo, príkaz skončil s chybou %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // Nastavenie pravidla OUTPUT ACCEPT pre firewall
    sprintf(cmd, "ip netns exec %s iptables -P OUTPUT ACCEPT", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri nastavovaní pravidla OUTPUT ACCEPT");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Nastavovanie pravidla OUTPUT ACCEPT zlyhalo, príkaz skončil s chybou %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    // Nastavenie pravidla FORWARD DROP pre firewall
    sprintf(cmd, "ip netns exec %s iptables -P FORWARD DROP", argv[1]);
    ret = system(cmd);
    if (ret == -1) {
        perror("Chyba pri nastavovaní pravidla FORWARD DROP");
        return EXIT_FAILURE;
    } else if (WEXITSTATUS(ret) != 0) {
        printf("Nastavovanie pravidla FORWARD DROP zlyhalo, príkaz skončil s chybou %d\n", WEXITSTATUS(ret));
        return EXIT_FAILURE;
    }

    return 0;
}
