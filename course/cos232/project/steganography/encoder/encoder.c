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

    printf("Input: %s\n", inputPath);
    printf("Output: %s\n", outputPath);
    printf("Message: %s\n", secretMessage);

    int is_ok = EXIT_FAILURE;

    // open bitmap
    FILE* fp = fopen(inputPath, "r");
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
    fread(&m, 28, 1, fp);

    // tell where the file pointer is positioned at
    long int p = ftell(fp);
    printf("Pixels start at: %ld\n", p);


    /*
    int c;
    char *string;
    string = malloc(1);
    while ((c = fgetc(fp)) != EOF) {
        string[0] = (char)c;
        fprintf(stdout,"%s\n", string);
    }

     */


    // open storage file
    FILE* fp2 = fopen(outputPath, "w");
    if(!fp2) {
        perror("The file didn't open");
        return is_ok;
    }

    return 0;
}