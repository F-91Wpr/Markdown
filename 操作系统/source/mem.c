#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    printf("(%d) memory address of p: %08x\n", getpid(), p);
    *p = 0;
    while (1)
    {
        sleep(1);
        *p += 1;
        printf("(%d) p: %d\n", getpid(), *p);
    }
    return 0;
}
