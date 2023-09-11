/********************************************************************
CSCI 480 - Assignment 2 - Fall 2023

Progammer: Matt Borek
Section:   1
TA:        Sai Dinesh Reddy Bandi
Date Due:  September 27, 2023

Purpose:   This program implements a microshell utilizing forks and
           a FCFS CPU Scheduling algorithm.
*********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void) //Example Code
{
    char buf[1024];
    pid_t pid;
    int status;
    printf("%% ");

    while (fgets(buf,1024,stdin) != NULL)
    {
        buf[strlen(buf) -1] = 0;
        if((pid = fork()) < 0)
        {
            printf("fork error");
        }
        else if(pid == 0)
        { /* child */execlp(buf, buf, (char *) 0);
            printf("couldn't execute: %s", buf); //better change to printing to stderr
            exit(127);
        }/* parent */
        if((pid = waitpid(pid, &status, 0)) < 0)
        {
            printf("waitpid error");printf("%% ");}exit(0);
    }
}
