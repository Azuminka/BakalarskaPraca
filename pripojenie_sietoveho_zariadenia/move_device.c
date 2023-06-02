#define _GNU_SOURCE     //Povolenie GNU rozsirenie
#include <stdlib.h>     //EXIT_FAILURE
#include <stdio.h>      //sprintf(), perror(), printf()
#include <unistd.h>     // access()

int main(int argc, char *argv[]) {
    char cmd[256];
    //Kontrola, ci bol zadany spravny pocet argumentov
    //Program ocakava dva argumenty: nazov menneho priestoru a nazov sietoveho zariadenia
    if (argc != 3) {
        printf("Použitie: %s [sieťový_menný_priestor] [sieťové_zariadenie]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Definujeme cesty k suborom, ktore reprezentuju sietove menne priestory
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    if (access(path, F_OK) == -1) {
        perror("Sieťový menný priestor neexistuje");
        return EXIT_FAILURE;
    }
    //Priprava prikazu na presun sietoveho zariadenia do menneho priestoru
    sprintf(cmd, "ip link set %s netns %s  ",argv[2],argv[1]);
    
    int status;
    //Spustenie prikazu + kontrola ci sa sietove zariadenie naslo
    if((status= system(cmd)) == 256){
        printf("Zlyhanie: sietove rozhranie sa nenaslo\n");
        return -1;
    }
    //Kontrola ci prikaz bol spusteny uspesne
    if(status == -1){
        perror("Zlyhanie: systemove volanie");
        return EXIT_FAILURE;
    }
    return 0;
}
