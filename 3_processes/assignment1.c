#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int res = fork();
    if (res == 0) {
        printf("Child process pid %d\n", getpid());
        return 0;
    } else if (res < 0) {
        printf("Forking failed\n");
        return 1;
    }

    printf("Parent process pid %d\n", getpid());
}