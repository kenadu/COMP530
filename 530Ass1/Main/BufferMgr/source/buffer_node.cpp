//
// Created by Kena Du on 2019-01-24.
//
#include <memory>
#include <buffer_node.h>
using namespace std;

Node::Node(char* addr){
    this->buffered_page = nullptr;
    this->bit = true;
    this->addr = addr;
}
Page* Node::get_page() {
    return this->buffered_page;
}
void Node::set_page(Page* the_page) {
    this->buffered_page = the_page;
}
void Node::set_bit(bool bit){
    this->bit = bit;
}
char* Node::get_addr() {
    return addr;
}

bool Node::get_bit(){
    return bit;
}