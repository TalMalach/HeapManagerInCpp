/*
 * MemoryManager.cpp
 *
 *  Created on: 29 בדצמ 2016
 *      Author: tal-pc
 */

#include "MemoryManager.h"
#include "Mempool.h"
#include "FirstFit.h"
#include "mem_control_block.h"


#include <iostream>


MemoryManager::MemoryManager(){}

MemoryManager::~MemoryManager()
{
	if(is_mem_pool_init == true)
	{

	mem_pool->freeMemory();
	free(method);
	}
}



void	MemoryManager::initComponents(const size_t& size,const int& type,const bool& is_valgrind)
{
	capacity = size * sizeof(struct mem_control_block*) * 24 * 2;

	is_mem_pool_init = false;
	mem_pool=Mempool::getInstance(capacity);
    if(mem_pool == NULL) return;
    is_mem_pool_init = true;

	last_allocation = 0;

	current_location = mem_pool->getBlock(last_allocation);

	isInit =0;

	if(type == method->FIRST_FIT)
	{

		FirstFit* ins = (FirstFit*)malloc(sizeof(FirstFit));
		method=	new (ins)FirstFit();
	}

	this->type = type;
	is_last_alloc_lock = 0;
	isInit = 1;
	 this->is_valgrind = is_valgrind;
    num_of_allocs = 0;
    num_of_frees = 0;
    num_of_blocks = 0;
    is_clear_list = 0;
}



void* MemoryManager::allocator(long num_of_bytes)
{
	void* memory_location = 0;

	if(num_of_bytes <0 || num_of_bytes > capacity) return memory_location; //checking input

	if(isInit == 1 && !method->isEmpty()) // checking if the list was initialized and the list is not empty
	{
		mem_control_block *mcb = method->find(num_of_bytes); //check if there is a fit block on the free list method
		 num_of_blocks ++;
		if(mcb == NULL) { return allocateFromLast(num_of_bytes);} // checking if the there was no fit block
		return setBlock(num_of_bytes, mcb); // we found the fit block so allocate the block and return the address
	}
	 num_of_blocks ++;
	memory_location = allocateFromLast(num_of_bytes); // if we didn't return so we do allocate from the last

	return memory_location;
}




void* MemoryManager::allocateFromLast(long& num_of_bytes)
{

	void* memory_location = 0;
	size_t min_block_size = capacity - num_of_bytes - sizeof(mem_control_block);

	if(last_allocation >  min_block_size) is_last_alloc_lock = true; //we can't allocate the current & next blocks
	if(is_last_alloc_lock) return memory_location; // if we turn the lock on we can't allocate from last

	memory_location = mem_pool->getBlock(last_allocation); // check if there is a free space in the memory pool
	if(memory_location == NULL) return memory_location;

	/** Taking the free block    **/
	mem_control_block *mcb = (mem_control_block *)(memory_location);
	mcb->is_available = 0;
	mcb->size = num_of_bytes;
	/***************************/


	last_allocation += sizeof(mem_control_block) + num_of_bytes; // increase last allocation to for the next allocation

	memory_location = memory_location+sizeof(mem_control_block);

	num_of_allocs += num_of_bytes+ sizeof(mem_control_block);
	num_of_frees += sizeof(mem_control_block); // we allocate the free blocks but we will never delete them manually

	return memory_location;
}


void* MemoryManager::setBlock(long& num_of_bytes, mem_control_block* mcb) //20 // 6
{

	void* memory_location = 0;
	int block_spacing = 0;

	if(mcb->is_available == 0) return memory_location;

	mcb->is_available = 0;
	block_spacing = mcb->size - num_of_bytes - sizeof(mem_control_block);
	mcb->size = num_of_bytes;


    num_of_allocs +=num_of_bytes;

	memory_location = (void*)mcb + sizeof(mem_control_block);
	int sizeofMem= sizeof(mem_control_block);
	if(block_spacing >sizeofMem)// sizeof(mem_control_block))
	{
		void* new_header_placement = memory_location + num_of_bytes;
		mem_control_block *mcb_spacing_header = (mem_control_block*)(new_header_placement);
		mcb_spacing_header->is_available = 1;
		mcb_spacing_header->size = block_spacing;

		last_allocation += sizeof(mem_control_block) + block_spacing;
		if(is_clear_list == 0) method->add(mcb_spacing_header);

		num_of_frees =num_of_frees+ (mcb_spacing_header->size + sizeof(mem_control_block));

		method->remove(mcb); // we remove the previous header

	}

	return memory_location;
}





