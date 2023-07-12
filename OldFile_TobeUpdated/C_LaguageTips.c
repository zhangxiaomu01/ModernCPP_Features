#include"stdio.h"
#include"stdlib.h"
#include"string.h"

int B[2][3];
//p is a pointer points to B[0], B[0] is a 1 D array!
int (*p)[3] = B;

int *test = (int*)malloc(20 * sizeof(int));


int main(){
    char c[20] = "Hello world!";
    printf("Test %s", c);
    printf("Test%d", 23);
}

