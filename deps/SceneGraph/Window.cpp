#include "Window.hpp"
#include "DefaultRenderer.hpp"
#include "Node.hpp"
#include <QQuickItem>
#include <cassert>

namespace SceneGraph {

Window::Window(QWindow* parent):
    QQuickView(parent),
    m_renderer(),
    m_focusItem() {

    m_root.setWindow(this);

    connect(this, &QQuickWindow::sceneGraphInitialized,
            this, &Window::onSceneGraphInitialized, Qt::DirectConnection);
    connect(this, &QQuickWindow::sceneGraphInvalidated,
            this, &Window::onSceneGraphInvalidated, Qt::DirectConnection);
    connect(this, &QQuickWindow::beforeRendering,
            this, &Window::onBeforeRendering, Qt::DirectConnection);
    connect(this, &QQuickWindow::beforeSynchronizing,
            this, &Window::onBeforeSynchronizing, Qt::DirectConnection);
    contentItem()->setFlag(QQuickItem::ItemHasContents);

    setResizeMode(SizeRootObjectToView);
    setClearBeforeRendering(false);
    setPersistentSceneGraph(false);
}

Window::~Window() {
    m_root.setWindow(nullptr);
}

void Window::setProjection(const QMatrix4x4& m) {
    m_projection = m;
    scheduleSynchronize();
}

void Window::scheduleSynchronize() {
    contentItem()->update();
    update();
}

QOpenGLTexture* Window::texture(const char* path) {
    return m_renderer->texture(path);
}

void Window::onSceneGraphInitialized() {
    m_renderer = new DefaultRenderer;
    m_renderer->setRoot(&m_root);
}

void Window::onSceneGraphInvalidated() {
    m_renderer->setRoot(nullptr);

    delete m_renderer;
}

void Window::onBeforeRendering() {
    resetOpenGLState();
    m_renderer->render();
}

void Window::onBeforeSynchronizing() {
    m_renderer->synchronize(this);
}

void Window::onItemDestroyed(Item* item) {
    cancelUpdate(item);

    if (m_timerMap.find(item) != m_timerMap.end()) {
        std::unordered_set<int> timer = m_timerMap[item];
        for (int t: timer)
            removeTimer(item, t);
    }

    m_destroyedItemNode.push_back(item->m_itemNode);
    m_destroyedNode.push_back(item->m_node);

    item->m_itemNode = nullptr;
    item->m_node = nullptr;

    if (m_focusItem == item)
        m_focusItem = nullptr;

    scheduleSynchronize();
}

void Window::scheduleUpdate(Item* item) {
    if (!(item->m_state & Item::ScheduledUpdate)) {
        m_updateItem.push_back(item);
        item->m_state |= Item::ScheduledUpdate;
    }
}

void Window::cancelUpdate(Item* item) {
    if (item->m_state & Item::ScheduledUpdate) {
        auto it = std::find(m_updateItem.begin(), m_updateItem.end(), item);
        if (it != m_updateItem.end())
            m_updateItem.erase(it);
        it = std::find(m_nextFrame.begin(), m_nextFrame.end(), item);
        if (it != m_nextFrame.end())
            m_nextFrame.erase(it);

        item->m_state &= ~Item::ScheduledUpdate;
    }
}

int Window::installTimer(Item* item, int interval) {
    int id = startTimer(interval);
    m_timerMap[item].insert(id);
    m_timerItem[id] = item;

    return id;
}

void Window::removeTimer(Item* item, int timerId) {
    std::unordered_set<int>& set = m_timerMap[item];
    assert(set.find(timerId) != set.end());

    set.erase(set.find(timerId));
    killTimer(timerId);

    if (set.empty())
        m_timerMap.erase(m_timerMap.find(item));
}

void Window::keyPressEvent(QKeyEvent* event) {
    QQuickView::keyPressEvent(event);
    if (event->isAccepted())
        return;

    Item* item = focusItem();
    while (item) {
        item->keyPressEvent(event);
        if (event->isAccepted())
            break;
        item = item->parent();
    }
}

void Window::keyReleaseEvent(QKeyEvent* event) {
    QQuickView::keyReleaseEvent(event);
    if (event->isAccepted())
        return;

    Item* item = focusItem();
    while (item) {
        item->keyReleaseEvent(event);
        if (event->isAccepted())
            break;
        item = item->parent();
    }
}

void Window::touchEvent(QTouchEvent* event) {
    QQuickView::touchEvent(event);
    if (event->isAccepted())
        return;

    Item* item = focusItem();
    while (item) {
        item->touchEvent(event);
        if (event->isAccepted())
            break;
        item = item->parent();
    }
}

void Window::mousePressEvent(QMouseEvent* event) {
    QQuickView::mousePressEvent(event);
    if (event->isAccepted())
        return;

    /*while (activeFocusItem())
        activeFocusItem()->setFocus(false);
    contentItem()->setFocus(true); */
}

void Window::mouseMoveEvent(QMouseEvent* event) {
    QQuickView::mouseMoveEvent(event);
    if (event->isAccepted())
        return;

    Item* item = focusItem();
    while (item) {
        item->mouseMoveEvent(event);
        if (event->isAccepted())
            break;
        item = item->parent();
    }
}

void Window::timerEvent(QTimerEvent* event) {
    if (m_timerItem.find(event->timerId()) != m_timerItem.end()) {
        m_timerItem[event->timerId()]->timerEvent(event);
    }

    QQuickView::timerEvent(event);
}

}

