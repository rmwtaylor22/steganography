#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 4096

int main(argc, argv)
        int     argc;
        char    *argv[];{

    // storing command line arguments
    const char *inputPath = argv[1];
    const char *outputPath = argv[2];
    const char *secretMessage = argv[3];

    const char *inputHolder = inputPath;
    const char *outputHolder = outputPath;

    printf("Input: %s\n", inputPath);
    printf("Output: %s\n", outputPath);
    printf("Message: %s\n", secretMessage);

    int is_ok = EXIT_FAILURE;

    // open bitmap
    FILE* fp = fopen(inputHolder, "r");
    if(!fp) {
        perror("The file didn't open");
        return is_ok;
    }



    // bitmap file header - 14 bits
    // DIB header - "BITMAPINFOHEADER" 40 bits
    // Bitmap starts at 54
    int n;
    int m;
    int d;
    // skip over the header (14) and the size of the header bytes (4)
    fseek(fp, 18, SEEK_SET);
    // read the bitmap width in pixels (signed int)
    fread(&n, 4, 1, fp);
    // read the bitmap height in pixels (signed int)
    fread(&m, 4, 1, fp);

    printf("Width: %d\n", n);
    printf("Height: %d\n", m);


    // skip over the rest of the header
    fread(&d, 28, 1, fp);

    // tell where the file pointer is positioned at
    long int p = ftell(fp);
    printf("Pixels start at: %ld\n", p);

    // get total number of pixels
    int totalPixels;
    if (m < 0){
        totalPixels = n*m*-1;
    } else {
        totalPixels = n*m;
    }
    printf("Total pixels: %d\n", totalPixels);

    int e;
    // Loop that iterates over pixels
    for (int i = 0; i < totalPixels; ++i) {
        fread(&e, 4, 1, fp);
        printf("Pixel: %d\n", e);
    }


    // Loop that iterates over bytes in secret message
    printf("The string length of the secret message: %lu\n", strlen(secretMessage));

    unsigned long len = strlen(secretMessage);
    for(int i=0; i < len; i++){
        printf("Byte value: %d\n", secretMessage[i]);
    }



    // have a struct and read it into a struct for four bytes that it can access
    // --> read in integer and then parse out four bytes/ bits? (Or one byte)
    // change one bit on each byte
    // If you read it in a an int, it will probably swap bytes because of endianess

    // open storage file
    FILE* fp2 = fopen(outputHolder, "w");
    if(!fp2) {
        perror("The file didn't open");
        return is_ok;
    }

    return 0;
}