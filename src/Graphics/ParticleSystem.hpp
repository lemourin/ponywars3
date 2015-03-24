#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP
#include "SceneGraph/Item.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Material.hpp"
#include "SceneGraph/Geometry.hpp"

struct Particle {
    qreal x, y, r;
    qreal dx, dy;
};

class ParticleMaterial: public SceneGraph::Material {
    private:
        QOpenGLTexture* m_particleTexture;

        class ParticleShader: public SceneGraph::Shader, public QOpenGLFunctions {
            private:
                GLint m_blendFunc[2];

                int m_matrix;
                int m_texture;

            public:
                void activate();
                void deactivate();

                void initialize();

                const char* vertexShader() const;
                const char* fragmentShader() const;

                void updateState(const Material *, const SceneGraph::RenderState &);
                std::vector<std::string> attribute() const;
        };

    public:
        ParticleMaterial();

        inline void setParticleTexture(QOpenGLTexture* t) { m_particleTexture = t; }
        inline QOpenGLTexture* particleTexture() const { return m_particleTexture; }

        inline SceneGraph::Shader* shader() const { return SceneGraph::Shader::get<ParticleShader>(); }
};

class ParticleSystem: public SceneGraph::Item {
    private:
        friend class ViewWorld;

        std::vector<Particle> m_particle;
        QSizeF m_size;
        QRectF m_visibleRect;
        uint m_time;

        class Node: public SceneGraph::GeometryNode {
            private:
                struct Vertex {
                    float x, y, tx, ty;

                    Vertex(QPointF pos, QPointF tcoord);
                };

                SceneGraph::Geometry m_geometry;
                ParticleMaterial m_material;

                void generateTriangleStrip(GLuint* index, uint size);

            public:
                Node();

                inline ParticleMaterial* material() { return &m_material; }

                void update(const std::vector<Particle>& p);
        };

        void visibleAreaChanged(QRectF);
        void updateMatrix(QSizeF size, QRectF rect);

    protected:
        SceneGraph::Node* synchronize(SceneGraph::Node*);

    public:
        ParticleSystem(SceneGraph::Item* = nullptr);
        ~ParticleSystem();

        void addParticle(Particle);
        void step();

        void addExplosion(QPointF p, qreal r, qreal v, uint particleCount);

        inline uint time() const { return m_time; }

        void setSize(QSizeF s);
        inline QSizeF size() const { return m_size; }

};

#endif // PARTICLESYSTEM_HPP
