#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

void pair(size_t count, uint8_t byte)
{
    static size_t unique_count = 0;
    static uint8_t unique_data[15];
    if (count == 1) {
        unique_data[unique_count++] = byte;
        assert(unique_count <= 15);
    }
    else {
        assert(count <= 15);
        uint8_t control = (count << 4) | unique_count;
        putchar(control);

        for (size_t i = 0; i < unique_count; i++) {
            putchar(unique_data[i]);
        }

        if (count != 0) {
            putchar(byte);
        }
        else {
            assert(control == 0);
        }

        unique_count = 0;
    }
}

int main(int argc, char *argv[])
{
    size_t count = 1;
    uint8_t byte = getchar();
    int new;
    size_t position = 0;

    while ((new = getchar()) != EOF) {
        if (byte == new) {
            count++;
        }
        else {
            pair(count, byte);
            byte = new;
            count = 1;
        }
    }

    pair(count, byte);
    pair(0, 0);
}
