#ifndef TEXTUREDPOLYGON_HPP
#define TEXTUREDPOLYGON_HPP

#include <QSGGeometryNode>
#include <QSGOpaqueTextureMaterial>
#include "TexturedItem.hpp"

class TexturedPolygon: public TexturedItem {
    private:
        Q_OBJECT

        QVector2D m_textureScale;
        std::vector<QPointF> m_vertices;

    protected:
        class Node: public QSGTransformNode {
            private:
                QSGGeometryNode m_geometryNode;
                QSGGeometry m_geometry;
                QSGOpaqueTextureMaterial m_material;

            public:
                Node(const std::vector<QPointF>& pts,
                     QVector2D textureScale);

                void setTexture(QSGTexture*);
        };

        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        explicit TexturedPolygon(QQuickItem *parent = nullptr);

        inline const std::vector<QPointF>& vertices() const { return m_vertices; }
        inline void setVertices(const std::vector<QPointF>& v) { m_vertices = v; }

        inline const QVector2D& textureScale() const { return m_textureScale; }
        inline void setTextureScale(const QVector2D& s) { m_textureScale = s; }

};

#endif // TEXTUREDPOLYGON_HPP
