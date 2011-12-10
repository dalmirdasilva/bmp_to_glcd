#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    BI_RGB = 0,
    BI_RLE8,
    BI_RLE4,
    BI_BITFIELDS, // Also Huffman 1D compression for BITMAPCOREHEADER2
    BI_JPEG, // Also RLE-24 compression for BITMAPCOREHEADER2
    BI_PNG,
} bitmap_compression_method_t;

typedef struct {
    uint8_t magic[2];
} bitmap_magic_t;

typedef struct {
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixel_array_offset;
} bitmap_file_header_t;

typedef struct {
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compress_type;
    uint32_t image_size;
    uint32_t x_pixel_per_m;
    uint32_t y_pixel_per_m;
    uint32_t colors;
    uint32_t import_colors;  
} bitmap_info_header_t;

typedef struct {
    bitmap_magic_t *bitmap_magic;
    bitmap_file_header_t *bitmap_file_header;
    bitmap_info_header_t *bitmap_info_header;
} bitmap_header_t;

FILE *bmp_input_file = NULL;
FILE *bmp_output_file = NULL;

bitmap_header_t *bmp_header;

/**
 * Loads the input image information into the proper structures
 * Also opens both files.
 * 
 * @param   The input file name
 * @param   The output file name
 * @return  void
 */
void bmp_load(char *bmp_input_file_name, char *bmp_output_file_name);

/**
 * Closes both input and output files
 * 
 * @param   void
 * @return  void 
 */
void bmp_close();

/**
 * Gets the image width
 * 
 * @param   void
 * @return  Image width
 */
uint32_t bmp_width();

/**
 * Gets the image height
 * 
 * @param   void
 * @return  Image height
 */
uint32_t bmp_height();

/**
 * Gets the number of bits per pixel
 * 
 * @param   void
 * @return  The number of bits
 */
uint16_t bmp_bits_per_pixel();

/**
 * Gets the offset from the fisrt byte of the file untill the begining
 * of the image contents
 * 
 * @param   void
 * @return  The offset
 */
uint16_t bmp_pixel_array_offset();

/**
 * Gets the number of bytes of a pixel
 * 
 * @param   void
 * @return  The pixel size
 */
uint8_t bmp_pixel_size();

/**
 * Gets the padding of the row
 * 
 * A row has a padding > 0 when its row isn't multiple by 4
 * 
 * @param   void
 * @return  The row padding
 */
uint8_t bmp_row_padding();

/**
 * Gets the input image size
 * 
 * NOTE: Image size is the size of the pixel matrix, if you want to know
 * the file size use the file_size attr from bitmap_file_header_t struct
 * 
 * @param   void
 * @return  The image size
 */
uint32_t bmp_image_size();

/**
 * Gets the input row size 
 * 
 * @param   void
 * @return  The row size
 */
uint16_t bmp_row_size();

/**
 * Reads a entire row from the input image 
 * 
 * @param   The givem row
 * @return  The row data
 */
uint8_t *bmp_read_row(uint16_t row);

/**
 * Writes a entire row into the output image 
 * 
 * @param   The givem row
 * @param   The data to be written
 * @return  The number of bytes written
 */
size_t bmp_write_row(uint16_t row, uint8_t *row_data);

/**
 * Make a binary copy from the input image to the output
 * 
 * @param   void
 * @return  void
 */
void copy_to_output();

/**
 * Return true if the input umage is valid false otherwise
 * 
 * The following entries are possible:
 * BM - Windows 3.1x, 95, NT, ... etc.; and it is not mandatory unless 
 *      file size is greater or equal to SIGNATURE
 * BA - OS/2 struct Bitmap Array
 * CI - OS/2 struct Color Icon
 * &CP - OS/2 const Color Pointer
 * IC - OS/2 struct Icon
 * PT - OS/2 Pointer
 * 
 * @param   void
 * @return  void
 */
uint8_t bmp_is_valid();
