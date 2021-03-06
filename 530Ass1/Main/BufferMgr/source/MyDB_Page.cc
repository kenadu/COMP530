#include <MyDB_Page.h>
#include <memory>
#include <iostream>
using namespace std;
Page::Page(){
	this->buffer_addr = NULL;
}
Page::Page(Node* buffer_addr, bool is_Pinned, bool is_Anonymous, bool is_Dirty){
	this->buffer_addr = buffer_addr;
	this->ref_counter = 0;
	this->is_Pinned = is_Pinned;
	this->is_Anonymous = is_Anonymous;
	this->is_Dirty = is_Dirty;
}

void Page::set_tp(MyDB_TablePtr tp) {
	this->tp = tp;
}

void Page::set_offset(long offset) {
	this->offset = offset;
}

long Page::get_offset() {
	return offset;
}

MyDB_TablePtr Page::get_tp() {
	return tp;

}

bool Page:: getAnonymous(){
	return is_Anonymous;
}
bool Page:: getPinned(){
	return is_Pinned;
}
bool Page:: getDirty(){
	return is_Dirty;
}
Node* Page::getBuffer_addr(){
	return this->buffer_addr;
}
void Page::setBuffer_addr(Node* buffer_addr){
	this->buffer_addr = buffer_addr;
}
void Page:: setAnonymous(bool a){
	this->is_Anonymous = a;
}
void Page:: setPinned(bool p){
	this->is_Pinned = p;
}
void Page:: setDirty(bool d){
	this->is_Dirty = d;
}
bool Page::check_cntr(){
	if(this->ref_counter<=0) return false;
	return true;
}
void Page::incre_cntr(){
	this->ref_counter++;
}
void Page::decre_cntr(){
	this->ref_counter--;
}
void Page::setData(char*data){
	this->data = data;
}
char* Page::getData(){
	return this->data;
}
char* Page::read_from_disk(int file_descriptor, long offset, int size){
	char* tmp = buffer_addr->get_addr();
	cout << "tmp === " << (void*) tmp << endl;
	lseek(file_descriptor, (int)(offset * size), SEEK_SET);
	read(file_descriptor, tmp, size);
	close(file_descriptor);
	return tmp;
}
void Page::write_back_to_disk(int file_descriptor, long offset, int size){
	char* tmp = buffer_addr->get_addr();
	lseek(file_descriptor, (int)(offset * size), SEEK_SET);
	write(file_descriptor, tmp, size);
	close(file_descriptor);
	this->setDirty(false);
}
void Page::set_temp_id(int temp_id) {
	this->temp_slot_id = temp_id;
}
int Page::get_temp_id() {
	return this->temp_slot_id;
}