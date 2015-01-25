#ifndef LIGHTBLENDER_HPP
#define LIGHTBLENDER_HPP
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Shader.hpp"
#include "SceneGraph/Material.hpp"
#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/ShaderSource.hpp"
#include <QColor>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

class QSGDynamicTexture;
class LightSystem;

const int DYNAMIC_LIGHTS_COUNT = 3;

class LightBlender: public SceneGraph::GeometryNode {
    private:
        struct Vertex { float x, y, tx, ty; };

        class Material: public SceneGraph::Material {
            private:
                class Shader: public SceneGraph::Shader, public QOpenGLFunctions {
                    private:
                        int m_id_matrix;
                        int m_id_opacity;
                        int m_id_light;
                        int m_id_lightTexture;
                        int m_id_ambient;
                        GLint m_blend[2];
                        GLint m_activeTexture;

                    public:
                        void initialize();

                        void activate();
                        void deactivate();

                        const char* vertexShader() const;
                        const char* fragmentShader() const;

                        void updateState(const SceneGraph::Material*, const SceneGraph::RenderState&);
                        std::vector< std::string > attribute() const;
                };

                SceneGraph::ShaderSource* m_light[DYNAMIC_LIGHTS_COUNT];
                SceneGraph::ShaderSource* m_lightTexture;
                QColor m_ambient;

            public:
                Material();

                inline Shader* shader() const { return Shader::get<Shader>(); }

                inline void setLightTexture(SceneGraph::ShaderSource* c) { m_lightTexture = c; }
                inline void setAmbient(QColor c) { m_ambient = c; }
                void setLights(SceneGraph::ShaderSource* array[DYNAMIC_LIGHTS_COUNT]);

                void update();
        } m_material;

        SceneGraph::Geometry m_geometry;

    protected:
        void preprocess();

    public:
        LightBlender();

        inline Material* material() { return &m_material; }
};
#endif // LIGHTBLENDER_HPP
