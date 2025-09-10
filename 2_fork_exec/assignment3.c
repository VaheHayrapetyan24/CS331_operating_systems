#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int res = fork();
    if (res == 0) {
        execl("/usr/bin/echo", "echo", "hello from the child process\n", NULL);
        perror("execl");
        return 0;
    } else if (res < 0) {
        printf("Forking failed\n");
        return 1;
    }

    printf("Parent process done \n");
}