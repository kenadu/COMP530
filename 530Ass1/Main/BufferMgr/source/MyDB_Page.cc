#include <MyDB_Page.h>
#include <memory>
using namespace std;
Page::Page(){
	this->buffer_addr = NULL;
}
Page::Page(char* buffer_addr, bool is_Pinned, bool is_Anonymous, bool is_Dirty){
	this->buffer_addr = buffer_addr;
	this->is_Pinned = is_Pinned;
	this->
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
char* Page::getBuffer_addr(){
	return this->buffer_addr;
}
void Page::setBuffer_addr(char* buffer_addr){
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
	if(this->ref_counter==0) return false;
	return true;
}
void Page::incre_cntr(){
	this->ref_counter++;
}
void Page::decre_cntr(){
	if(check_cntr()) this->ref_counter--;
	else cout<<"counter goes 0"<<endl;
}
char* Page::getData(){
	return this->data;
}