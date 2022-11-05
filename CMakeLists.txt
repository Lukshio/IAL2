cmake_minimum_required(VERSION 3.23)
project(ial_2022_du2 C)

set(CMAKE_C_STANDARD 11)

#include_directories(btree)
#include_directories(btree/iter)
include_directories(hashtable)

add_executable(
        ial_2022_du2
        btree/iter/btree.c
        btree/iter/stack.c
        btree/iter/stack.h
        #btree/rec/btree.c
        btree/btree.c
        btree/btree.h
        btree/test.c
        btree/test_util.c
        btree/test_util.h
        #hashtable/hashtable.c
        #ashtable/hashtable.h
        #hashtable/test.c
        #hashtable/test_util.c
        #hashtable/test_util.h
)
