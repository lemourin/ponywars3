#include "Primitives.hpp"
#include "Triangle.hpp"
#include "Circle.hpp"
#include "Functions.hpp"
#include <QSGGeometry>
#include <QSGFlatColorMaterial>

void Geometry::toTriangleStrip(std::vector<QPointF> convexPolygon,
                               std::vector<QPointF> &output) {
    output = std::vector<QPointF>(convexPolygon.size());

    size_t i = 1, j = convexPolygon.size()-1, index = 1;
    bool left = false;

    QPointF p0 = convexPolygon.front();
    output[0] = p0;

    while (index < (size_t)convexPolygon.size()) {
        if (left) {
            output[index] = convexPolygon[i];
            i++;
        }
        else {
            output[index] = convexPolygon[j];
            j--;
        }
        left = !left, index++;
    }
}

void Geometry::toTriangles(std::vector<QPointF> polygon,
                           std::vector<QPointF>& output) {
    std::vector<Triangle> triangles;
    Geometry::triangulate(polygon, triangles);

    output.resize(triangles.size()*3);
    uint id = 0;
    for (uint i=0; i<triangles.size(); i++)
        for (QPointF p: triangles[i])
            output[id++] = p;

}

ConvexPolygonGeometry::ConvexPolygonGeometry(const std::vector<QPointF>& pts):
    QSGGeometry(defaultAttributes_Point2D(), pts.size()) {
    setVertexDataPattern(StaticPattern);
    setDrawingMode(GL_TRIANGLE_FAN);
    for (size_t i=0; i<pts.size(); i++)
        vertexDataAsPoint2D()[i].set(pts[i].x(), pts[i].y());
}

ConvexPolygonNode::ConvexPolygonNode(const std::vector<QPointF>& pts):
    m_geometry(pts) {

    setGeometry(&m_geometry);
    setMaterial(&m_material);
}

void ConvexPolygonNode::setColor(QColor color) {
    m_material.setColor(color);
}

PolygonGeometry::PolygonGeometry(const std::vector<QPointF>& pts):
    QSGGeometry(defaultAttributes_Point2D(), 3*(pts.size()-2)) {

    std::vector<Triangle> triangles;
    Geometry::triangulate(pts, triangles);

    setVertexDataPattern(StaticPattern);
    setDrawingMode(GL_TRIANGLES);

    int index = 0;
    for (const Triangle& tri: triangles)
        for (const QPointF& p: tri)
            vertexDataAsPoint2D()[index++].set(p.x(), p.y());

}

PolygonNode::PolygonNode(const std::vector<QPointF>& pts):
    m_geometry(pts) {

    setGeometry(&m_geometry);
    setMaterial(&m_material);
}

void PolygonNode::setColor(QColor color) {
    m_material.setColor(color);
}

EdgeNode::EdgeNode(): EdgeNode(QPointF(), QPointF()) {
}

EdgeNode::EdgeNode(QPointF p1, QPointF p2):
    m_geometry(QSGGeometry::defaultAttributes_Point2D(), 2) {

    setGeometry(&m_geometry);
    setMaterial(&m_material);

    m_geometry.setDrawingMode(GL_LINES);
    m_geometry.setVertexDataPattern(QSGGeometry::StaticPattern);

    setP1(p1);
    setP2(p2);
}

void EdgeNode::setP1(QPointF p1) {
    m_geometry.vertexDataAsPoint2D()[0].set(p1.x(), p1.y());
}

void EdgeNode::setP2(QPointF p2) {
    m_geometry.vertexDataAsPoint2D()[1].set(p2.x(), p2.y());
}

void EdgeNode::setColor(QColor color) {
    m_material.setColor(color);
}

CircleNode::CircleNode(QPointF pos, float radius, int accuracy) {
    std::vector<QPointF> points(accuracy);

    for (int i=0; i<accuracy; i++) {
        qreal angle = i*2*M_PI/accuracy;
        qreal x = cos(angle)*radius+pos.x();
        qreal y = sin(angle)*radius+pos.y();

        points[i] = {x, y};
    }

    setGeometry(m_geometry = new ConvexPolygonGeometry(points));
    setMaterial(&m_material);
}

CircleNode::CircleNode(const Circle &circle, int accuracy):
    CircleNode((QPointF)circle.pos(), circle.radius(), accuracy) {
}

void CircleNode::setColor(QColor color) {
    m_material.setColor(color);
}

CircleNode::~CircleNode() {
    delete m_geometry;
}

