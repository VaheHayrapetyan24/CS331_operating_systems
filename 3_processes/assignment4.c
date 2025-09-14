#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int children[2];
    int statuses[2];

    for (int i = 0; i < 2; ++i) {
        children[i] = fork();
        if (children[i] == 0) {
            sleep(i * 10);
            printf("Child process pid %d\n", getpid());
            exit(i);
        } else if (children[i] < 0) {
            printf("Forking failed\n");
            return 1;
        }
    }

    int cont = 1;
    while (cont) {
        int j = 0;
        for (int i = 0; i < 2; ++i) {
            if (statuses[i] != 0) continue;
            int res = waitpid(children[i], statuses + i, WNOHANG);
            if (res == 0) continue;
            ++j;
        }

        if (j == 2) break;
        sleep(1);
    }
    

    for (int i = 0; i < 2; ++i)
        printf("Parent process pid %d, child process pid %d, exit status %d\n", getpid(), children[i], WEXITSTATUS(statuses[i]));
}