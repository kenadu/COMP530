//
// Created by Kena Du on 2019-02-09.
//
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_RecordIterator.h"
#include "MyDB_PageReaderWriter.h"
using namespace std;


class MyDB_PageRecordIterator;
typedef shared_ptr <MyDB_PageRecordIterator> MyDB_PageRecordIteratorPtr;



class MyDB_PageRecordIterator: public MyDB_RecordIterator {
private:
    MyDB_PageReaderWriter& point_to_page;
    MyDB_RecordPtr point_to_rec;
    void* binaryData;
    size_t cursor;
public:

    // put the contents of the next record in the file/page into the iterator record
    // this should be called BEFORE the iterator record is first examined
    void getNext () ;

    // return true iff there is another record in the file/page
    bool hasNext () ;

    // destructor and contructor
    MyDB_PageRecordIterator (MyDB_PageReaderWriter&, MyDB_RecordPtr);
    ~MyDB_PageRecordIterator () ;

};

#endif