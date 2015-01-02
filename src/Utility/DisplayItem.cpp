#include "DisplayItem.hpp"
#include "BaseItem.hpp"
#include "SceneGraph/Window.hpp"

DisplayItem::DisplayItem(DisplayItemFrame* parent):
    SceneGraph::Item(parent),
    m_frame(parent),
    m_focusedObject(),
    m_flickable(),
    m_distance(),
    m_scale(1),
    m_image(this) {

    m_image.setSource(":/resources/bricks.jpg");
    //m_image.rmatrix().translate(500, 500);
    //m_image.rmatrix().scale(10, 10);

    //setFlag(ItemIsFocusScope);
    //setTransformOrigin(TopLeft);

    //setAcceptedMouseButtons(Qt::LeftButton);
    //setAcceptHoverEvents(true);

    m_frame->m_displayItem = this;
}

DisplayItem::~DisplayItem() {
    if (m_focusedObject)
        m_focusedObject->m_displayItem = nullptr;
    if (m_frame)
        m_frame->m_displayItem = nullptr;
}

void DisplayItem::setSize(QSizeF size) {
    if (size != m_size) {
        m_size = size;
        updateVisibleArea();
    }
}

void DisplayItem::setScale(qreal f) {
    if (f != m_scale) {
        m_scale = f;
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

    rmatrix().setToIdentity();
    rmatrix().translate(0.5*m_frame->size().width(), 0.5*m_frame->size().height());
    rmatrix().scale(scale());
    rmatrix().translate(-effectiveLookAt().x(), -effectiveLookAt().y());

    QMatrix4x4 t = effectiveMatrix().inverted() * m_frame->effectiveMatrix();
    setVisibleArea(t.mapRect(QRectF(QPointF(), m_frame->size())));
}

void DisplayItem::focusedObjectPositionChanged() {
    setLookAt(-focusedObject()->mapToItem(this, QPointF()));
}

void DisplayItem::returnLookAtToBounds() {
    QPointF p = lookAt();
    qreal fwidth = m_frame->size().width();
    qreal fheight = m_frame->size().height();
    qreal width = size().width();
    qreal height = size().height();

    p.rx() = std::max(p.x(), fwidth/(2*scale()));
    p.rx() = std::min(p.x(), width-fwidth/(2*scale()));
    p.ry() = std::max(p.y(), fheight/(2*scale()));
    p.ry() = std::min(p.y(), height-fheight/(2*scale()));

    setEffectiveLookAt(p);
}

void DisplayItem::returnScaleToBounds() {
    qreal w = size().width();
    qreal h = size().height();

    if (!qIsNull(w) && !qIsNull(h)) {
        qreal aspect = std::max(m_frame->size().width()/w,
                                m_frame->size().height()/h);
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

void DisplayItem::visibleAreaChanged() {
}

void DisplayItem::mouseMoved(QPointF) {
}

void DisplayItem::mousePressed(QPointF) {
}

void DisplayItem::mouseReleased(QPointF) {
}

void DisplayItemFrame::sizeChanged() {
}

DisplayItemFrame::DisplayItemFrame(Item* parent):
    SceneGraph::Item(parent) {
}

DisplayItemFrame::~DisplayItemFrame() {
    if (m_displayItem)
        m_displayItem->m_frame = nullptr;
}

void DisplayItemFrame::setSize(QSizeF s) {
    if (m_size != s) {
        m_size = s;
        if (m_displayItem)
            m_displayItem->updateVisibleArea();
        sizeChanged();
    }
}

