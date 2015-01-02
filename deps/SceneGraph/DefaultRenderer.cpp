#include "DefaultRenderer.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Node.hpp"
#include "Geometry.hpp"
#include <cassert>

namespace SceneGraph {

DefaultRenderer::DefaultRenderer(): Renderer() {
    initializeOpenGLFunctions();
}

void DefaultRenderer::renderGeometryNode(GeometryNode* node) {
    Material* material = node->material();
    assert(material);
    Shader* shader = material->shader();
    assert(shader);

    if (!shader->initialized())
        shader->initialize();

    shader->bind();
    shader->activate();
    shader->updateState(material, renderState());

    Geometry* g = node->geometry();
    assert(g);

    g->bind(shader->attributeLocation());
    if (g->indexCount())
        glDrawElements(g->drawingMode(), g->indexCount(), g->indexType(), g->indexData());
    else
        glDrawArrays(g->drawingMode(), 0, g->vertexCount());

    shader->deactivate();
}

void DefaultRenderer::render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    Renderer::render();
}

}
