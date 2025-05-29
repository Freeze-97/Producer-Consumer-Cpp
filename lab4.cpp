/*
 * Laboration 4 - Producer / Consumer
 * Toya1800
 * December 2021
 */

#include <semaphore.h>
#include <unistd.h>
#include <random>
#include <sys/wait.h>
#include <memory>
#include <new>
#include "Queue.h"
#include "SharedMem.h"

// Global const variable
const size_t BUFFSIZE = 10;

// Functions
void produce(int nums, Queue *queue, sem_t *spaceAvail, sem_t *itemAvail);
void consume(int nums, Queue *queue, sem_t *spaceAvail, sem_t *itemAvail);

int main(int argc, char *argv[]) {
    const int standard_nums = 5000;
    int nums = standard_nums;

    // If the user has specified their own num
    if(argc >= 2) {
        nums = std::stoi(argv[1]);
    }

    // The shared memory
    // Second parameter is the memory size
    auto *shared_memory = new SharedMem(IPC_PRIVATE,
                                        (2 * sizeof(sem_t)) + sizeof(Queue));

    // Attach memory
    if(shared_memory->attach() == -1) {
        perror("ERROR: Failed to attach memory allocation");
        return EXIT_FAILURE;
    }

    // Create queue
    auto *queue = new(shared_memory->getAddr()) Queue;

    /*
     * Semaphores
     * spaceAvail: checks for empty spot to put in an int value
     * itemAvail: checks for int value available for consuming
    */
    auto *spaceAvail = new(shared_memory->getAddr() +
                           sizeof(Queue)) sem_t;
    sem_init(spaceAvail, true, BUFFSIZE);

    auto *itemAvail = new(shared_memory->getAddr() +
                          sizeof(Queue) +
                          sizeof(sem_t)) sem_t;
    sem_init(itemAvail, true, 0);

    std::cout << "Program starting..." << std::endl << std::endl;

    // Initialize producer and consumer processes
    pid_t producer_process;
    pid_t consumer_process;

    // Create the child processes
    (producer_process = fork()) && (consumer_process = fork());

    // Producer process
    if (producer_process == 0) { // Producer process
        try {
            produce(nums, queue, spaceAvail, itemAvail);
        } catch (std::exception &e) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS; // Producer process has done its job
    } else if(consumer_process == 0) { // Consumer process
        try {
            consume(nums, queue, spaceAvail, itemAvail);
        } catch (std::exception &e) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS; // Consumer process has done its job
    } else if(producer_process == -1 || consumer_process == -1) {
        perror("ERROR: Failed to fork");

        // Clear memory
        // Destroy semaphores
        // Clear the shared memory
        sem_destroy(spaceAvail);
        sem_destroy(itemAvail);

        if(shared_memory->remove() == -1)  {
            perror("ERROR: Failed to remove memory");
        }
        if(shared_memory->detach() == -1) {
            perror("ERROR: Failed to detach memory");
        }
        delete shared_memory;

        return EXIT_FAILURE;
    }
    else { // Parent process
        // Just wait for the child processes to finish
        for(int i = 0; i < 2; i++) {
            // Store status in int variable
            int status;
            do {
                pid_t pid = wait(&status);

                if(WIFEXITED(status)) {
                    std::cout << "Process: " + std::to_string(pid) + " terminated without any problems. Return value: " +
                            std::to_string(WEXITSTATUS(status)) << std::endl;
                } else if(WIFSTOPPED(status)) {
                    std::cout << "Process: " << std::to_string(pid) + " got stopped by signal " +
                    std::to_string(WIFSTOPPED(status))<< std::endl;
                } else if(WIFSIGNALED(status)) {
                    std::cout << "Process: " << std::to_string(pid) + " got killed by signal "+
                    std::to_string(WTERMSIG(status)) << std::endl;
                } else if(WIFCONTINUED(status)) {
                    std::cout << "Process: " << std::to_string(pid) + " continued executing." << std::endl;
                }
            } while (!WIFSIGNALED(status) && !WIFEXITED(status));
        }
    }

    // Clear the shared memory,
    // Destroy semaphores
    sem_destroy(spaceAvail);
    sem_destroy(itemAvail);

    if(shared_memory->remove() == -1)  {
        perror("ERROR: Failed to remove memory");
    }
    if(shared_memory->detach() == -1) {
        perror("ERROR: Failed to detach memory");
    }
    delete shared_memory;

    return EXIT_SUCCESS;
}

void produce(int nums, Queue *queue, sem_t *spaceAvail, sem_t *itemAvail) {
    // Here we produce the ints and put them in the queue
    int tmp_nums = 0;

    while(tmp_nums < nums) {
        sem_wait(spaceAvail);

        // Generate random number
        std::random_device rd;
        std::uniform_int_distribution<int> distribution(1, 9999);
        int random_number = distribution(rd);

        // 0 means it was possible to enqueue, otherwise it returns -1
        if(queue->enqueue(random_number) == 0) {
            tmp_nums++;
        }

        sem_post(itemAvail);
    }
}

void consume(int nums, Queue *queue, sem_t *spaceAvail, sem_t *itemAvail) {
    // Here we consume the ints from the queue
    int tmp_nums = 0;

    while(tmp_nums < nums) {
        sem_wait(itemAvail);

        int consumed_num;
        if (queue->dequeue(consumed_num) == 0) {
            tmp_nums++;
        }

        sem_post(spaceAvail);
    }
}