void MemoryManager::free(void* firstbyte)
{
	firstbyte = firstbyte - sizeof(struct mem_control_block);
	mem_control_block *mcb = (mem_control_block *)firstbyte;
	mcb->is_available = 1;
	if(mcb->size > sizeof(struct mem_control_block) && is_clear_list == 0) method->add(mcb);// check edge case of adding a empty header
    num_of_frees +=mcb->size;
    num_of_blocks --;
	return;
}


void MemoryManager::printFreeList() const
{
	method->print();
}

void MemoryManager::clear()
{

	is_clear_list=1;
   method->clear();
	is_clear_list=0;



}

void MemoryManager::printValgrind() const
{
 if(this->is_valgrind == true)
 {
	 cout << "//*******************************************\\" << endl;
	 cout << "Bytes allocated: " << num_of_allocs << endl;
	 cout << "Bytes Freed: " << num_of_frees << endl;
	 cout << "Total Bytes: " << num_of_allocs - num_of_frees << endl;
	 cout << "Amount of blocks that was not freed: " << num_of_blocks << endl;
	 cout << "//*******************************************\\" << endl;
 }
}




/*
 *

	Test 8 For free list and get a free block between 2 allocations:

				Heap - |0,1,2 ... 2000|

			    1. allocate a block of 20 bytes
				2. free
				3. allocate a block of 32 bytes
				4. free
				5. allocate a block of 40 bytes
				6. allocate a block of 15 bytes
				7. use free list printer


needs to print out:

 **********************************************
	Block No. 1:
	The Size of the Block: 20
	The Block is available!
 **********************************************

 **********************************************
	Block No. 2:
	The Size of the Block: 4
	The Block is available!
 **********************************************


	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);


	m->free(ptr1);



	void* ptr2 = m->allocator(40);

	m->free(ptr2);



	void* ptr3 = m->allocator(32);


	m->printFreeList();


 */







/*
	Test 7 For allocation of free space & allocate again with smaller space to check if free list works:

			Heap - |0,1,2 ... 2000|

		    1. allocate a block of 20 bytes
			2. free
			3. allocate a block of 15 bytes
			4. use free list printer

			needs to print out:
 **********************************************
			Block No. 1:
			The Size of the Block: 1
			The Block is available!
 **********************************************


	code:
	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);


	m->free(ptr1);



	void* ptr2 = m->allocator(15);

	m->printFreeList();

 */


/*
 *
		Test 6 For allocation of 5 blocks and free each block to check the free list:

		Heap - |0,1,2 ... 2000|

	    1. allocate 5 difference blocks
	    2. free each ptr that was returned from allocator
	    3. use free list printer



code:
	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);
	void* ptr2 = m->allocator(30);
	void* ptr3 = m->allocator(60);
	void* ptr4 = m->allocator(10);
	void* ptr5 = m->allocator(15);

	m->free(ptr1);
	m->free(ptr2);
	m->free(ptr3);
	m->free(ptr4);
	m->free(ptr5);

	m->printFreeList();

 */


/*

	Test 5 For allocation of block of 20 free and check the free list:

		Heap - |0,1,2 ... 2000|

	    1. allocate 20 bytes
	    2. free the ptr1 that was returned from allocator
	    3. use free list printer

	Need to print out:

 **********************************************
	Block No. 1:
	The Size of the Block: 20
	The Block is available!
 **********************************************


	code:
	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);


	m->free(ptr1);

	m->printFreeList();


 *
 */









/*
 *

	Test 4 For allocation of 2 blockes of 20 & 30 each and free them:

	Heap - |0,1,2 ... 2000|

   1. allocate 20 bytes
   2. allocate another 30 bytes
   3. get a first block pointer back size of header past
   4. get a second block pointer back size of header past
   5. free the ptr1 that was returned from allocator
   6. free the ptr2 that was returned from allocator
   7. print the variables of the header

Need to print out:

	The Size of the Block: 20
	The Block was freed!
 ************************************************************
	The Size of the Block: 30
	The Block was freed!


code:

	size_t capacity = 2000;
		MemoryManager *m = new MemoryManager(capacity);



		void* ptr1 = m->allocator(20);

		void* ptr2 = m->allocator(30);

		void* ptr1_free = ptr1;
		void* ptr2_free = ptr2;

		ptr1 -= sizeof(mem_control_block);
		ptr2 -= sizeof(mem_control_block);


		m->free(ptr1_free);
		m->free(ptr2_free);



		mem_control_block* mcb =(mem_control_block*)ptr1;


		cout << "The Size of the Block: " <<mcb->size << endl;
		if( mcb->is_available ? cout << "The Block was freed!" : cout << "The Block was not freed!" << endl);

	cout << endl << "************************************************************" << endl;

		mcb =(mem_control_block*)ptr2;

		cout << "The Size of the Block: " <<mcb->size << endl;
		if( mcb->is_available ? cout << "The Block was freed!" : cout << "The Block was not freed!" << endl);



 */



