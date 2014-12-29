#include "Shader.hpp"

namespace SceneGraph {

Shader::Shader(): m_initialized() {
}

void Shader::initialize() {
    program()->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader());
    program()->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader());

    program()->link();

    m_initialized = true;
}

}

