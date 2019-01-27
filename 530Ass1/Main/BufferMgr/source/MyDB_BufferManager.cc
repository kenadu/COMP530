
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <string>
#include <iterator>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr whichTable, long i) {
    map<pair<MyDB_TablePtr, long>, Page*>::iterator it = table_page.find(make_pair(whichTable,i));
    Page * res_page = nullptr;
    //not in table, create new page, new phb
    if(it==table_page.end()){
        res_page = new Page(nullptr, false, false, false);
        res_page->set_offset(i);
        res_page->set_tp(whichTable);
        table_page.insert({make_pair(whichTable,i), res_page});
    }
        //in table, create new phb
    else{
        res_page = it->second;
    }
    MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>(res_page, this);

    return create_phb;
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {
	int slot_id = get_next_slot_id();
	Page* create_page = new Page(nullptr, false, true, false);
	//anony_pages[slot_id] = create_page;
	create_page->set_temp_id(slot_id);
	MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>(create_page, this);
	return create_phb;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr whichTable, long i) {
	map<pair<MyDB_TablePtr, long>, Page*>::iterator it = table_page.find(make_pair(whichTable,i));
	Page * res_page = nullptr;
	//not in table, create new page, new phb
	if(it==table_page.end()){	
		res_page = new Page(nullptr, true, false, false);
		res_page->set_offset(i);
		res_page->set_tp(whichTable);
        table_page.insert({make_pair(whichTable,i), res_page});
	}
	//in table, create new phb
	else{
		res_page = it->second;
	}
	MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>(res_page, this);

	return create_phb;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
	int slot_id = get_next_slot_id();
	Page* create_page = new Page(nullptr, true, true, false);
	//anony_pages[slot_id] = create_page;
	create_page->set_temp_id(slot_id);
	MyDB_PageHandle create_phb = make_shared<MyDB_PageHandleBase>(create_page, this);
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
    if(clock_ptr == numPages) clock_ptr = 0;
}
Node* MyDB_BufferManager::next_empty_node() {
	for(int i = 0; i < numPages; i++){
		//cout << "empty node: " << endl;
		//this node is empty
		if(clock[i]->get_page()== nullptr)
			return clock[i];
	}
	return nullptr;
}

int MyDB_BufferManager::get_next_slot_id(){
	if(!available_slot_id.empty()){
		int next_slot = available_slot_id.front();
		available_slot_id.pop();
		return next_slot;
	}
	int next_slot = next_slot_id++;
	return next_slot;
}

MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile) {
	this->pageSize = pageSize;
	this->numPages = numPages;
	this->tempFile = tempFile;
	int tempFileDesc = open_file(tempFile);
	this->next_slot_id = 0;
	this->tempFileDesc = tempFileDesc;
	clock_ptr = 0;
	char* buffer = (char*) malloc(pageSize * numPages);
	for(int i = 0; i < pageSize; i++){
		clock.push_back(new Node(buffer + i * pageSize));
	}
}
bool MyDB_BufferManager::get_node_bit() {
	//pinned page is unevictable
	if(clock[clock_ptr]->get_page()->getPinned()) {
		cout<<"here clock ptr is "<<clock_ptr<<endl;
		cout<<"IS PINNED "<<endl;
		return false;
	}
	return clock[clock_ptr]->get_bit();
}
void MyDB_BufferManager::read_and_write(Page * new_page) {
	if(!new_page->getAnonymous()){
		int file_descript = open_file(new_page->get_tp()->getStorageLoc());
		cout << "file desc : " << file_descript << endl;
		new_page->read_from_disk(file_descript, new_page->get_offset(), pageSize);
	}else{
		new_page->read_from_disk(tempFileDesc, new_page->get_temp_id(), pageSize);
	}
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
        int file_descript = open_file(whichTable.first->getStorageLoc());
        evPage->write_back_to_disk(file_descript,whichTable.second, pageSize);
    }
	//remove_anony(evPage);
    //not dirty throw
    tmp->set_page(nullptr);
    evPage->setData(nullptr);
    new_page->setData(tmp->get_addr());
    tmp->set_bit(false);
    //insert new page in.
    tmp->set_page(new_page);
    new_page->setBuffer_addr(tmp);
	read_and_write(new_page);



}


void MyDB_BufferManager::print_pin_state(){
	for(int i = 0 ; i < this->clock.size() ; i++){
		if(clock[i]->get_page() != nullptr){
			cout << "pin state " << clock[i]->get_page()->getPinned() << endl;
		}else{
			cout << "pin state " << "empty" << endl;
		}

	}
}
int MyDB_BufferManager :: open_file(string filename) {
	int file_desc = open(filename.c_str(),O_CREAT|O_RDWR|O_FSYNC,0666);
	return file_desc;
}

MyDB_BufferManager :: ~MyDB_BufferManager () {
	for(auto node : clock){
		if(node->get_page() != nullptr){
			if(node->get_page()->getAnonymous()){
				if(node->get_page()->getDirty()){
					node->get_page()->write_back_to_disk(tempFileDesc, node->get_page()->get_temp_id(), pageSize);
				}
			}else{
				if(node->get_page()->getDirty()){
					int file_desc = open_file(node->get_page()->get_tp()->getStorageLoc());
					node->get_page()->write_back_to_disk(file_desc, node->get_page()->get_offset(), pageSize);
				}
			}
		}
	}

	for(auto x : table_page){
		delete x.second;
	}

}
	
#endif


