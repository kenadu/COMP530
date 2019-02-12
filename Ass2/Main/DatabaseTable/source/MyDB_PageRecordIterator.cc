
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"
MyDB_PageRecordIterator::MyDB_PageRecordIterator() {

}

MyDB_PageRecordIterator::MyDB_PageRecordIterator (MyDB_PageReaderWriterPtr point_to_page, MyDB_RecordPtr point_to_rec){
    this->point_to_page = point_to_page;
    this->point_to_rec = point_to_rec;
    PageInfo* pinfo = (PageInfo*) point_to_page->getPage_handle()->getBytes();
    this->cursor = (size_t)pinfo->data;
    this->binaryData = NULL;
}

void MyDB_PageRecordIterator::getNext() {
    this->point_to_rec->fromBinary(this->binaryData);
    this->cursor += this->point_to_rec->getBinarySize();
}

bool MyDB_PageRecordIterator::hasNext() {
    PageInfo* pinfo = (PageInfo*) this->point_to_page->getPage_handle()->getBytes();
    // cursor points at the lastbyte (which is the end of "has record")
    if(this->cursor==pinfo->lastByte) return false;
    this->binaryData = (void*)pinfo->data[this->cursor];
    return true;
}

MyDB_PageRecordIterator::~MyDB_PageRecordIterator() {

}
#endif
