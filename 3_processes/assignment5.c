#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int res = fork();
    if (res == 0) {
        // sleep(1);
        for (int i = 1000000000; i > 0; --i){}
        printf("Child process pid %d\n", getpid());
        return 0;
    } else if (res < 0) {
        printf("Forking failed\n");
        return 1;
    }
    
    wait(NULL);
    // sleep(10);


    printf("Parent process pid %d exited\n", getpid());
}