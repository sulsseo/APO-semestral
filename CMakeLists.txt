cmake_minimum_required(VERSION 3.6)
project(semestral)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        font_types.h
        font_rom8x16.c
        font_prop14x16.c
        mzapo_phys.h
        mzapo_phys.c
        mzapo_regs.h
        mzapo_parlcd.h
        mzapo_parlcd.c
        my_painter.h
        my_painter.cpp
        my_knobs.h
        my_knobs.cpp
        main.cpp
        my_const.h)

add_executable(semestral ${SOURCE_FILES})