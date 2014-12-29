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
        parent()->removeChild(this);
        if (item)
            item->appendChild(this);
    }
}

QMatrix4x4& Item::matrix() {
    m_state = ModelMatrixChanged;
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

void Item::setFocus(bool enabled) {
    if (enabled)
        m_state |= HasFocus;
    else
        m_state &= ~HasFocus;
}

void Item::setVisible(bool enabled) {
    if (enabled)
        m_state |= Visible;
    else
        m_state &= ~Visible;
}

void Item::setWindow(Window* engine) {
    if (m_window == engine)
        return;
    if (m_window)
        m_window->onItemDestroyed(this);

    m_window = engine;

    update();

    if (engine) {
        if (focus())
            engine->m_focusItem = this;
    }

    for (Item* item = firstChild(); item; item = item->next())
        item->setWindow(engine);
}

Node* Item::synchronize(Node*) {
    return nullptr;
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
    if (window()) {
        window()->scheduleUpdate(this);
        window()->scheduleSynchronize();
    }
}

}
