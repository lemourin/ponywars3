#include "Shader.hpp"
#include <cassert>

namespace SceneGraph {

Shader::Shader() : m_initialized() {}

void Shader::initialize() {
  program()->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader());
  program()->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader());

  assert(program()->link());

  int id = 0;
  for (const std::string& name : attribute()) {
    m_attributeLocation[id] = program()->attributeLocation(name.c_str());
    assert(m_attributeLocation[id] != -1);
    id++;
  }

  m_initialized = true;
}
}
