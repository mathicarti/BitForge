#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "storage.h"
#include "types.h"

// Little-endian helpers, so the file format never depends on the host CPU.
static void write_u16_le(uint8_t out[2], uint16_t value)
{
    // TODO: low byte first. out[0] = ?, out[1] = ?
    return;
}

static uint16_t read_u16_le(const uint8_t in[2])
{
    // TODO: rebuild the value from the two bytes
    return 0;
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
    if (byte_offset == NULL || bit == NULL || !(sprite < bank->count && x < bank->width && y < bank->height))
        return BF_ERR_ARGS;
    
    switch (bank->orientation)
    {
        case ORIENTATION_HORIZONTAL:
            *byte_offset = (sprite * (bank->width / BYTE) * bank->height) + (y * (bank->width / BYTE) + x / BYTE);
            *bit = x % BYTE;
            return BF_OK;

        case ORIENTATION_VERTICAL:
            *byte_offset = (sprite * (bank->height / BYTE) * bank->width) + ((y / BYTE) * bank->width + x);
            *bit = x * BYTE;
            return BF_OK;
        default:
            return BF_ERR_ARGS;
    }
}

BF_status bank_init(Sprite_bank *bank, uint8_t width, uint8_t height, uint8_t orientation, uint16_t capacity)
{
    if (bank == NULL || width > UINT8_MAX || height > UINT8_MAX)
        return BF_ERR_ARGS;

    bank->width = width;
    bank->height = height;
    bank->orientation = orientation;
    bank->count = 0;
    bank->capacity = capacity;
    bank->image_size = 0;
    bank->data = NULL;

    size_t image_size = bf_image_size(width, height, orientation);
    if (image_size == 0)
        return BF_ERR_ARGS;

    bank->image_size = image_size;
    
    uint8_t *temp = calloc(capacity, image_size);
    if (temp == NULL)
        return BF_ERR_ALLOC;

    bank->data = temp;
    
    return BF_OK;
}

BF_status bank_add_image(Sprite_bank *bank, uint16_t *new_index)
{
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
    uint8_t data_size = bank->capacity * bank->image_size;
    uint8_t to_index_size = *new_index * bank->image_size;
    uint8_t *bytes_start = bank->data + to_index_size;
    memset(bytes_start, 0, data_size - to_index_size);

    return BF_OK;
}

void bank_free(Sprite_bank *bank)
{
    free(bank->data);
    bank->data = NULL;
    bank->count = bank->capacity = 0;
    return;
}

BF_status bank_get_pixel(const Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out)
{
    // TODO: bank_locate, then (byte >> bit) & 1
    return BF_ERR_ARGS;
}

BF_status bank_set_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out)
{
    // TODO: bank_locate, then OR in (1 << bit)
    return BF_ERR_ARGS;
}

BF_status bank_clear_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out)
{
    // TODO: bank_locate, then AND with the inverse of (1 << bit)
    return BF_ERR_ARGS;
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
