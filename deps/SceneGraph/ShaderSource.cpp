#include "ShaderSource.hpp"
#include "Renderer.hpp"
#include <cassert>
#include <QTime>

namespace SceneGraph {

ShaderSource::ShaderSource(Item* parent):
    SceneGraph::Item(parent),
    m_sourceItem(this),
    m_sourceRect(-1, -1, 2, 2),
    m_background(Qt::white),
    m_node() {

    m_sourceItem.setVisible(false);

    setBackground(Qt::transparent);
}

ShaderSource::~ShaderSource() {
}

void ShaderSource::setSourceRect(QRectF rect) {
    m_sourceRect = rect;
    update();
}

void ShaderSource::setTextureSize(QSize size) {
    m_textureSize = size;
    update();
}

void ShaderSource::setBackground(QColor color) {
    m_background = color;
    update();
}

Node* ShaderSource::synchronize(Node *old) {
    ShaderNode* node = static_cast<ShaderNode*>(old);

    if (!node) {
        node = new ShaderNode(textureSize());
        m_node = node;
    }

    node->update(this);

    return node;
}

ShaderSource::ShaderNode::ShaderNode(QSize size, Node* parent):
    Node(parent),
    m_fbo(new QOpenGLFramebufferObject(size)),
    m_capturedNode() {
    initializeOpenGLFunctions();
}

ShaderSource::ShaderNode::~ShaderNode() {
    delete m_fbo;
}

void ShaderSource::ShaderNode::updateTexture() {
    m_fbo->bind();

    assert(renderer());
    if (m_capturedNode) {
        glClearColor(m_background.redF(),
                     m_background.greenF(),
                     m_background.blueF(),
                     m_background.alphaF());
        glClear(GL_COLOR_BUFFER_BIT);

        QMatrix4x4 matrix;
        matrix.ortho(m_viewport.left(),
                     m_viewport.right(),
                     m_viewport.top(),
                     m_viewport.bottom(),
                     -1, 1);
        renderer()->render(m_capturedNode, RenderState(matrix));
    }

    m_fbo->release();
}

void ShaderSource::ShaderNode::update(ShaderSource *i) {
    m_capturedNode = i->m_sourceItem.m_itemNode;
    m_background = i->m_background;
    m_viewport = i->m_sourceRect;

    if (m_fbo->size() != i->m_textureSize) {
        delete m_fbo;
        m_fbo = new QOpenGLFramebufferObject(i->m_textureSize);
    }
}

}
