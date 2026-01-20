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
  protected:
    bool shouldDrawNode(meshtastic_NodeInfoLite *node);
};

} // namespace NicheGraphics::InkHUD

#endif