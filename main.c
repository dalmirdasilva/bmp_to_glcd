
#include "bmp.c"
#include "convert.c"
#include "glcd.c"
#include "main.h"

void main(int argc, char *argv[]) {
    bmp_load(argv[1], argv[2]);
    printf("bitmap_magic: %c%c\n", bmp_header->bitmap_magic->magic[0], bmp_header->bitmap_magic->magic[1]);
    printf("bmp_width(): %d\n", bmp_width());
    printf("bmp_row_size(): %d\n", bmp_row_size());
    printf("bmp_height(): %d\n", bmp_height());
    printf("bmp_bits_per_pixel(): %d\n", bmp_bits_per_pixel());
    printf("bmp_pixel_array_offset(): %d\n", bmp_pixel_array_offset());
    printf("bmp_row_padding(): %d\n", bmp_row_padding());
    convert_to_black_and_white();
    glcd_draw_image_into_buffer();
    glcd_draw_buffer_into_file("/tmp/s.txt");
    glcd_generate_code_from_buffer("/tmp/s2.txt");
}
