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
  // printf("TODO: unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red)\n");
  unsigned char averageIntensity = (blue + green + red) / 3;
  return averageIntensity;
}

void applyGrayscaleToPixel(unsigned char* pixel) {
  // printf("TODO: void applyGrayscaleToPixel(unsigned char* pixel)\n");
  unsigned char red = *pixel;
  unsigned char green = *(pixel + 1);
  unsigned char blue = *(pixel + 2);
  unsigned char newVal = getAverageIntensity(blue, green, red);
  *pixel = newVal;
  *(pixel + 1) = newVal;
  *(pixel + 2) = newVal;
}

void applyThresholdToPixel(unsigned char* pixel) {
  // printf("TODO: void applyThresholdToPixel(unsigned char* pixel)\n");
  unsigned char red = *pixel;
  unsigned char green = *(pixel + 1);
  unsigned char blue = *(pixel + 2);
  unsigned char newVal = getAverageIntensity(blue, green, red);
  if (newVal < 128) {
      *pixel = 0x00;
      *(pixel + 1) = 0x00;
      *(pixel + 2) = 0x00;
  }
  else {
      *pixel = 0xff;
      *(pixel + 1) = 0xff;
      *(pixel + 2) = 0xff;
  }
}

void applyFilterToPixel(unsigned char* pixel, int isGrayscale) {
  // printf("TODO: void applyFilterToPixel(unsigned char* pixel, int isGrayscale)\n");
  if (isGrayscale) {
      applyGrayscaleToPixel(pixel);
  }
  else {
      applyThresholdToPixel(pixel);
  }
}

void applyFilterToRow(unsigned char* row, int width, int isGrayscale) {
  // printf("TODO: void applyFilterToRow(unsigned char* row, int width, int isGrayscale)\n");
  for (int i = 0; i < width; ++i) {
      if (i > 0) {
          row = row + 3;
      }
      applyFilterToPixel(row, isGrayscale);
  }
}

void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale) {
  int padding = width % 4;
  // printf("TODO: compute the required amount of padding from the image width");

#ifdef DEBUG
  printf("padding = %d\n", padding);
#endif

  // printf("TODO: void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale)\n");
  for (int i = 0; i < height; ++i) {
      if (i > 0) {
          pixelArray = pixelArray + (3 * width) + padding;
      }
      applyFilterToRow(pixelArray, width, isGrayscale);
  }
}

void parseHeaderAndApplyFilter(unsigned char* bmpFileAsBytes, int isGrayscale) {
  int offsetFirstBytePixelArray = *((int *)(bmpFileAsBytes + 10));
  int width = *((int *)(bmpFileAsBytes + 18));
  int height = *((int *)(bmpFileAsBytes + 22));
  unsigned char* pixelArray = bmpFileAsBytes + offsetFirstBytePixelArray;

//  printf("TODO: set offsetFirstBytePixelArray\n");
//  printf("TODO: set width\n");
//  printf("TODO: set height\n");
//  printf("TODO: set the pixelArray to the start of the pixel array\n");

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