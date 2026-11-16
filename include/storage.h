#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t** Binary_image;

/* Header of the binary file:
 * Identifier: 'B' 'F' 'B' 'F' (BitForge Binary File) = 4 * uint8_t
 * Per image_width = 1 * uint16_t
 * Per image_height = 1 * uint6_t
 * Number of images in file = 1 * uint16_t
 * Data of all images
 */
typedef struct
{
    uint8_t identifier[4];

    uint16_t image_height;
    uint16_t image_width;

    uint16_t image_count;
    Binary_image *image_at;
} Binary_file;

Binary_file init_binary_file(uint16_t count, uint16_t height, uint16_t width);
void set_image_data_at_index(uint16_t index, Binary_image image_data, Binary_file binary_file);

Binary_file read_binary_file(FILE *binary_fp);
int write_binary_file(Binary_file binary_file, FILE *binary_fp);
void free_binary_file(Binary_file binary_file);

#endif