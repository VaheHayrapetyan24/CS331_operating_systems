#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int res = fork();
    if (res == 0) {
        execl("/usr/bin/ls", "ls", NULL);
        perror("execl");
        return 0;
    }

    printf("Parent process done \n");
}