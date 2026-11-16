#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "storage.h"
#include "types.h"

// Writes a Binary_file struct to a desired binary file
int write_binary_file(Binary_file binary_file, FILE *binary_fp)
{
    // Write header
    fwrite(&(binary_file.identifier), 4 * sizeof(uint8_t), 1, binary_fp);
    fwrite(&(binary_file.image_count), sizeof(uint16_t), 1, binary_fp);
    fwrite(&(binary_file.image_width), sizeof(uint16_t), 1, binary_fp);
    fwrite(&(binary_file.image_height), sizeof(uint16_t), 1, binary_fp);
    
    // Write image data
    for (int index = 0; index < binary_file.image_count; index++)
    {
        for (int row = 0; row < binary_file.image_height; row++)
        {
            for (int col = 0; col < ceil(binary_file.image_width / BYTE); col++)
            {
                fwrite(&(binary_file.image_at[index][row][col]), sizeof(uint8_t), 1, binary_fp);
            }
        }
    }
    return 0;
}

Binary_file read_binary_file(FILE *binary_fp);

// Initialises a Binary file to be later written to and itself write to a binary
// Returns {0} if memory allocation fails
Binary_file init_binary_file(uint16_t count, uint16_t height, uint16_t width)
{
    Binary_file binary_file = {
        // Custom footprint
        .identifier = {'B', 'F', 'B', 'F'},
        .image_count = count,
        .image_height = height,
        .image_width = width,
    };
    Binary_file alloc_err = {0};

    binary_file.image_at = (Binary_image *) malloc(sizeof(Binary_image) * count);
    if (binary_file.image_at == NULL)
    {
        free(binary_file.image_at);
        return alloc_err;
    }

    // Allocate memory for the image array
    for (int index = 0; index < count; index++)
    {
        binary_file.image_at[index] = (Binary_image) malloc(sizeof(uint8_t *) * height);
        if (binary_file.image_at[index] == NULL)
        {
            return alloc_err;
        }

        for (int row = 0; row < height; row++)
        {
            binary_file.image_at[index][row] = (uint8_t *) malloc(sizeof(uint8_t) * ceil(width / BYTE));
            if (binary_file.image_at[index][row] == NULL)
                return alloc_err;
        }
    }
    return binary_file;
}

void set_image_data_at_index(uint16_t index, Binary_image image_data, Binary_file binary_file)
{
    for (int row = 0; row < binary_file.image_height; row++)
    {
        for (int col = 0; col < ceil(binary_file.image_width / BYTE); col++)
        {
            binary_file.image_at[index][row][col] = image_data[row][col];
        }
    }
}

void free_binary_file(Binary_file binary_file)
{
    for (int index = 0; index < binary_file.image_count; index++)
    {
        for (int row = 0; row < binary_file.image_height; row++)
        {
            free(binary_file.image_at[index][row]);
        }
        free(binary_file.image_at[index]);
    }
    free(binary_file.image_at);
}

