#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_COUNT 5

const char* root_directory_path;
int chroot_executed = 0;

void print_root_directory(const char *message) {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("getcwd");
        exit(1);
    }

    printf("%s: %s\n", message, buffer);
}

void* new_thread(void *arg) {
    printf("=== Thread %ld ===\n", (long)arg);

    print_root_directory("Root directory before chroot");


    if ((long)arg == 0 && chroot_executed == 0) {
        if(chroot("/home/kali/odpoved") != 0) {
            perror("chroot");
            exit(1);
        }

        if(chdir("/") != 0) {
            perror("chdir");
            exit(1);
        }

        chroot_executed = 1;
    }

    print_root_directory("Root directory after chroot");

    return NULL;
}

int main() {
    pthread_t thread_ids[THREAD_COUNT];

    root_directory_path = "/";

    for (long i = 0; i < THREAD_COUNT; ++i) {
        if(pthread_create(&thread_ids[i], NULL, new_thread, (void*)i) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        if(pthread_join(thread_ids[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    return 0;
}
