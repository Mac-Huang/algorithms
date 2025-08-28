#include <stddef.h>
#include <stdio.h>

void main() {
    size_t width = 16;
    size_t a = (((size_t)'A') << (2 * width)) | ((size_t)'B' << width) | (size_t)'C';
    size_t b = (((size_t)'B') << (2 * width)) | ((size_t)'C' << width) | (size_t)'A';
    size_t c = (((size_t)'C') << (2 * width)) | ((size_t)'B' << width) | (size_t)'A';
    printf("%lu %lu %lu\n", a, b, c);
    size_t m = 3 * 3 * 3;
    printf("%lu %lu %lu\n", a % m, b % m, c % m);
}
