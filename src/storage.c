#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "storage.h"
#include "types.h"

// File format stores multi-byte values low byte first, 
// so saved files don't depend on the host CPU. 
// Example: 0x19F2 -> {F2, 19}
static void write_u16_le(uint8_t out[2], uint16_t value)
{
    out[0] = (uint8_t) value & 0xFF;
    out[1] = (uint8_t) value >> BYTE; // unsigned; defined behaviour
}

// Inverse of write_u16_le: {F2, 19} -> 0x19F2.
static uint16_t read_u16_le(const uint8_t in[2])
{
    return (uint16_t) in[0] | (uint16_t) in[1] << BYTE;
}

size_t bf_image_size(uint8_t width, uint8_t height, uint8_t orientation)
{
    switch (orientation)
    {
    case ORIENTATION_HORIZONTAL:
        return ((width + BYTE - 1) / BYTE) * height; // Round up logic for safe byte allocation

    case ORIENTATION_VERTICAL:
        return width * ((height + BYTE - 1) / BYTE);
    
    default:
        return 0;
    }
}

BF_status bank_locate(const Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, size_t *byte_offset, uint8_t *bit)
{
    if (bank == NULL || byte_offset == NULL || bit == NULL)
        return BF_ERR_ARGS;

    if (sprite >= bank->count || x >= bank->width || y >= bank->height)
        return BF_ERR_ARGS;
    
    switch (bank->orientation)
    {
    case ORIENTATION_HORIZONTAL:
        // Round up bytes to accommodate any size
        *byte_offset = sprite * bank->image_size + y * ((bank->width + BYTE - 1) / BYTE) + x / BYTE;
        *bit = x % BYTE;
        return BF_OK;

    case ORIENTATION_VERTICAL:
        *byte_offset = sprite * bank->image_size + ((y + BYTE - 1) / BYTE) * bank->width + x;
        *bit = y % BYTE;
        return BF_OK;
    default:
        return BF_ERR_ARGS;
    }
}

BF_status bank_init(Sprite_bank *bank, uint8_t width, uint8_t height, uint8_t orientation, uint16_t capacity)
{
    if (bank == NULL)
        return BF_ERR_ARGS;

    bank->width = width;
    bank->height = height;
    bank->orientation = orientation;
    bank->count = 0;
    bank->capacity = 0;
    bank->image_size = 0;
    bank->data = NULL;

    size_t image_size = bf_image_size(width, height, orientation);
    if (image_size == 0)
        return BF_ERR_ARGS;

    bank->image_size = image_size;
    
    uint8_t *temp = calloc(capacity, image_size);
    if (temp == NULL)
        return BF_ERR_ALLOC;

    bank->capacity = capacity;
    bank->data = temp;
    
    return BF_OK;
}

BF_status bank_add_image(Sprite_bank *bank, uint16_t *new_index)
{
    if (bank == NULL || new_index == NULL || bank->image_size == 0)
        return BF_ERR_ARGS;

    // Bounds against underflow when count++ (and making sure we dont double the capacity next time around_)
    if (bank->count == UINT16_MAX)
        return BF_ERR_ALLOC;
    
    // Grow bank data size if needed
    if (bank->count >= bank->capacity)
    {
        size_t new_capacity = bank->capacity == 0 ? 1 : bank->capacity * 2;
        if (new_capacity > UINT16_MAX)
            return BF_ERR_ALLOC;

        uint8_t *temp = realloc(bank->data, new_capacity * bank->image_size);
        if (temp == NULL)
            return BF_ERR_ALLOC;

        bank->capacity = new_capacity;
        bank->data = temp;
    }
    *new_index = bank->count++;

    // Set the bytes for the new sprite to 0
    uint8_t *bytes_start = bank->data + (*new_index * bank->image_size);
    memset(bytes_start, 0, bank->image_size);

    return BF_OK;
}

void bank_free(Sprite_bank *bank)
{
    if (bank == NULL)
        return;

    free(bank->data);
    bank->data = NULL;
    bank->count = bank->capacity = 0;
}

BF_status bank_get_pixel(const Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out)
{
    size_t byte_offset = 0;
    uint8_t bit = 0;

    BF_status status_loc = bank_locate(bank, sprite, x, y, &byte_offset, &bit);
    if (status_loc != BF_OK)
        return status_loc;
    
    // Get the value of the bit at the index of the byte
    *out = (bank->data[byte_offset] >> bit) & 1;
    return BF_OK;
}

BF_status bank_set_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y)
{
    uint8_t pixel_value;
    BF_status status_get = bank_get_pixel(bank, sprite, x, y, &pixel_value);
    if (status_get != BF_OK)
        return status_get;

    // If target pixel already ON
    if (pixel_value == PIXEL_ON)
        return BF_OK;
    
    // Set it ON
    size_t byte_offset = 0;
    uint8_t bit = 0;

    BF_status status_loc = bank_locate(bank, sprite, x, y, &byte_offset, &bit);
    if (status_loc != BF_OK)
        return status_loc;
    
    bank->data[byte_offset] |= (1 << bit);
    return BF_OK;
}

BF_status bank_clear_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y)
{
    uint8_t pixel_value;
    BF_status status_get = bank_get_pixel(bank, sprite, x, y, &pixel_value);
    if (status_get != BF_OK)
        return status_get;

    // If target pixel already OFF
    if (pixel_value == PIXEL_OFF)
        return BF_OK;
    
    // Set it OFF
    size_t byte_offset = 0;
    uint8_t bit = 0;

    BF_status status_loc = bank_locate(bank, sprite, x, y, &byte_offset, &bit);
    if (status_loc != BF_OK)
        return status_loc;
    
    bank->data[byte_offset] &= ~(1 << bit);
    return BF_OK;
}

BF_status bank_save(const Sprite_bank *bank, const char *path)
{
    // TODO: fopen "wb", check NULL
    //       pack the 10 header bytes into a small array (use write_u16_le)
    //       fwrite header, then ONE fwrite for count * image_size payload
    //       check each return value, fclose, return the status
    return BF_ERR_ARGS;
}

BF_status bank_load(Sprite_bank *bank, const char *path)
{
    // TODO: follow the load pseudocode from before
    //       validate magic, version, sizes, orientation
    //       compare 10 + count * image_size to the real file size
    //       allocate, ONE fread, close
    //       only fill *bank on success
    return BF_ERR_ARGS;
}

const char *bf_status_string(BF_status status)
{
    // TODO: switch returning a short message for each status
    return "";
}
