#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("(pid:%d) hello world\n", getpid());
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        printf("(pid:%d) hello, I am child\n", getpid());
    }
    else
    {
        printf("(pid:%d) hello, I am parent of %d\n", getpid(), rc);
    }
    return 0;
}
