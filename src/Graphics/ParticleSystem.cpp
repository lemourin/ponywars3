#include "ParticleSystem.hpp"
#include "Geometry/Circle.hpp"
#include "SceneGraph/Renderer.hpp"
#include "Utility/Window.hpp"
#include <cassert>

ParticleSystem::ParticleSystem(Item *parent):
    SceneGraph::Item(parent),
    m_time(),
    m_lightSystem() {
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::addParticle(Particle p) {
    m_particle.push_back(p);
    update();
}

void ParticleSystem::step() {
    for (Particle& p: m_particle) {
        p.x += p.dx;
        p.y += p.dy;
    }
    m_time++;

    update();

}

void ParticleSystem::addExplosion(QPointF p, qreal r, qreal v, uint particleCount) {
    for (uint i=0; i<particleCount; i++) {
        qreal angle = i*2*M_PI/particleCount;
        qreal dx = cos(angle);
        qreal dy = sin(angle);

        addParticle({ p.x(), p.y(), r,  dx*v, dy*v, 0, time() });
    }
}

void ParticleSystem::setSize(QSizeF s) {
    m_size = s;
    updateMatrix(m_size, m_visibleRect);

}

void ParticleSystem::visibleAreaChanged(QRectF rect) {
    m_visibleRect = rect;
    updateMatrix(m_size, m_visibleRect);
}

void ParticleSystem::updateMatrix(QSizeF size, QRectF rect) {
    QMatrix4x4 m;
    m.scale(0.5*size.width(), 0.5*size.height());
    m.translate(1, 1);
    m.ortho(rect.left(), rect.right(), rect.top(), rect.bottom(), -1, 1);

    setMatrix(m);
}

SceneGraph::Node *ParticleSystem::synchronize(SceneGraph::Node* old) {
    Node* node = static_cast<Node*>(old);
    if (!node) {
        node = new Node;
        node->material()->setColor(QColor(255, 0, 0, 255));
        node->material()->setLightPosition(QVector3D(0.5, 0.5, 0.1));

        assert(lightSystem());
        assert(lightSystem()->normalMap());
        node->material()->setNormalMap(lightSystem()->normalMap()->shaderNode());
    }

    node->update(m_particle);
    node->material()->setTime(time());

    return node;
}

void ParticleSystem::Node::generateTriangleStrip(GLuint *index, uint size) {
    uint it = 0;
    for (uint i=0; i<std::min(5u, size); i++) {
        index[i] = it;
        if (i != 3)
            it++;
    }

    for (uint i=5; i<size; i++) {
        index[i] = it;
        if (i%6 != 5 && i%6 != 3)
            it++;
    }
}

ParticleSystem::Node::Node():
    m_geometry({ { 2, GL_FLOAT }, { 2, GL_FLOAT } }, 0, sizeof(Vertex)) {
    setMaterial(&m_material);
    setGeometry(&m_geometry);

    m_geometry.setDrawingMode(GL_TRIANGLE_STRIP);
    setFlag(UsePreprocess);
}

void ParticleSystem::Node::preprocess() {
    m_material.update();
}

void ParticleSystem::Node::update(const std::vector<Particle>& set) {
    if (m_geometry.vertexDataSize() < 4*set.size()) {
        m_geometry.allocate(set.size()*4*2, set.size()*6*2-2);
        generateTriangleStrip(m_geometry.indexData<GLuint>(),
                              m_geometry.indexDataSize());
    }

    Vertex* array = m_geometry.vertexData<Vertex>();

    int it = 0;
    for (const Particle& p: set) {
        array[4*it+0] = Vertex(QPointF(p.x-p.r, p.y-p.r), QPointF(0, 0));
        array[4*it+1] = Vertex(QPointF(p.x+p.r, p.y-p.r), QPointF(1, 0));
        array[4*it+2] = Vertex(QPointF(p.x-p.r, p.y+p.r), QPointF(0, 1));
        array[4*it+3] = Vertex(QPointF(p.x+p.r, p.y+p.r), QPointF(1, 1));
        it++;
    }

    m_geometry.setVertexCount(set.size()*4);
    m_geometry.setIndexCount(set.size()*6-2);
    m_geometry.updateVertexData();
}

void ParticleMaterial::ParticleShader::activate() {
    glGetIntegerv(GL_BLEND_SRC_RGB, m_blendFunc+0);
    glGetIntegerv(GL_BLEND_DST_RGB, m_blendFunc+1);
    glBlendFunc(GL_ONE, GL_ONE);
}

void ParticleMaterial::ParticleShader::deactivate() {
    glBlendFunc(m_blendFunc[0], m_blendFunc[1]);
}

void ParticleMaterial::ParticleShader::initialize() {
    SceneGraph::Shader::initialize();

    initializeOpenGLFunctions();

    m_matrix = program()->uniformLocation("matrix");
    m_normalmap = program()->uniformLocation("normalmap");
    m_color = program()->uniformLocation("color");
    m_lightposition = program()->uniformLocation("lightposition");
}

const char *ParticleMaterial::ParticleShader::vertexShader() const {
    return GLSL(
        attribute vec4 position;
        attribute vec2 tcoord;
        uniform mat4 matrix;

        varying vec2 texcoord;
        varying vec2 normalcoord;
        varying float opacity;

        void main() {
            texcoord = tcoord;
            gl_Position = matrix*position;

            normalcoord.x = 0.5*(gl_Position.x+1.0);
            normalcoord.y = 1.0-0.5*(gl_Position.y+1.0);

            opacity = 1.0;
        }
    );
}

const char *ParticleMaterial::ParticleShader::fragmentShader() const {
    return GLSL(
        uniform sampler2D normalmap;
        uniform vec4 color;
        uniform vec3 lightposition;

        varying vec2 texcoord;
        varying vec2 normalcoord;
        varying float opacity;

        void main() {
            vec3 normalcolor = texture2D(normalmap, normalcoord).rgb;
            vec3 lightdir = lightposition-vec3(texcoord, 0.0);
            float d = length(lightdir);

            vec3 n = normalize(normalcolor*2.0-1.0);
            vec3 l = normalize(lightdir);
            vec3 diffuse = color.rgb*color.a*max(dot(n, l), 0.0);

            gl_FragColor = 2.0*opacity*(color-2.0*d)*vec4(diffuse, 0.0);
        }
    );
}

void ParticleMaterial::ParticleShader::updateState(const SceneGraph::Material* t,
                                                   const SceneGraph::RenderState& state) {
    const ParticleMaterial* m = static_cast<const ParticleMaterial*>(t);
    program()->setUniformValue(m_matrix, state.matrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m->normalMap()->texture()->texture());
    program()->setUniformValue(m_normalmap, 0);

    program()->setUniformValue(m_color, m->color());
    program()->setUniformValue(m_lightposition, m->lightPosition());
}

std::vector<std::string> ParticleMaterial::ParticleShader::attribute() const {
    return {
        "position",
        "tcoord"
    };
}

ParticleSystem::Node::Vertex::Vertex(QPointF pos, QPointF tcoord):
    x(pos.x()), y(pos.y()), tx(tcoord.x()), ty(tcoord.y()) {
}

ParticleMaterial::ParticleMaterial():
    m_normalMap(),
    m_time() {
}

void ParticleMaterial::update() {
    assert(m_normalMap);
    m_normalMap->updateTexture();
}
