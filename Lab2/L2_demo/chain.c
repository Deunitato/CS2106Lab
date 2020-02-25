#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int i = 0;
    //open fille
    //argsc is count, argv is vectors
    //argsc sjld have 3 variables (COnfim wif cher)

    //argv[1] is the file to be dupe (mult)
    //argv[2] is number
    char *progName = argv[1];
    int times = atoi(argv[2]); //get the number of times need run
    pid_t childPid;

    int p[2 * times];

    for (i = 0; i < times; i++)
    {
        pipe(p + (i * 2)); //init the pipe
    }

    childPid = fork(); //first child
    if (childPid == 0)
    {

        dup2(p[1], 1);
        for (i = 0; i < times * 2; i++)
        {
            close(p[i]); //remove reference to pipe
        }
        int input;
        scanf("%d", &input);
        printf("%d", input);
        return 1; //return me or i will run continuously
    }

    int j =0;
    while (j < times-1)
    {
        childPid = fork(); //next child
        // pid == 0 means child process created
        if (childPid == 0)
        { //if this is child..child should write to parent

            // printf("test\n");
            //redirects the stdin of this process frm    prev pip
            dup2(p[j*2], STDIN_FILENO); 

            //redirects the stdout to the next pipe
            dup2(p[(j*2) + 3], STDOUT_FILENO); //redirects the stdout of this process to the next pipe

            for (i = 0; i < times * 2; i++)
            {
                close(p[i]); //remove my pointer pipe 
            }
            char str[10];
            sprintf(str, "%d", j+1);
            execlp(progName, progName, str, NULL);
        }
    
        j ++;
    }

    childPid = fork(); //last child
    // pid == 0 means child process created
    if (childPid == 0)
    {                              //if this is child..child should write to parent
        dup2(p[times * 2 - 2], 0); //redirects the stdout of this process to the pipe

        for (i = 0; i < times * 2; i++)
        {
            close(p[i]); //close all ref to child
        }

        char str[10];
        sprintf(str, "%d", times);

        execlp(progName, progName, str, NULL);
    }

    for (i = 0; i < times * 2; i++)
    {
        close(p[i]);
    }

    // for my parent to wait
    for (i = 0; i <times + 1; i++)
        wait(NULL);

    //waitpid(childPid, &childResult, 0); //get my result from child

    //dupe my file and pass in my childresult to my thing
}
