#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/wait.h>

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
        int wc = wait(NULL);
        printf("(pid:%d) hello, I am parent of %d (wait:%d)\n", getpid(), rc, wc);
    }
    return 0;
}
