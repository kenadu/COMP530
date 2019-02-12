//
// Created by Kena Du on 2019-02-09.
//

#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

#include "MyDB_RecordIterator.h"
#include "MyDB_PageReaderWriter.h"

using namespace std;

class MyDB_TableRecordIterator;
typedef shared_ptr <MyDB_TableRecordIterator> MyDB_TableRecordIteratorPtr;
//: public shared_ptr<::MyDB_RecordIterator>;


class MyDB_TableRecordIterator: public MyDB_RecordIterator {
private:
    MyDB_TableReaderWriter &point_to_table;
    MyDB_RecordIteratorPtr point_to_page_iter;
    MyDB_RecordPtr point_to_rec;
    size_t cursor;
public:

    // put the contents of the next record in the file/page into the iterator record
    // this should be called BEFORE the iterator record is first examined
    void getNext () ;

    // return true iff there is another record in the file/page
    bool hasNext () ;

    // destructor and contructor
    MyDB_TableRecordIterator (MyDB_TableReaderWriter &, MyDB_RecordPtr);
    ~MyDB_TableRecordIterator () ;

};
#endif //TABLE_REC_ITER_H
