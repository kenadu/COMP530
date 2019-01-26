#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <iostream>
#include <unistd.h>
#include "buffer_node.h"
using namespace std;
class Page;
/*
	types of pages-> bool anonymous
	pinning pages bool pin = true -> can't evict 
	after calling wrotebyte, bool isDirty = true
*/
class Page{
private:
	char* data;
	Node* buffer_addr;
	int ref_counter;
	//buf_addr
	bool is_Anonymous;
	bool is_Pinned;
	bool is_Dirty;
public: 
	Page();
	Page(Node*, bool, bool, bool);
	bool getAnonymous();
	Node* getBuffer_addr();
	char* read_from_disk(int, long, int);
	void write_back_to_disk(int, long, int);
	void setBuffer_addr(Node*);
	void incre_cntr();
	void decre_cntr();
	bool check_cntr();
	void setAnonymous(bool);
	bool getPinned();
	void setPinned(bool);
	bool getDirty();
	void setDirty(bool);
	char* getData();

};

#endif