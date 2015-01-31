#include "DeleteItem.hpp"
#include "Entities/World.hpp"
#include "QBox2D/QBody.hpp"

DeleteItem::DeleteItem(MapEditor* p): SubAction(p) {
    //setAcceptedMouseButtons(Qt::LeftButton);
}

void DeleteItem::mousePressEvent(QMouseEvent*) {
}

void DeleteItem::mouseReleaseEvent(QMouseEvent *event) {

    QBody* body = world()->bodyUnderPoint(event->localPos());
    if (!body)
        event->ignore();

    delete body;
}

