#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

#define BAD_NUMBER_ARGS 1
#define BAD_OPTION 2
#define FSEEK_ERROR 3
#define FREAD_ERROR 4
#define MALLOC_ERROR 5
#define FWRITE_ERROR 6

/**
 * Parses the command line.
 *
 * argc:      the number of items on the command line (and length of the
 *            argv array) including the executable
 * argv:      the array of arguments as strings (char* array)
 * grayscale: the integer value is set to TRUE if grayscale output indicated
 *            outherwise FALSE for threshold output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *isGrayscale) {
  if (argc > 2) {
    fprintf(stderr, "Usage: %s [-g]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2) {
    if (strcmp(argv[1], "-g") == 0) {
      *isGrayscale = TRUE;

    } else if (strcmp(argv[1], "-s") == 0) {
      // set isscale here

    } else {
      fprintf(stderr, "Unknown option: '%s'\n", argv[1]);
      fprintf(stderr, "Usage: %s [-g]\n", argv[0]);
      exit(BAD_OPTION);
    }
  }

  return stdin;
}

unsigned getFileSizeInBytes(FILE* stream) {
  unsigned fileSizeInBytes = 0;

  rewind(stream);
  if (fseek(stream, 0L, SEEK_END) != 0) {
    exit(FSEEK_ERROR);
  }
  fileSizeInBytes = ftell(stream);

  return fileSizeInBytes;
}

void getBmpFileAsBytes(unsigned char* ptr, unsigned fileSizeInBytes, FILE* stream) {
  rewind(stream);
  if (fread(ptr, fileSizeInBytes, 1, stream) != 1) {
#ifdef DEBUG
    printf("feof() = %x\n", feof(stream));
    printf("ferror() = %x\n", ferror(stream));
#endif
    exit(FREAD_ERROR);
  }
}

unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red) {
  printf("TODO: unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red)\n");

  return 0;
}

void applyGrayscaleToPixel(unsigned char* pixel) {
  printf("TODO: void applyGrayscaleToPixel(unsigned char* pixel)\n");
  // TODO: call getAverageIntensity to get the new value of the pixel
  // Each pixel in the image is converted to a gray pixel by setting all three color values equal to the average of the three original values.
  // setting all three color values equal to the average of the three original values
}

void applyThresholdToPixel(unsigned char* pixel) {
  printf("TODO: void applyThresholdToPixel(unsigned char* pixel)\n");
  // TODO: call getAverageIntensity to get the new value of the pixel
  // Each pixel in the image is converted to either white or black based on the average intensity of its three color values.
  // Pixels in the original image with an average intensity of 128 or more will become white (all colors 0xff);
  // those with average intensities below 128 will become black (all colors 0x00).
}

void applyFilterToPixel(unsigned char* pixel, int isGrayscale) {
  printf("TODO: void applyFilterToPixel(unsigned char* pixel, int isGrayscale)\n");
  // TODO: on each pixel, either call applyGrayscale or applyThreshold
}

void applyFilterToRow(unsigned char* row, int width, int isGrayscale) {
  printf("TODO: void applyFilterToRow(unsigned char* row, int width, int isGrayscale)\n");
  // TODO: on each pixel in the row, call applyFilterToPixel
}

void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale) {
  int padding = 0;
  printf("TODO: compute the required amount of padding from the image width");
  // padding is just mod 4, because there are 3 bytes per pixel, do width * 3 mod 4 to get padding
  // padding will be added to the end of each row of pixels

#ifdef DEBUG
  printf("padding = %d\n", padding);
#endif

  printf("TODO: void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale)\n");
  // TODO: on each row of the pixel array, call applyFilterToRow
}

void parseHeaderAndApplyFilter(unsigned char* bmpFileAsBytes, int isGrayscale) {
  int offsetFirstBytePixelArray = 0;        // TODO: entry point for starting code is here
  // located in bmp Header which is 14 bytes total
  // offset is found 2 bytes in, it is at bytes 3-6
  int width = 0;                            // TODO: literally just call each function moving up to do one part
  // width = number of pixels in each row
  // width is found in DIB header, 4 bytes into this header, so 18 bytes in total
  int height = 0;
  // height = number of rows of pixels in the image
  // height is found in DIB header, 8 bytes into this header, so 22 bytes in total
  unsigned char* pixelArray = NULL;
  // each row starts on a 4-byte aligned address boundary

  printf("TODO: set offsetFirstBytePixelArray\n");
  printf("TODO: set width\n");
  printf("TODO: set height\n");
  printf("TODO: set the pixelArray to the start of the pixel array\n");

#ifdef DEBUG
  printf("offsetFirstBytePixelArray = %u\n", offsetFirstBytePixelArray);
  printf("width = %u\n", width);
  printf("height = %u\n", height);
  printf("pixelArray = %p\n", pixelArray);
#endif

  applyFilterToPixelArray(pixelArray, width, height, isGrayscale);
}

int main(int argc, char **argv) {
  int grayscale = FALSE;
  unsigned fileSizeInBytes = 0;
  unsigned char* bmpFileAsBytes = NULL;
  FILE *stream = NULL;

  stream = parseCommandLine(argc, argv, &grayscale);
  fileSizeInBytes = getFileSizeInBytes(stream);

#ifdef DEBUG
  printf("fileSizeInBytes = %u\n", fileSizeInBytes);
#endif

  bmpFileAsBytes = (unsigned char *)malloc(fileSizeInBytes);
  if (bmpFileAsBytes == NULL) {
    exit(MALLOC_ERROR);
  }
  getBmpFileAsBytes(bmpFileAsBytes, fileSizeInBytes, stream);

  parseHeaderAndApplyFilter(bmpFileAsBytes, grayscale);

#ifndef DEBUG
  if (fwrite(bmpFileAsBytes, fileSizeInBytes, 1, stdout) != 1) {
    exit(FWRITE_ERROR);
  }
#endif

  free(bmpFileAsBytes);
  return 0;
}