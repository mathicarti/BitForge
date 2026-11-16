#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "storage.h"
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
        printf("Please enter a filename %s <binary_file_name.bin>\n", argv[0]);
        exit(1);
    }

    FILE *binary_fp = fopen(argv[1], "w+b");
    if (binary_fp == NULL)
        exit(2);

    Binary_file test_binary = init_binary_file(5, 16, 16);
    if (test_binary.image_at == NULL)
        exit(3);

    // Dummy binary image
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
    
    for (int i = 0; i < test_binary.image_count; i++)
    {
        set_image_data_at_index(i, binary_image, test_binary);
    }
    write_binary_file(test_binary, binary_fp);
    
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

    free_binary_file(test_binary);
    fclose(binary_fp);

    return 0;
}
