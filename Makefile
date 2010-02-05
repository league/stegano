CXXFLAGS=-Wall `Magick++-config --cppflags --cxxflags`
LDFLAGS=`Magick++-config --ldflags --libs`

default: all

all: decode encode shifty
