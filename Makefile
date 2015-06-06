CXX=c++
CXXFLAGS=-Wall `GraphicsMagick++-config --cppflags --cxxflags`
LDFLAGS=`GraphicsMagick++-config --ldflags --libs`
PROGS=decode encode shifty tweaky spy encode-file decode-file

default: all

all: $(PROGS)

%: %.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) $(PROGS)
	$(RM) -r *.dSYM
