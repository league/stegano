// Steganographic encoder, requires GraphicsMagick
// Copyright 2009 Christopher League
// This is free software; you may distribute & modify it under GNU GPLv3
#include <Magick++.h>
#include <iostream>
#include <cassert>

using std::cout;
using std::string;
using namespace Magick;

Quantum mergeQuanta(Quantum pub, Quantum priv)
{
  // reduce private color to just 2 bits
  priv >>= 6;
  // zero out lowest 2 bits of public color
  pub &= 0xFC;
  // merge
  return pub | priv;
}

Color mergeColors(Color pub, Color priv)
{
  pub.redQuantum(mergeQuanta(pub.redQuantum(), priv.redQuantum()));
  pub.greenQuantum(mergeQuanta(pub.greenQuantum(), priv.greenQuantum()));
  pub.blueQuantum(mergeQuanta(pub.blueQuantum(), priv.blueQuantum()));
  return pub;
}

int main(int argc, char** argv, char** envp)
{
  InitializeMagick(*argv);
  assert(argc == 4);

  const string publicPath = argv[1];
  const string privatePath = argv[2];
  const string outputPath = argv[3];

  cout << "Loading " << publicPath << '\n';
  Image pub (publicPath);
  Geometry g = pub.size();
  assert(pub.depth() <= 8);
  assert(pub.type() == TrueColorType);

  cout << "Loading " << privatePath << '\n';
  Image priv (privatePath);
  assert(priv.size() == g);
  assert(priv.depth() <= 8);
  assert(priv.type() == TrueColorType);

  for(unsigned x = 0; x < g.width(); x++) {
    for(unsigned y = 0; y < g.height(); y++) {
      pub.pixelColor(x, y, mergeColors(pub.pixelColor(x, y),
                                       priv.pixelColor(x, y)));
    }
  }

  cout << "Writing " << outputPath << '\n';
  pub.write(outputPath);
  return 0;
}
