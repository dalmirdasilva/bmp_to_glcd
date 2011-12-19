#include <stdint.h>
#include <stdlib.h>

int gbm_buffer_size;
uint8_t *gbm_buffer;

void gbm_init();
void gbm_plot(uint8_t x, uint8_t y, enum glcd_color_t color);
void gbm_draw_image_into_buffer();
void gbm_generate_code_from_buffer(char *output_file_name);
