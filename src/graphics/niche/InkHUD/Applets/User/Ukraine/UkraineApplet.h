#ifdef MESHTASTIC_INCLUDE_INKHUD

/*

Extend Positions Applet and filter for Ukraine bounding box
*/

#pragma once

#include "graphics/niche/InkHUD/Applets/User/Positions/PositionsApplet.h"

namespace NicheGraphics::InkHUD
{

class UkraineApplet : public PositionsApplet
{
  public:
    void onRender(bool full);
    // virtual void getMapCenter(float *lat, float *lng);
    // virtual void getMapSize(uint32_t *widthMeters, uint32_t *heightMeters)
  protected:
    bool shouldDrawNode(meshtastic_NodeInfoLite *node);
};

} // namespace NicheGraphics::InkHUD

#endif