
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"

void *MyDB_PageHandleBase :: getBytes () {
	return nullptr;
}

void MyDB_PageHandleBase :: wroteBytes () {
	
}

MyDB_PageHandleBase :: ~MyDB_PageHandleBase () {
}

Page* get_my_page(){
	return my_page;
}

void set_my_page(Page* my_page){
	this->my_page = my_page;
}

#endif

