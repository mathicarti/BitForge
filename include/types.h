#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

#define BYTE 8

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

    uint16_t image_width;
    uint16_t image_height;

    uint16_t image_count;
    Binary_image *image_at;
} Binary_file;

#endif