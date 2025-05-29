# 🛠️ Producer / Consumer with Shared Memory and Semaphores

## 📚 Description

This lab demonstrates the **Producer/Consumer problem** using **shared memory** and **POSIX semaphores** in C++. The goal is to build a bounded queue (buffer) where:
- A **producer** process generates and adds integers.
- A **consumer** process retrieves and processes integers.

To ensure data integrity and prevent race conditions, synchronization is enforced using **semaphores**:
- The producer must wait if the queue is full.
- The consumer must wait if the queue is empty.

Only one process can access the shared queue at a time (ensuring **atomicity**).

## 🧪 Key Concepts
- **Shared Memory**: Used for communication between the producer and consumer processes.
- **Semaphores**: Ensure mutual exclusion and synchronization between processes.
- **Processes**: Created using `fork()` to run producer and consumer in parallel.
- **Queue**: Circular buffer implementation with limited size.

## 🛠️ Compilation

To compile the program, run:

```bash
make
```

## ▶️ Usage

```bash
./lab4 [num]
```

- ***num***: (optional) Number of items to produce/consume. Defaults to 5000 if not provided.

⚙️ Behavior
- The producer generates random integers between 1 and 9999.

- The consumer reads and discards these values.

- Both processes terminate after processing num items.

- The parent process waits for both children and prints their exit statuses.

- Semaphores and shared memory are cleaned up automatically at the end.
