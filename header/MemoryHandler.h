/*
 * MemoryHandler.h
 *
 *  Created on: 8 בינו 2017
 *      Author: tal-pc
 */

#ifndef MEMORYHANDLER_H_
#define MEMORYHANDLER_H_
#include "MemoryManager.h"


#include <stdlib.h>
using namespace std;


class MemoryHandler {
public:
	MemoryHandler(const size_t& size,const int& type,const bool& is_valgrind);
	virtual ~MemoryHandler();



};



#endif /* MEMORYHANDLER_H_ */
