#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int16_t signatute;
    int32_t file_size;
    int16_t reserved1;
    int16_t reserved2;
    int32_t pixel_array_offset;
} bitmap_file_header_t;

typedef struct {
    int32_t DIB_header_size;
    int32_t image_width;
    int32_t image_height;
    int16_t planes;
    int16_t bits_per_pixel;
} DIB_header_t;

typedef struct {
  bitmap_file_header_t *bitmap_file_header;
  DIB_header_t *DIB_header;
} bmp_header_t;

FILE *bmp_input_file = NULL;
FILE *bmp_output_file = NULL;
bmp_header_t *bmp_header;

void bmp_load(char *bmp_input_file_name, char *bmp_output_file_name);

int32_t bmp_get_width();


