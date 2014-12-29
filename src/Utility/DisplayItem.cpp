#include "DisplayItem.hpp"
#include "BaseItem.hpp"
#include <QQuickWindow>

DisplayItem::DisplayItem(DisplayItemFrame* parent):
    QQuickItem(parent),
    m_frame(parent),
    m_focusedObject(),
    m_flickable(),
    m_distance() {

    setFlag(ItemIsFocusScope);
    setTransformOrigin(TopLeft);

    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);

    m_frame->m_displayItem = this;
}

DisplayItem::~DisplayItem() {
    if (m_focusedObject)
        m_focusedObject->m_displayItem = nullptr;
    if (m_frame)
        m_frame->m_displayItem = nullptr;
}

void DisplayItem::setScale(qreal f) {
    if (f != scale()) {
        QQuickItem::setScale(f);
        updateVisibleArea();
    }
}

void DisplayItem::setVisibleArea(QRectF rect) {
    if (m_visibleArea == rect)
        return;
    m_visibleArea = rect;
    visibleAreaChanged();
}

void DisplayItem::setEffectiveLookAt(QPointF p) {
    if (m_effectiveLookAt == p)
        return;
    m_effectiveLookAt = p;

    updateVisibleArea();
}

void DisplayItem::setLookAt(QPointF p) {
    if (m_lookAt == p)
        return;
    m_lookAt = p;

    returnLookAtToBounds();

}

void DisplayItem::setFocusedObject(BaseItem* item) {
    if (m_focusedObject == item)
        return;

    if (focusedObject()) {
        focusedObject()->m_displayItem = nullptr;
    }

    m_focusedObject = item;

    if (focusedObject()) {
        focusedObject()->m_displayItem = this;
        focusedObjectPositionChanged();
    }
}

void DisplayItem::setFlickable(bool enable) {
    m_flickable = enable;
}

void DisplayItem::updateVisibleArea() {
    returnScaleToBounds();
    returnLookAtToBounds();

    QPointF size(m_frame->width(), m_frame->height());
    setPosition(-scale()*effectiveLookAt()+0.5*size);

    setVisibleArea(mapRectFromItem(m_frame, QRectF(QPointF(), size)));
}

void DisplayItem::focusedObjectPositionChanged() {
    setLookAt(focusedObject()->position());
}

void DisplayItem::returnLookAtToBounds() {
    QPointF p = lookAt();
    p.rx() = std::max(p.x(), m_frame->width()/(2*scale()));
    p.rx() = std::min(p.x(), width()-m_frame->width()/(2*scale()));
    p.ry() = std::max(p.y(), m_frame->height()/(2*scale()));
    p.ry() = std::min(p.y(), height()-m_frame->height()/(2*scale()));

    setEffectiveLookAt(p);
}

void DisplayItem::returnScaleToBounds() {
    if (!qIsNull(width()) && !qIsNull(height())) {
        qreal aspect = std::max(m_frame->width()/width(),
                                m_frame->height()/height());
        setScale(std::max(scale(), aspect));
    }
}

void DisplayItem::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0)
        setScale(scale()*1.1);
    else
        setScale(scale()/1.1);
}

void DisplayItem::mousePressEvent(QMouseEvent* event) {
    if (!flickable()) {
        mousePressed(event->localPos());
        return;
    }

    m_startPoint = event->localPos();
}

void DisplayItem::mouseMoveEvent(QMouseEvent* event) {
    if (!flickable()) {
        mouseMoved(event->localPos());
        return;
    }

    setLookAt(effectiveLookAt()-event->localPos()+m_startPoint);
}

void DisplayItem::mouseReleaseEvent(QMouseEvent* event) {
    if (!flickable())
        mouseReleased(event->localPos());
}

void DisplayItem::hoverMoveEvent(QHoverEvent* event) {
    if (!flickable())
        mouseMoved(event->posF());
}

void DisplayItem::touchEvent(QTouchEvent* event) {
    if (event->touchPointStates() & Qt::TouchPointPressed) {
        if (event->touchPoints().size() == 1) {
            mousePressed(event->touchPoints().front().pos());
        }
        else if (event->touchPoints().size() == 2) {
            QPointF p1 = event->touchPoints()[0].pos();
            QPointF p2 = event->touchPoints()[1].pos();

            m_distance = QVector2D(p1-p2).length();
        }
    }

    if (event->touchPointStates() & Qt::TouchPointReleased) {
        if (event->touchPoints().size() == 1) {
            mouseReleased(event->touchPoints().front().pos());
        }
    }

    if (event->touchPoints().size() == 1) {
        if (flickable()) {
            QPointF p1 = event->touchPoints().front().lastPos();
            QPointF p2 = event->touchPoints().front().pos();

            setLookAt(effectiveLookAt()-p2+p1);
        }
        else {
            mouseMoved(event->touchPoints().front().pos());
        }
    }
    else if (event->touchPoints().size() == 2) {
        QPointF p1 = event->touchPoints()[0].pos();
        QPointF p2 = event->touchPoints()[1].pos();

        qreal distance = QVector2D(p1-p2).length();

        setScale(scale()*distance/m_distance);
    }
}

void DisplayItem::geometryChanged(const QRectF& newGeometry,
                                  const QRectF& oldGeometry) {
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    updateVisibleArea();
}

void DisplayItem::visibleAreaChanged() {
}

void DisplayItem::mouseMoved(QPointF) {
}

void DisplayItem::mousePressed(QPointF) {
}

void DisplayItem::mouseReleased(QPointF) {
}

DisplayItemFrame::DisplayItemFrame(Item*) {
}

DisplayItemFrame::~DisplayItemFrame() {
    if (m_displayItem)
        m_displayItem->m_frame = nullptr;
}

void DisplayItemFrame::geometryChanged(const QRectF& newGeometry,
                                       const QRectF& oldGeometry) {
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (m_displayItem)
        m_displayItem->updateVisibleArea();

}

