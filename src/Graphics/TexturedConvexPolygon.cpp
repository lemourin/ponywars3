#include "TexturedConvexPolygon.hpp"
#include "Utility/Window.hpp"

TexturedConvexPolygon::TexturedConvexPolygon(SceneGraph::Item *parent):
    TexturedPolygon(parent) {
}

/*QSGNode* TexturedConvexPolygon::updatePaintNode(QSGNode* old, UpdatePaintNodeData*) {
    Node* node = static_cast<Node*>(old);

    if (!node) {
        node = new Node(vertices(),
                        textureScale());
    }

    node->setMatrix(matrix());
    node->setTexture(texture());

    return node;
}*/

TexturedConvexPolygon::Node::Node(const std::vector<QPointF>& pts,
                                  QVector2D scale):
    m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), pts.size()) {

    m_geometryNode.setGeometry(&m_geometry);
    m_geometryNode.setMaterial(&m_material);
    appendChildNode(&m_geometryNode);

    m_geometry.setVertexDataPattern(QSGGeometry::StaticPattern);
    m_geometry.setDrawingMode(GL_TRIANGLE_FAN);

    QSGGeometry::TexturedPoint2D* array = m_geometry.vertexDataAsTexturedPoint2D();
    for (size_t i=0; i<pts.size(); i++)
        array[i].set(pts[i].x(), pts[i].y(),
                     pts[i].x()*scale.x(), pts[i].y()*scale.y());

    m_material.setFiltering(QSGTexture::Linear);
    m_material.setMipmapFiltering(QSGTexture::Linear);
    m_material.setHorizontalWrapMode(QSGTexture::Repeat);
    m_material.setVerticalWrapMode(QSGTexture::Repeat);
}

void TexturedConvexPolygon::Node::setTexture(QSGTexture* t) {
    m_material.setTexture(t);
}
