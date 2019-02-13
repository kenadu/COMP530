
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
	//cout<<"\ntablereaderwriter constructor"<<endl;
	//cout<<"\n!!!my table size "<<my_table->lastPage()<<" my page rw pt size "<<my_page_RW_Ptrs.size()<<endl;
	//pushback k number of pagereaderwriter depends on how many pages table has
	if(this->my_table->lastPage() == -1){
		this->my_table->setLastPage(0);
		shared_ptr<MyDB_PageReaderWriter> tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, 0);
		tmp->clear();
		my_page_RW_Ptrs.push_back(tmp);
	}
	else {

		for (size_t i = 0; i <= this->my_table->lastPage(); i++) {
			shared_ptr<MyDB_PageReaderWriter> tmp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table,
																					   i);
			//tmp->clear();
			my_page_RW_Ptrs.push_back(tmp);
			//clear in constructor????
		}
	}
	//cout<<"\n???my table size "<<my_table->lastPage()<<" my page rw pt size "<<my_page_RW_Ptrs.size()<<endl;

}

MyDB_PageReaderWriter &MyDB_TableReaderWriter :: operator [] (size_t i) {
	shared_ptr <MyDB_PageReaderWriter> temp;
	//while index is out of the range, increment the page size
	while(i>my_table->lastPage()){
		my_table->setLastPage(my_table->lastPage()+1);
		temp = make_shared<MyDB_PageReaderWriter>(this->my_buffer, this->my_table, this->my_table->lastPage());
		temp->clear();
		my_page_RW_Ptrs.push_back(temp);
	}
	//let the pagereaderwriter point to this last page, accessing the page
	return *my_page_RW_Ptrs[i];
}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {
    //get an empty record to have this table's schema
    //MyDB_RecordPtr empty= make_shared<MyDB_RecordPtr>(this->my_table->getSchema());
   //cout<<"get an empty record"<<endl;
	return make_shared<MyDB_Record>(this->my_table->getSchema());
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter :: last () {
    //get the last page, or we can do the alternate accessing index one above
	return *my_page_RW_Ptrs.back();
}


void MyDB_TableReaderWriter :: append (MyDB_RecordPtr myRecPtr) {

	shared_ptr <MyDB_PageReaderWriter> tmp = my_page_RW_Ptrs.back();
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
		my_page_RW_Ptrs.push_back(tmp);
	}
}
MyDB_TablePtr MyDB_TableReaderWriter::getMy_table() {
    return this->my_table;
}

void MyDB_TableReaderWriter :: loadFromTextFile (string fromMe) {
	//reinitialize my table and the MyDB_PageReaderWriter. and make a new pagereaderwriter
	this->my_table->setLastPage(0);
	for(auto p: my_page_RW_Ptrs){
		p->clear();
	}
	//erase all except the first one at index 0.
	my_page_RW_Ptrs.erase(my_page_RW_Ptrs.begin()+1,my_page_RW_Ptrs.end());
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

	//MyDB_TableReaderWriterPtr linkthis = make_shared<MyDB_TableReaderWriter>(this);
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

