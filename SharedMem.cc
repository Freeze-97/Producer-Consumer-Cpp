/* 

File:		Sharedmem.cc

Objective:	Implementation of class Sharedmemory

Created:	October 2017 / Orjan Sterner

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <syslog.h>
#include <cstdlib>
#include <cstdio>
#include "SharedMem.h"

SharedMem::SharedMem(key_t key, int size):_id(-1), _key(key), _size(size)
{
	if (size == 0) {
		perror( "SharedMem::SharedMem(key_t,int): Size of shared memory segment is 0, aborting...\n");
		exit(1);
 	}
	if ((_id = shmget(key, size, S_IRUSR | S_IWUSR | IPC_CREAT)) == -1) {
		perror( "SharedMem::SharedMem(key_t,int): Can't create shared memory: %m,aborting...\n");
		exit(1);
	}
}

void SharedMem::allocate(key_t key, int size)
{
	if (size == 0) {
		perror( "void SharedMem::initOpen(key_t,int): Size of shared memory segment is 0,aborting...\n");
		exit(1);
	}
	if ((_id = shmget(key, size, S_IRUSR | S_IWUSR | IPC_CREAT)) == -1) {
		char msg[100];
		sprintf(msg, "void SharedMem::initOpen(key_t,int): Can't create shared memory,key: %d, size: %d: %m, aborting...\n", key, size);
		perror(msg);
		exit(1);
	}
	_key = key;
 	_size = size;

}

int SharedMem::attach(void)
{

	if ((_mem = (char*)shmat(_id, 0, 0)) == (void *) -1) {
		perror( "int SharedMem::attach(void): Can't attach to shared memory: %m\n");
		return -1;
	}
	return 0;
}

int SharedMem::attach(int id)
{
	if (( _mem = (char*)shmat(id, 0, 0)) == (void *) -1) {
		perror( "int SharedMem::attach(int): Can't attach to shared memory: %m\n");
		return -1;
	}
	_id = id;

	return 0;
}

int SharedMem::detach(void)
{

	if ((shmdt(_mem)) == -1) {
		perror( "int SharedMem::detach(void): Can't detach from shared memory: %m\n");
		return -1;
	}
	return 0;
}

int SharedMem::remove(void)
{
	if (shmctl(_id, IPC_RMID, nullptr) == -1) {
		perror( "int SharedMem::remove(void): Can't remove shared memory: %m\n");
		return -1;
	}
	return 0;
}

char* SharedMem::getAddr(void)
{
	return _mem;
}


int SharedMem::id(void)
{
	return _id;
}
