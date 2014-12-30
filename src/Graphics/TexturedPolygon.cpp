#include "TexturedPolygon.hpp"
#include "Utility/Window.hpp"
#include "Geometry/Primitives.hpp"

TexturedPolygon::TexturedPolygon(SceneGraph::Item *parent):
    TexturedItem(parent),
    m_textureScale(1, 1) {
}

/*QSGNode* TexturedPolygon::updatePaintNode(QSGNode* old, UpdatePaintNodeData*) {
    Node* node = static_cast<Node*>(old);

    if (!node)
        node = new Node(vertices(), textureScale());

    node->setMatrix(matrix());
    node->setTexture(texture());

    return node;
}*/


TexturedPolygon::Node::Node(const std::vector<QPointF>& pts,
                            QVector2D scale):
    m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 3*(pts.size()-2)) {

    m_geometryNode.setGeometry(&m_geometry);
    m_geometryNode.setMaterial(&m_material);
    appendChildNode(&m_geometryNode);

    m_geometry.setVertexDataPattern(QSGGeometry::StaticPattern);
    m_geometry.setDrawingMode(GL_TRIANGLES);

    QSGGeometry::TexturedPoint2D* array = m_geometry.vertexDataAsTexturedPoint2D();
    std::vector<QPointF> p;
    Geometry::toTriangles(pts, p);
    for (size_t i=0; i<p.size(); i++)
        array[i].set(p[i].x(), p[i].y(), p[i].x()*scale.x(), p[i].y()*scale.y());

    m_material.setFiltering(QSGTexture::Linear);
    m_material.setMipmapFiltering(QSGTexture::Linear);
    m_material.setHorizontalWrapMode(QSGTexture::Repeat);
    m_material.setVerticalWrapMode(QSGTexture::Repeat);
}


void TexturedPolygon::Node::setTexture(QSGTexture* texture) {
    m_material.setTexture(texture);
}
