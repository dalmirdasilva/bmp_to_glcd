
#include "main.h"
#include "bmp.c"

void main(int argc, char *argv[]) {
    bmp_load(argv[1], argv[2]);
    copy_to_output();
    printf("bmp_get_width(): %d\n", bmp_get_width());
    printf("bmp_get_height(): %d\n", bmp_get_height());
    printf("bmp_get_bits_per_pixel(): %d\n", bmp_get_bits_per_pixel());
    printf("bmp_get_pixel_array_offset(): %d\n", bmp_get_pixel_array_offset());
    printf("bmp_get_row_padding(): %d\n", bmp_get_row_padding());
    printf("Generating output code\n");
    generate_output_code();
    printf("Generating output bit image\n");
    generate_output_bit_image();
    bmp_close();
    printf("Done!\n");
}

void generate_output_code() {
    FILE *f = fopen("/tmp/output_code.txt", "w+");
    int i, j;
    int8_t rgb[3];
    uint8_t *bmp_row, k, z;
    for(k = 0, j = bmp_get_height() - 1; j >= 0; j--, k++) {
        bmp_row = bmp_get_row(j);
        int8_t color;
        for(z = 0, i = 0; i < (bmp_get_row_size() - bmp_get_row_padding()); z++, i += 3) {
            rgb[0] = bmp_row[i];
            rgb[1] = bmp_row[i+1];
            rgb[2] = bmp_row[i+2];
            fprintf(f, "%d", to_black_and_white(rgb));
            if(to_black_and_white(rgb)) {
                color = GLCD_COLOR_BLACK;
            } else {
                color = GLCD_COLOR_WHITE;
            }
            glcd_buffered_plot(z, k, color);
            bmp_row[i] = color;
            bmp_row[i+1] = color;
            bmp_row[i+2] = color;
        }
        bmp_set_row(j, bmp_row);
        fprintf(f, "\n");
    }
    fclose(f);
}


void generate_output_bit_image() {
    FILE *f = fopen("/tmp/output_bit_image.txt", "w+");
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

int to_black_and_white(uint8_t rgb[]) {
    float gray_scale = (rgb[0] * .3) + (rgb[1] * .59) + (rgb[2] * .59);
    if(gray_scale > 120) {
        return 0;
    } else {
        return 1;
    }
}

uint8_t glcd_buffered_plot(uint8_t x, uint8_t y, enum glcd_color_t color) {
    uint8_t byte;
    uint8_t chip = GLCD_CHIP_1;
    if(x > GLCD_WIDTH || y > GLCD_HEIGHT) {
        return 0;
    }
    if(x >= GLCD_HALF_WIDTH) {
        x -= GLCD_HALF_WIDTH;
        chip = GLCD_CHIP_2;
    }
    byte = glcd_buffer[chip][y / 8][x];
    if(color) {
        glcd_set_bit_at(byte, (y % 8));
    } else {
        glcd_clr_bit_at(byte, (y % 8));
    }
    glcd_buffer[chip][y / 8][x] = byte;
    return 1;
}
