cmake_minimum_required(VERSION 3.13)
project(Ass2)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)
include_directories(Main/Qunit)
include_directories(Main/BufferMgr/headers)
include_directories(Main/BufferMgr/source)
include_directories(Main/BufferTest/source)
include_directories(Main/Catalog/headers)
include_directories(Main/Catalog/source)
include_directories(Main/Qunit/headers)
include_directories(Main/DatabaseTable/headers)
include_directories(Main/DatabaseTable/source)
include_directories(Main/Record/headers)
include_directories(Main/Record/source)
include_directories(Main/RecordTest/source)

set(SOURCE_FILES

        Main/BufferMgr/headers/MyDB_BufferManager.h
        Main/BufferMgr/headers/MyDB_PageHandle.h
        Main/BufferMgr/headers/MyDB_Page.h
        Main/BufferMgr/headers/PageCompare.h
        Main/BufferMgr/headers/TableCompare.h
        Main/BufferMgr/headers/CheckLRU.h

        Main/BufferMgr/source/MyDB_BufferManager.cc
        Main/BufferMgr/source/MyDB_Page.cc


        Main/Catalog/headers/MyDB_AttType.h
        Main/Catalog/headers/MyDB_Table.h
        Main/Catalog/headers/MyDB_Catalog.h
        Main/Catalog/headers/MyDB_Schema.h
        Main/Catalog/headers/MyDB_PageType.h

        Main/Catalog/source/MyDB_Table.cc
        Main/Catalog/source/MyDB_Catalog.cc
        Main/Catalog/source/MyDB_Schema.cc

        Main/DatabaseTable/headers/MyDB_PageReaderWriter.h
        Main/DatabaseTable/headers/MyDB_RecordIterator.h
        Main/DatabaseTable/headers/MyDB_TableReaderWriter.h

        Main/DatabaseTable/source/MyDB_PageReaderWriter.cc
        Main/DatabaseTable/source/MyDB_TableReaderWriter.cc




        Main/Record/headers/MyDB_AttVal.h
        Main/Record/headers/MyDB_Record.h

        Main/Record/source/MyDB_AttVal.cc
        Main/Record/source/MyDB_Record.cc

        Main/Qunit/headers/QUnit.h

        Main/RecordTest/source/RecordQUnit.cc

       )
add_executable(Ass2 ${SOURCE_FILES})