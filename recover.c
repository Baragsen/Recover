#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;

    // checking for args and file validity
    if (argc != 2)
    {
        printf("Usage : .\\recover.exe  name of a forensic image from which to recover JPEGs");
        return 1;
    }

    FILE *forensic = fopen(argv[1] , "r");

    if (forensic == NULL)
    {
        printf("Cant open the file (insupported format or file not found)");
        return 1;

    }
    //filename
    char *filename = malloc(8 * sizeof(char));
    if (filename == NULL)
        return 1;

    // FAT file system Blocks
    int Block_Size = 512 ;
    // Buffer for hex blocks
    BYTE buffer[Block_Size];
    int image_count = 0;

    //recover the JPEGs

    while (fread(buffer, sizeof(BYTE), Block_Size, forensic) == Block_Size)
    {
        // Check for JPEG's signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8  && buffer[2] == 0xff && (buffer[3] & 11110000) == 11100000)
        {
            //write the JPEG filenames
            sprintf(filename , "%03i.jpg" , image_count);

            //open the file for writing the buffer and increasing the counter
            FILE *output_file = fopen(filename , "w");
            image_count++;

            if (output_file != NULL)
                fwrite(buffer , sizeof(BYTE) , Block_Size , output_file);
            
            fclose(output_file);
        }
    }
    free(filename);
    fclose(forensic);
}