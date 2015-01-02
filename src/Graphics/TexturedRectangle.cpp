#include "TexturedRectangle.hpp"
#include "Utility/Window.hpp"
#include <QQuickWindow>
#include <cassert>

TexturedRectangle::TexturedRectangle(SceneGraph::Item* parent):
    TexturedItem(parent),
    m_textureRect(0, 0, 1, 1) {
}

void TexturedRectangle::setTextureRect(QRectF rect) {
    if (m_textureRect != rect) {
        m_textureRect = rect;
        update();
    }
}

SceneGraph::Node* TexturedRectangle::synchronize(SceneGraph::Node* old) {
    Node* node = static_cast<Node*>(old);
    if (!node)
        node = new Node;
    node->synchronize(this);
    //qDebug() << "shake" << old << node->parent() << this;
    //qDebug() << "wa" << effectiveMatrix() * QPointF(0, 0);
    //update();
    return node;
}

TexturedRectangle::Node::Node():
    m_geometry({ { 2, GL_FLOAT }, { 2, GL_FLOAT } }, 4, sizeof(Vertex)) {
    m_geometryNode.setGeometry(&m_geometry);
    m_geometryNode.setMaterial(&m_material);
    appendChild(&m_geometryNode);

    //m_geometry.setVertexDataPattern(QSGGeometry::StaticPattern);
    m_geometry.setDrawingMode(GL_TRIANGLE_STRIP);
}

void TexturedRectangle::Node::synchronize(const TexturedRectangle* item) {
    updateMaterial(item);
    updateGeometry(item);
}

void TexturedRectangle::Node::updateMaterial(const TexturedRectangle* item) {
    m_material.setTexture(item->texture());
    //m_material.setFiltering(QSGTexture::Linear);
    //m_material.setMipmapFiltering(QSGTexture::Linear);
    //m_material.setHorizontalWrapMode(QSGTexture::Repeat);
    //m_material.setVerticalWrapMode(QSGTexture::Repeat);

    //markDirty(DirtyMaterial);
}

void TexturedRectangle::Node::updateGeometry(const TexturedRectangle* item) {
    QRectF trect = item->textureRect();
    Vertex* array = m_geometry.vertexData<Vertex>();

    array[0].set(0, 0, trect.left(), trect.top());
    array[1].set(1, 0, trect.right(), trect.top());
    array[2].set(0, 1, trect.left(), trect.bottom());
    array[3].set(1, 1, trect.right(), trect.bottom());
    m_geometry.updateVertexData();

    //m_geometry.markVertexDataDirty();
    //markDirty(DirtyGeometry);
}

void TexturedRectangle::Node::Vertex::set(float x, float y, float tx, float ty) {
    this->x = x;
    this->y = y;
    this->tx = tx;
    this->ty = ty;
}
