#ifndef TEXTUREDCONVEXPOLYGON_HPP
#define TEXTUREDCONVEXPOLYGON_HPP
#include "TexturedPolygon.hpp"

class TexturedConvexPolygon: public TexturedPolygon {
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
        explicit TexturedConvexPolygon(Item* = nullptr);

};

#endif // TEXTUREDCONVEXPOLYGON_HPP
