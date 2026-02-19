#include "TeleprinterApplet.h"

#include "graphics/niche/InkHUD/Applet.h"
#include "graphics/niche/InkHUD/InkHUD.h"
#include "graphics/niche/InkHUD/WindowManager.h"
#include "rle.h"

// Temporary buffer for decompressed image data
// Max payload is 250 bytes, so we need just enough for that
static uint8_t decompressedImage[256];

// Temporary buffer for XBM format image data
// Size: 256 pixels = 32 bytes (for 1-bit monochrome, 8 pixels per byte)
static uint8_t xbmImage[32];

namespace NicheGraphics::InkHUD
{

/**
 * Called when the applet is activated (brought to foreground).
 */
void TeleprinterApplet::onActivate()
{
    // Register for bitmap message notifications from teleprinter module
    if (teleprinterModule) {
        bitmapMessageObserver.observe(teleprinterModule);
    }
}

/**
 * Called when the applet is deactivated (sent to background).
 */
void TeleprinterApplet::onDeactivate()
{
    // Unregister from bitmap message notifications
    if (teleprinterModule) {
        bitmapMessageObserver.unobserve(teleprinterModule);
    }
}

/**
 * Convert decompressed pixel stream to XBM format.
 *
 * @param input Decompressed pixel data (stream of 0/1 values)
 * @param input_size Size of input data in bytes
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param output Buffer to receive XBM data
 * @param max_output_size Maximum size of output buffer
 * @return Actual size of XBM data in bytes, or 0 on error
 */
static size_t convertToXBM(const uint8_t *input, size_t input_size, uint32_t width, uint32_t height, uint8_t *output,
                           size_t max_output_size)
{
    if (!input || !output || input_size == 0 || width == 0 || height == 0) {
        return 0;
    }

    size_t output_pos = 0;

    // Process each row
    for (uint32_t y = 0; y < height && output_pos < max_output_size; y++) {
        uint8_t byte = 0;
        int bit = 7; // Start with MSB

        // Process each pixel in the row
        for (uint32_t x = 0; x < width && output_pos < max_output_size; x++) {
            uint32_t pixelIndex = y * width + x;
            if (pixelIndex >= input_size) {
                break;
            }

            uint8_t pixelValue = input[pixelIndex];
            // Set the bit: 1 = white, 0 = black
            if (pixelValue) {
                byte |= (1 << bit);
            }

            // Move to next bit
            bit--;
            if (bit < 0) {
                // Byte is full, write it and start a new one
                output[output_pos++] = byte;
                byte = 0;
                bit = 7;
            }
        }

        // Write any remaining bits in the last byte
        if (bit != 7) {
            output[output_pos++] = byte;
        }
    }

    return output_pos;
}

/**
 * Called to render the applet on screen.
 *
 * @param full True if this is a full redraw, false if just updating changed areas.
 */
void TeleprinterApplet::onRender(bool full)
{
    // Check if we have an image to display
    if (!latestImageData || latestImageSize == 0) {
        printAt(X(0.5), Y(0.5), "No Image", CENTER, MIDDLE);
        return;
    }

    // Convert the decompressed pixel stream to XBM format
    size_t xbm_size = convertToXBM(latestImageData, latestImageSize, latestWidth, latestHeight, xbmImage, sizeof(xbmImage));

    if (xbm_size == 0) {
        printAt(X(0.5), Y(0.5), "Invalid Image", CENTER, MIDDLE);
        return;
    }

    // Draw the bitmap image using drawXBitmap
    uint8_t displayWidth = latestWidth > 200 ? 200 : latestWidth;
    uint8_t displayHeight = latestHeight > 200 ? 200 : latestHeight;

    // Center the image on screen
    int16_t xPos = (width() - displayWidth) / 2;
    int16_t yPos = (height() - displayHeight) / 2;

    // Draw using drawXBitmap with appropriate color
    uint16_t color = WHITE; // Use white for the image
    drawXBitmap(xPos, yPos, xbmImage, displayWidth, displayHeight, color);

    // Show image dimensions in the corner
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%dx%d", latestWidth, latestHeight);
    printAt(X(0.95), Y(0.05), buffer, RIGHT, TOP);
}

/**
 * Callback for receiving bitmap messages.
 *
 * @param p The mesh packet containing the bitmap image
 * @return 0 to continue notifying other observers, non-zero to stop
 */
int TeleprinterApplet::onReceiveBitmap(const meshtastic_MeshPacket *p)
{
    // Abort if applet fully deactivated
    if (!isActive())
        return 0;

    // Extract width and height from the packed uint32
    latestWidth = 0;
    latestHeight = 0;

    if (p->which_payload_variant == meshtastic_MeshPacket_decoded_tag) {
        meshtastic_BitmapPayload payload;
        memset(&payload, 0, sizeof(payload));
        if (pb_decode_from_bytes(p->decoded.payload.bytes, p->decoded.payload.size, &meshtastic_BitmapPayload_msg, &payload)) {
            latestWidth = payload.width_height & 0xFF;         // Lower 8 bits
            latestHeight = (payload.width_height >> 8) & 0xFF; // Upper 8 bits

            // Decompress the RLE data
            // Note: We don't actually need width/height for decompression since it's just a stream of pixels
            size_t decompressedSize =
                rle_decompress(p->decoded.payload.bytes, p->decoded.payload.size, decompressedImage, sizeof(decompressedImage));

            if (decompressedSize > 0) {
                latestImageData = decompressedImage;
                latestImageSize = decompressedSize;
            }
        }
    }

    requestAutoshow(); // Want to become foreground, if permitted
    requestUpdate();   // Want to update display, if applet is foreground

    // Return zero: no issues here, carry on notifying other observers!
    return 0;
}

/**
 * Determine which notifications this applet should suppress.
 *
 * @param n The notification to check
 * @return true if this notification should be suppressed, false otherwise
 */
bool TeleprinterApplet::approveNotification(Notification &n)
{
    // Suppress text message notifications when we're showing an image
    // This prevents duplicate notifications for the same content
    return (n.type == Notification::Type::NOTIFICATION_MESSAGE_BROADCAST ||
            n.type == Notification::Type::NOTIFICATION_MESSAGE_DIRECT);
}

} // namespace NicheGraphics::InkHUD
