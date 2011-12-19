#include "gbm.h"

void gbm_init() {
    gbm_buffer_size = bmp_width() * (bmp_height() / 8) + 2;
    gbm_buffer = (uint8_t *) malloc(gbm_buffer_size);
}

void gbm_plot(uint8_t x, uint8_t y, enum glcd_color_t color) {
    uint8_t page, bit, byte;
    uint16_t index;
    page = (y / 8);
    bit = (y % 8);
    index = (page * bmp_width()) + x + 2;
    byte = gbm_buffer[index];
    if(color) {
        byte |= 1 << bit;
    } else {
        byte &= ~(1 << bit);
    }
    gbm_buffer[index] = byte;
}

void gbm_draw_image_into_buffer() {
    int i, j, k, x, y;
    uint8_t rgb[bmp_pixel_size()];
    uint8_t *bmp_row;
    uint8_t color;
    for(y = 0, i = bmp_height() - 1; i >= 0; i--, y++) {
        bmp_row = bmp_read_row(i);
        for(x = 0, j = 0; j < (bmp_row_size() - bmp_row_padding()); j += bmp_pixel_size(), x++) {
            for(k = 0; k < bmp_pixel_size(); k++) {
               rgb[k] = bmp_row[j + k];
            }
            if(rgb_to_black_and_white(rgb)) {
                color = GLCD_COLOR_WHITE;
            } else {
                color = GLCD_COLOR_BLACK;
            }
            gbm_plot(x, y, color);
        }
    }
}

void gbm_generate_code_from_buffer(char *output_file_name) {
    int i;
    FILE *f = fopen(output_file_name, "w+");
    fprintf(f, "0x%x, ", (bmp_width() & 0xff));
    fprintf(f, "0x%x, ", (bmp_height() & 0xff));
    printf("%d\n", gbm_buffer_size);
    for(i = 2; i < gbm_buffer_size; i++) {
        fprintf(f, "0x%x, ", gbm_buffer[i]);
	}
    fclose(f);
}
