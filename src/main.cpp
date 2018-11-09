/*
 * main.cpp
 *
 *  Created on: Dec 28, 2016
 *      Author: nadav
 */



#include "Mempool.h"
#include "MemoryManager.h"
#include "MemoryHandler.h"

#include "FirstFit.h"
#include "Method.h"
#include "List.h"


#include <iostream>
#include <list>
#include <math.h>
#include <string.h>

using namespace std;


void printHelp(){
	cout << "hello" << endl;
	cout << "how to use the program: " << endl;
	cout << "./prog.o -S <number to size> -v <optional> -f <1 to Firstfit and 2 to Bestfit> \n"; //the input must be 1 because i didn't implement BestFIt yet
}

void commandHandler(int argc, char**argv,size_t &capacity,bool &is_valgrind,size_t &flag){
	if (argc < 2){
		printHelp();
		exit(0);
	}
	else if(argv[1]!=nullptr){
		if ((strcmp(argv[1],"-S") != 0) || (strcmp(argv[1],"-help") == 0 )){
			printHelp();
			exit(0);
		}
		if(argv[3]!=nullptr){
			if ((strcmp(argv[3],"-v")!=0)){
				if ((strcmp(argv[3],"-f")!=0)){
					printHelp();
					exit(0);
				}
				else{
					if (argv[4]!=nullptr){
						is_valgrind=0;
						flag=atoi(argv[4]);
						capacity=atoi(argv[2]);
					}
					else{
						printHelp();
						exit(0);
					}
				}
			}

			else if(argv[4] != nullptr){
				if(strcmp(argv[4],"-f")!=0){
					printHelp();
					exit(0);
				}
				else{
					if (argv[5]!=nullptr){
						capacity=atoi(argv[2]);
						is_valgrind=1;
						flag=atoi(argv[5]);
					}
					else{
						printHelp();
						exit(0);
					}
				}
			}
			else{
				printHelp();
				exit(0);
			}
		}
		else{
			printHelp();
			exit(0);
		}
	}
	else{
		printHelp();
		exit(0);
	}
}

int main(int argc, char **argv){

	size_t  capacity=0;
	bool is_valgrind=0;
	size_t flag=1;


	commandHandler(argc,argv,capacity,is_valgrind,flag);


	MemoryHandler mem(capacity,is_valgrind, flag);


	char* A = new char('A');
	long* id = new long(302926456);


	char* B = new char('B');
	long* seq1 = new long(123456789);


	char* C = new char('C');
	long* seq2 = new long(1122334455);


	char* D = new char('D');
	long* seq3 = new long(9999999999);

	char* E = new char('E');
	long* seq4 = new long(3939393939);


	List<std::pair<char*, long*>> *list1 = new List<std::pair<char*, long*>>();

	list1->push_back(std::make_pair(A,id));
	list1->push_back(std::make_pair(B,seq1));
	list1->push_back(std::make_pair(C,seq2));
	list1->push_back(std::make_pair(D,seq3));
	list1->push_back(std::make_pair(E,seq4));


	cout << list1->size() << endl; //suppose to be 5!

	cout << *(list1->getHead()->value.second) << endl; // suppose to be my id


	cout << *(list1->getHead()->getNext()->value.second) << endl; // suppose to be the sequence 1

	list1->remove(3);

	cout << list1->size() << endl; //suppose to be 4!

	list1->remove(2);
	list1->remove(1);
	cout << list1->size() << endl; //suppose to be 2!


	delete list1;



	//checking operator overloading///

	List<std::pair<char*, long*>> *list2 = new List<std::pair<char*, long*>>(40);

	List<std::pair<char*, long*>> *list3 = new List<std::pair<char*, long*>>(20);

	cout << list2->size() << endl; //suppose to be 40!


	cout << list3->size() << endl; //suppose to be 20!


	list3 = list2;


	cout << list3->size() << endl; //suppose to be 40!




	delete list2;
	delete list3;





	//****Checking allocation and de-allocation****/////

	List<std::pair<char*, long*>> *list_4 = new List<std::pair<char*, long*>>(25);

	List<std::pair<char*, long*>> *list_5 = new List<std::pair<char*, long*>>(20);

	List<std::pair<char*, long*>> *list_6 = new List<std::pair<char*, long*>>(15);

	List<std::pair<char*, long*>> *list_7 = new List<std::pair<char*, long*>>(14);

	List<std::pair<char*, long*>> *list_8 = new List<std::pair<char*, long*>>(13);

	List<std::pair<char*, long*>> *list_9 = new List<std::pair<char*, long*>>(1);

	List<std::pair<char*, long*>> *list_10 = new List<std::pair<char*, long*>>();


	cout << "List 4: Size: " << list_4->size() << endl; //25
	delete list_4;
	cout<<"FREED" << endl;
	cout << "List 5: Size: " << list_5->size() << endl; //20
	delete list_5;
	cout<<"FREED" << endl;
	cout << "List 6: Size: " << list_6->size() << endl; //15
	delete list_6;
	cout<<"FREED" << endl;
	cout << "List 7: Size: " << list_7->size() << endl;//14
	delete list_7;
	cout<<"FREED" << endl;
	cout << "List 8: Size: " << list_8->size() << endl;//13
	delete list_8;
	cout<<"FREED" << endl;
	cout << "List 9: Size: " << list_9->size() << endl;//1
	delete list_9;
	cout<<"FREED" << endl;
	cout << "List 10: Size: " << list_10->size() << endl;//0
	delete list_10;
	cout<<"FREED" << endl;


	delete A;
	delete B;
	delete C;
	delete D;
	delete E;
	delete id;
	delete seq1;
	delete seq2;
	delete seq3;
	delete seq4;

	cout << "Checking Plan Done Successfully" << endl;

	return 0;
}




