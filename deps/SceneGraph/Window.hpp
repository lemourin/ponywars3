#ifndef SCENEGRAPH_WINDOW_HPP
#define SCENEGRAPH_WINDOW_HPP
#include <QQuickView>
#include <QQuickItem>
#include "Item.hpp"

class QOpenGLTexture;

namespace SceneGraph {

class Renderer;

class Window: public QQuickView {
    private:
        friend class Item;
        friend class Renderer;

        Renderer* m_renderer;
        QMatrix4x4 m_projection;

        Item m_root;
        Item* m_focusItem;
        std::vector<Item*> m_updateItem;
        std::vector<Item*> m_nextFrame;
        std::vector<Node*> m_destroyedItemNode;
        std::vector<Node*> m_destroyedNode;

        void onSceneGraphInitialized();
        void onSceneGraphInvalidated();
        void onBeforeRendering();
        void onBeforeSynchronizing();
        void onItemDestroyed(Item*);

        void scheduleUpdate(Item*);
        void cancelUpdate(Item*);

    protected:
        void keyPressEvent(QKeyEvent *);
        void touchEvent(QTouchEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

    public:
        Window(QWindow* window = nullptr);
        ~Window();

        inline Item* rootItem() { return &m_root; }
        inline Item* focusItem() const { return m_focusItem; }

        inline const QMatrix4x4& projection() const { return m_projection; }
        void setProjection(const QMatrix4x4& m);

        inline void scheduleSynchronize() { contentItem()->update(); }

        QOpenGLTexture* texture(const char* path);
};

}

#endif // SCENEGRAPH_WINDOW_HPP
