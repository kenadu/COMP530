
#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_TableRecordIterator.h"

MyDB_TableRecordIterator::MyDB_TableRecordIterator() {

}

MyDB_TableRecordIterator::MyDB_TableRecordIterator (MyDB_TableReaderWriterPtr point_to_table, MyDB_RecordPtr point_to_rec){
    this->point_to_table = point_to_table;
    this->point_to_rec = point_to_rec;
    this->point_to_page_iter = point_to_table->operator[](0).getIterator(point_to_rec);
    this->cursor = 0;
}

void MyDB_TableRecordIterator::getNext() {
    //assume has checked hasNext, point_to_page_iter is pointing to the current
    point_to_page_iter->getNext();
}

bool MyDB_TableRecordIterator::hasNext() {
    while(this->cursor <= this->point_to_table->getMy_table()->lastPage()){
        point_to_page_iter = point_to_table->operator[](this->cursor).getIterator(this->point_to_rec);
        if(point_to_page_iter->hasNext()) return true;
        this->cursor++;
    }
    return false;
}

MyDB_TableRecordIterator::~MyDB_TableRecordIterator() {

}
#endif
