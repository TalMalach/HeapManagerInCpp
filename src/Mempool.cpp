/*
 * Mempool.cpp
 *
 *  Created on: 27 בדצמ 2016
 *      Author: tal-pc
 */

#include "Mempool.h"
#include <iostream>

bool Mempool::instanceFlag = false;
Mempool* Mempool::single = NULL;



Mempool::Mempool(size_t& capacity)
{
	pool = (char*)malloc(capacity);

}




Mempool* Mempool::getInstance(size_t& capacity)
{
	if(! instanceFlag)
	{
		single = (Mempool*)malloc(sizeof(Mempool));
		new (single)Mempool(capacity);
		instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}



Mempool::~Mempool(){}


void* Mempool::getBlock(const size_t& index) const {return pool + index;}


void  Mempool::freeMemory()
{
	  free(pool);

	  instanceFlag = false;

	  free(single);




}


