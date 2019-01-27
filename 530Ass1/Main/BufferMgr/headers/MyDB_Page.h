#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <iostream>
#include <unistd.h>
#include <MyDB_Table.h>
#include "buffer_node.h"
using namespace std;
/*
	types of pages-> bool anonymous
	pinning pages bool pin = true -> can't evict 
	after calling wrotebyte, bool isDirty = true
*/
class Node;
class MyDB_Table;
class Page{
private:
	char* data;
	Node* buffer_addr;
	int ref_counter;
	//buf_addr
	bool is_Anonymous;
	bool is_Pinned;
	bool is_Dirty;
	int temp_slot_id;
	long offset;
	MyDB_TablePtr tp;

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
	void setData(char*);
	void set_tp(MyDB_TablePtr);
	void set_offset(long);
	MyDB_TablePtr get_tp();
	long get_offset();
	void set_temp_id(int);
	int get_temp_id();

};

#endif