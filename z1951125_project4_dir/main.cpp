#include <string>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

std::string sharedString = "All work and no play makes Jack a dull boy."; //Test string.
sem_t rw_sem; //Semaphore for reader-writer synchronization.
sem_t cs_sem; //Semaphore for critical section of reader.
int read_count = 0;

/**
 * @brief Writer thread process,
 * 
 * Process for writer threads, synchronizes with reader threads. 
 * Removes the last character of the shared string each write.
 * 
 * @param param Unused but part of standard.
 * @return void* Function does not return with any return statements.
 */
void *writer(void *param)
{   
    while(!sharedString.empty()) //While string is not fully unwritten.
    {
        sem_wait(&rw_sem); //Enter critical section.

        if(!sharedString.empty()) //If non-empty, clip last character.
        {
            sharedString.pop_back();
        }

        if(!sharedString.empty())
        {
            printf("writer x is writing ...\n");
            fflush(stdout);
        }

        sem_post(&rw_sem); //Exit critical section.

        sleep(1); //Sleep for 1 second.
    }

    printf("writer x is exiting ...\n");
    pthread_exit(0); //Close thread.
}

/**
 * @brief Reader thread process
 * 
 * Process for reader threads, synchronizes with writer threads.
 * Increments and decrements read count and prints contents of shared string.
 * 
 * @param param Unused but part of standard.
 * @return void* Function does not return with any return statements.
 */
void *reader(void* param)
{
    while(!sharedString.empty())
    {
        sem_wait(&cs_sem); //Enter critical section.
        read_count++;
        printf("read_count increments to: %d\n", read_count);
        if(read_count == 1)
        {
            sem_wait(&rw_sem); //First reader locks out writers.
        }
        sem_post(&cs_sem); //Exit critical section.

        printf("reader x is reading ... content : %s\n", sharedString.c_str()); //Read and print the shared string.
        fflush(stdout);
        
        
        sem_wait(&cs_sem); //Enter critical section.
        read_count--;
        printf("read_count decrements to: %d\n", read_count);
        if(read_count == 0)
        {
            sem_post(&rw_sem); //Last reader unlocks writers.
        }
        sem_post(&cs_sem); //Exit critical section.
        
        sleep(1); //Sleep for 1 second.

        if(sharedString.empty()) //Leave loop if string was unwritten during wait.
        {
            break;
        }
    }

    printf("reader x is exiting ...\n");
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
    printf("*** Reader-Writer Problem Simulation *** \n");
    if(sem_init(&rw_sem, 0, 1) != 0 || sem_init(&cs_sem, 0, 1) != 0) //Initialization of semaphores.
    {   
        perror("Semaphore initialization failed");
        return 1;
    }

    int numReaders = std::stoi(argv[1]); //Number of reader actors.
    printf("Number of reader threads: %d\n", numReaders);
    int numWriters = std::stoi(argv[2]); //Nummber of writer actors.
    printf("Number of writer threads: %d\n", numWriters);

    std::vector<pthread_t> readerThreads(numReaders);
    std::vector<int> readerIds(numReaders);
    std::vector<pthread_t> writerThreads(numWriters);
    std::vector<int> writerIds(numWriters);

    for (int i = 0; i < numReaders; i++) //Create reader threads.
    {
        pthread_create(&readerThreads[i], NULL, reader, &readerIds[i]);
    }

    for (int i = 0; i < numWriters; i++) //Create writer threads.
    {
        pthread_create(&writerThreads[i], NULL, writer, &writerIds[i]);
    }

    for (int i = 0; i < numReaders; i++) //Wait for reader threads to finish.
    {
        pthread_join(readerThreads[i], NULL);
    }

    for (int i = 0; i < numWriters; i++) //Wait for writer threads to finish.
    {
        pthread_join(writerThreads[i], NULL);
    }

    printf("All threads are done. \n");
    sem_destroy(&rw_sem); //Destroy semaphores.
    sem_destroy(&cs_sem); 

    printf("Resources cleaned up. \n");
    return 0;
}
