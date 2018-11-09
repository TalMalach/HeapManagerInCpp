/*
 * Mempool.h
 *
 *  Created on: 27 בדצמ 2016
 *      Author: tal-pc
 */

#ifndef MEMPOOL_H_
#define MEMPOOL_H_
#include <stdlib.h>

using namespace std;

class Mempool
{
public:
	/* Destructor */


	/* Getters & Setters */

	void* getBlock(const size_t& index) const;

	/*********************/


	static Mempool* getInstance(size_t& capacity) ;

void freeMemory();

private:
	~Mempool();

	char* pool;
	static Mempool *single;
	static bool instanceFlag;
	Mempool(size_t&  capacity);

};

#endif /* MEMPOOL_H_ */
