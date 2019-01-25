cmake_minimum_required(VERSION 3.13)
project(530Ass1)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)
include_directories(Main/Qunit)
include_directories(Main/BufferMgr/headers)
include_directories(Main/BufferMgr/source)
include_directories(Main/Catalog/headers)
include_directories(Main/Catalog/source)
include_directories(Main/Qunit/headers)


set(SOURCE_FILES Main/BufferTest/source/BufferQUnit.cc
        Main/BufferMgr/headers/MyDB_BufferManager.h
        Main/BufferMgr/headers/MyDB_PageHandle.h
        Main/BufferMgr/headers/MyDB_Page.h
        Main/BufferMgr/source/MyDB_BufferManager.cc
        Main/BufferMgr/source/MyDB_PageHandle.cc
        Main/BufferMgr/source/MyDB_Page.cc
        Main/BufferTest/source/BufferQUnit.cc
        Main/Catalog/headers/MyDB_Table.h
        Main/Catalog/source/MyDB_Table.cc
        Main/BufferMgr/source/MyDB_Page.cc
        Main/BufferMgr/headers/MyDB_Page.h

        Main/BufferMgr/headers/buffer_node.h Main/BufferMgr/source/buffer_node.cpp)
add_executable(530Ass1 ${SOURCE_FILES})