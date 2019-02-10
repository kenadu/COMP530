//
// Created by Kena Du on 2019-02-09.
//
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_RecordIterator.h"
using namespace std;

// This pure virtual class is used to iterate through the records in a page or file
// Instances of this class will be created via calls to MyDB_PageReaderWriter.getIterator ()
// or MyDB_FileReaderWriter.getIterator ().
//
class MyDB_PageRecordIterator;
typedef shared_ptr <MyDB_PageRecordIterator> MyDB_PageRecordIteratorPtr;

// DO NOT MODIFY!

class MyDB_PageRecordIterator: public MyDB_RecordIterator {

public:

    // put the contents of the next record in the file/page into the iterator record
    // this should be called BEFORE the iterator record is first examined
    void getNext () ;

    // return true iff there is another record in the file/page
    bool hasNext () ;

    // destructor and contructor
    MyDB_PageRecordIterator () ;
    ~MyDB_PageRecordIterator () ;

};

#endif