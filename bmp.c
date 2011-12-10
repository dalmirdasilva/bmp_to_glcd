#include "bmp.h"

void bmp_load(char *bmp_input_file_name, char *bmp_output_file_name) {
    bmp_input_file = fopen(bmp_input_file_name, "r+");
    bmp_output_file = fopen(bmp_output_file_name, "w+");
    if(bmp_input_file == NULL) {
        printf("Could not open the image.");
        exit(1);
    }
    copy_to_output();
    bmp_header = (bitmap_header_t *) malloc(sizeof(bitmap_header_t));
    bmp_header->bitmap_magic = (bitmap_magic_t *) malloc(sizeof(bitmap_magic_t));
    bmp_header->bitmap_file_header = (bitmap_file_header_t *) malloc(sizeof(bitmap_file_header_t));
    bmp_header->bitmap_info_header = (bitmap_info_header_t *) malloc(sizeof(bitmap_info_header_t));
    rewind(bmp_input_file);
    fread(bmp_header->bitmap_magic, sizeof(bitmap_magic_t), 1, bmp_input_file);
    fread(bmp_header->bitmap_file_header, sizeof(bitmap_file_header_t), 1, bmp_input_file);
    fread(bmp_header->bitmap_info_header, sizeof(bitmap_info_header_t), 1, bmp_input_file);
}

void bmp_close() {
    fclose(bmp_input_file);
    fclose(bmp_output_file);
}

uint32_t bmp_width() {
    return bmp_header->bitmap_info_header->width;
}

uint32_t bmp_height() {
    return bmp_header->bitmap_info_header->height;
}

uint16_t bmp_bits_per_pixel() {
    return bmp_header->bitmap_info_header->bits_per_pixel;
}

uint16_t bmp_pixel_array_offset() {
    return bmp_header->bitmap_file_header->pixel_array_offset;
}

uint8_t bmp_pixel_size() {
    return bmp_bits_per_pixel() / 8;
}

uint8_t bmp_row_padding() {
    uint16_t row_size_with_padding = bmp_row_size();
    uint16_t row_size_without_padding = bmp_width() * bmp_pixel_size();
    return (row_size_with_padding - row_size_without_padding);
}

uint32_t bmp_image_size() {
    return bmp_header->bitmap_info_header->image_size;
}

uint16_t bmp_row_size() {
    return bmp_image_size() / bmp_height();
}

uint8_t *bmp_read_row(uint16_t row) {
    uint8_t *row_data;
    row_data = (uint8_t *) malloc(bmp_row_size());
    fseek(bmp_input_file, bmp_pixel_array_offset() + (bmp_row_size() * row), SEEK_SET);
    fread(row_data, bmp_row_size(), 1, bmp_input_file);
    return row_data;
}

size_t bmp_write_row(uint16_t row, uint8_t *row_data) {
    fseek(bmp_output_file, bmp_pixel_array_offset() + (bmp_row_size() * row), SEEK_SET);
    return fwrite(row_data, bmp_row_size(), 1, bmp_output_file);
}

void copy_to_output() {
    char buffer;
    rewind(bmp_input_file);
    rewind(bmp_output_file);
    while(!feof(bmp_input_file)) {
        fread(&buffer, 1, 1, bmp_input_file);
        fwrite(&buffer, 1, 1, bmp_output_file);
    }
}

uint8_t bmp_is_valid() {
    uint8_t c0 = bmp_header->bitmap_magic->magic[0];
    uint8_t c1 = bmp_header->bitmap_magic->magic[1];
    if((c0 == 'B' && (c1 == 'P' || c1 == 'A')) || 
        (c0 == 'C' && (c1 == 'I') || (c1 == 'P')) ||
        (c0 == 'I' && c1 == 'C') ||
        (c0 == 'P' && c1 == 'T')) {
        return 1;
    }
    return 0;
}

uint8_t bmp_is_argb_image() {
    return bmp_bits_per_pixel() > 24;
}

