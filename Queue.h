/*
 * Created by Toya1800
 * Circular queue
 * A lot is inspired from the web regarding the circular queue
 */

#ifndef LAB4_QUEUE_H
#define LAB4_QUEUE_H

#include <cstddef>
#include <atomic>
#include <iostream>
#define MAXSIZE 10 // Size of the queue
using std::atomic;

class Queue {
private:
    // Pointers for head and tail
    atomic <size_t> rear, front;

    // The current size of the queue
    atomic <size_t> size;

    // Queue
    int array[MAXSIZE];

public:
    // Constructor
    Queue();

    // Destructor
    ~Queue();

    // Add item to the queue
    int enqueue(int item);

    // Remove item from queue
    int dequeue(int &item);

    // Get the size of the queue
    size_t getSize();

    // Check if the queue is full
    bool isFull();

    // Check if the queue is empty
    bool isEmpty();
};

#endif //LAB4_QUEUE_H
