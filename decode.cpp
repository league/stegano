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
  return (q & 3) << 6;
}

int main(int argc, char** argv)
{
  InitializeMagick(*argv);
  assert(argc == 3);

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
      cout << (string)c << " --> ";
      c.redQuantum(revealDisguisedColor(c.redQuantum()));
      c.greenQuantum(revealDisguisedColor(c.greenQuantum()));
      c.blueQuantum(revealDisguisedColor(c.blueQuantum()));
      cout << (string)c << '\n';
      im.pixelColor(x, y, c);
    }
  }

  cout << "Writing " << outputPath << '\n';
  im.write(outputPath);
  return 0;
}
