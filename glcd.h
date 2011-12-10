
#include <stdint.h>

#define GLCD_WIDTH                          128
#define GLCD_HALF_WIDTH                     64
#define GLCD_HEIGHT                         64
#define GLCD_HALF_HEIGHT                    32
#define GLCD_CHIPS                          2

#define GLCD_BUFFERED_CHIPS					2
#define GLCD_BUFFERED_CHIP_PAGES			8
#define GLCD_BUFFERED_PAGE_LINES			64

#define glcd_set_at(target, bit)            (target) |= (1 << (bit))
#define glcd_clr_at(target, bit)            (target) &= ~(1 << bit)
#define glcd_is_set_at(target, bit)         (target) & (1 << (bit))
#define glcd_is_clr_at(target, bit)         (target) & ~(1 << bit)

enum glcd_chip_t {
    GLCD_CHIP_1 = 0,
    GLCD_CHIP_2 = 1,
    GLCD_CHIP_ALL = 0xff
};

enum glcd_color_t {
    GLCD_COLOR_BLACK = 0x00,
    GLCD_COLOR_WHITE = 0xff
};

enum glcd_draw_color_t {
    GLCD_DRAW_COLOR_BLACK = '@',
    GLCD_DRAW_COLOR_WHITE = ' '
};

uint8_t glcd_xy_to_chip(uint8_t x, uint8_t y);
uint8_t glcd_xy_to_page(uint8_t x, uint8_t y);
uint8_t glcd_xy_to_line(uint8_t x, uint8_t y);
uint8_t glcd_xy_to_bit(uint8_t x, uint8_t y);

uint8_t glcd_buffer[GLCD_BUFFERED_CHIPS][GLCD_BUFFERED_CHIP_PAGES][GLCD_BUFFERED_PAGE_LINES];

void glcd_draw_image_into_buffer();
void glcd_generate_code_from_buffer(char *output_file_name);
void glcd_draw_buffer_into_file(char *output_file_name);

