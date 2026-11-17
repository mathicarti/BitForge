#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "storage.h"
#include "types.h"

size_t bf_image_size(uint8_t width, uint8_t height, uint8_t orientation)
{
    return 0;
}

BF_status bank_locate(const Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, size_t *byte_offset, uint8_t *bit_offset)
{
    return BF_ERR_ARGS;
}

BF_status bank_init(Sprite_bank *bank, uint8_t width, uint8_t height, uint8_t orientation, uint16_t capacity)
{
    return BF_ERR_ARGS;
}

BF_status bank_add_image(Sprite_bank *bank, uint16_t *new_sprite)
{
    return BF_ERR_ARGS;
}

void bank_free(Sprite_bank *bank)
{
    return;
}

BF_status bank_get_pixel(const Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out)
{
    return BF_ERR_ARGS;
}

BF_status bank_set_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out)
{
    return BF_ERR_ARGS;
}

BF_status bank_clear_pixel(Sprite_bank *bank, uint16_t sprite, uint8_t x, uint8_t y, uint8_t *out)
{
    return BF_ERR_ARGS;
}

BF_status bank_save(const Sprite_bank *bank, const char *path)
{
    return BF_ERR_ARGS;
}

BF_status bank_load(Sprite_bank *bank, const char *path)
{
    return BF_ERR_ARGS;
}

const char *bf_status_string(BF_status status)
{
    return "";
}
