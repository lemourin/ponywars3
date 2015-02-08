#include "DeleteItem.hpp"
#include "Entities/World.hpp"
#include "QBox2D/QBody.hpp"

DeleteItem::DeleteItem(MapEditor* p): SubAction(p), m_object(this) {
}

void DeleteItem::mousePressEvent(QMouseEvent*) {
}

void DeleteItem::mouseReleaseEvent(QMouseEvent *event) {

    QBody* body = world()->bodyUnderPoint(mapFromScreen(event->pos()));
    if (!body)
        event->ignore();

    body->destroyBody();
}

