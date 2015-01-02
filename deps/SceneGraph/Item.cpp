#include "Item.hpp"
#include "Window.hpp"
#include "Node.hpp"
#include "Renderer.hpp"
#include <cassert>

namespace SceneGraph {

Item::Item(Item* parent):
    BaseObject(parent),
    m_window(),
    m_itemNode(),
    m_node(),
    m_state(ParentChanged | Visible) {

    if (parent && parent->window()) {
        setWindow(parent->window());
    }
}

Item::~Item() {
    setWindow(nullptr);
}

Item* Item::firstChild() const {
    return static_cast<Item*>(BaseObject::firstChild());
}

Item* Item::next() const {
    return static_cast<Item*>(BaseObject::next());
}

Item* Item::parent() const {
    return static_cast<Item*>(BaseObject::parent());
}

void Item::setParent(Item* item) {
    if (parent() != item) {
        if (parent())
            parent()->removeChild(this);
        if (item)
            item->appendChild(this);
    }
}

QMatrix4x4& Item::rmatrix() {
    m_state |= ModelMatrixChanged;
    update();

    return m_matrix;
}

void Item::appendChild(Item* item) {
    item->setWindow(window());

    item->m_state |= ParentChanged;
    update();

    BaseObject::appendChild(item);
}

void Item::removeChild(Item* item) {
    item->m_state |= ParentChanged;
    update();

    item->setWindow(nullptr);
    BaseObject::removeChild(item);
}

void Item::setMatrix(const QMatrix4x4& m) {
    m_matrix = m;
    m_state |= ModelMatrixChanged;

    update();
}

QMatrix4x4 Item::effectiveMatrix() const {
    QMatrix4x4 matrix;
    const Item* item = this;
    while (item) {
        matrix = item->matrix() * matrix;
        item = item->parent();
    }

    return window() ? window()->projection() * matrix : matrix;
}

QPointF Item::mapToItem(Item* item, QPointF p) {
    return item->effectiveMatrix() * mapToScreen(p);
}

QPointF Item::mapToScreen(QPointF p) {
    return effectiveMatrix().inverted() * p;
}

QPointF Item::mapFromItem(Item* item, QPointF p) {
    return item->mapToItem(this, p);
}

QPointF Item::mapFromScreen(QPointF p) {
    return effectiveMatrix() * p;
}

void Item::setFocus(bool enabled) {
    unsigned state = m_state;
    if (enabled)
        state |= HasFocus;
    else
        state &= ~HasFocus;

    if (m_state != state) {
        m_state = state;
        focusChanged();
    }
}

void Item::setVisible(bool enabled) {
    unsigned state = m_state;
    if (enabled)
        state |= Visible;
    else
        state &= ~Visible;

    if (m_state != state) {
        m_state = state;

        if (enabled)
            update();

        visibleChanged();
    }
}

void Item::setWindow(Window* window) {
    if (m_window == window)
        return;
    if (m_window)
        m_window->onItemDestroyed(this);

    m_window = window;
    update();

    if (window) {
        if (focus())
            window->m_focusItem = this;
    }

    for (Item* item = firstChild(); item; item = item->next())
        item->setWindow(window);
}

Node* Item::synchronize(Node*) {
    return nullptr;
}

void Item::visibleChanged() {
}

void Item::focusChanged() {
}

void Item::keyPressEvent(QKeyEvent* e) {
    e->ignore();
}

void Item::touchEvent(QTouchEvent* e) {
    e->ignore();
}

void Item::mouseMoveEvent(QMouseEvent* e) {
    e->ignore();
}

void Item::update() {
    if (window() && visible()) {
        window()->scheduleUpdate(this);
        window()->scheduleSynchronize();
    }
}

}
