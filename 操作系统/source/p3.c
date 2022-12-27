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
        char *myArgs[3];
        myArgs[0] = "wc";
        myArgs[1] = "p3.c";
        myArgs[2] = NULL;
        execvp(myArgs[0], myArgs);
        printf("this shouldn't print out");
    }
    else
    {
        int wc = wait(NULL);
        printf("(pid:%d) hello, I am parent of %d (wait:%d)\n", getpid(), rc, wc);
    }
    return 0;
}