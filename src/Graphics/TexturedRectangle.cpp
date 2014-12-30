#include "TexturedRectangle.hpp"
#include "Utility/Window.hpp"
#include <QQuickWindow>
#include <QSGTextureProvider>
#include <cassert>

TexturedRectangle::TexturedRectangle(SceneGraph::Item* parent):
    TexturedItem(parent),
    m_textureRect(0, 0, 1, 1) {
}

void TexturedRectangle::setTextureRect(QRectF rect) {
    m_textureRect = rect;

    //if (flags() & ItemHasContents)
    //    update();
}

/*QSGNode* TexturedRectangle::updatePaintNode(QSGNode* old,
                                            UpdatePaintNodeData*) {
    Node* node = static_cast<Node*>(old);

    if (!node)
        node = new Node;

    node->synchronize(this);

    return node;
}*/

TexturedRectangle::Node::Node():
    m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4) {
    m_geometryNode.setGeometry(&m_geometry);
    m_geometryNode.setMaterial(&m_material);
    appendChildNode(&m_geometryNode);

    m_geometry.setVertexDataPattern(QSGGeometry::StaticPattern);
    m_geometry.setDrawingMode(GL_TRIANGLE_STRIP);
}

void TexturedRectangle::Node::synchronize(const TexturedRectangle* item) {
    updateMaterial(item);
    updateGeometry(item);

    setMatrix(item->matrix());
}

void TexturedRectangle::Node::updateMaterial(const TexturedRectangle* item) {
    m_material.setTexture(item->texture());
    m_material.setFiltering(QSGTexture::Linear);
    m_material.setMipmapFiltering(QSGTexture::Linear);
    m_material.setHorizontalWrapMode(QSGTexture::Repeat);
    m_material.setVerticalWrapMode(QSGTexture::Repeat);

    markDirty(DirtyMaterial);
}

void TexturedRectangle::Node::updateGeometry(const TexturedRectangle* item) {
    /*QSGGeometry::updateTexturedRectGeometry(&m_geometry,
                                            item->boundingRect(),
                                            item->textureRect());*/
    m_geometry.markVertexDataDirty();
    markDirty(DirtyGeometry);
}
