#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <QMatrix4x4>
#include <unordered_map>
#include <vector>

class QOpenGLTexture;

namespace SceneGraph {

class Node;
class GeometryNode;
class Item;
class Window;

class RenderState {
    private:
        friend class Renderer;

        QMatrix4x4 m_matrix;

        inline void setMatrix(QMatrix4x4 m) { m_matrix = m; }

    public:
        inline const QMatrix4x4& matrix() const { return m_matrix; }
};

class Renderer {
    private:
        Node* m_root;
        RenderState m_state;
        QSize m_size;
        std::unordered_map< std::string, QOpenGLTexture* > m_texture;

        void updateItem(Item*);
        void updateNodes(Window*);
        void destroyNodes(Window*);

        void render(Node*);

    protected:
        virtual void renderGeometryNode(GeometryNode* node) = 0;

    public:
        Renderer();
        virtual ~Renderer();

        void initialize();
        void destroy();

        virtual void render();

        void synchronize(Window* window);

        void setSize(QSize);
        void setRoot(Item*);

        inline const RenderState& renderState() const { return m_state; }
        inline Node* root() const { return m_root; }

        QOpenGLTexture* texture(const char* path);
};

}

#endif // RENDERER_HPP
