/**
 * Simple Run-Length Encoding (RLE) compression/decompression for bitmap images.
 *
 * Format: Alternating count and value bytes
 * Example: 0x03, 0xFF, 0x05, 0x00 = three 0xFF pixels followed by five 0x00 pixels
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

/**
 * Compress a bitmap using RLE.
 *
 * @param input The input bitmap data (1-bit per pixel, row-major)
 * @param width Width of the image in pixels
 * @param height Height of the image in pixels
 * @param output Buffer to receive compressed data (must be allocated by caller)
 * @param max_output_size Maximum size of output buffer
 * @return Actual size of compressed data, or 0 on error
 */
size_t rle_compress(const uint8_t *input, uint32_t width, uint32_t height, uint8_t *output, size_t max_output_size);

/**
 * Decompress RLE-compressed bitmap data.
 *
 * @param input The compressed data
 * @param input_size Size of compressed data
 * @param output Buffer to receive decompressed data (must be allocated by caller)
 * @param max_output_size Maximum size of output buffer in bytes
 * @return Actual size of decompressed data in bytes, or 0 on error
 */
size_t rle_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t max_output_size);
