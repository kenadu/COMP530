
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"

using namespace std;

MyDB_TableReaderWriter :: MyDB_TableReaderWriter (MyDB_TablePtr forMe, MyDB_BufferManagerPtr myBuffer) {
	this->my_table = forMe;
	this->my_buffer = myBuffer;
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter :: operator [] (size_t i) {
	MyDB_PageReaderWriterPtr temp;
	while(i>my_table->lastPage()){
		my_table->setLastPage(my_table->lastPage()+1);
	}
	temp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, i);
	return *temp;
}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {

	return *make_shared<MyDB_RecordPtr>(this->my_table->getSchema());
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter :: last () {
	return (*this)[this->my_table->lastPage()];
}


void MyDB_TableReaderWriter :: append (MyDB_RecordPtr myRecPtr) {
	this->my_table->
}

void MyDB_TableReaderWriter :: loadFromTextFile (string) {
}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr) {
	return nullptr;
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string) {
}

#endif

