//
// Created by Kena Du on 2019-01-24.
//

#ifndef INC_530ASS1_BUFFER_NODE_H
#define INC_530ASS1_BUFFER_NODE_H
#include "MyDB_Page.h"
using namespace std;
class Node;

class Node{
private:
    Page* buffered_page;
    char* addr;
    //bit is true -> bit = 1 (can be evicted)
    //bit is false -> bit = 0 (can not be evicted)
    bool bit;
public:
    Node(char*);
    Page* get_page();
    void set_bit(bool);
    bool get_bit();
    void set_page(Page*);
};


#endif //INC_530ASS1_BUFFER_NODE_H
