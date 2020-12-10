TEMPLATE = app
QMAKE_CXXFLAGS += -openmp
QMAKE_LFLAGS += -openmp
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
HEADERS += HuffmanAlgorithm.h
HEADERS += HuffmanNode.h
HEADERS += HuffmanTree.h
HEADERS += FileManager.h
HEADERS += Clock.h
HEADERS += LinkedList.h
