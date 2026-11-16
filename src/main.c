#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "raylib.h"

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
        printf("Please enter a filename %s/<binary_file_name.bin>\n", argv[0]);
        exit(1);
    }

    FILE *bin_fp = fopen(argv[1], "wrb");

    uint8_t binary_image[][2] = {
        {0xFF, 0xFF},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0x80, 0x01},
        {0xFF, 0xFF}
    };
    
    Binary_file test_data = {
        .identifier = {'B', 'F', 'B', 'F'},
        .image_count = 5,
        .image_width = 16,
        .image_height = 16,
    };

    // Write header
    fwrite(&(test_data.identifier), 4 * sizeof(uint8_t), 1, bin_fp);
    fwrite(&(test_data.image_count), sizeof(uint16_t), 1, bin_fp);
    fwrite(&(test_data.image_width), sizeof(uint16_t), 1, bin_fp);
    fwrite(&(test_data.image_height), sizeof(uint16_t), 1, bin_fp);
    
    // Write image data
    for (int i = 0; i < test_data.image_count; i++)
    {
        for (int row = 0; row < test_data.image_height; row++)
        {
            for (int col = 0; col < (test_data.image_width / BYTE); col++)
            {
                fwrite(&(binary_image[row][col]), sizeof(uint8_t), 1, bin_fp);
            }
        }
    }
    
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

    return 0;
}
