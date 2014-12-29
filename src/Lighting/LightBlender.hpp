#ifndef LIGHTBLENDER_HPP
#define LIGHTBLENDER_HPP
#include <QSGGeometryNode>
#include <QSGMaterialShader>
#include <QColor>
#include <QOpenGLFunctions>

class QSGDynamicTexture;
class LightSystem;

const int DYNAMIC_LIGHTS_COUNT = 1;

class LightBlender: public QSGGeometryNode {
    private:
        class Material: public QSGMaterial {
            private:
                class Shader: public QSGMaterialShader, public QOpenGLFunctions {
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

                        const char* const* attributeNames() const;
                        const char* vertexShader() const;
                        const char* fragmentShader() const;

                        void updateState(const RenderState &state,
                                         QSGMaterial *newMaterial,
                                         QSGMaterial *oldMaterial);
                };

                QSGDynamicTexture* m_light[DYNAMIC_LIGHTS_COUNT];
                QSGDynamicTexture* m_lightTexture;
                QColor m_ambient;

            public:
                Material();

                inline QSGMaterialShader* createShader() const { return new Shader; }
                inline QSGMaterialType* type() const { static QSGMaterialType t; return &t; }

                inline void setLightTexture(QSGDynamicTexture* c) { m_lightTexture = c; }
                inline void setAmbient(QColor c) { m_ambient = c; }
                void setLights(QSGDynamicTexture* array[DYNAMIC_LIGHTS_COUNT]);

                void update();
        } m_material;

        QSGGeometry m_geometry;

    public:
        LightBlender();

        inline Material* material() { return &m_material; }

        void updateGeometry(LightSystem*);
        void preprocess();
};
#endif // LIGHTBLENDER_HPP
