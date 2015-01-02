#include "Shader.hpp"

namespace SceneGraph {

Shader::Shader(): m_initialized() {
}

void Shader::initialize() {
    program()->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader());
    program()->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader());

    program()->link();

    int id = 0;
    for (const std::string& name: attribute()) {
        m_attributeLocation[id] = program()->attributeLocation(name.c_str());
        id++;
    }

    m_initialized = true;
}

}

