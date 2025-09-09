#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {

    char a[2][5] = {
        "ls",
        "date"
    };
    int res = 1;
    for (int i = 0; i < 2; ++i) {
        printf("%s pid %d\n", a[i], getpid());
        res = fork();
        if (res == 0) {
            char b[15] = "/usr/bin/";
            strcat(b, a[i]);
            execl(b, a[i], NULL);
            perror("execl");
        }
    }


    printf("Parent process done \n");
}