/*
 * FirstFit.cpp
 *
 *  Created on: 6 בינו 2017
 *      Author: tal-pc
 */

#include "FirstFit.h"

#include <iostream>

#include "mem_control_block.h"



FirstFit::FirstFit(){ }

FirstFit::~FirstFit() {
}



mem_control_block* FirstFit::find(long& num_of_bytes)
{

	mem_control_block *mcb;


		for (auto it = this->free_list.begin(); it != this->free_list.end(); ++it)
		{
			if ((*it)->size >= num_of_bytes)
			{

				mcb =  (mem_control_block*)(*it);
				free_list.remove(*it);
				return mcb;
			}
		}
		return NULL;


}

void FirstFit::add(mem_control_block* mcb)
{

	if(mcb->size > 0)free_list.push_back(mcb);
}


void FirstFit::remove(mem_control_block* mcb)
{

	free_list.remove(mcb);

}

bool FirstFit::isEmpty()
{
	return free_list.empty();
}

void FirstFit::clear()
{
	free_list.clear();
}


void FirstFit::print()
{
	mem_control_block *mcb;

	size_t index = 1;

	for (auto it = this->free_list.begin(); it != this->free_list.end(); ++it) {
		cout << endl << "**********************************************" << endl;
		cout << "Block No. " << index << ":" << endl;
		cout << "The Size of the Block: " <<(size_t)((mem_control_block*)(*it))->size << endl;

		if( (mem_control_block*)(*it)->is_available ? cout << "The Block is available!" : cout << "The Block is not available!" << endl);
		cout << endl << "**********************************************" << endl;

		index++;
	}
}
