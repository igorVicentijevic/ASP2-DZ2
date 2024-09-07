cmake_minimum_required(VERSION 3.26)
project(dz2)

set(CMAKE_CXX_STANDARD 17)

add_executable(dz2 main.cpp
        Btree.cpp
        Btree.h
        FileIO.cpp
        FileIO.h
        List.cpp
        List.h
        ListCustomers.cpp
        ListCustomers.h)
