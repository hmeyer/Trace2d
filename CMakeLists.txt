cmake_minimum_required(VERSION 2.6)

PROJECT(trace2d)


#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-2.4)
add_subdirectory(src)
