#include "bmp.h"

void bmp_load(char *bmp_input_file_name, char *bmp_output_file_name) {
    bmp_input_file = fopen(bmp_input_file_name, "r+");
    bmp_output_file = fopen(bmp_output_file_name, "w+");
    if(bmp_input_file == NULL) {
        printf("Could not open the image.");
        exit(1);
    }
    bmp_header = (bmp_header_t *) malloc(sizeof(bmp_header_t));
    bmp_header->bitmap_file_header = (bitmap_file_header_t *) malloc(sizeof(bitmap_file_header_t));
    bmp_header->DIB_header = (DIB_header_t *) malloc(sizeof(DIB_header_t));
    fseek(bmp_input_file, 0, SEEK_SET);
    fread(&bmp_header->bitmap_file_header->signatute, 2, 1, bmp_input_file);
    fread(&bmp_header->bitmap_file_header->file_size, 4, 1, bmp_input_file);
    fread(&bmp_header->bitmap_file_header->reserved1, 2, 1, bmp_input_file);
    fread(&bmp_header->bitmap_file_header->reserved1, 2, 1, bmp_input_file);
    fread(&bmp_header->bitmap_file_header->pixel_array_offset, 4, 1, bmp_input_file);
    fseek(bmp_input_file, 14, SEEK_SET);
    fread(&bmp_header->DIB_header->DIB_header_size, 4, 1, bmp_input_file);
    fread(&bmp_header->DIB_header->image_width, 4, 1, bmp_input_file);
    fread(&bmp_header->DIB_header->image_height, 4, 1, bmp_input_file);
    fread(&bmp_header->DIB_header->planes, 2, 1, bmp_input_file);
    fread(&bmp_header->DIB_header->bits_per_pixel, 2, 1, bmp_input_file);
}

void bmp_close() {
    fclose(bmp_input_file);
    fclose(bmp_output_file);
}

int32_t bmp_get_width() {
    return bmp_header->DIB_header->image_width;
}

int32_t bmp_get_height() {
    return bmp_header->DIB_header->image_height;
}

int16_t bmp_get_bits_per_pixel() {
    return bmp_header->DIB_header->bits_per_pixel;
}

int16_t bmp_get_pixel_array_offset() {
    return bmp_header->bitmap_file_header->pixel_array_offset;
}

int16_t bmp_get_bytes_per_pixel() {
    return bmp_get_bits_per_pixel() / 8;
}

int16_t bmp_get_row_padding() {
    int usefull_bytes_per_row = bmp_get_width() * bmp_get_bytes_per_pixel();
    int mod = usefull_bytes_per_row % 4;
    if(mod) {
        return 4 - mod;
    }
    return 0;
}

int16_t bmp_get_row_size() {
    return bmp_get_width() * bmp_get_bytes_per_pixel() + bmp_get_row_padding();
}

uint8_t *bmp_get_row(int row) {
    int i, j;
    uint8_t *row_data;
    int16_t width = bmp_get_width();
    int16_t bytes_per_pixel = bmp_get_bytes_per_pixel();
    row_data = (int8_t *) malloc(bmp_get_row_size() - bmp_get_row_padding());
    fseek(bmp_input_file, bmp_get_pixel_array_offset() + (bmp_get_row_size() * row), SEEK_SET);
    for(i = 0; i < width; i++) {
        for(j = 0; j < bytes_per_pixel; j++) {
            fread(&row_data[(i * bytes_per_pixel) + j], 1, 1, bmp_input_file);
        }
    }
    return row_data;
}

void bmp_set_row(int row, uint8_t *row_data) {
    int i, j;
    int16_t width = bmp_get_width();
    int16_t bytes_per_pixel = bmp_get_bytes_per_pixel();
    fseek(bmp_output_file, bmp_get_pixel_array_offset() + (bmp_get_row_size() * row), SEEK_SET);
    for(i = 0; i < width; i++) {
        for(j = 0; j < bytes_per_pixel; j++) {
            fwrite(&row_data[(i * bytes_per_pixel) + j], 1, 1, bmp_output_file);
        }
    }
}

void copy_to_output() {
    char buffer;
    fseek(bmp_input_file, 0, SEEK_SET);
    fseek(bmp_output_file, 0, SEEK_SET);
    while(!feof(bmp_input_file)) {
        fread(&buffer, 1, 1, bmp_input_file);
        fwrite(&buffer, 1, 1, bmp_output_file);
    }
}
