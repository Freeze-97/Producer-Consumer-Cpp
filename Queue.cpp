// Created by Toya1800
#include "Queue.h"

Queue::Queue() : size(0){
    front.store(0);
    rear.store(0);
}

Queue::~Queue() {
    // Delete all data from the queue while it's not empty
    while (!isEmpty()) {
        int item;
        dequeue(item);
    }
    front.store(0);
    rear.store(0);
}

int Queue::enqueue(int item) {
    auto tmp_tail = rear.load(); // Get the current tail
    auto new_tail = (tmp_tail + 1) % (MAXSIZE + 1);

    if(new_tail != front.load()) {
        std::cout << "Prod: " << item << " nBuff=" << size.operator++() << std::endl;
        array[tmp_tail] = item;
        rear.store(new_tail);
        return 0;
    }
    return -1; // Failed - Full queue
}

int Queue::dequeue(int &item) {
    auto tmp_front = front.load();
    if (tmp_front == rear.load()) {

        return -1; // it's empty
    } else {
        item = array[tmp_front];
        std::cout << "    Cons: " << item << " nBuff=" << size.operator--() << std::endl;
        front.store((tmp_front + 1) % (MAXSIZE + 1));
        return 0;
    }
}

size_t Queue::getSize() {
    return size.load();
}

bool Queue::isFull() {
    return size.load() == MAXSIZE;
}

bool Queue::isEmpty() {
    return size.load() == 0;
}

