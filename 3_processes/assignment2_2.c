#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int res = fork();
    if (res == 0) {
        printf("Child process pid %d\n", getpid());
        exit(1);
    } else if (res < 0) {
        printf("Forking failed\n");
        return 1;
    }

    int child_status;
    waitpid(res, &child_status, 0);

    printf("Parent process pid %d, child process pid %d, child wait result %d\n", getpid(), res, WEXITSTATUS(child_status));
}