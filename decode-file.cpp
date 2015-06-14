#include <Magick++.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include <bitset>

using namespace std;
using namespace Magick;

struct bufstate {
    unsigned char byte;
    unsigned bits;
};

void pushTwoBits(ostream& out, bufstate& bs, Quantum q) {
    char bits = q & 3;
    // cout << "Pushing " << hex << q << " as bits " << bitset<2>(bits) << '\n';
    bs.byte <<= 2;
    bs.byte |= bits;
    bs.bits += 2;
    if(bs.bits == 8) {
        out.put(bs.byte);
        // cout << "Dumping byte " << bitset<8>(bs.byte) << '\n';
        bs.byte = 0;
        bs.bits = 0;
    }
}


int main(int argc, char** argv)
{
    InitializeMagick(*argv);
    if(argc != 3) {
        cout << "Usage: " << argv[0] << " input.png secret.data\n";
        exit(1);
    }

    const string inputPath = argv[1];
    const string secretPath = argv[2];

    cout << "Loading " << inputPath << '\n';
    Image im (inputPath);
    Geometry g = im.size();
    assert(im.depth() <= 8);
    assert(im.type() == TrueColorType);

    cout << "Writing " << secretPath << '\n';
    ofstream out(secretPath.c_str(), ios::binary);
    bufstate bs = { 0, 0 };

    for(unsigned y = 0; y < g.height(); y++) {
        for(unsigned x = 0; x < g.width(); x++) {
            Color c = im.pixelColor(x, y);
            pushTwoBits(out, bs, c.redQuantum());
            pushTwoBits(out, bs, c.greenQuantum());
            pushTwoBits(out, bs, c.blueQuantum());
        }
    }

    out.close();
    return 0;
}
