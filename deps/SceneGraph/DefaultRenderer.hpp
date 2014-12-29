#ifndef DEFAULTRENDERER_HPP
#define DEFAULTRENDERER_HPP
#include "Renderer.hpp"
#include <QOpenGLFunctions>

namespace SceneGraph {

class DefaultRenderer: public Renderer, public QOpenGLFunctions {
    protected:
        void renderGeometryNode(GeometryNode *node);

    public:
        DefaultRenderer();

        void render();

};

}

#endif // DEFAULTRENDERER_HPP
