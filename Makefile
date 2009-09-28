CXXFLAGS=-Wall `GraphicsMagick++-config --cppflags --cxxflags`
LDFLAGS=`GraphicsMagick++-config --ldflags --libs`

default: all

all: decode encode shifty
