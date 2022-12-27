#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// #include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        close(STDOUT_FILENO);
        open("./tmp/p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        char *myArgs[3];
        myArgs[0] = "wc";
        myArgs[1] = "p4.c";
        myArgs[2] = NULL;
        execvp(myArgs[0], myArgs);
        printf("this shouldn't print out");
    }
    else
    {
        int wc = wait(NULL);
    }
    return 0;
}