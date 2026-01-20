#ifdef MESHTASTIC_INCLUDE_INKHUD

#include "./UkraineApplet.h"

using namespace NicheGraphics;

void InkHUD::UkraineApplet::onRender(bool full)
{
    drawHeader("Ukraine");
    PositionsApplet::onRender(full);
}

bool InkHUD::UkraineApplet::shouldDrawNode(meshtastic_NodeInfoLite *node)
{
    if (!node->has_position)
        return false;

    if (node->position.latitude_i > 524500000 or node->position.latitude_i < 442700000)
        return false;

    if (node->position.longitude_i > 400800000 or node->position.longitude_i < 221300000)
        return false;

    return true;
}

#endif
