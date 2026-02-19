#pragma once
#include "Observer.h"
#include "ProtobufModule.h"

/**
 * Teleprinter/MESHFAX module for sending bitmap images over the mesh.
 *
 * This module handles BitmapPayload messages which contain RLE-compressed images.
 */
class TeleprinterModule : public ProtobufModule<meshtastic_BitmapPayload>, public Observable<const meshtastic_MeshPacket *>
{
  public:
    /** Constructor */
    TeleprinterModule();

  protected:
    /**
     * Called to handle a particular incoming message with BitmapPayload data.
     *
     * @param mp The mesh packet containing the bitmap image
     * @param decoded The decoded BitmapPayload protobuf message
     * @return true if you've guaranteed you've handled this message and no other handlers should be considered for it.
     */
    virtual bool handleReceivedProtobuf(const meshtastic_MeshPacket &mp, meshtastic_BitmapPayload *decoded) override;

    /**
     * Called to make changes to a particular incoming message before it's processed.
     */
    virtual void alterReceivedProtobuf(meshtastic_MeshPacket &mp, meshtastic_BitmapPayload *decoded) override{};
};

extern TeleprinterModule *teleprinterModule;
