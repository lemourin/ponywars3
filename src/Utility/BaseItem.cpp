#include "BaseItem.hpp"
#include "DisplayItem.hpp"

BaseItem::BaseItem(Item* parent):
    SceneGraph::Item(parent),
    m_displayItem() {
}

BaseItem::~BaseItem() {
    if (displayItem())
        displayItem()->setFocusedObject(nullptr);
}

void BaseItem::geometryChanged(const QRectF& newGeometry,
                               const QRectF& oldGeometry) {
    //QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry.topLeft() != oldGeometry.topLeft() && displayItem())
        displayItem()->focusedObjectPositionChanged();

}
