/********************************************************************
CSCI 480 - Assignment 2 - Fall 2023

Progammer: Matt Borek
Section:   1
TA:        Sai Dinesh Reddy Bandi
Date Due:  September 27, 2023

Purpose:   This program implements a microshell utilizing forks and
           a FCFS CPU Scheduling algorithm.
*********************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 64 //Maximum arguments to parse.

void execCMD(char *buffer)
{
    char *args[MAX_ARGS];
    int argc = 0;
    char *token = strtok(buffer, " \t\n");

    while(token != NULL)
    {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " \t\n");
    }

    args[argc] = NULL; //Set last array position to NULL.

    if(argc > 0 && (strcmp(args[0], "quit") == 0 || strcmp(args[0], "q") == 0)) //Check exit conditon.
    {
        exit(0);
    }

    char *outFile = NULL;
    for(int  i = 0; i < argc; i++)
    {
        if(strcmp(args[i], ">") == 0)
        {
            if((i + 1) < argc)
            {
                outFile = args[i+1];
                args[i] = NULL;
                break;
            }
            else
            {
                fprintf(stderr, "Error: no output file name provided.\n");
                return;
            }
        }
    }

    pid_t pid = fork();

    if(pid == 0)
    {
        if(outFile)
        {
            int fd1 = open(outFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(fd1 == -1)
            {
                perror("open");
                exit(1);
            }
            if(dup2(fd1, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            }

            close(fd1); //Close file.
        }

        if(execvp(args[0], args) == -1)
        {
            //perror("execvp"); Does not match refrence output when included
            exit(1);
        }
    }
    else if(pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);

        if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            fprintf(stderr, "Error executing command: %s\n", args[0]);
        }
    }
    else
    {
        //perror("fork");
        exit(1);
    }
}

int main(void)
{

    while(1)
    {
        printf("myshell>");
        char buffer[1024];
        fgets(buffer, sizeof(buffer), stdin);

        if(strlen(buffer) == 1) //Check input isn't empty.
        {
            continue;
        }

        execCMD(buffer);
    }

    return 0;
}
