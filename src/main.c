#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    uint8_t horizontal_binary_image[] = {
        0x3F, 0x03, 0x03, 0x0F, 0x03, 0x03, 0x03, 0x00
    };
    uint8_t vertical_binary_image[] = {
        0x7F, 0x7F, 0x09, 0x09, 0x01, 0x01, 0x00, 0x00
    };

    uint16_t new_index = 0;
    Sprite_bank sprite_bank = {0};
    bank_init(&sprite_bank, 8, 16, ORIENTATION_VERTICAL, 1);
    
    bank_add_image(&sprite_bank, &new_index);
    printf("%d\n", new_index);
    bank_add_image(&sprite_bank, &new_index);
    printf("%d\n", new_index);
    bank_add_image(&sprite_bank, &new_index);
    printf("%d\n", new_index);
    bank_add_image(&sprite_bank, &new_index);
    printf("%d\n", new_index);

    bank_free(&sprite_bank);
    
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
