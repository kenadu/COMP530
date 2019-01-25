#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <iostream>
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
	char* buffer_addr;
	int ref_counter;
	//buf_addr
	bool is_Anonymous;
	bool is_Pinned;
	bool is_Dirty;
public: 
	Page();
	Page(char*, bool, bool, bool);
	bool getAnonymous();
	char* getBuffer_addr();
	void setBuffer_addr(char*);
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