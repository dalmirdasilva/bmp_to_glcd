
#include "glcd.h"

uint8_t glcd_xy_to_chip(uint8_t x, uint8_t y) {
    return (x >= GLCD_HALF_WIDTH) ? GLCD_CHIP_2 : GLCD_CHIP_1;
}

uint8_t glcd_xy_to_page(uint8_t x, uint8_t y) {
    return y / 8;
}

uint8_t glcd_xy_to_line(uint8_t x, uint8_t y) {
    return (x >= GLCD_HALF_WIDTH) ? x - GLCD_HALF_WIDTH : x;
}

uint8_t glcd_xy_to_bit(uint8_t x, uint8_t y) {
    return y % 8;
}

uint8_t glcd_buffered_plot(uint8_t x, uint8_t y, enum glcd_color_t color) {
    uint8_t byte;
    uint8_t chip, page, line;
    if(x > GLCD_WIDTH || y > GLCD_HEIGHT) {
        return 0;
    }
    chip = glcd_xy_to_chip(x, y);
    page = glcd_xy_to_page(x, y);
    line = glcd_xy_to_line(x, y);
    byte = glcd_buffer[chip][page][line];
    if(color) {
        glcd_set_at(byte, glcd_xy_to_bit(x, y));
    } else {
        glcd_clr_at(byte, glcd_xy_to_bit(x, y));
    }
    glcd_buffer[chip][page][line] = byte;
    return 1;
}

void glcd_draw_image_into_buffer() {
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
                color = GLCD_COLOR_BLACK;
            } else {
                color = GLCD_COLOR_WHITE;
            }
            glcd_buffered_plot(x, y, color);
        }
    }
}

void glcd_generate_code_from_buffer(char *output_file_name) {
    FILE *f = fopen(output_file_name, "w+");
    uint8_t chip, page, line;
    fprintf(f, "{\n");
    for(chip = 0; chip < GLCD_BUFFERED_CHIPS; chip++) {
        fprintf(f, "\t{\n");
		for(page = 0; page < GLCD_BUFFERED_CHIP_PAGES; page++) {
            fprintf(f, "\t\t{\n\t\t\t\t");
			for(line = 0; line < GLCD_BUFFERED_PAGE_LINES; line++) {
                if(line % 8 == 0) {
                    fprintf(f, "\n\t\t\t\t");
                }
                fprintf(f, "0x%x", glcd_buffer[chip][page][line]);
                if(line < GLCD_BUFFERED_PAGE_LINES - 1) {
                    fprintf(f, ", ");
                }
			}
            fprintf(f, "\n\t\t},\n");
		}
        fprintf(f, "\t},\n");
	}
    fprintf(f, "}\n");
    fclose(f);
}

void glcd_draw_buffer_into_file(char *output_file_name) {
    FILE *f = fopen(output_file_name, "w+");
    uint8_t x, y, byte;
    for(y = 0; y < GLCD_HEIGHT; y++) {
        for(x = 0; x < GLCD_WIDTH; x++) {
            byte = glcd_buffer[glcd_xy_to_chip(x, y)][glcd_xy_to_page(x, y)][glcd_xy_to_line(x, y)];
            if(glcd_is_set_at(byte, glcd_xy_to_bit(x, y))) {
                fprintf(f, "%c", GLCD_DRAW_COLOR_BLACK);
            } else {
                fprintf(f, "%c", GLCD_DRAW_COLOR_WHITE);
            }
		}
        fprintf(f, "\n");
	}
    fclose(f);
}
