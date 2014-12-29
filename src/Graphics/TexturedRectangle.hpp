#ifndef TEXTUREDRECTANGLE_HPP
#define TEXTUREDRECTANGLE_HPP

#include <QSGGeometryNode>
#include <QSGOpaqueTextureMaterial>
#include "TexturedItem.hpp"

class TexturedRectangle: public TexturedItem {
    private:
        QRectF m_textureRect;

    protected:
        class Node: public QSGTransformNode {
            private:
                QSGGeometryNode m_geometryNode;
                QSGGeometry m_geometry;
                QSGOpaqueTextureMaterial m_material;

            public:
                Node();

                void synchronize(const TexturedRectangle*);

                void updateMaterial(const TexturedRectangle*);
                void updateGeometry(const TexturedRectangle*);

        };

        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        explicit TexturedRectangle(SceneGraph::Item* = nullptr);

        inline QRectF textureRect() const { return m_textureRect; }
        void setTextureRect(QRectF);

};

#endif // TEXTUREDRECTANGLE_HPP
