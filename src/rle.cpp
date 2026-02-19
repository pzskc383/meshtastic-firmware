/**
 * Simple Run-Length Encoding (RLE) for 1-bit monochrome images.
 *
 * Format: Alternating count and value bytes
 * Count is the number of pixels, value is 0 or 1
 * Example: 0x03, 0x01, 0x05, 0x00 = three white pixels followed by five black pixels
 */

#include "rle.h"

/**
 * Decompress RLE-compressed bitmap data.
 *
 * Assumes input is a stream of (count, value) pairs where:
 * - count: number of consecutive pixels (1-255)
 * - value: pixel value (0 or 1)
 *
 * @param input The compressed data
 * @param input_size Size of compressed data
 * @param output Buffer to receive decompressed data (must be allocated by caller)
 * @param max_output_size Maximum size of output buffer in bytes
 * @return Actual size of decompressed data in bytes, or 0 on error
 */
size_t rle_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t max_output_size)
{
    if (!input || !output || input_size == 0 || max_output_size == 0) {
        return 0;
    }

    size_t output_pos = 0;

    // Process each (count, value) pair
    for (size_t i = 0; i < input_size;) {
        if (i + 1 >= input_size) {
            // Incomplete pair
            return 0;
        }

        uint8_t count = input[i++];
        uint8_t value = input[i++];

        // Clamp count to reasonable values
        if (count == 0 || count > 255) {
            return 0;
        }

        // Write the pixel value 'count' times
        for (uint8_t j = 0; j < count && output_pos < max_output_size; j++) {
            output[output_pos++] = value;
        }

        if (output_pos >= max_output_size) {
            break; // Output buffer full
        }
    }

    return output_pos;
}
