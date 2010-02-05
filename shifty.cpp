#include <Magick++.h>
#include <iostream>
#include <sstream>
#include <cassert>

using std::cout;
using std::string;
using namespace Magick;

int main(int argc, char** argv)
{
  InitializeMagick(*argv);
  if(argc != 3) {
      cout << "Usage: " << argv[0] << " input.png out\n"
           << "Output files will be named out0.png, out1.png, etc.\n";
      exit(1);
  }

  const string samplePath = argv[1];
  const string outputPath = argv[2];

  cout << "Loading " << samplePath << '\n';
  Image im (samplePath);
  Geometry g = im.size();

  cout << "Dimensions are " << (string)g << '\n';
  cout << "Depth is " << im.depth() << '\n';
  assert(im.type() == TrueColorType);

  for(unsigned i = 0; i <= 8; i++) {
    std::ostringstream filePath;
    filePath << outputPath << i << ".png";
    cout << "Writing " << filePath.str() << '\n';
    im.write(filePath.str());
    for(unsigned x = 0; x < g.width(); x++) {
      for(unsigned y = 0; y < g.height(); y++) {
        Color c = im.pixelColor(x, y);
        c.redQuantum(c.redQuantum() << 1);
        c.greenQuantum(c.greenQuantum() << 1);
        c.blueQuantum(c.blueQuantum() << 1);
        im.pixelColor(x, y, c);
      }
    }
  }
  return 0;
}
