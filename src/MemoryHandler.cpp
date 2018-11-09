/*
 * MemoryHandler.cpp
 *
 *  Created on: 8 בינו 2017
 *      Author: tal-pc
 */

#include "MemoryHandler.h"

#include <iostream>
#include "mem_control_block.h"


static MemoryManager memory_manager;


MemoryHandler::MemoryHandler(const size_t& size,const int& type,const bool& is_valgrind)
{
memory_manager.initComponents(size,type, is_valgrind);
}

MemoryHandler::~MemoryHandler()
{
	memory_manager.clear();
	memory_manager.printValgrind();
}

void* operator new(size_t size)
{
	return (void*)memory_manager.allocator(size);
}

	void operator delete(void* firstbyte)
	{
		memory_manager.free(firstbyte);
	}

	void* operator new[](size_t size){

		return (void*)memory_manager.allocator(size);
	}

	void operator delete[](void* firstbyte){
		memory_manager.free(firstbyte);
	}





