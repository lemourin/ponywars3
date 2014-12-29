#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "Shader.hpp"
#include <QColor>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

namespace SceneGraph {

class Material {
    public:
        Material();

        virtual Shader* shader() const = 0;
};

class ColorMaterial: public Material {
    private:
        class ColorShader: public Shader {
            private:
                int m_matrix;
                int m_color;

            protected:
                void initialize();

                inline void activate() override { }
                inline void deactivate() override { }

                const char* vertexShader() const override;
                const char* fragmentShader() const override;

                void updateState(const Material* m, const RenderState& state);
        };

    protected:
        inline Shader* shader() const { return Shader::get<ColorShader>(); }

    public:
        QColor m_color;
};

class TextureMaterial: public Material {
    private:
        class TextureShader: public Shader, public QOpenGLFunctions {
            private:
                int m_matrix;
                int m_texture;

            protected:
                void initialize();

                inline void activate() override { }
                inline void deactivate() override { }

                const char* vertexShader() const override;
                const char* fragmentShader() const override;

                void updateState(const Material*, const RenderState &);
        };

    protected:
        inline Shader* shader() const { return Shader::get<TextureShader>(); }

    public:
        QOpenGLTexture* m_texture = nullptr;
};

class VertexColorMaterial: public Material {
    private:
        class VertexColorShader: public Shader {
            private:
                int m_matrix;

            protected:
                void initialize();

                inline void activate() override { }
                inline void deactivate() override { }

                const char* vertexShader() const override;
                const char* fragmentShader() const override;

                void updateState(const Material*, const RenderState &);
        };

    protected:
        inline Shader* shader() const { return Shader::get<VertexColorShader>(); }
};

}

#endif // MATERIAL_HPP
