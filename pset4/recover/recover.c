#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
BYTE buffer[512];

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: recover [forensic image]\n");
        return 1;
    }

    // open forensic image
    FILE *file = fopen(argv[1], "r");
    // ensure file can be opened
    if (file == NULL)
    {
        return 1;
    }

    // variables for output file naming
    int count = 0;
    char name[8];

    // read through file one block (512 bytes) at a time
    while (fread(buffer, 1, 512, file) == 512)
    {
        // if jpg signature found, open new outptr file and begin writing
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] != 0x00)
        {
            // filename generation
            sprintf(name, "%03i.jpg", count);
            // increment next filename
            count++;
            FILE *outptr = fopen(name, "w");
            fwrite(buffer, 1, 512, outptr);
            fclose(outptr);
        }
        // append next block to outptr file
        else if (count > 0)
        {
            FILE *outptr = fopen(name, "a");
            fwrite(buffer, 1, 512, outptr);
            fclose(outptr);
        }
    }
    fclose(file);
}