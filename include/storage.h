#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdlib.h>

#define BF_IDENTIFIER_SIZE 4
#define BF_HEADER_SIZE 10
#define BF_VERSION 1

#define ORIENTATION_HORIZONTAL 0
#define ORIENTATION_VERTICAL 1

// Enum to classify errors
typedef enum
{
    BF_OK = 0,
    BF_ERR_ARGS,        // bad dimensions, index, NULL pointer...
    BF_ERR_ALLOC,
    BF_ERR_OPEN,
    BF_ERR_IO,          // short read/write
    BF_ERR_BAD_ID,
    BF_ERR_BAD_VERSION,
    BF_ERR_CORRUPT      // size mismatch, bad orientation, zero dimension
} BF_status;

/* Binary file header of 10 bytes:
 * Unique identifier: 'B' 'F' 'B' 'F'
 * Version number (future proofing)
 * Image Height: up to 255 pixels
 * Image Width: up to 255 pixels
 * Storage orientation: 0 (horizontal stacking), 1 (vertical stacking)
 * Image count: uint16_t
 */
typedef struct
{
    uint8_t identifier[BF_IDENTIFIER_SIZE];
    uint8_t version;
    uint8_t height;
    uint8_t width;
    uint8_t orientation;
    uint16_t count;
} BF_header;

// In memory representation of the custom sprites data
typedef struct
{
    uint8_t height;
    uint8_t width;
    uint8_t orientation;

    uint16_t count; // Current sprite count in bank
    uint16_t capacity; // Images that are in *data before growing
    size_t image_size; // Cached bytes of one image

    uint8_t *data; // One data block of capacity * image_size
} Sprite_bank;

/* --- Sprite Bank sizing and addressing --- */
// Sizing an image in a sprite bank
size_t bf_image_size(uint8_t width, uint8_t height, uint8_t orientation);
// Locating a bit in a sprite bank
BF_status bank_locate(const Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, size_t *byte_offset, uint8_t *bit_offset);

/* Sprite Bank Lifecycle */
BF_status bank_init(Sprite_bank *bank, uint8_t width, uint8_t height, uint8_t orientation, uint16_t capacity);
// Adds a sprite to an existing sprite bank (reallocs if needed)
BF_status bank_add_image(Sprite_bank *bank, uint16_t *new_sprite);
void bank_free(Sprite_bank *bank);

/* --- Sprite Bank pixel manipulation --- */
// Gets the value of a target pixel
BF_status bank_get_pixel(const Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out);
// Sets a target pixel to white
BF_status bank_set_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out);
// Sets a target pixel to black
BF_status bank_clear_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out);

/* --- File and sys I/O --- */
// Saves a sprite bank to a target binary file
BF_status bank_save(const Sprite_bank *bank, const char *path);
// Loads a valid binary sprite bank to memory
BF_status bank_load(Sprite_bank *bank, const char *path);
// Return status specific strings to be printed
const char *bf_status_string(BF_status status);

#endif