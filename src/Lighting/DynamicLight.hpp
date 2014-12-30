#ifndef DYNAMICLIGHT_HPP
#define DYNAMICLIGHT_HPP

#include "Light.hpp"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGSimpleMaterial>
#include <QSGSimpleMaterialShader>

class World;
class ViewWorld;
class StaticLight;

class ShadowNode: public QSGGeometryNode {
    private:
        QSGGeometry m_geometry;
        QSGFlatColorMaterial m_material;

        static const QSGGeometry::AttributeSet& attributes();

    public:
        ShadowNode(QPointF p1, QPointF p2);

        void setVertices(QPointF p1, QPointF p2);
        void setColor(QColor);
};

class DynamicLight: public Light {
    private:
        StaticLight* m_boundLight;

        class DynamicNode: public Light::Node {
            private:
                QSGNode m_shadows;
                std::vector<ShadowNode*> m_unused;

            public:
                DynamicNode();
                ~DynamicNode();

                void synchronize(DynamicLight*, ViewWorld* display);
                void makeShadowNode(DynamicLight* light,
                                    QPointF p1,
                                    QPointF p2);
        };

        bool castingShadow(QPointF p1, QPointF p2) const;

    protected:
        //QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData *);

    public:
        explicit DynamicLight(SceneGraph::Item* = nullptr);
        ~DynamicLight();

        void bindLight(StaticLight* light);

};

#endif // DYNAMICLIGHT_HPP
