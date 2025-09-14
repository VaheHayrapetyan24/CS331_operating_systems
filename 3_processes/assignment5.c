#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


void print_positive() {
    printf("the world is not going to end\n");
}

void print_negative() {
    printf("everything printed after me is a lie\n");
}

int main() {
    int res = fork();

    atexit(print_positive);
    atexit(print_negative);
    if (res == 0) {
        printf("Child process pid %d\n", getpid());
        exit(1);
    } else if (res < 0) {
        printf("Forking failed\n");
        return 1;
    }


    printf("Parent process pid %d exited\n", getpid());
}