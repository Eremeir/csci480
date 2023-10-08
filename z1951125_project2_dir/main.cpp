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
 * @brief Calculate First Come First Serve.
 * 
 * Calculates timings for a First-Come-First-Serve CPU Scheduling simulation and prints statistics to standard output.
 * 
 * @param processCount The number of processes to simulate.
 */
void calcFCFS(int processCount)
{
    srand(10); //Specifications state that simulation should be run with seed 10.
    int prevWaitTime = 0;
    int waitTime = 0;

    printf("FCFS CPU scheduling simulation with %d processes.\n", processCount);

    for(int i = 1; i <= processCount; i++)
    {
        int burstTime = rand() % 100 + 1; //Randomizes between 1-100
        printf("CPU burst: %d ms\n", burstTime);

        waitTime += prevWaitTime; 
        prevWaitTime += burstTime; //Store last total waiting time to add to future waits.
    }

    printf("Total waiting time in the ready queue: %d ms\n", waitTime);
    double avgWaitTime = static_cast<double>(waitTime) / processCount;
    printf("Average waiting time in the ready queue: %.0f ms\n", avgWaitTime);
}

/**
 * @brief Execute Command.
 * 
 * Parses and attempts to execute a given command from input stream.
 * 
 * Also handles output redirection and custom command "fcfs".
 * 
 * @param buffer Input stream to parse for commands and arguments.
 */
void execCMD(char *buffer)
{
    char *args[MAX_ARGS];  //Argument Array
    int argc = 0;          //Argument counter
    char *token = strtok(buffer, " \t\n"); //Create first argument token.

    while(token != NULL) //While input stream still has tokens
    {
        args[argc] = token; //Add argument tokens to args array.
        argc++;

        if(argc >= MAX_ARGS - 1) //Check for overflow arguments
        {
            fprintf(stderr, "Error: Too many args\n");
            return;
        }

        token = strtok(NULL, " \t\n"); //Create next argument token.
    }

    args[argc] = NULL; //Null terminate the array.

    if(argc > 0 && (strcmp(args[0], "quit") == 0 || strcmp(args[0], "q") == 0)) //Check exit conditon.
    {
        exit(0);
    }
    
    char *outFile = NULL; //Setup output redirection.
    bool redirection = false;
    for(int  i = 0; i < argc; i++) //Check argument array for output redirection operator.
    {   
        if(strcmp(args[i], ">") == 0)
        {
            if((i + 1) < argc) //If found and there is another argument after the operator
            {
                outFile = args[i + 1]; //Fetch output file name from next argument.
                args[i] = NULL; //Delete redirection operator from array.
                redirection = true; //Indicate that output should be redirected as operator has been found.
                argc--;
                break;
            }
            else
            {
                fprintf(stderr, "Error: no output file name provided.\n");
                return;
            }
        }
        else if(args[i][0] == '>') //If output rediretion operator and outfile are conjoined.
        {
            outFile = args[i] + 1;
            args[i] = NULL; //Delete redirection operator from array.
            redirection = true; //Indicate that output should be redirected as operator has been found.
            argc--;
            break;
        }
    }
    
    pid_t pid = fork(); //Fork program process.

    if(pid == 0) //If child process
    {
        if(outFile && redirection) //If the output file has been given and command specifies to redirect
        {
            int fd1 = open(outFile, O_WRONLY | O_CREAT | O_APPEND, 0644); //Open new file, create and empty it with write permissions.
            if(fd1 == -1) //Check if file creation and opening was successful.
            {
                perror("open");
                exit(1);
            }
            if(dup2(fd1, STDOUT_FILENO) == -1) //Redirect standard output to file.
            {
                perror("dup2");
                exit(1);
            }

            close(fd1); //Close the file.
        }

        if(argc > 0 && strcmp(args[0], "fcfs") == 0)  //If fcfs was found as an argument
        {
            int processCount = 5; //Default if no number specified.
            if(argc > 1 && args[1]) //If there are at least than two arguments and the second argument isn't null.
            {
                processCount = atoi(args[1]);
            }
            
            calcFCFS(processCount);
            exit(0);
        }
        else if(execvp(args[0], args) == -1) //Use system call to run given argument.
        {
            //perror("execvp"); Does not match refrence output when included
            exit(1);
        }
    }
    else if(pid > 0) //If parent process
    {
        int status;
        waitpid(pid, &status, 0); //Wait for child process to finish execution.

        if(WIFEXITED(status) && WEXITSTATUS(status) != 0) //Check if child process exited abnormally.
        {
            fprintf(stderr, "Error executing command: %s\n", args[0]);
        }
    }
    else //Fork did not work.
    {
        perror("fork");
        exit(1);
    }
}

/**
 * @brief Main function to test microshell implimentation.
 * 
 * Creates an input stream and loops forever accepting user input for microshell commands, 
 * until user enters "quit" or "q".
 * 
 * @return int indicating program's exit status.
 */
int main(void)
{
    char buffer[1024];
    while(printf("myshell>") && fgets(buffer, sizeof(buffer), stdin)) //Read input from standard input.
    {
        if(strlen(buffer) == 1) //Check input isn't empty.
        {
            continue;
        }

        execCMD(buffer); //Parse and execute command from buffer.
    }

    return 0;
}
