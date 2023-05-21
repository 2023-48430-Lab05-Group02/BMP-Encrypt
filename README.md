# BMP-Encrypt

A program to encrypt and decrypt BMP files, compress and decompress them and
get info about them.

#### BMP Implementation Details.
- Support for BITMAPINFOHEADER but not BITMAPCOREHEADER [OS2], BITMAPV4HEADER or BITMAPV5HEADER
- Support for RLE8 but not RLE4 compression (8-bit images but not 4-bit images).
- Significant error checking capabilities.
- Custom extension to BMP format using RSV1 for encryption state.

### How To Use

The program contains three modes:

- Batch mode:

This mode allows specifying an entire folder structure for conversion with
encryption, decryption, compression or decompression. All files must use
the same conversion parameters (e.g. encryption key). In encryption mode,
"e" will be appended to file extension. In decryption mode "d' will be
appended. In compress mode "c" will be appended and in decompress mode
"b' will be appended. This is so you can differentiate the input and
output files from the program and in case of any damage to your files
you will continue to have the original file.

- Command line:

Short Arguments | Long Arguments:

    *  -B : Batch : --batch
    *  -I : Interactive : --interactive
    *  -E : Encrypt : --encrypt
    *  -D : Decrypt : --decrypt
    *  -K : Key : --key
    *  -P : Password : --password
    *  -I : Input file : --input
    *  -FF: Ignore non-fatal: --force-nonfatal
    *  -FC: Force compress output file if possible : --force-compress
    *  -FD: Force decompress output file if possible --force-decompress

- Interactive mode:

Enter the interactive mode with -I and follow the prompts on screen.
