
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <string>
#include <iterator>
#include <unistd.h>

using namespace std;

MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr whichTable, long i) {
	MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>();
	map<pair<MyDB_TablePtr, long>, Page*>::iterator it = table_page.find(make_pair(whichTable,i));
	
	//not in table, create new page, new phb
	if(it==table_page.end()){	
		Page* create_page = new Page(nullptr, false, false, false);
		table_page.insert({make_pair(whichTable,i), create_page});
		create_phb->set_my_page(create_page);
	}
	//in table, create new phb
	else{
		Page* existed_page = it->second;
		create_phb->set_my_page(existed_page);
	}

	return create_phb;		
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {
	MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>();
	Page* create_page = new Page(nullptr, false, true, false);
	create_phb->set_my_page(create_page);
	anony_pages.insert(create_page);
	return create_phb;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr whichTable, long i) {
	MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>();
	map<pair<MyDB_TablePtr, long>, Page*>::iterator it = table_page.find(make_pair(whichTable,i));
	
	//not in table, create new page, new phb
	if(it==table_page.end()){	
		Page* create_page = new Page(nullptr, true, false, false);
		create_phb->set_my_page(create_page);
	}
	//in table, create new phb
	else{
		Page* existed_page = it->second;
		create_phb->set_my_page(existed_page);
	}

	return create_phb;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
	MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>();
	Page* create_page = new Page(nullptr, true, true, false);
	create_phb->set_my_page(create_page);
	anony_pages.insert(create_page);
	return create_phb;
}

void MyDB_BufferManager :: unpin (MyDB_PageHandle unpinMe) {
	unpinMe->get_my_page()->setPinned(false);
}

int MyDB_BufferManager::getClock_ptr(){
    return clock_ptr;
}

void MyDB_BufferManager::increClock_ptr(){
	//ptr increment only when clock is full;
	//set the node to be evictable, bit is true
	//then, ++
	clock[clock_ptr]->set_bit(true);
    clock_ptr++;
    if(clock_ptr==pageSize) clock_ptr = 0;
}
Node* MyDB_BufferManager::next_empty_node() {
	for(int i = 0; i<numPages; i++){
		//this node is empty
		if(clock[i]->get_page()== nullptr)
			return clock[i];
	}
	return nullptr;
}
MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile) {
	this->pageSize = pageSize;
	this->numPages = numPages;
	this->tempFile = tempFile;
	clock_ptr = 0;
	char* buffer = (char*) malloc(pageSize*numPages);
	for(int i = 0; i< pageSize; i++){
		clock.push_back(new Node(buffer+i*pageSize));
	}
}
bool MyDB_BufferManager::get_node_bit() {
	//pinned page is unevictable
	if(clock[clock_ptr]->get_page()->getPinned()) return false;
	return clock[clock_ptr]->get_bit();
}
void MyDB_BufferManager::evictPage_and_add(Page* new_page) {
    Node* tmp = clock[clock_ptr];
    Page* evPage = tmp->get_page();
    //dirty writeback;
    if(evPage->getDirty()) {
        map<pair<MyDB_TablePtr, long>, Page *>::iterator it;
        pair<MyDB_TablePtr, long> whichTable;
        for (it = table_page.begin(); it != table_page.end(); ++it){
            if (it->second == evPage)
                whichTable = it->first;
        }
        int file_descript=open(whichTable.first->getStorageLoc().c_str(),O_CREAT|O_RDWR|O_FSYNC,0666);
        evPage->write_back_to_disk(file_descript,whichTable.second,pageSize);
    }
    //not dirty throw
    tmp->set_page(nullptr);
    tmp->set_bit(false);
    //insert new page in.
    tmp->set_page(new_page);
    new_page->setBuffer_addr(tmp);

    tmp = nullptr;
    evPage = nullptr;
}
MyDB_BufferManager :: ~MyDB_BufferManager () {

}
	
#endif


