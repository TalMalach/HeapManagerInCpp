
#ifndef METHOD_H_
#define METHOD_H_

class MemoryManager;

class mem_control_block;

using namespace std;

class Method{
public:
	  enum AlgorithmType { FIRST_FIT = 1, BEST_FIT = 2};



	    virtual void add(mem_control_block* mcb) = 0;

	    virtual mem_control_block* find(long& num_of_bytes) = 0;

	    virtual void remove(mem_control_block* mcb) = 0;

	    virtual void clear() = 0;

	    virtual bool isEmpty() = 0;


	    virtual  void print() = 0;





};












#endif /* METHOD_H_ */
