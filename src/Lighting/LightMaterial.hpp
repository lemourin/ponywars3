#ifndef LIGHTMATERIAL_HPP
#define LIGHTMATERIAL_HPP
#include <QSGMaterial>
#include <QOpenGLFunctions>
#include <QColor>

class QSGDynamicTexture;

class LightMaterial: public QSGMaterial {
    private:
        class Shader: public QSGMaterialShader, public QOpenGLFunctions {
            private:
                int m_id_matrix;
                int m_id_opacity;
                int m_id_lightPosition;
                int m_id_attenuation;
                int m_id_color;
                int m_id_normalMap;

                GLint m_blendEquation;
                GLint m_blendFunc[2];
                GLint m_activeTexture;

                void initialize();
                void activate();
                void deactivate();
                const char* vertexShader() const;
                const char* fragmentShader() const;

            public:
                const char* const* attributeNames() const;
                void updateState(const RenderState &state,
                                 QSGMaterial *newMaterial,
                                 QSGMaterial *oldMaterial);
        };

        QColor m_color;
        QVector3D m_lightPosition;
        QVector3D m_attenuation;
        QSGDynamicTexture* m_normalMap;

    public:
        LightMaterial();

        QSGMaterialShader* createShader() const;
        QSGMaterialType* type() const;

        inline QColor color() const { return m_color; }
        inline void setColor(QColor c) { m_color = c; }

        inline QVector3D lightPosition() const { return m_lightPosition; }
        inline void setLightPosition(QVector3D p) { m_lightPosition = p; }

        inline QVector3D attenuation() const { return m_attenuation; }
        inline void setAttenuation(QVector3D att) { m_attenuation = att; }

        inline QSGDynamicTexture* normalMap() const { return m_normalMap; }
        inline void setNormalMap(QSGDynamicTexture* t) { m_normalMap = t; }
};
#endif // LIGHTMATERIAL_HPP
