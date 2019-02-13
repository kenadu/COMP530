
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"

MyDB_PageRecordIterator::MyDB_PageRecordIterator (MyDB_PageReaderWriter& point_to_page, MyDB_RecordPtr point_to_rec):MyDB_RecordIterator(),point_to_page(point_to_page){
    //this->point_to_page = point_to_page;

    this->point_to_rec = point_to_rec;

    PageInfo* pinfo = (PageInfo*) point_to_page.getPage_handle()->getBytes();

    this->cursor = 0;
    //cout<<"page cursor initialize at "<<this->cursor<<endl;
    this->binaryData = NULL;
    has_next = false;
}

void MyDB_PageRecordIterator::getNext() {
    if(!has_next) return;
    this->point_to_rec->fromBinary(this->binaryData);
    this->cursor += this->point_to_rec->getBinarySize();
    has_next= false;
}

bool MyDB_PageRecordIterator::hasNext() {
    PageInfo* pinfo = (PageInfo*) this->point_to_page.getPage_handle()->getBytes();
    //cout<<"page cursor at "<<this->cursor<<" and this page head's last byte is at "<<pinfo->lastByte<<endl;
    // cursor points at the lastbyte (which is the end of "has record")
    if(this->cursor>=pinfo->lastByte) return false;
    this->binaryData = (void*)(pinfo->data+this->cursor);
    //cout<<"a read of binarydata "<<this->binaryData<<endl;
    has_next = true;
    return true;
}

MyDB_PageRecordIterator::~MyDB_PageRecordIterator() {

}
#endif
