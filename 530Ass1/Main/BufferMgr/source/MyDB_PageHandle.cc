
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"

MyDB_PageHandleBase::MyDB_PageHandleBase() {

}

MyDB_PageHandleBase::MyDB_PageHandleBase(Page *page, MyDB_BufferManager *buf_mgr) {
	this->my_page = page;
	this->my_page->incre_cntr();
	this->bm = buf_mgr;
}

void * MyDB_PageHandleBase :: getBytes () {

	Node* tmp = my_page->getBuffer_addr();


	//in buffer put it's eviction bit to false
	if(tmp!= nullptr){
		tmp->set_bit(false);
	}
	// not in buffer put it to the next available slot
	else{

		tmp = bm->next_empty_node();
		cout << "tmp node" << tmp << endl;
		//if not full
		if(tmp != nullptr){
			my_page->setBuffer_addr(tmp);
			tmp->set_page(my_page);
			my_page->setData(tmp->get_addr());
			bm->read_and_write(my_page);
            //bm->print_pin_state();
		}
		else {
			cout << "buffer full" << endl;
			bool bit = bm->get_node_bit();
			//bit == 0 not evictable
			//bm->print_pin_state();
			while (!bit) {
				cout << "bit : " << bit << endl;

				//increment ptr set the current node bit to evictable
				bm->increClock_ptr();
				bit = bm->get_node_bit();
			}
			//after while, ptr points to the lru, the evictable node
			bm->evictPage_and_add(my_page);
		}
	}
	cout << "my_page get node" << my_page->getBuffer_addr() << endl;
	cout << "my_page get data" << (void*)my_page->getData() << endl;
	return (void*)my_page->getData();
}



void MyDB_PageHandleBase :: wroteBytes () {
	my_page->setDirty(true);
}

MyDB_PageHandleBase :: ~MyDB_PageHandleBase () {

	my_page->decre_cntr();
	cout << "~~my_page node" << my_page->getBuffer_addr() << endl;
	//if not handle left
	if(!my_page->check_cntr()){
		cout << "anony" << endl;
		//set unpin
		my_page->setPinned(false);
		//if anonymous, evict from buffer
		if(my_page->getAnonymous() && my_page->getBuffer_addr() != nullptr){
			cout << my_page->getBuffer_addr() << endl;
			my_page->getBuffer_addr()->set_page(nullptr);
			my_page->setBuffer_addr(nullptr);
			my_page->setData(nullptr);
			//bm->remove_anony(my_page);

		}
	}
	cout << "destructor" << endl;

}

Page* MyDB_PageHandleBase::get_my_page(){
	return my_page;
}

void MyDB_PageHandleBase:: set_my_page(Page* my_page){
	this->my_page = my_page;
	this->my_page->incre_cntr();
}

#endif

