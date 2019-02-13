
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecordIterator.h"

MyDB_PageReaderWriter::MyDB_PageReaderWriter( MyDB_BufferManagerPtr myBuffer, MyDB_TablePtr which_table, size_t which_Page) {
    this->my_buffer = myBuffer;
	this->my_page_hb = myBuffer->getPage(which_table, which_Page);
	this->pageType = MyDB_PageType :: RegularPage;
	this->pageSize = myBuffer->getPageSize();
	//clear();
}
void MyDB_PageReaderWriter :: clear () {
	PageInfo* pinfo = (PageInfo*) this->my_page_hb->getBytes();
	pinfo->lastByte = 0;
	//set this page dirty
	this->my_page_hb->wroteBytes();
}
MyDB_PageHandle MyDB_PageReaderWriter::getPage_handle() {
	return this->my_page_hb;
}
MyDB_PageType MyDB_PageReaderWriter :: getType () {
	return this->pageType;
	//return MyDB_PageType :: RegularPage;
}
// return an iterator over this page... each time returnVal->next () is
// called, the resulting record will be placed into the record pointed to
// by iterateIntoMe
MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr myRecPtr) {
	//MyDB_PageReaderWriterPtr linkthis = make_shared<MyDB_PageReaderWriter>();
	return  make_shared<MyDB_PageRecordIterator>(*this, myRecPtr);

}

void MyDB_PageReaderWriter :: setType (MyDB_PageType pageType) {
	//wrotebyte??
	this->pageType = pageType;
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr myRecPtr) {
	PageInfo* pinfo = (PageInfo*) (this->my_page_hb->getBytes());

	if((sizeof(pinfo)+pinfo->lastByte+myRecPtr->getBinarySize())<=this->pageSize) {
		//write copy of the record to the next available address in page
		//cout<<"lastbyte is "<<pinfo->lastByte<<" and record size is "<<myRecPtr->getBinarySize()<<endl;
		myRecPtr->toBinary((void*)(pinfo->data+pinfo->lastByte));
		//cout<<"check in trying to append in page"<<endl;
		//cout<<"update lastbyte to ";
		pinfo->lastByte+=myRecPtr->getBinarySize();
		//cout<<pinfo->lastByte<<endl;
		//cout<<"lastbyte AFTER ADDING REC SIZE IS "<<pinfo->lastByte<<endl;
		this->my_page_hb->wroteBytes();
		return true;
	}
	return false;
}

#endif
