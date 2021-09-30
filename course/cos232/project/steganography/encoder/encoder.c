#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(argc, argv)
        int     argc;
        char    *argv[];{

    // storing command line arguments
    const char *inputPath = argv[1];
    const char * outputPath = argv[2];
    const char *secretMessage = argv[3];

    printf("Input: %s\n", inputPath);
    printf("Output: %s\n", outputPath);
    printf("Message: %s\n", secretMessage);


    // open bitmap
    int is_ok = EXIT_FAILURE;

    FILE* fp = fopen(inputPath, "r");
    if(!fp) {
        perror("The file didn't open");
        return is_ok;
    }


    // open storage file
    FILE* fp2 = fopen(outputPath, "w");
    if(!fp2) {
        perror("The file didn't open");
        return is_ok;
    }


    int b;
    int n;
    int m;
    int d;

    // skip over the header (14) and the size of the header bytes (4)
    fread(&b, 18, 1, fp);
    fwrite(&b,18,1, fp2);

    // read pixel width
    fread(&n, 4, 1, fp);
    fwrite(&n,4,1, fp2);

    // read pixel height
    fread(&m, 4, 1, fp);
    fwrite(&m,4,1, fp2);

    // print results
    printf("Width: %d\n", n);
    printf("Height: %d\n", m);

    // skip over the rest of the header
    fread(&d, 28, 1, fp);
    fwrite(&d,28,1, fp2);

    // locate file pointer
    long int p = ftell(fp);
    printf("Pixels start at: %ld\n", p);


    // If pixelByteCount%4 = 0, it's an alpha byte
    int pixelByteCount = 0;
    int e;

    // loop through secret msg
    for(int i=0; i < strlen(secretMessage); i++){
        //printf("Byte value: %d\n", secretMessage[i]);

        // msgByte = secret message byte
        int msgByte = (unsigned char) secretMessage[i];
        // newByte will hold value of new byte
        int newByte;

        // loop through 8 bits in message byte
        for(int counter=0; counter < 8; counter++){
            // grab bit by masking msg byte
            int msgBit = msgByte & 1;
            // read next color byte & increase pixel byte count
            fread(&e, 1, 1, fp);
            pixelByteCount ++;

            // if alpha byte, write it to file and read next byte
            if (pixelByteCount%4 == 0) {
                fwrite(&e, 1, 1, fp2);
                fread(&e, 1, 1, fp);
                pixelByteCount++;
            }
            // put message bit in color byte LSB & write it to the file
            if (msgBit == 1) {
                newByte = e | 1;
                fwrite(&newByte, 1, 1, fp2);
            } else {
                newByte = e & 1;
                fwrite(&newByte, 1, 1, fp2);
            }
            // shift the bits in secret message
            msgByte = msgByte >> 1;
        }
    }


    // put null byte in last 8 positions
    int newByte;
    for(int counter=0; counter < 8; counter++) {
        // read next color byte
        fread(&e, 1, 1, fp);
        pixelByteCount++;

        // if alpha, write to file and skip
        if (pixelByteCount%4 == 0) {
            fwrite(&e, 1, 1, fp2);
            fread(&e, 1, 1, fp);
            pixelByteCount++;
        }
        // put 0 bit in color byte LSB & write it to the file
        newByte = e & 1;
        fwrite(&newByte, 1, 1, fp2);
    }


    // attach the rest of the image onto the file
    int r;
    do {
        fread(&r, 1, 1, fp);
        fwrite(&r, 1, 1, fp2);
    } while (getc(fp) != EOF);

    // close file
    fclose(fp);


    // If you read it in as an int, it will probably swap bytes because of endianess

    return 0;
}