#define _GNU_SOURCE     //Povolenie GNU rozsireni 
#include <sched.h>      //clone()
#include <unistd.h>     //getpid()
#include <stdlib.h>     //malloc(), free(), EXIT_FAILURE
#include <stdio.h>      //sprintf(), perror(), printf()
#include <sys/types.h>  //pid_t
#include <sys/wait.h>   //waitpid() a s nim spojene makra
#include <fcntl.h> 
#include <sys/stat.h>   //mkdir()


#define STACK_SIZE (1024 * 1024) // Velkost zasobnika pre detsky proces

//Funkcia ktora sa spusti v novom procese vytvorenom clone()
static int child_func(void *arg) {
    char cmd[256];
    mkdir("/var/run/netns",0755);
    //Priprava prikazu na vytvorenie noveho sietoveho menneho priestoru
    sprintf(cmd, "touch /var/run/netns/%d &&mount --bind /proc/%d/ns/net /var/run/netns/%d",getpid(),getpid(),getpid());
    
    //Spustenie prikazu
    int status = system(cmd);
    
    //Kontrola ci prikaz bol spusteny uspesne
    if(status == -1){
        perror("Zlyhanie: systemove volanie");
        return EXIT_FAILURE;
    }
    
    return 0;
}
int main(int argc, char *argv[]) {
    pid_t pid;

    //Alokacia pamate pre zasobnik detskeho procesu
    char *stack = malloc(STACK_SIZE);

    //Kontrola ci sa pamat spravne alokovala
     if(stack == NULL){
        perror("Zlyhanie: alokacia pamate zasobiku");
        return EXIT_FAILURE;
    }

    //Vytvorenie detskeho procesu ktory ma svoj vlastny sietovy menny priestor
    pid = clone(child_func,stack+(1024 * 1024),CLONE_NEWNET | SIGCHLD,argv[1]);

    //Kontrola ci sa proces uspesne vytvoril
    if (pid == -1){
        perror("Zlyhanie: clone funkcia");
        return EXIT_FAILURE;
    }

   
    int status;

    //Cakanie kym detsky proces skonci a zachytenie navratovej hodnoty
    if(waitpid(pid, &status, 0) == -1){
	perror("Zlyhanie: waitpid");
	return EXIT_FAILURE;
    }
      

    //Kontrola ci proces skoncil normalne
    if (WIFEXITED(status)) {
        //Zachytenie ukoncovacieho statusu detskeho procesu
        const int es = WEXITSTATUS(status);
        //Ak skoncil 0 => uspech, inak => chyba
        if (es != 0) {
            printf("Zlyhanie: proces skoncil so statusom %d\n", es);
            return EXIT_FAILURE;
        }
    } else {
        printf("Zlyhanie: proces neskoncil normalne\n");
        return EXIT_FAILURE;
    }
    //Uvolnenie pamate alokovanej pre zasobnik detskeho procesu
    free(stack);
    return 0;
}

