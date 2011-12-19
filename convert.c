#include "convert.h"

float bmp_rgb_to_gray_scale(uint8_t rgb[]) {
    return (rgb[0] * .3) + (rgb[1] * .59) + (rgb[2] * .59);
}

uint8_t rgb_to_black_and_white(uint8_t rgb[]) {
    return (bmp_rgb_to_gray_scale(rgb) > 255) ? 1 : 0;
}

void bmp_read_pixel_from(uint8_t *pixel_data, uint8_t *bmp_row) {
    int8_t i;
    for(i = bmp_pixel_size(); i >= 0; i--) {
        pixel_data[i] = bmp_row[i];
    }
}

void bmp_write_pixel_to(uint8_t *pixel_data, uint8_t *bmp_row) {
    int8_t i;
    for(i = bmp_pixel_size() - 1; i >= 0; i--) {
        bmp_row[i] = pixel_data[i];
    }
}

void bmp_rgb_pixel_to_bw_pixel(uint8_t *pixel_data, uint8_t inverted) {
    int8_t i = 0;
    uint8_t mask = (inverted) ? 0x00 : 0xff;
    //printf("%f\n", bmp_rgb_to_gray_scale(pixel_data));
    if(bmp_rgb_to_gray_scale(pixel_data) > 255) {
        mask = (inverted) ? 0xff : 0x00;
    }
    for(; i < 3; i++) {
        pixel_data[i] = mask;
    }
}

void convert_to_black_and_white() {
    uint8_t pixel_data[bmp_pixel_size()];
    uint8_t *bmp_row, k;
    uint16_t x, y;
    for(y = 0; y < bmp_height(); y++) {
        bmp_row = bmp_read_row(y);
        for(x = 0; x < (bmp_row_size() - bmp_row_padding()); x += bmp_pixel_size()) {
            bmp_read_pixel_from(pixel_data, &bmp_row[x]);
            bmp_rgb_pixel_to_bw_pixel(pixel_data, 0);
            bmp_write_pixel_to(pixel_data, &bmp_row[x]);
        }
        bmp_write_row(y, bmp_row);
    }
}