/*
 *


	Test 3 For allocation of 2 blockes of 20 & 30 each :

	Heap - |0,1,2 ... 2000|

   1. allocate 20 bytes
   2. allocate another 30 bytes
   3. get the first block pointer back size of header past
   4. get the second block pointer back size of header past
   5. print the variables of the header

Need to print out:

	The Size of the Block: 20
	The Block is not available!
 ************************************************************
	The Size of the Block: 30
	The Block is not available!


code:

	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);

	void* ptr2 = m->allocator(30);


	ptr1 -= sizeof(mem_control_block);
	ptr2 -= sizeof(mem_control_block);

	mem_control_block* mcb =(mem_control_block*)ptr1;

	cout << "The Size of the Block: " <<mcb->size << endl;
	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);

cout << "************************************************************" << endl;

	mcb =(mem_control_block*)ptr2;

	cout << "The Size of the Block: " <<mcb->size << endl;
	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);


 */








/*
 *

	Test 1 For allocation of 20 bytes :

	Heap - |0,1,2 ... 2000|

   1. allocate 20 bytes
   2. get the pointer back size of header past
   3. print the variables of the header

Need to print out:

Heap Capacity: 2000 Bytes.
The Size of the Block: 20
The Block is not available!


code:

  size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);


	cout << "Heap Capacity: " << capacity << " Bytes." <<  endl;

   void* ptr = m->allocator(20);

	ptr -= sizeof(mem_control_block);

	mem_control_block* mcb =(mem_control_block*)ptr;



cout << "The Size of the Block: " <<mcb->size << endl;

if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);

 */










/*
 *

	Test 2 For allocation of 20 bytes and free :

		Heap - |0,1,2 ... 2000|

	    1. allocate 20 bytes
	    2. get the pointer back size of header past
	    3. use free function
	    4. print the variables of the header

	The Size of the Block: 20
	The Block is available!


	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);


	   void* ptr = m->allocator(20);

	   m->free(ptr);
	ptr -= sizeof(mem_control_block);

	mem_control_block* mcb =(mem_control_block*)ptr;

	cout << "The Size of the Block: " <<mcb->size << endl;

	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);


 */












/*

	ptr -= sizeof(mem_control_block);
	mem_control_block* mcb =(mem_control_block*)ptr;
	cout << "The Size of the Block: " <<mcb->size << endl;
 *
 *	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);
 *
 */


//m->free(ptr1);

//m->printFreeList();

//m->free(ptr1);

// cout << m->getSize() << endl; // 358











/*
 *

	Test 8 For free list and get a free block between 2 allocations:

				Heap - |0,1,2 ... 2000|

			    1. allocate a block of 20 bytes
				2. free
				3. allocate a block of 32 bytes
				4. free
				5. allocate a block of 40 bytes
				6. allocate a block of 15 bytes
				7. use free list printer


needs to print out:

 **********************************************
	Block No. 1:
	The Size of the Block: 20
	The Block is available!
 **********************************************

 **********************************************
	Block No. 2:
	The Size of the Block: 4
	The Block is available!
 **********************************************


	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);


	m->free(ptr1);



	void* ptr2 = m->allocator(40);

	m->free(ptr2);



	void* ptr3 = m->allocator(32);


	m->printFreeList();


 */







/*
	Test 7 For allocation of free space & allocate again with smaller space to check if free list works:

			Heap - |0,1,2 ... 2000|

		    1. allocate a block of 20 bytes
			2. free
			3. allocate a block of 15 bytes
			4. use free list printer

			needs to print out:
 **********************************************
			Block No. 1:
			The Size of the Block: 1
			The Block is available!
 **********************************************


	code:
	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);


	m->free(ptr1);



	void* ptr2 = m->allocator(15);

	m->printFreeList();

 */


/*
 *
		Test 6 For allocation of 5 blocks and free each block to check the free list:

		Heap - |0,1,2 ... 2000|

	    1. allocate 5 difference blocks
	    2. free each ptr that was returned from allocator
	    3. use free list printer



code:
	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);
	void* ptr2 = m->allocator(30);
	void* ptr3 = m->allocator(60);
	void* ptr4 = m->allocator(10);
	void* ptr5 = m->allocator(15);

	m->free(ptr1);
	m->free(ptr2);
	m->free(ptr3);
	m->free(ptr4);
	m->free(ptr5);

	m->printFreeList();

 */


