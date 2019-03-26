#include "layerdrawer.h"

LayerDrawer::LayerDrawer(const QColor & color, const QColor & highLightColor, int layerIndex):
    m_color(color),
    m_highLightColor(highLightColor),
    m_layerIndex(layerIndex)
{

}

LayerDrawer::~LayerDrawer()
{

}

int LayerDrawer::layerIndex() const
{
    return m_layerIndex;
}
