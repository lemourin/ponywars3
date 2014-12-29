#include "Triangle.hpp"
#include "Triangulate.hpp"
#include <QPointF>

Triangle::Triangle() {
    m_vertices.resize(3);
}

Triangle::Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3) {
    m_vertices.resize(3);
    m_vertices[0] = p1, m_vertices[1] = p2, m_vertices[2] = p3;
}

void Triangle::setPoint(int id, const QPointF &p) {
    m_vertices[id] = p;
}

const QPointF& Triangle::getPoint(int id) const {
    return m_vertices[id];
}

const QPointF& Triangle::operator[](int it) const {
    return m_vertices[it];
}

QPointF& Triangle::operator[](int it) {
    return m_vertices[it];
}

namespace Geometry {

void triangulate(const std::vector<QPointF> &polygon,
                 std::vector<Triangle> &result) {
    TrStuff::Vector2dVector vec, res;
    for (size_t i=0; i<polygon.size(); i++)
        vec.push_back(TrStuff::Vector2d(polygon[i].x(), polygon[i].y()));
    TrStuff::Triangulate::Process(vec, res);

    result.clear();
    for (size_t i=0; i<res.size(); i+=3) {
        Triangle triangle;
        triangle.setPoint(0, QPointF(res[i].GetX(), res[i].GetY()));
        triangle.setPoint(1, QPointF(res[i+1].GetX(), res[i+1].GetY()));
        triangle.setPoint(2, QPointF(res[i+2].GetX(), res[i+2].GetY()));

        result.push_back(triangle);
    }
}

}

