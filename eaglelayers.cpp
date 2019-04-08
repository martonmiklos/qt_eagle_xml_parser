#include "eaglelayers.h"

EagleLayers::EagleLayers()
{

}

EagleLayers::PCBLayers EagleLayers::oppositeLayer(const EagleLayers::PCBLayers layer)
{
    if (Top < layer && layer < Route8)
        return static_cast<PCBLayers>(Bottom - (layer - 1));
    if (Route7 < layer && layer < Bottom)
        return static_cast<PCBLayers>(Top + (layer - Bottom));

    switch (layer) {
    case EagleLayers::Top: return Bottom;
    case EagleLayers::Bottom: return Top;
    case EagleLayers::tPlace: return bPlace;
    case EagleLayers::bPlace: return tPlace;
    case EagleLayers::tOrigins: return bOrigins;
    case EagleLayers::bOrigins: return tOrigins;
    case EagleLayers::tNames: return bNames;
    case EagleLayers::bNames: return tNames;
    case EagleLayers::tValues: return bValues;
    case EagleLayers::bValues: return tValues;
    case EagleLayers::tStop: return bStop;
    case EagleLayers::bStop: return tStop;
    case EagleLayers::tCream: return bCream;
    case EagleLayers::bCream: return tCream;
    case EagleLayers::tFinish: return bFinish;
    case EagleLayers::bFinish: return tFinish;
    case EagleLayers::tGlue: return bGlue;
    case EagleLayers::bGlue: return tGlue;
    case EagleLayers::tTest: return bTest;
    case EagleLayers::bTest: return tTest;
    case EagleLayers::tKeepout: return bKeepout;
    case EagleLayers::bKeepout: return tKeepout;
    case EagleLayers::tRestrict: return bRestrict;
    case EagleLayers::bRestrict: return tRestrict;
    case EagleLayers::tDocu: return bDocu;
    case EagleLayers::bDocu: return tDocu;
    default: return layer;
    }
    return layer;
}