/*

	Test 5 For allocation of block of 20 free and check the free list:

		Heap - |0,1,2 ... 2000|

	    1. allocate 20 bytes
	    2. free the ptr1 that was returned from allocator
	    3. use free list printer

	Need to print out:

 **********************************************
	Block No. 1:
	The Size of the Block: 20
	The Block is available!
 **********************************************


	code:
	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);


	m->free(ptr1);

	m->printFreeList();


 *
 */









/*
 *

	Test 4 For allocation of 2 blockes of 20 & 30 each and free them:

	Heap - |0,1,2 ... 2000|

    1. allocate 20 bytes
    2. allocate another 30 bytes
    3. get a first block pointer back size of header past
    4. get a second block pointer back size of header past
    5. free the ptr1 that was returned from allocator
    6. free the ptr2 that was returned from allocator
    7. print the variables of the header

Need to print out:

	The Size of the Block: 20
	The Block was freed!
 ************************************************************
	The Size of the Block: 30
	The Block was freed!


code:

	size_t capacity = 2000;
		MemoryManager *m = new MemoryManager(capacity);



		void* ptr1 = m->allocator(20);

		void* ptr2 = m->allocator(30);

		void* ptr1_free = ptr1;
		void* ptr2_free = ptr2;

		ptr1 -= sizeof(mem_control_block);
		ptr2 -= sizeof(mem_control_block);


		m->free(ptr1_free);
		m->free(ptr2_free);



		mem_control_block* mcb =(mem_control_block*)ptr1;


		cout << "The Size of the Block: " <<mcb->size << endl;
		if( mcb->is_available ? cout << "The Block was freed!" : cout << "The Block was not freed!" << endl);

	cout << endl << "************************************************************" << endl;

		mcb =(mem_control_block*)ptr2;

		cout << "The Size of the Block: " <<mcb->size << endl;
		if( mcb->is_available ? cout << "The Block was freed!" : cout << "The Block was not freed!" << endl);



 */



/*
 *


	Test 3 For allocation of 2 blockes of 20 & 30 each :

	Heap - |0,1,2 ... 2000|

    1. allocate 20 bytes
    2. allocate another 30 bytes
    3. get the first block pointer back size of header past
    4. get the second block pointer back size of header past
    5. print the variables of the header

Need to print out:

	The Size of the Block: 20
	The Block is not available!
 ************************************************************
	The Size of the Block: 30
	The Block is not available!


code:

	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);



	void* ptr1 = m->allocator(20);

	void* ptr2 = m->allocator(30);


	ptr1 -= sizeof(mem_control_block);
	ptr2 -= sizeof(mem_control_block);

	mem_control_block* mcb =(mem_control_block*)ptr1;

	cout << "The Size of the Block: " <<mcb->size << endl;
	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);

cout << "************************************************************" << endl;

	mcb =(mem_control_block*)ptr2;

	cout << "The Size of the Block: " <<mcb->size << endl;
	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);


 */








/*
 *

	Test 1 For allocation of 20 bytes :

	Heap - |0,1,2 ... 2000|

    1. allocate 20 bytes
    2. get the pointer back size of header past
    3. print the variables of the header

Need to print out:

Heap Capacity: 2000 Bytes.
The Size of the Block: 20
The Block is not available!


code:

   size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);


	cout << "Heap Capacity: " << capacity << " Bytes." <<  endl;

    void* ptr = m->allocator(20);

	ptr -= sizeof(mem_control_block);

	mem_control_block* mcb =(mem_control_block*)ptr;



cout << "The Size of the Block: " <<mcb->size << endl;

if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);

 */










/*
 *

	Test 2 For allocation of 20 bytes and free :

		Heap - |0,1,2 ... 2000|

	    1. allocate 20 bytes
	    2. get the pointer back size of header past
	    3. use free function
	    4. print the variables of the header

	The Size of the Block: 20
	The Block is available!


	size_t capacity = 2000;
	MemoryManager *m = new MemoryManager(capacity);


	   void* ptr = m->allocator(20);

	   m->free(ptr);
	ptr -= sizeof(mem_control_block);

	mem_control_block* mcb =(mem_control_block*)ptr;

	cout << "The Size of the Block: " <<mcb->size << endl;

	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);


 */












/*

	ptr -= sizeof(mem_control_block);
	mem_control_block* mcb =(mem_control_block*)ptr;
	cout << "The Size of the Block: " <<mcb->size << endl;
 *
 *	if( mcb->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);
 *
 */


//m->free(ptr1);

//m->printFreeList();

//m->free(ptr1);

// cout << m->getSize() << endl; // 358




