#include "AddPolygon.hpp"
#include "QBox2D/QWorld.hpp"
#include "Geometry/Functions.hpp"
#include "Graphics/Primitives.hpp"
#include "QBox2D/Fixture/Box2DPolygon.hpp"
#include "AddBody.hpp"
#include <QSGSimpleRectNode>

AddPolygon::AddPolygon(AddBody* parent):
    AddFixture(parent),
    m_object(this) {
    //setAcceptedMouseButtons(Qt::LeftButton);
    //setFlag(ItemHasContents);
}

void AddPolygon::reset() {
    m_pts.clear();
    update();
}

QFixture* AddPolygon::fixture() const {
    if (m_pts.size() < 3)
        return nullptr;
    Box2DPolygon* polygon = new Box2DPolygon;
    polygon->setVertices(m_pts);

    return polygon;
}

void AddPolygon::mousePressEvent(QMouseEvent*) {
}

void AddPolygon::mouseReleaseEvent(QMouseEvent* event) {
    event->accept();

    m_pts.push_back(event->localPos());

    if (m_pts.size() >= 3) {
        std::vector<Vector2d> vec;
        for (QPointF p: m_pts)
            vec.push_back(Vector2d(p));

        Geometry::convexHull(vec);

        m_pts.clear();
        for (Vector2d p: vec)
            m_pts.push_back(QPointF(p));
    }

    update();
}

/*QSGNode* AddPolygon::updatePaintNode(QSGNode* n, UpdatePaintNodeData*) {
    if (n)
        delete n;

    if (m_pts.size() < 3) {
        QSGNode* node = new QSGNode;

        for (QPointF p: m_pts) {
            QSGSimpleRectNode* rect = new QSGSimpleRectNode;
            rect->setFlag(QSGNode::OwnedByParent);
            rect->setRect(QRectF(p, QSizeF(1, 1)));
            node->appendChildNode(rect);
        }

        return node;
    }

    ConvexPolygonNode* node = new ConvexPolygonNode(m_pts);
    QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
    material->setColor("yellow");
    node->setMaterial(material);

    return node;
}*/

