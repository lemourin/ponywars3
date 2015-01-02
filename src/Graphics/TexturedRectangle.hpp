#ifndef TEXTUREDRECTANGLE_HPP
#define TEXTUREDRECTANGLE_HPP

#include "TexturedItem.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/Material.hpp"

class TexturedRectangle: public TexturedItem {
    private:
        QRectF m_textureRect;
        QRectF m_boundingRect;

    protected:
        class Node: public SceneGraph::Node {
            private:
                SceneGraph::GeometryNode m_geometryNode;
                SceneGraph::Geometry m_geometry;
                SceneGraph::TextureMaterial m_material;

                struct Vertex {
                    float x, y, tx, ty;

                    void set(float, float, float, float);
                };

            public:
                Node();

                void synchronize(const TexturedRectangle*);

                void updateMaterial(const TexturedRectangle*);
                void updateGeometry(const TexturedRectangle*);

        };

        SceneGraph::Node* synchronize(SceneGraph::Node* old);

    public:
        explicit TexturedRectangle(SceneGraph::Item* = nullptr);

        inline QRectF textureRect() const { return m_textureRect; }
        void setTextureRect(QRectF);

        inline QPointF position() const { return m_boundingRect.topLeft(); }
        inline void setPosition(QPointF p) { m_boundingRect.setTopLeft(p); }

        inline QSizeF size() const { return m_boundingRect.size(); }
        inline void setSize(QSizeF s) { m_boundingRect.setSize(s); }

        inline QRectF boundingRect() const { return m_boundingRect; }

};

#endif // TEXTUREDRECTANGLE_HPP
