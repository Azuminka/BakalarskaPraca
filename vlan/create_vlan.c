#define _GNU_SOURCE //Povolenie GNU rozsirenie
#include <stdlib.h> //EXIT_FAILURE
#include <stdio.h>  //sprintf(), perror(), printf()
#include <string.h> //atoi()
#include <unistd.h>    // access()

int main(int argc, char *argv[]) {
    if (argc != 3) {
        //Kontrola, ci bol zadany spravny pocet argumentov
        //Program ocakava dva argumenty: nazov menneho priestoru a VLAN id
        printf("Použitie: %s [sieťový_menný_priestor] [VLAN ID]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Prevod VLAN ID zo stringu na integer
    int vlan_id = atoi(argv[2]);   

    if (vlan_id < 1 || vlan_id > 4094) {
        printf("VLAN ID musí byť z rozsahu 1 - 4094.\n");
        return EXIT_FAILURE;
    }

    // Kontrola, ci sietovy menny priestor existuje
    char netns_path[256];
    sprintf(netns_path, "/var/run/netns/%s", argv[1]);
    if (access(netns_path, F_OK) == -1) {
        printf("Sieťový menný priestor neexistuje.\n");
        return EXIT_FAILURE;
    }

    char cmd[256];
    char buf[256];
    FILE *fp;

    // Skontrolujeme, ci uz existuje sietove rozhranie s danym VLAN ID
    sprintf(cmd, "ip link show | grep 'vlan%s@'", argv[2]);
    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Chyba pri spustení príkazu\n");
        return EXIT_FAILURE;
    }

    // Ak sa nam vrati nejaky vystup, znamena to, ze VLAN ID uz existuje
    if (fgets(buf, sizeof(buf), fp) != NULL) {
        printf("VLAN ID už existuje.\n");
        pclose(fp);
        return EXIT_FAILURE;
    }
    pclose(fp);

    // Vytvorenie VLAN rozhrania v sietovom mennom priestore
    sprintf(cmd, "ip netns exec %s ip link add link eth0 name eth0.%s type vlan id %s", argv[1], argv[2], argv[2]);
    if (system(cmd) != 0) {
        printf("Chyba pri vytváraní VLAN rozhrania\n");
        return EXIT_FAILURE;
    }

    // Nastavenie VLAN rozhrania na UP
    sprintf(cmd, "ip netns exec %s ip link set eth0.%s up", argv[1], argv[2]);
    if (system(cmd) != 0) {
        printf("Chyba pri nastavovaní VLAN rozhrania na UP\n");
        return EXIT_FAILURE;
    }

    return 0;
}
