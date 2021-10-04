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

    FILE* fp = fopen(inputPath, "rb");
    if(!fp) {
        perror("The file didn't open");
        return is_ok;
    }


    // open storage file
    FILE* fp2 = fopen(outputPath, "wb");
    if(!fp2) {
        perror("The file didn't open");
        return is_ok;
    }

    int totalBytesRead = 0;

    int b;
    char d;
    // skip over beginning of file header (2)
    fread(&b, 2, 1, fp);
    fwrite(&b,2,1, fp2);
    totalBytesRead = totalBytesRead + 2;

    // size of BMP file (4)
    fread(&b, 4, 1, fp);
    fwrite(&b,4,1, fp2);
    int size = b;
    printf("Size: %d\n", b);
    totalBytesRead = totalBytesRead + 4;

    // skip over more of file header (4)
    fread(&b, 4, 1, fp);
    fwrite(&b,4,1, fp2);
    totalBytesRead = totalBytesRead + 4;

    // find starting position of bitmap file (4)
    fread(&b, 4, 1, fp);
    fwrite(&b,4,1, fp2);
    printf("Pixels start at: %d\n", b);
    totalBytesRead = totalBytesRead + 4;


    // and the size of the header bytes in DIB (4)
    fread(&b, 4, 1, fp);
    fwrite(&b,4,1, fp2);
    totalBytesRead = totalBytesRead + 4;


    // read pixel width (4)
    fread(&b, 4, 1, fp);
    fwrite(&b,4,1, fp2);
    printf("Width: %d\n", b);
    totalBytesRead = totalBytesRead + 4;


    // read pixel height (4)
    fread(&b, 4, 1, fp);
    fwrite(&b,4,1, fp2);
    printf("Height: %d\n", b);
    totalBytesRead = totalBytesRead + 4;


    // skip over the rest of the header
    for (int i = 0; i < 28; i ++){
        fread(&d, 1, 1, fp);
        fwrite(&d,1,1, fp2);
        totalBytesRead ++;
    }

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
                totalBytesRead++;
                fread(&e, 1, 1, fp);
                pixelByteCount++;
            }
            // put message bit in color byte LSB & write it to the file
            if (msgBit == 1) {
                newByte = e | 1;
                fwrite(&newByte, 1, 1, fp2);
                totalBytesRead++;
            } else {
                newByte = e & ~1;
                fwrite(&newByte, 1, 1, fp2);
                totalBytesRead++;
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
            totalBytesRead++;
            fread(&e, 1, 1, fp);
            pixelByteCount++;
        }
        // put 0 bit in color byte LSB & write it to the file
        newByte = e & ~1;
        fwrite(&newByte, 1, 1, fp2);
        totalBytesRead++;
    }

    // attach the rest of the image onto the file
    while(totalBytesRead < size){
        fread(&d, 1, 1, fp);
        fwrite(&d,1,1, fp2);
        totalBytesRead++;
    }

    // close file
    fclose(fp);

    return 0;
}