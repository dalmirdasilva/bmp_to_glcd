
#include <stdint.h>

#define GLCD_WIDTH                          128
#define GLCD_HALF_WIDTH                     64
#define GLCD_HEIGHT                         64
#define GLCD_HALF_HEIGHT                    32
#define GLCD_CHIPS                          2

#define GLCD_BUFFERED_CHIPS					2
#define GLCD_BUFFERED_CHIP_PAGES			8
#define GLCD_BUFFERED_PAGE_LINES			64

#define glcd_set_bit_at(target, bit)    (target) |= (1 << (bit))
#define glcd_clr_bit_at(target, bit)    (target) &= ~(1 << bit)

enum glcd_chip_t {
    GLCD_CHIP_1 = 0,
    GLCD_CHIP_2 = 1,
    GLCD_CHIP_ALL = 0xff
};

enum glcd_color_t {
    GLCD_COLOR_BLACK = 0x00,
    GLCD_COLOR_WHITE = 0xff
};

/**
 * Representation of data inside the buffer
 */
uint8_t glcd_buffer[GLCD_BUFFERED_CHIPS][GLCD_BUFFERED_CHIP_PAGES][GLCD_BUFFERED_PAGE_LINES];

void generate_output_code();

void generate_output_bit_image();

int to_black_and_white(uint8_t rgb[]);

uint8_t glcd_buffered_plot(uint8_t x, uint8_t y, enum glcd_color_t color);
