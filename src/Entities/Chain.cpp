#include "Chain.hpp"
#include "QBox2D/Fixture/Box2DChain.hpp"
#include "QBox2D/Fixture/Box2DEdge.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QFixture.hpp"
#include "Geometry/Primitives.hpp"
#include "Geometry/Circle.hpp"
#include "Geometry/Vector2d.hpp"
#include "Geometry/Functions.hpp"
#include "Player.hpp"
#include "QBox2D/QWorld.hpp"
#include "Entities/Gun.hpp"
#include <QSGFlatColorMaterial>
#include <QPainterPath>
#include <QPolygonF>
#include <QQuickWindow>

static b2Vec2 tob2Vec2(QPointF p) {
    return b2Vec2(p.x(), p.y());
}

Chain::Chain(Item* parent): QBody(parent) {
    //setFlag(ItemHasContents);
    //setZ(1);
}

Chain::~Chain() {
}

void Chain::setVertices(const std::vector<QPointF>& v) {
    m_vertices = v;
    createChain();
}

void Chain::cutCircle(Circle circle) {
    if (m_vertices.size() == 0)
        return;
    circle.setCenter(circle.pos());

    QPainterPath cr;
    cr.addEllipse(circle.x-circle.r, circle.y-circle.r,
                  2*circle.r, 2*circle.r);

    QPolygonF polygon;
    for (QPointF p: m_vertices)
        polygon.append(p);
    QPainterPath chain;
    chain.addPolygon(polygon);

    if (!chain.intersects(cr))
        return;

    chain = chain.subtracted(cr);

    for (const QPolygonF& poly: chain.toSubpathPolygons()) {
        std::vector<Vector2d> pts(poly.begin(), poly.end()-1);
        pts = Geometry::simplifyPolygon(pts, 0.2);

        //for (Vector2d& p: pts)
        //    p = Vector2d(mapFromItem(parentItem(), QPointF(p)));

        if (std::fabs(Geometry::area(pts.begin(), pts.end())) > 5.f) {
            std::vector<QPointF> list;
            for (Vector2d p: pts)
                list.push_back(QPointF(p.x, p.y));

            Chain* chain = new Chain(world());
            chain->setVertices(list);
            chain->initializeLater();
        }
    }

    m_vertices.clear();
    //deleteLater();
}

bool Chain::testPoint(const QPointF &point) const {
    std::vector<Vector2d> pts;
    for (QVariant v: m_vertices)
        pts.push_back((Vector2d)v.toPointF());

    return Geometry::pointInPolygon(pts.begin(), pts.end(), Vector2d(point));
}

void Chain::createChain() {
    std::vector<QPointF> pts;
    for (const QVariant& p: vertices())
        pts.push_back(p.toPointF());

    size_t it = pts.size()-1, prev = it-1, next = 1;
    for (size_t i=0; i<pts.size(); i++) {
        QPointF vec = pts[i]-pts[it];
        QPointF vecp = pts[it]-pts[prev], vecn = pts[next]-pts[i];

        Box2DEdge* fixture = new Box2DEdge(this);
        fixture->setVisible(false);
        fixture->setShadowCaster(false);
        addFixture(fixture);

        b2EdgeShape& edge = fixture->edgeShape();
        edge.Set(tob2Vec2(pts[it]), tob2Vec2(pts[i]));

        if (QPointF::dotProduct(vec, vecp) > 0) {
            edge.m_hasVertex0 = true;
            edge.m_vertex0 = tob2Vec2(pts[prev]);
        }
        if (QPointF::dotProduct(vec, vecn) > 0) {
            edge.m_hasVertex3 = true;
            edge.m_vertex3 = tob2Vec2(pts[next]);
        }

        prev = it, it = i;
        next = next+1 < pts.size() ? next+1 : 0;
    }
    Box2DChain* chain = new Box2DChain;
    chain->setVertices(pts);
    chain->setSensor(true);
    //chain->setFlag(ItemHasContents);

    addFixture(chain);
}

void Chain::beginContact(QFixture* other, b2Contact*) {
    //if (!qobject_cast<Bullet*>(other->body()))
    //    return;

    cutCircle(Circle(Vector2d(other->body()->worldCenter()), 5));
}

/*QSGNode* Chain::updatePaintNode(QSGNode* n, UpdatePaintNodeData*) {
    if (n)
        return n;

    PolygonNode* node = new PolygonNode(m_vertices);
    node->setColor(Qt::red);

    return node;
}*/
