
#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_TableRecordIterator.h"


MyDB_TableRecordIterator::MyDB_TableRecordIterator (MyDB_TableReaderWriter& point_to_table, MyDB_RecordPtr point_to_rec):point_to_table(point_to_table){
    //this->point_to_table = point_to_table;
    this->point_to_rec = point_to_rec;
    //cout<<"table iterator constructor after rec"<<endl;
    this->point_to_page_iter = point_to_table[0].getIterator(point_to_rec);
    this->has_next = false;
    this->cursor = 0;
    this->tmp_cursor = 0;
}

void MyDB_TableRecordIterator::getNext() {
    //assume has checked hasNext, point_to_page_iter is pointing to the current
    if(!this->has_next) return;
    this->cursor = this->tmp_cursor;
    this->point_to_page_iter = this->tmp_page_iter;
    this->point_to_page_iter->getNext();
    this->has_next = false;
}

bool MyDB_TableRecordIterator::hasNext() {
    //cout<<"check for table cursor "<<this->cursor<<endl;
    this->tmp_cursor = this->cursor;
    this->tmp_page_iter = this->point_to_page_iter;
    while(!tmp_page_iter->hasNext()){

        this->tmp_cursor++;
        if(this->tmp_cursor > this->point_to_table.getMy_table()->lastPage()) return false;
        this->tmp_page_iter = this->point_to_table[this->tmp_cursor].getIterator(this->point_to_rec);
    }
    //cout<<"found the next avail table cursor "<<this->tmp_cursor<<endl;
    this->has_next = true;
    return true;

}

MyDB_TableRecordIterator::~MyDB_TableRecordIterator() {

}
#endif
