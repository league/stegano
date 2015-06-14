#include <Magick++.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <bitset>

using namespace std;
using namespace Magick;

int main(int argc, char** argv)
{
  InitializeMagick(*argv);
  if(argc < 4) {
      cout << "Usage: " << argv[0] << " X Y image1.png [image2.png ...]\n"
           << "Identifies the color of the specified pixel in each image\n";
      exit(1);
  }

  const int x = atoi(argv[1]);
  const int y = atoi(argv[2]);

  for(int i = 3; i < argc; i++) {
      Image im (argv[i]);
      Color c = im.pixelColor(x, y);
      cout << setw(20) << argv[i] << ": "
           << setfill('0') << setw(2) << hex << (c.redQuantum() & 0xff)
           << setfill('0') << setw(2) << hex << (c.greenQuantum() & 0xff)
           << setfill('0') << setw(2) << hex << (c.blueQuantum() & 0xff)
           << "  ";
      bitset<8> r(c.redQuantum());
      bitset<8> g(c.greenQuantum());
      bitset<8> b(c.blueQuantum());
      cout << r << ' ' << g << ' ' << b << '\n';
  }

  return 0;
}
