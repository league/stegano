CXX=c++
CXXFLAGS=-Wall `GraphicsMagick++-config --cppflags --cxxflags`
LDFLAGS=`GraphicsMagick++-config --ldflags --libs`

default: all

all: decode encode shifty

%: %.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) decode encode shifty
	$(RM) -r *.dSYM
