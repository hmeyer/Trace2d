cmake_minimum_required(VERSION 2.6)

PROJECT(trace2d)


#if you don't want the full compiler output, remove the following line

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)

FIND_PACKAGE(Qt4 REQUIRED)


FIND_PACKAGE(ITK REQUIRED)


FIND_PACKAGE(Boost COMPONENTS signals REQUIRED)


FIND_PATH(POTRACE_PATH potracelib.h
	../../potrace/potrace-1.8/src
)


add_subdirectory(src)
