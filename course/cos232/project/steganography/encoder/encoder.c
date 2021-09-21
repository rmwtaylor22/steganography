#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    // open storage file
    FILE* fp2 = fopen(outputPath, "w");
    if(!fp2) {
        perror("The file didn't open");
        return is_ok;
    }

    return 0;
}