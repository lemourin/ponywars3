#include "Material.hpp"
#include "Renderer.hpp"
#include <cassert>

namespace SceneGraph {

Material::Material() {
}

void ColorMaterial::ColorShader::initialize() {
    Shader::initialize();

    m_matrix = program()->uniformLocation("matrix");
    m_color = program()->uniformLocation("color");
}

const char* ColorMaterial::ColorShader::vertexShader() const {
    return GLSL(
        attribute vec3 position;
        uniform mat4 matrix;
        void main() {
            gl_Position = matrix*vec4(position, 1.0);
        }
    );
}

const char* ColorMaterial::ColorShader::fragmentShader() const {
    return GLSL(
        uniform vec4 color;
        void main() {
            gl_FragColor = color;
        }
    );
}

void ColorMaterial::ColorShader::updateState(const Material* m,
                                             const RenderState& state) {
    const ColorMaterial* data = static_cast<const ColorMaterial*>(m);

    program()->setUniformValue(m_matrix, state.matrix());
    program()->setUniformValue(m_color, data->m_color);
}


void TextureMaterial::TextureShader::initialize() {
    Shader::initialize();

    initializeOpenGLFunctions();
    m_matrix = program()->uniformLocation("matrix");
    m_texture = program()->uniformLocation("texture");
}

const char* TextureMaterial::TextureShader::vertexShader() const {
    return GLSL(
        attribute vec3 position;
        attribute vec2 tcoord;
        uniform mat4 matrix;
        varying vec2 texcoord;

        void main() {
            texcoord = tcoord;
            gl_Position = matrix*vec4(position, 1.0);
        }
    );
}

const char* TextureMaterial::TextureShader::fragmentShader() const {
    return GLSL(
        uniform sampler2D texture;
        varying vec2 texcoord;

        void main() {
            gl_FragColor = texture2D(texture, texcoord);
        }
    );
}

void TextureMaterial::TextureShader::updateState(const Material* material,
                                                 const RenderState& state) {
    const TextureMaterial* m = static_cast<const TextureMaterial*>(material);

    program()->setUniformValue(m_matrix, state.matrix());

    assert(m->m_texture);
    m->m_texture->bind(0);
    program()->setUniformValue(m_texture, 0);

}

void VertexColorMaterial::VertexColorShader::initialize() {
    Shader::initialize();

    m_matrix = program()->uniformLocation("matrix");
}

const char* VertexColorMaterial::VertexColorShader::vertexShader() const {
    return GLSL(
        attribute vec3 position;
        attribute vec4 color;
        uniform mat4 matrix;
        varying vec4 fcolor;

        void main() {
            fcolor = color;
            gl_Position = matrix*vec4(position, 1.0);
        }
    );
}

const char* VertexColorMaterial::VertexColorShader::fragmentShader() const {
    return GLSL(
        varying vec4 fcolor;

        void main() {
            gl_FragColor = fcolor;
        }
    );
}

void VertexColorMaterial::VertexColorShader::updateState(const Material*,
                                                         const RenderState& state) {
    program()->setUniformValue(m_matrix, state.matrix());
}

TextureMaterial::TextureMaterial(): m_texture() {

}

}
