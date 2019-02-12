
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecordIterator.h"
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
	//while index is out of the range, increment the page size
	while(i>my_table->lastPage()){
		my_table->setLastPage(my_table->lastPage()+1);
	}
	//let the pagereaderwriter point to this last page, accessing the page
	temp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, i);
	return *temp;
}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {
    //get an empty record to have this table's schema
    //MyDB_RecordPtr empty= make_shared<MyDB_RecordPtr>(this->my_table->getSchema());
	return make_shared<MyDB_Record>(this->my_table->getSchema());
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter :: last () {
    //get the last page, or we can do the alternate accessing index one above
	return (*this)[this->my_table->lastPage()];
}


void MyDB_TableReaderWriter :: append (MyDB_RecordPtr myRecPtr) {

	MyDB_PageReaderWriterPtr tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
	//try to append it if this page still has empty spot

	bool try_to_do_in_PageRdwt = tmp->append(myRecPtr);

	if(!try_to_do_in_PageRdwt){
	    //add one more page in the table
		this->my_table->setLastPage(this->my_table->lastPage()+1);
		//make this new added page as the new page reader writer,
		//clear its record because we are building a new empty page and append this record
		tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
		tmp->clear();
		tmp->append(myRecPtr);
	}
}
MyDB_TablePtr MyDB_TableReaderWriter::getMy_table() {
    return this->my_table;
}

void MyDB_TableReaderWriter :: loadFromTextFile (string fromMe) {
	//reinitialize my table and the MyDB_PageReaderWriter. and make a new pagereaderwriter
	this->my_table->setLastPage(0);
	MyDB_PageReaderWriterPtr tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
	tmp->clear();
    //reading from string line by line, parse each string as a record
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

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr myRecPtr) {

	return make_shared<MyDB_TableRecordIterator>(*this, myRecPtr);
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string toMe) {
	ofstream outFile;
	outFile.open (toMe);

	MyDB_RecordPtr empty_rec = getEmptyRecord ();

	MyDB_RecordIteratorPtr table_rec_iter= getIterator (empty_rec);
	while (table_rec_iter->hasNext()) {
		table_rec_iter->getNext ();
	}
	outFile.close ();

}

#endif

