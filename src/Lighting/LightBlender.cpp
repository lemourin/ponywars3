#include "LightBlender.hpp"
#include "LightSystem.hpp"
#include <QSGDynamicTexture>
#include <QTime>
#define GLSL(shader) #shader

LightBlender::LightBlender():
    m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4) {
    setGeometry(&m_geometry);
    setMaterial(&m_material);

    m_geometry.setVertexDataPattern(QSGGeometry::StaticPattern);
    m_geometry.setDrawingMode(GL_TRIANGLE_STRIP);

    setFlag(UsePreprocess);
}

void LightBlender::updateGeometry(LightSystem* item) {
    QSGGeometry::updateTexturedRectGeometry(&m_geometry,
                                            item->boundingRect(),
                                            QRectF(0, 0, 1, 1));
    markDirty(DirtyGeometry);
}

void LightBlender::preprocess() {
    m_material.update();
}

void LightBlender::Material::Shader::initialize() {
    QSGMaterialShader::initialize();
    initializeOpenGLFunctions();

    m_id_matrix = program()->uniformLocation("matrix");
    m_id_opacity = program()->uniformLocation("opacity");
    m_id_light = program()->uniformLocation("light");
    m_id_lightTexture = program()->uniformLocation("lightTexture");
    m_id_ambient = program()->uniformLocation("ambient");
}

void LightBlender::Material::Shader::activate() {
    QSGMaterialShader::activate();

    glGetIntegerv(GL_BLEND_SRC_RGB, m_blend+0);
    glGetIntegerv(GL_BLEND_DST_RGB, m_blend+1);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &m_activeTexture);

    glBlendFunc(GL_DST_COLOR, GL_ZERO);
}

void LightBlender::Material::Shader::deactivate() {
    QSGMaterialShader::deactivate();

    glBlendFunc(m_blend[0], m_blend[1]);
    glActiveTexture(m_activeTexture);
}

const char* const* LightBlender::Material::Shader::attributeNames() const {
    static const char* const attribute[] = {
        "vertex",
        "texcoord",
        0
    };
    return attribute;
}

const char* LightBlender::Material::Shader::vertexShader() const {
    return GLSL(
        attribute vec4 vertex;
        attribute vec4 texcoord;
        uniform mat4 matrix;
        varying vec2 coord;

        void main() {
           coord = texcoord.xy;
           gl_Position = matrix*vertex;
        }
    );
}

const char* LightBlender::Material::Shader::fragmentShader() const {
    QString shader = GLSL(
        const int lightCount = %1;
        uniform sampler2D light[lightCount];
        uniform sampler2D lightTexture;
        uniform vec4 ambient;
        uniform float opacity;
        varying vec2 coord;

        void main() {
            vec4 res = max(ambient, texture2D(lightTexture, coord));
            for (int i=0; i<lightCount; i++)
               res = max(res, texture2D(light[i], coord));
            gl_FragColor = opacity*res;
        }
    );

    static QByteArray data = shader.arg(DYNAMIC_LIGHTS_COUNT).toLocal8Bit();
    return data;
}

void LightBlender::Material::Shader::updateState(const RenderState& state,
                                                 QSGMaterial* mat,
                                                 QSGMaterial*) {

    Material* material = static_cast<Material*>(mat);

    GLint array[DYNAMIC_LIGHTS_COUNT];
    for (int i=0; i<DYNAMIC_LIGHTS_COUNT; i++) {
        glActiveTexture(GL_TEXTURE0+i);
        material->m_light[i]->bind();

        array[i] = i;
    }

    glActiveTexture(GL_TEXTURE0+DYNAMIC_LIGHTS_COUNT);
    material->m_lightTexture->bind();

    program()->setUniformValueArray(m_id_light, array, DYNAMIC_LIGHTS_COUNT);
    program()->setUniformValue(m_id_lightTexture, DYNAMIC_LIGHTS_COUNT);
    program()->setUniformValue(m_id_ambient, material->m_ambient);

    if (state.isMatrixDirty())
        program()->setUniformValue(m_id_matrix, state.combinedMatrix());
    if (state.isOpacityDirty())
        program()->setUniformValue(m_id_opacity, state.opacity());
}

LightBlender::Material::Material():
    m_lightTexture() {
    setFlag(Blending);
}

void LightBlender::Material::setLights(QSGDynamicTexture* array[]) {
    memcpy(m_light, array, sizeof(QSGDynamicTexture*)*DYNAMIC_LIGHTS_COUNT);
}

void LightBlender::Material::update() {
    for (QSGDynamicTexture* t: m_light)
        t->updateTexture();
    m_lightTexture->updateTexture();
}
