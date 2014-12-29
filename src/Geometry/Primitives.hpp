#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include <QSGGeometryNode>
#include <QPointF>
#include <vector>
#include <QSGFlatColorMaterial>

class Circle;

namespace Geometry {

void toTriangleStrip(std::vector<QPointF> convexPolygon,
                     std::vector<QPointF>& output);
void toTriangles(std::vector<QPointF> polygon,
                 std::vector<QPointF>& output);

}

class ConvexPolygonGeometry: public QSGGeometry  {
    private:

    public:
        ConvexPolygonGeometry(const std::vector<QPointF>& pts);
};

class ConvexPolygonNode: public QSGGeometryNode {
    private:
        ConvexPolygonGeometry m_geometry;
        QSGFlatColorMaterial m_material;

    public:
        ConvexPolygonNode(const std::vector<QPointF>& pts);
        void setColor(QColor);
};

class PolygonGeometry: public QSGGeometry {
    public:
        PolygonGeometry(const std::vector<QPointF>& pts);
};

class PolygonNode: public QSGGeometryNode {
    private:
        PolygonGeometry m_geometry;
        QSGFlatColorMaterial m_material;

    public:
        PolygonNode(const std::vector<QPointF>& pts);

        void setColor(QColor color);
}; 

class EdgeNode: public QSGGeometryNode {
    private:
        QSGGeometry m_geometry;
        QSGFlatColorMaterial m_material;

    public:
        EdgeNode();
        EdgeNode(QPointF p1, QPointF p2);

        void setP1(QPointF p1);
        void setP2(QPointF p2);

        void setColor(QColor color);
};

class CircleNode: public QSGGeometryNode {
    private:
        ConvexPolygonGeometry* m_geometry;
        QSGFlatColorMaterial m_material;

    public:
        CircleNode(QPointF pos, float radius, int accuracy = 32);
        CircleNode(const Circle& circle, int accuracy = 32);

        void setColor(QColor color);

        ~CircleNode();
};

#endif // PRIMITIVES_HPP
