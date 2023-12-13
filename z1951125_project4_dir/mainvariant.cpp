/********************************************************************
CSCI 480 - Assignment 4 - Fall 2023 Variant 2

Progammer: Matt Borek
Section:   1
TA:        Sai Dinesh Reddy Bandi
Date Due:  November 5, 2023

Purpose:   This program simulates the reader-writer problem using
           synchronized threads and semaphores.
*********************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

std::string sharedString = "All work and no play makes Jack a dull boy."; //Test string.
sem_t sem1; //Semaphores for alternating reader and writer access.
sem_t sem2; 

int numReaders; //Number of readers
int numWriters; //Number of writers, should always be 1 in this variant.

/**
 * @brief Writer thread process,
 * 
 * Process for writer threads, synchronizes with reader threads. 
 * Removes the last character of the shared string each write.
 * 
 * @param param Pointer to a unique writer ID.
 * @return void* Function does not return with any return statements.
 */
void *writer(void *param)
{   
    int writerID = *(int *)param;
    while(!sharedString.empty()) //While string is not fully unwritten.
    {
        sem_wait(&sem1); //Wait for write access.

        if(!sharedString.empty()) //If non-empty, clip last character.
        {
            sharedString.pop_back();
            printf("writer %d is writing ...\n", writerID);
            fflush(stdout);
        }

        sem_post(&sem2); //Signal Reader.

        sleep(1); //Sleep for 1 second.
    }

    printf("writer %d is exiting ...\n", writerID);
    pthread_exit(0); //Close thread.
}

/**
 * @brief Reader thread process
 * 
 * Process for reader threads, synchronizes with writer threads.
 * Prints contents of shared string.
 * 
 * @param param Pointer to a unique reader ID.
 * @return void* Function does not return with any return statements.
 */
void *reader(void* param)
{
    int readerID = *(int *)param;
    while(!sharedString.empty())
    {
        sem_wait(&sem2); //Wait for read access.
        
        if(!sharedString.empty())
        {
            printf("reader %d is reading ... content : %s\n", readerID, sharedString.c_str()); //Read and print the shared string.
            fflush(stdout);
        }

        sem_post(&sem1); //Signal next writer.
        
        sleep(1); //Sleep for 1 second.
    }

    printf("reader %d is exiting ...\n", readerID);
    int waitingWriters = (numWriters - numReaders);
    printf("There are still %d writers waiting on the semaphore...\n", waitingWriters);
    for(int i = 0; i < waitingWriters; i++)
    {
        sem_post(&sem1); //Signal remaining writers.
    }
    
    pthread_exit(0); //Close thread.
}

/**
 * @brief Main function to test reader-writer problem synchronization.
 * 
 * Accepts input from command line for the number of readers and writers to use
 * in simulating the reader-writer synchronization problem.
 * 
 * Creates a thread for each actor and logs simulation to stdout.
 * 
 * @param argc Count of arguments.
 * @param argv Argument Variable array.
 * @return int indicating program's exit status.
 */
int main(int argc, char* argv[]) //Get command line arguments.
{
    if(argc != 3) //If exact number of arguments provided.
    {
        fprintf(stderr, "Error: Invalid number of arguments. Usage: %s <numReaders> <numWriters>\n", argv[0]);
        exit(1);
    }

    try //Ensure arguments are valid.
    {
        numReaders = std::stoi(argv[1]); //Number of reader actors.
        numWriters = std::stoi(argv[2]); //Nummber of writer actors.
    }
    catch(const std::invalid_argument &e)
    {
        fprintf(stderr, "Invalid Argument: %s\n", e.what());
        exit(1);
    }

    if(numReaders <= 0 || numWriters <= 0) //Ensure arguments are positive.
    {
        fprintf(stderr, "Error: numReaders and numWriters must be positive.\n");
        exit(1);
    }

    printf("*** Reader-Writer Problem Simulation ***\n");
    if(sem_init(&sem1, 0, 1) != 0 || sem_init(&sem2, 0, 0) != 0) //Initialization of semaphores.
    {   
        perror("Error: Semaphore initialization failed");
        return 1;
    }

    printf("Number of reader threads: %d\n", numReaders);
    printf("Number of writer threads: %d\n", numWriters);

    std::vector<pthread_t> readerThreads(numReaders);
    std::vector<int> readerIDs(numReaders);
    std::vector<pthread_t> writerThreads(numWriters);
    std::vector<int> writerIDs(numWriters);

    for(int i = 0; i < numReaders; i++) //Create reader threads.
    {
        readerIDs[i] = i; //Assign the reader ID
        int result = pthread_create(&readerThreads[i], NULL, reader, &readerIDs[i]);
        if(result != 0) //Thread creation failed.
        {
            fprintf(stderr, "Error: Return code of pthread_create() on reader thread %d is %d\n", readerIDs[i], result);
            exit(-1);
        }
    }

    for(int j = 0; j < numWriters; j++) //Create writer threads.
    {
        writerIDs[j] = j; //Assign the writer ID.
        int result = pthread_create(&writerThreads[j], NULL, writer, &writerIDs[j]);
        if(result != 0) //Thread creation failed.
        {
            fprintf(stderr, "Error: Return code of pthread_create() on writer thread %d is %d\n", writerIDs[j], result);
            exit(-1);
        }
    }

    for(int i = 0; i < numReaders; i++) //Wait for reader threads to finish.
    {
        pthread_join(readerThreads[i], NULL);
    }

    for(int j = 0; j < numWriters; j++) //Wait for writer threads to finish.
    {
        pthread_join(writerThreads[j], NULL);
    }

    printf("All threads are done. \n");
    sem_destroy(&sem1); //Destroy semaphores.
    sem_destroy(&sem2); 

    printf("Resources cleaned up. \n");
    return 0;
}
