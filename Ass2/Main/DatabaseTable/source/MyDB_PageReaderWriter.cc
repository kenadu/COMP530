
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"

MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_BufferManagerPtr myBuffer, MyDB_TablePtr which_table, size_t which_Page) {
    this->my_buffer = myBuffer;
	this->my_page_hb = myBuffer->getPage(which_table, which_Page);
}
void MyDB_PageReaderWriter :: clear () {
}

MyDB_PageType MyDB_PageReaderWriter :: getType () {
	return MyDB_PageType :: RegularPage;
}
// return an iterator over this page... each time returnVal->next () is
// called, the resulting record will be placed into the record pointed to
// by iterateIntoMe
MyDB_RecordIteratorPtr MyDB_PageReaderWriter :: getIterator (MyDB_RecordPtr) {
	return nullptr;
}

void MyDB_PageReaderWriter :: setType (MyDB_PageType) {
}

bool MyDB_PageReaderWriter :: append (MyDB_RecordPtr) {
	return true;
}

#endif
