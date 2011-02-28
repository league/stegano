// Steganographic decoder, requires GraphicsMagick
// Copyright 2009 Christopher League
// This is free software; you may distribute & modify it under GNU GPLv3
#include <Magick++.h>
#include <iostream>
#include <cassert>

using std::cout;
using std::string;
using namespace Magick;

Quantum revealDisguisedColor(Quantum q)
{
  // just keep lowest 2 bits, then amplify by 2^6
  Quantum r = (q & 3) << 6;
#if QuantumDepth == 16
  // for some reason, 2-byte quanta are doubled-up, like 0x7a7a
  r = (r << 8) | r;
#endif
  return r;
}

int main(int argc, char** argv)
{
  InitializeMagick(*argv);
  if(argc != 3) {
      cout << "Usage: " << argv[0] << " input.png output.png\n";
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

  for(unsigned x = 0; x < g.width(); x++) {
      for(unsigned y = 0; y < g.height(); y++) {
      Color c = im.pixelColor(x, y);
      c.redQuantum(revealDisguisedColor(c.redQuantum()));
      c.greenQuantum(revealDisguisedColor(c.greenQuantum()));
      c.blueQuantum(revealDisguisedColor(c.blueQuantum()));
      im.pixelColor(x, y, c);
    }
  }

  cout << "Writing " << outputPath << '\n';
  im.write(outputPath);
  return 0;
}
