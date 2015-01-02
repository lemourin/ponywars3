#include "AddRectangle.hpp"
#include "AddBody.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include <QSGSimpleRectNode>
#include <QSGFlatColorMaterial>

AddRectangle::AddRectangle(AddBody* p): AddFixture(p), m_state() {
    //setAcceptedMouseButtons(Qt::LeftButton);
    //setAcceptHoverEvents(true);
    //setFlag(ItemHasContents);
}

void AddRectangle::reset() {
    m_state = Reset;

    update();
}

QFixture* AddRectangle::fixture() const {
    Box2DBox* box = new Box2DBox;

    box->rmatrix().translate(std::min(m_p1.x(), m_p2.x()),
                             std::min(m_p1.y(), m_p2.y()));

    box->setSize(QSizeF(std::abs(m_p1.x()-m_p2.x()),
                        std::abs(m_p1.y()-m_p2.y())));

    return box;
}

void AddRectangle::mousePressEvent(QMouseEvent *) {
}

void AddRectangle::mouseReleaseEvent(QMouseEvent *event) {
    if (!(m_state & SetFirst)) {
        m_p1 = event->localPos();
        m_state |= SetFirst;
    }
    else if (!(m_state & SetSecond)) {
        m_p2 = event->localPos();
        m_state |= SetSecond;
        emit finished();
    }
}

/*void AddRectangle::hoverMoveEvent(QHoverEvent *event) {
    if ((m_state & SetFirst) && !(m_state & SetSecond)) {
        m_cursor = event->posF();
        m_state |= MovedCursor;
        update();
    }
}*/

/*QSGNode* AddRectangle::updatePaintNode(QSGNode* n, UpdatePaintNodeData *) {
    QSGSimpleRectNode* node = static_cast<QSGSimpleRectNode*>(n);

    if (!node) {
        node = new QSGSimpleRectNode;
        node->setMaterial(new QSGFlatColorMaterial);
        node->setFlag(QSGNode::OwnsMaterial);
    }

    if (m_state & MovedCursor) {
        m_state ^= MovedCursor;
        node->setRect(QRectF(m_p1, m_cursor));
    }

    if (m_state & Reset) {
        m_state ^= Reset;
        delete node;
        node = nullptr;
    }

    return node;
}*/

