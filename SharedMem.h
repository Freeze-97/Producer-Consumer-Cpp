/************************************************************************** 
File:	Sharedmem.h

Objective:	Provide a wrapper class to POSIX shared memory IPC.
Uses system calls shmget, shmctl, shmmat and shmdt.

Created:	December 2004 / Orjan Sterner

**************************************************************************/

#ifndef SHAREDMEM_H
#define SHAREDMEM_H

/* 

Semantics:
   
A Sharedmem object encapsulates a POSIX shared memory abstraction. This is achieved by using several system calls as there is no singel system call that directly can fulfill this task. Two or more processes can be attached to a shared memory segment in order to implement an IPC channel between them. A Sharedmem object serves as a wrapper to map a simple semantics and terminology to the underlying system call interface. 


Usage:

A shared memory segment must be be allocated using a unique keyvalue. For related processes, parent-child, to share a memory segment the key IPC_PRIVATE is sufficient. For unrelated processes another unique key must be supplied. Some keyvalues are reserved by the system and cannot be used. The C-library function ftok(.....) provides one way of getting one. See man ftok.
	
A typical usage of the Sharedmem class in a parent-child scenario could be:

-	Parent allocates a shared memory segment:
	Sharedmem(IPC_PRIVATE,int)   
		or 
	Sharedmem() + allocate(key_t,int) later on
	
	NOTE that a Sharedmem object itself holds no shared memory, just some information about it. 

-	Parent forks one or several new processes. Parent + children now have copies of the Sharedmem object i.e the same information about it.

-	Every forked child process that wishes to use the shared memory must attach it to the process using member attach(void). Unrelated processes must use attach(int) with the proper id as argument.

-	To access the shared memory a pointer of the right type is assigned the return value from member getAddr(void). The value must be cast to the right type before assignment. 

-	The processes can now use the shared memory. It is quite obvious that the usage requires synchronization among the processes and a protocol how to communicate.

-	When a process is done with the shared memory it must detach it by using the member detach(void).

-	Eventually one of the processes returns the shared memory to the system by using the member remove(void). The Sharedmem object itself obeys the regular C++ rules for scope and lifetime.
     
Information about the usage of shared memory could be obtained from the command 'ipcs -m'. Hand-crafted removal can be done using the 'ipcrm -m  <id>' command.

NB you probably already have some shared memory segments allocated before you use this class. Run ipcs -m and check before you use it and note the ids in use so you won't destroy something that is not to be destroyed... Every new shared memory segments will probably get a higher id than those already present.

*/
   

#include <sys/ipc.h>

class SharedMem
{
	private:
		int _id;  // id returned from Kernel
		key_t _key; // key used when allocated
		int _size; // shared memory size
		char *_mem; // start adress of shared memory
	public:
		SharedMem (void) {}
		/* pre  - 
			post - A SharedMem object is created, but no shared memory is neither allocated nor attached to the process.		
		*/

	SharedMem (key_t key, int size);
	/* pre  - 
	   post - A SharedMem object is created. A corresponding shared memory segment with size bytes is allocated. The allocation is based on the unique value key. If a shared memory segment could not be allocated the process is terminated.		
	*/


	void allocate (key_t, int size);
	/* pre  - This SharedMem object has no shared memory allocated.
	   post - A SharedMem object is created. A shared memory segment with size bytes is allocated. The allocation is based on the unique value key. If a shared memory segment could not be allocated the process is terminated.		
	*/

	int attach (void);
	/* pre  - This Sharedmem object has allocated a shared memory segment.
	   post - The shared memory segment is attached to the calling process and 0 is returned. If attach fails -1 is returned.
	*/

	int attach (int id);
	/* pre  - This Sharedmem object has NOT allocated a shared memory segment.
	   post - The shared memory segment with id as identifier is attached to the calling process and 0 is returned. If attach fails -1 is returned.
	*/

	int detach (void);
	/* pre  - This Sharedmem object is attached to a shared memory segment.
	   post - The calling process is detached from the shared memory and 0 is returned. If detach fails -1 is returned.
	*/


	int remove (void);
	/* pre  - This Sharedmem object is detached from its shared memory segment.
	   post - The shared memory segment is deallocated and 0 is returned. If remove fails -1 is returned.		
	*/


	char *getAddr (void);
	/* pre  - This Sharedmem object is attached to a shared memory segment.
	   post - The start address of the shared memory segment is returned.
	*/

	int id(void);
	/* pre  - This Sharedmem object is attached to a shared memory segment.
	   post - The id of the shared memory segment is returned.
	*/

};

#endif

