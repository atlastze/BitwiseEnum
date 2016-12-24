############################################################
# Project file
# Makefile will include this project file
############################################################

# Specify C++11
CXXFLAGS += -std=c++11

# Specify target name
TARGET = example 

# Specify the #include directories which should be searched when compiling the project.
INCLUDEPATH = .. .

# Specify the source directories which should be searched when compiling the project.
DEPENDPATH = .

# Defines the header files for the project.
HEADERS = bitwise_enum.h

# Defines the source files for the project.
SOURCES = main.cpp
