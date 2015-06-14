# Image steganography

These are C++ programs to hide and reveal data kept in low-order bits of
lossless image files. See the video at
<https://www.youtube.com/watch?v=-7FBPgQDX5o>

## Building

Building them requires a C++ compiler and the GraphicsMagick library.
(GraphicsMagick is a variant of ImageMagick, and the latter might be made to
work as well.)

On a GNU/Linux system, you can probably install the `libgraphicsmagick++1-dev`
package, and the equivalent of `build-essential`, and then type `make` at the
prompt.

I will happily include contributions of working build instructions for other
platforms, but I cannot support them directly.

## Running

These are the programs that are available:

`./encode-file input.png secret.data output.png`

Encodes the secret data file into the two low-order bits of the input image,
producing the output image. No guarantees if the secret data does not fit. A
small change to the code determines whether it fills any remaining bits
randomly, or leaves them as they were.

`./decode-file input.png secret.data`

Extracts the two low-order bits of each channel, writing them to the secret
data file. Note: uses the entire image, so there will be some binary data left
over at the end if the secrets were shorter than the original image.

`./encode public.png private.png output.png`

Embeds a private imagine within a public one, using 6 bits per channel from
public, and 2 bits per channel from private. The result is stored in the given
output filename.

`./decode input.png output.png`

Extracts the lowest two bits per channel from the input image, to form the
output image.

`./tweaky N input.png out`

Specify N = the number of bits (1-7) to tweak. The output files will be named
out-0s.png, out-1s.png, out-rand.png, where it tweaks the lowest N bits in the
input image.

`./shifty input.png out`

Simulate left-shifting the channel representations, one bit at a time. The
output files will be named out0.png, out1.png, etc.

`./spy X Y image1.png [image2.png ...]`

Identify the color of the specified X,Y coordinate in each image. Prints the
color in hexadecimal and binary.

