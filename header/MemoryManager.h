/*
 * MemoryManager.h
 *
 *  Created on: 29 בדצמ 2016
 *      Author: tal-pc
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_


#include <stdlib.h>
#include <string>


using namespace std;

class Mempool;
class Method;
class mem_control_block;

class MemoryManager {
public:

	MemoryManager();
	void	initComponents(const size_t& size,const int& type,const bool& is_valgrind);
	virtual ~MemoryManager();



	void* allocator(long num_of_bytes);
	void* allocateFromLast(long& num_of_bytes);
	void* setBlock(long& num_of_bytes, mem_control_block* mcb);

	void free(void* firstbyte);


	size_t getSize() ;


	void* getCurrentLoc() const;

	 void clear();

	void printFreeList() const;


	void printValgrind() const;


private:

	Mempool *mem_pool;
	Method* method;

	void* current_location;
	size_t capacity;
	size_t last_allocation;

	bool is_last_alloc_lock; // lock if the we pass over the whole memory allocation
	bool is_clear_list;
	size_t isInit;
	size_t type;


	bool is_mem_pool_init;
    bool is_valgrind;
    short num_of_allocs;
    short num_of_frees;

    size_t num_of_blocks;
};

#endif /* MEMORYMANAGER_H_ */
