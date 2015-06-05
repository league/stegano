#include <Magick++.h>
#include <iostream>
#include <sstream>
#include <cassert>

using std::cout;
using std::string;
using std::endl;
using namespace Magick;

int main(int argc, char** argv)
{
  InitializeMagick(*argv);
  int numBits = argc > 1? atoi(argv[1]) : 0;
  if(argc != 4 || numBits < 1 || numBits > 7) {
      cout << "Usage: " << argv[0] << " N input.png out\n"
           << "where N is the number of bits (1-7) to tweak.\n"
           << "Output files will be named out-0s.png, out-1s.png, out-rand.png\n";
      exit(1);
  }

  const string samplePath = argv[2];
  const string outputPath = argv[3];
  cout << "Loading " << samplePath << '\n';
  Image im (samplePath);
  Geometry g = im.size();

  cout << "Dimensions are " << (string)g << '\n';
  cout << "Depth is " << im.depth() << '\n';
  assert(im.type() == TrueColorType);

  Quantum mask = (1 << numBits) - 1;
#if QuantumDepth == 16
  mask = (mask << 8) | mask;
#endif

  // Zero out the lowest N bits
  for(unsigned x = 0; x < g.width(); x++) {
      for(unsigned y = 0; y < g.height(); y++) {
          Color c = im.pixelColor(x, y);
          c.redQuantum(c.redQuantum() & ~mask);
          c.greenQuantum(c.greenQuantum() & ~mask);
          c.blueQuantum(c.blueQuantum() & ~mask);
          im.pixelColor(x, y, c);
      }
  }
  cout << "Writing " << outputPath << "-0s.png" << endl;
  im.write(outputPath + "-0s.png");

  // Replace lowest N bits with ones
  for(unsigned x = 0; x < g.width(); x++) {
      for(unsigned y = 0; y < g.height(); y++) {
          Color c = im.pixelColor(x, y);
          c.redQuantum(c.redQuantum() | mask);
          c.greenQuantum(c.greenQuantum() | mask);
          c.blueQuantum(c.blueQuantum() | mask);
          im.pixelColor(x, y, c);
      }
  }
  cout << "Writing " << outputPath << "-1s.png" << endl;
  im.write(outputPath + "-1s.png");

  // Replace lowest N bits with random
  srand(time(NULL));
  for(unsigned x = 0; x < g.width(); x++) {
      for(unsigned y = 0; y < g.height(); y++) {
          Color c = im.pixelColor(x, y);
          int r = rand();
          c.redQuantum((c.redQuantum() & ~mask) | (r & mask));
          r >>= 8;
          c.greenQuantum((c.greenQuantum() & ~mask) | (r & mask));
          r >>= 8;
          c.blueQuantum((c.blueQuantum() & ~mask) | (r & mask));
          im.pixelColor(x, y, c);
      }
  }
  cout << "Writing " << outputPath << "-rand.png" << endl;
  im.write(outputPath + "-rand.png");


  return 0;
}
