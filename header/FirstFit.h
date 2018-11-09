/*
 * FirstFit.h
 *
 *  Created on: 6 בינו 2017
 *      Author: tal-pc
 */

#ifndef FIRSTFIT_H_
#define FIRSTFIT_H_

#include <list>

#include "Method.h"

class mem_control_block;


class FirstFit : public Method{
public:

	FirstFit();

	virtual ~FirstFit();


    void add(mem_control_block* mcb);

   mem_control_block* find(long& num_of_bytes);

    void remove(mem_control_block* mcb);

    virtual bool isEmpty();

    void clear();

    void print();



	list<mem_control_block*> free_list;


};


#endif /* FIRSTFIT_H_ */
