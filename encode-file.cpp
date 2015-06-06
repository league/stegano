#include <Magick++.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include <bitset>

using namespace std;
using namespace Magick;

struct bufstate {
    char byte;
    unsigned bitsLeft;
};

Quantum mergeNextTwoBits (ifstream& in, bufstate& bs, Quantum q)
{
    if(bs.bitsLeft == 0) {
        if(in.get(bs.byte)) {
            // cout << "Read byte: " << bitset<8>(bs.byte) << '\n';
            bs.bitsLeft = 8;
        }
        else {
            return 0;
        }
    }
    bs.bitsLeft -= 2;
    Quantum result = (bs.byte & (3 << bs.bitsLeft)) >> bs.bitsLeft;
    result = (result << 8) | result;
    Quantum merged = (q & ~0x0303) | result;
    // cout << "Next two: " << bitset<2>(result) << " merged into "
    //      << hex << q << " is " << merged << '\n';
    return merged;
}

int main(int argc, char** argv)
{
    InitializeMagick(*argv);
    if(argc != 4) {
        cout << "Usage: " << argv[0] << " input.png secret.data output.png\n";
        exit(1);
    }

    const string inputPath = argv[1];
    const string secretPath = argv[2];
    const string outputPath = argv[3];

    cout << "Loading " << inputPath << '\n';
    Image im (inputPath);
    Geometry g = im.size();
    assert(im.depth() <= 8);
    assert(im.type() == TrueColorType);

    cout << "Opening " << secretPath << '\n';
    ifstream in(secretPath.c_str(), ios::binary);
    bufstate bs = { 0, 0 };

    for(unsigned x = 0; x < g.width(); x++) {
        for(unsigned y = 0; y < g.height(); y++) {
            Color c = im.pixelColor(x, y);
            c.redQuantum(mergeNextTwoBits(in, bs, c.redQuantum()));
            c.greenQuantum(mergeNextTwoBits(in, bs, c.greenQuantum()));
            c.blueQuantum(mergeNextTwoBits(in, bs, c.blueQuantum()));
            im.pixelColor(x, y, c);
            if(!in) goto done;
        }
    }

done:
    in.close();
    cout << "Writing " << outputPath << '\n';
    im.write(outputPath);


    return 0;
}
