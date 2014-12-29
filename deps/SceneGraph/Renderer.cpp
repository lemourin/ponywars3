#include "Renderer.hpp"
#include "Node.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include <cassert>
#include <QColor>
#include <QOpenGLTexture>

namespace SceneGraph {

Renderer::Renderer(): m_root() {
}

Renderer::~Renderer() {
    for (const auto& p: m_texture)
        delete p.second;
}

void Renderer::updateItem(Item* item) {
    if (item->m_itemNode == nullptr) {
        item->m_itemNode = new TransformNode;
    }

    if (item->m_state & Item::ModelMatrixChanged) {
        item->m_itemNode->setMatrix(item->matrix());
        item->m_state &= ~Item::ModelMatrixChanged;
    }

    if (item->m_state & Item::ParentChanged) {
        Node* current = item->m_itemNode;
        Node* parent = item->parent() ? item->parent()->m_itemNode : nullptr;
        if (current->parent() != parent) {
            if (current->parent())
                current->parent()->removeChild(current);
            if (parent)
                parent->appendChild(current);
        }

        item->m_state &= ~Item::ParentChanged;
    }

    Node* node = item->synchronize(nullptr);
    if (node != item->m_node) {
        delete item->m_node;
        item->m_node = node;

        if (node)
            item->m_itemNode->appendChild(node);
    }
}

void Renderer::updateNodes(Window* window) {
    if (!window->m_updateItem.empty())
        window->update();

    for (size_t i=0; i<window->m_updateItem.size(); i++) {
        Item* item = window->m_updateItem[i];
        item->m_state &= ~Item::ScheduledUpdate;
        updateItem(item);

        if (item->m_state & Item::ScheduledUpdate) {
            window->m_nextFrame.push_back(item);
            window->m_updateItem.pop_back();
        }
    }

    window->m_updateItem = window->m_nextFrame;
    if (window->m_updateItem.size() > 0) {
        window->scheduleSynchronize();
    }
}

void Renderer::destroyNodes(Window* window) {
    while (!window->m_destroyedItemNode.empty()) {
        Node* itemNode = window->m_destroyedItemNode.back();
        window->m_destroyedItemNode.pop_back();
        while (itemNode->firstChild())
            itemNode->removeChild(itemNode->firstChild());
        delete itemNode;
    }

    while (!window->m_destroyedNode.empty()) {
        Node* node = window->m_destroyedNode.back();
        window->m_destroyedNode.pop_back();
        delete node;
    }
}

void Renderer::render(Node* root) {
    RenderState state = m_state;

    if (root->type() == Node::Type::GeometryNode)
        renderGeometryNode(static_cast<GeometryNode*>(root));
    else if (root->type() == Node::Type::TransformNode) {
        TransformNode* node = static_cast<TransformNode*>(root);
        m_state.setMatrix(state.matrix()*node->matrix());
    }

    for (Node* node = root->firstChild(); node; node = node->next())
        render(node);

    m_state = state;
}

void Renderer::render() {
    render(m_root);
}

void Renderer::synchronize(Window* window) {
    if (m_size != window->size()) {
        setSize(window->size());
        window->update();
    }

    updateNodes(window);
    destroyNodes(window);
}

void Renderer::setSize(QSize size) {
    m_size = size;

    QMatrix4x4 matrix;
    matrix.perspective(45.0, (float)size.width()/size.height(), 0.1, 100);

    m_state.setMatrix(matrix);
}

void Renderer::setRoot(Item* item) {
    if (item == nullptr) {
        m_root = nullptr;
        return;
    }

    if (!item->m_itemNode)
        item->m_itemNode = new TransformNode;

    m_root = item->m_itemNode;
}

QOpenGLTexture* Renderer::texture(const char* path) {
    if (m_texture.find(path) == m_texture.end()) {
        QImage image(path);
        assert(!image.isNull());

        QOpenGLTexture* texture = new QOpenGLTexture(image);
        texture->setMinMagFilters(QOpenGLTexture::Linear,
                                  QOpenGLTexture::Linear);
        m_texture[path] = texture;

    }

    return m_texture[path];
}

}
