#include "TeleprinterModule.h"
#include "MeshService.h"

TeleprinterModule *teleprinterModule;

/**
 * Constructor for TeleprinterModule.
 *
 * Sets up the module to handle BitmapPayload messages on port MESHFAX_APP (78).
 */
TeleprinterModule::TeleprinterModule()
    : ProtobufModule("teleprinter", meshtastic_PortNum_MESHFAX_APP, &meshtastic_BitmapPayload_msg)
{
}

/**
 * Handle a received bitmap image message.
 *
 * This method is called when a BitmapPayload message is received on the mesh.
 * You can implement logic here to display, store, or forward the image.
 *
 * @param mp The mesh packet containing the bitmap image
 * @param decoded The decoded BitmapPayload protobuf message (may be NULL if decoding failed)
 * @return true if this module has handled the message and no other handlers should process it.
 */
bool TeleprinterModule::handleReceivedProtobuf(const meshtastic_MeshPacket &mp, meshtastic_BitmapPayload *decoded)
{
    // If we have a valid decoded payload
    if (decoded) {
        // Extract width and height from the packed uint32
        uint8_t width = decoded->width_height & 0xFF;         // Lower 8 bits
        uint8_t height = (decoded->width_height >> 8) & 0xFF; // Upper 8 bits

        LOG_INFO("Received bitmap image: %dx%d, data size: %d bytes", width, height, mp.decoded.payload.size);

        // Notify observers about the new bitmap message
        notifyObservers(&mp);

        return true; // We've handled this message
    }

    return false; // Let other modules try to handle it
}
