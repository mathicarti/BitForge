#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "storage.h"
#include "raylib.h"

// Gets the index of a byte of a specific sprite in *data
ssize_t byte_sprite_index(uint8_t sprite_index, uint8_t x, uint8_t y, Sprite_bank sprite_bank)
{
    // Bounds checking
    if (!(sprite_index <= sprite_bank.count && x < sprite_bank.width && y < sprite_bank.height))
        return -1;

    switch (sprite_bank.orientation)
    {
        case ORIENTATION_HORIZONTAL:
            return y * (sprite_bank.width / BYTE) + (x / BYTE);
        
        case ORIENTATION_VERTICAL:
            return (y / BYTE) * sprite_bank.width + x;

        default:
            return -1;
    }
}

int main(int argc, char **argv)
{
    /*
    // Window init
    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "First window");
    SetTargetFPS(60);
    */

    // Testing functions and debug
    
    // Get the filename from the program args
    if (argc != 2)
    {
        printf("Please enter a filename %s <binary_file_name.bin>\n", argv[0]);
        exit(1);
    }

    FILE *binary_fp = fopen(argv[1], "w+b");
    if (binary_fp == NULL)
        exit(2);

    // Dummy binary image
    uint8_t vertical_binary_image[] = {
        0x7F, 0x7F, 0x09, 0x09, 0x01, 0x01, 0x00, 0x00
    };

    uint8_t horizontal_binary_image[] = {
        0x3F, 0x03, 0x03, 0x0F, 0x03, 0x03, 0x03, 0x00
    };

    // Init sprite banks and copy data into them
    Sprite_bank dummy_vertical_bank = {
        .height = 8,
        .width = 8,
        .orientation = ORIENTATION_VERTICAL,
        .count = 0,
        .capacity = 1,
    };
    dummy_vertical_bank.image_size = dummy_vertical_bank.height * (dummy_vertical_bank.width / BYTE);
    dummy_vertical_bank.data = (uint8_t *) malloc(dummy_vertical_bank.capacity * dummy_vertical_bank.image_size);

    if ((dummy_vertical_bank.capacity - dummy_vertical_bank.count) <= 0)
        // realloc more space
        printf("Bank ran out of memory!\n");
    
    memcpy(dummy_vertical_bank.data, vertical_binary_image, sizeof(vertical_binary_image));
    dummy_vertical_bank.count++;

    Sprite_bank dummy_horizontal_bank = {
        .height = 8,
        .width = 8,
        .orientation = ORIENTATION_HORIZONTAL,
        .count = 0,
        .capacity = 1,
    };
    dummy_horizontal_bank.image_size = dummy_horizontal_bank.height * (dummy_horizontal_bank.width / BYTE);
    dummy_horizontal_bank.data = (uint8_t *) malloc(dummy_horizontal_bank.capacity * dummy_horizontal_bank.image_size);

    if ((dummy_horizontal_bank.capacity - dummy_horizontal_bank.count) <= 0)
        // realloc more space
        printf("Bank ran out of memory!\n");
    
    memcpy(dummy_horizontal_bank.data, horizontal_binary_image, sizeof(horizontal_binary_image));
    dummy_horizontal_bank.count++;

    uint8_t probe_x = 2, probe_y = 3;

    ssize_t vertical_index = byte_sprite_index(1, probe_x, probe_y, dummy_horizontal_bank);
    ssize_t horizontal_index = byte_sprite_index(1, probe_x, probe_y, dummy_vertical_bank);

    uint8_t vertical_data = vertical_index < 0 ? 0xFF : dummy_vertical_bank.data[vertical_index];
    uint8_t horizontal_data = horizontal_index < 0 ? 0xFF : dummy_horizontal_bank.data[horizontal_index];

    printf("  Vertical data | Byte for pixel x=%d, y=%d : %1zd | Data : 0x%2x\n", probe_x, probe_y, vertical_index, vertical_data);
    printf("Horizontal data | Byte for pixel x=%d, y=%d : %1zd | Data : 0x%2x\n", probe_x, probe_y, horizontal_index, horizontal_data);

    free(dummy_vertical_bank.data);
    free(dummy_horizontal_bank.data);
    
    /*
    // Main program loop
    while (!WindowShouldClose())
    {
        // Logic

        // Drawing to screen
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }
    CloseWindow();
    */
    
    fclose(binary_fp);

    return 0;
}
