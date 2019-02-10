
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"

using namespace std;

MyDB_TableReaderWriter :: MyDB_TableReaderWriter (MyDB_TablePtr forMe, MyDB_BufferManagerPtr myBuffer) {
	this->my_table = forMe;
	this->my_buffer = myBuffer;
	//if the table has no last page
	if(this->my_table->lastPage()==-1){
		this->my_table->setLastPage(this->my_table->lastPage()+1);
		MyDB_PageReaderWriterPtr tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
		tmp->clear();
	}
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
	MyDB_PageReaderWriterPtr tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
	bool try_to_do_in_PageRdwt = tmp->append(myRecPtr);
	if(!try_to_do_in_PageRdwt){
		this->my_table->setLastPage(this->my_table->lastPage()+1);
		tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
		tmp->clear();
		tmp->append(myRecPtr);
	}
}

void MyDB_TableReaderWriter :: loadFromTextFile (string fromMe) {
	//reinitialize my table and the MyDB_PageReaderWriter.
	this->my_table->setLastPage(0);
	MyDB_PageReaderWriterPtr tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
	tmp->clear();

	string STRING;
	ifstream infile;
	infile.open (fromMe);
	MyDB_RecordPtr rcd = getEmptyRecord();
	while(!infile.eof()) // To get you all the lines.
	{
		getline(infile,STRING); // Saves the line in STRING.
		rcd->fromString(STRING); // Parse this line
		append(rcd); //append this record by pagereadwriter
	}
	infile.close();

}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr) {
	return nullptr;
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string) {
}

#endif

