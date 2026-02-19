#ifdef MESHTASTIC_INCLUDE_INKHUD

/*

Displays the latest received bitmap image from the teleprinter module.

This applet listens for BitmapPayload messages and displays them on screen.

*/

#pragma once

#include "configuration.h"

#include "graphics/niche/InkHUD/Applet.h"

#include "modules/TeleprinterModule.h"

namespace NicheGraphics::InkHUD
{

class Applet;

class TeleprinterApplet : public Applet
{
  public:
    void onRender(bool full) override;

    void onActivate() override;
    void onDeactivate() override;

    // Callback for receiving bitmap messages
    int onReceiveBitmap(const meshtastic_MeshPacket *p);

    bool approveNotification(Notification &n) override; // Which notifications to suppress

  protected:
    // The latest received bitmap image data
    uint8_t *latestImageData = nullptr;
    size_t latestImageSize = 0;
    uint8_t latestWidth = 0;
    uint8_t latestHeight = 0;

    // Used to register our bitmap message callback
    CallbackObserver<TeleprinterApplet, const meshtastic_MeshPacket *> bitmapMessageObserver =
        CallbackObserver<TeleprinterApplet, const meshtastic_MeshPacket *>(this, &TeleprinterApplet::onReceiveBitmap);
};

} // namespace NicheGraphics::InkHUD

#endif
