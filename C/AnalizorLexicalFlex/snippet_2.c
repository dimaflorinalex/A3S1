/* Code Snippet 2 */
#include <stdio.h>
#define MAX_SIZE 100

int main() {
    // This is a comment
    int x = 42;
    float y = 3.14;  // Comment on same line with code

    /*
        Numbers
        Hexadecimal, Binary, Decimal
    */
    int hex_number = 0x1A3F;
    int oct_number = 037;
    int bin_number = 0b1101;
    int dec_number = 10;

    x += 5;
    y *= 2;

    char c = 'a';
    char* str = "Hello, world!";

    if (x > y) {
        x = x / 2;
    }

    return 0;
}