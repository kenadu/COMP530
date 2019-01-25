
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <string>
#include <iterator>

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

MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile) {
	this->pageSize = pageSize;
	this->numPages = numPages;
	this->tempFile = tempFile;
	char* buffer = (char*) malloc(pageSize*numPages);
	for(int i = 0; i< pageSize; i++){
		clock.push_back(new Node(buffer+i*pageSize));
	}
}

MyDB_BufferManager :: ~MyDB_BufferManager () {

}
	
#endif


