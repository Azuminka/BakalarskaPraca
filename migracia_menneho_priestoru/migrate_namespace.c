#define _GNU_SOURCE     //Povolenie GNU rozsireni
#include <sched.h>      //setns()
#include <unistd.h>     //access(), fork()
#include <fcntl.h>      //O_RDONLY
#include <sys/wait.h>   //waitpid()
#include <stdlib.h>     //EXIT_FAILURE, EXIT_SUCCESS 
#include <stdio.h>      //printf(), perror()

int main(int argc, char *argv[]) {
    //Kontrola, ci bol zadany spravny pocet argumentov
    //Program ocakava argument: nazov menneho priestoru
    if (argc != 2) {
        printf("Použitie: %s [sieťový_menný_priestor]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Definujeme cesty k suborom, ktoré reprezentuju sietove menne priestory
    char path[256];
    sprintf(path, "/var/run/netns/%s", argv[1]);
    //Kontrola, ci sietove menne priestory existuju
    if(access(path, F_OK) != 0) {
        printf("Sieťový menný priestor %s neexistuje\n", argv[1]);
        return EXIT_FAILURE;
    }

    pid_t pid;
    int fd;
    char cmd[256];

    // vytvorenie dvoch procesov
    pid = fork();
    if (pid == -1) {
        perror("Nepodarilo sa vytvoriť proces");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // detsky proces - prepojenie na sietovy menny priestor
        sprintf(cmd, "/var/run/netns/%s", argv[1]);
        fd = open(cmd, O_RDONLY);
        if (fd == -1) {
            perror("Nepodarilo sa otvorit menný priestor");
            return EXIT_FAILURE;
        }

        if (setns(fd, CLONE_NEWNET) == -1) {
            perror("Nepodarilo sa prepnúť do menného priestoru");
            return EXIT_FAILURE;
        }

        // detsky proces vykonava nejaku sietovu aktivitu
        system("ping -c 3 8.8.8.8");
    } else {
        // rodicovsky proces caka na dokoncenie detinskeho procesu
        waitpid(pid, NULL, 0);
    }

    return EXIT_SUCCESS;
}

