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
#include <ctime>

#define MAX_ARGS 64 //Maximum arguments to parse.

/**
 * @brief 
 * 
 * @param processCount 
 */
void calcFCFS(int processCount)
{
    srand(10);
    int prevWaitTime = 0;
    int waitTime = 0;

    printf("FCFS CPU scheduling simulation with %d processes.\n", processCount);

    for(int i = 1; i <= processCount; i++)
    {
        int burstTime = (rand() % 100) + 1;
        printf("CPU burst: %d ms\n", burstTime);

        waitTime += prevWaitTime;
        prevWaitTime += burstTime;
    }

    printf("Total waiting time in the ready queue: %d ms\n", waitTime);
    double avgWaitTime = static_cast<double>(waitTime) / processCount;
    printf("Average waiting time in the ready queue: %.0f ms\n", avgWaitTime);
}

/**
 * @brief 
 * 
 * @param buffer 
 */
void execCMD(char *buffer)
{
    char *args[MAX_ARGS];
    int argc = 0;
    char *token = strtok(buffer, " \t\n");

    while(token != NULL)
    {
        args[argc] = token;
        argc++;

        if(argc >= MAX_ARGS - 1)
        {
            fprintf(stderr, "Error: Too many args\n");
            return;
        }

        token = strtok(NULL, " \t\n");
    }

    args[argc] = NULL; //Null terminate the array.

    if(argc > 0 && (strcmp(args[0], "quit") == 0 || strcmp(args[0], "q") == 0)) //Check exit conditon.
    {
        exit(0);
    }
    
    char *outFile = NULL;
    bool redirection = false;
    for(int  i = 0; i < argc; i++)
    {   
        if(strcmp(args[i], ">") == 0)
        {
            if((i + 1) < argc)
            {
                outFile = args[i + 1];
                args[i] = NULL;
                redirection = true;
                break;
            }
            else
            {
                fprintf(stderr, "Error: no output file name provided.\n");
                return;
            }
        }
    }
    
    /*
    if(argc > 0 && strcmp(args[0], "fcfs") == 0) //ORIGINAL NOFORK IMPLEMENTATION
    {
        int processCount = 5; //Default if no number specified.
        if(argc == 2 || argc == 4)
        {
            processCount = atoi(args[1]);
        } 
        
        int mainOutput = dup(STDOUT_FILENO); 
        if(outFile && redirection)
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

            calcFCFS(processCount);

            if (dup2(mainOutput, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            }
            close(mainOutput); 

            return;
        }
        
        calcFCFS(processCount);
        
        return;
    }
    */
    
    pid_t pid = fork();

    if(pid == 0)
    {
        if(outFile && redirection)
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

            close(fd1); //Close the file.
        }

        if(argc > 0 && strcmp(args[0], "fcfs") == 0)
        {
            int processCount = 5; //Default if no number specified.
            if(argc == 2 || argc == 4)
            {
                processCount = atoi(args[1]);
            }
            else
            {
                perror("fcfs");
                exit(1);
            } 
            
            calcFCFS(processCount);
            exit(0);
        }
        else if(execvp(args[0], args) == -1)
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
        perror("fork");
        exit(1);
    }
}

/**
 * @brief 
 * 
 * @return int 
 */
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
