#include <stdio.h>


void assignment1() {
    int a = 10;
    int *b = &a;
    printf("var %d, pointer %d\n", a, *b);

    *b = 13;
    printf("var %d, pointer %d\n", a, *b);
}

void assignment2() {
    int a[5] = {1,2,3,4,5};
    int* b = a;
    for (int i = 0; i < 5; ++i) {
        printf("%d ", *(b + i));
    }
    printf("\n");

    for (int i = 0; i < 5; ++i) {
        *(b + i) = i * 10;
    }

    for (int i = 0; i < 5; ++i) {
        printf("%d %d, ", a[i], *(b + i));
    }
    printf("\n");
}


void swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void assignment3() {
    int a = 10, b = 15;
    printf("a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("a = %d, b = %d\n", a, b);
}

void assignment4() {
    int a = 10;
    int *b = &a;
    int **c = &b;

    printf("%d, %d\n", *b, **c);
}

void assignment5() {
    char str[] = "Hello";
    char* a = str;
    for (int i = 0; i < 5; ++i) {
        printf("%c", *(a + i));
    }
    printf("\n");

    int len = 0;
    while(*(a + len++) != '\0'){}
    printf("len %d\n", len);
}

int main() {
    assignment1();
    assignment2();
    assignment3();
    assignment4();
    assignment5();
}