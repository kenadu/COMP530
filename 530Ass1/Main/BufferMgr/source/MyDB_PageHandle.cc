
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"

void *MyDB_PageHandleBase :: getBytes () {
	Node* tmp = my_page->getBuffer_addr();


	//in buffer put it's eviction bit to false
	if(tmp!= nullptr){
		tmp->set_bit(false);
	}
	// not in buffer put it to the next available slot
	else{
		tmp = bm->next_empty_node();
		//if not full
		if(tmp!=nullptr){
			my_page->setBuffer_addr(tmp);
			tmp->set_page(my_page);
		}
		else {
			bool bit = bm->get_node_bit();
			//bit == 0 not evictable
			while (!bit) {
				//increment ptr set the current node bit to evictable
				bm->increClock_ptr();
				bit = bm->get_node_bit();
			}
			//after while, ptr points to the lru, the evictable node
			bm->evictPage_and_add(my_page);
		}
	}
	tmp = nullptr;
	return nullptr;
}

void MyDB_PageHandleBase :: wroteBytes () {
	my_page->setDirty(true);
}

MyDB_PageHandleBase :: ~MyDB_PageHandleBase () {
	my_page->decre_cntr();
	//if not handle left
	if(!my_page->check_cntr()){
		//set unpin
		my_page->setPinned(false);
		//if anonymous, evict from buffer
		if(my_page->getAnonymous()){
			my_page->getBuffer_addr()->set_page(nullptr);
			my_page->setBuffer_addr(nullptr);
		}
	}

}

Page* MyDB_PageHandleBase::get_my_page(){
	return my_page;
}

void MyDB_PageHandleBase:: set_my_page(Page* my_page){
	this->my_page = my_page;
	this->my_page->incre_cntr();
}

#endif

