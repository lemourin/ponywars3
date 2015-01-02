#include "AddChain.hpp"
#include "QBox2D/QWorld.hpp"
#include "Entities/World.hpp"
#include "Entities/Chain.hpp"
#include "Utility/Utility.hpp"
#include <QSGSimpleRectNode>
#include <QSGFlatColorMaterial>
#include <QCursor>
#include <QQuickWindow>

AddChain::AddChain(MapEditor* p):
    MapEditorAction(p),
    m_state() {

    //setAcceptedMouseButtons(Qt::LeftButton);
    //setAcceptHoverEvents(true);

    //setFlag(ItemHasContents);
}

void AddChain::reset() {
    m_state = 0;
    m_pts.clear();
    update();
}

void AddChain::mousePressEvent(QMouseEvent*) {
}

void AddChain::mouseReleaseEvent(QMouseEvent *event) {
    m_pts.push_back(event->localPos());
    m_state |= DirtyState::Points;
    update();
}

/*void AddChain::hoverMoveEvent(QHoverEvent* event) {
    m_mousePos = event->posF();

    m_state |= DirtyState::MousePos;
    update();

    Action::hoverMoveEvent(event);
}*/

void AddChain::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return) {

        Chain* chain = new Chain(world());
        chain->setVertices(m_pts);
        chain->initialize(world());

        m_state |= DirtyState::Finished;
        update();

        emit finished();
    }
}

/*QSGNode* AddChain::updatePaintNode(QSGNode* n, UpdatePaintNodeData*) {
    if (pts().size() == 0) {
        delete n;
        return nullptr;
    }

    Node* node = static_cast<Node*>(n);
    if (!node)
        node = new Node;

    if (m_state & DirtyState::Points) {
        node->setGeometry(node->createGeometry(pts()));
        m_state ^= DirtyState::Points;
    }

    if (m_state & DirtyState::MousePos) {
        QSGGeometryNode* line = (QSGGeometryNode*)(node->firstChild());

        if (!line) {
            line = new QSGGeometryNode;
            line->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);
            line->setFlag(QSGNode::OwnedByParent);
            line->setMaterial(new QSGFlatColorMaterial);
        }

        QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        geometry->setDrawingMode(GL_LINES);
        geometry->vertexDataAsPoint2D()[0].set(pts().back().x(),
                                               pts().back().y());

        geometry->vertexDataAsPoint2D()[1].set(m_mousePos.x(),
                                               m_mousePos.y());
        line->setGeometry(geometry);

        if (!line->parent())
            node->appendChildNode(line);

        m_state ^= DirtyState::MousePos;
    }

    if (m_state & DirtyState::Finished) {
        delete node;
        node = nullptr;

        m_pts.clear();
        m_state ^= DirtyState::Finished;
    }

    return node;
}*/

AddChain::Node::Node() {
    setFlags(OwnsGeometry | OwnsMaterial);
    setMaterial(new QSGFlatColorMaterial);
}

QSGGeometry* AddChain::Node::createGeometry(const std::vector<QPointF>& pts) {
    QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),
                                            pts.size());
    geometry->setDrawingMode(GL_LINE_STRIP);
    geometry->setLineWidth(2);

    for (size_t i = 0; i < pts.size(); i++)
        geometry->vertexDataAsPoint2D()[i].set(pts[i].x(), pts[i].y());

    return geometry;
}
