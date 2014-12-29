#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <QSGGeometryNode>
#include "QBox2D/QBody.hpp"
#include "LightMaterial.hpp"

class LightSystem;
class QSGTexture;

class Light: public QBody {
    private:
        Q_OBJECT

        QVector3D m_attenuation;
        qreal m_radius;
        qreal m_renderFraction;
        QColor m_color;
        LightSystem* m_lightSystem;

    protected:
        class Node: public QSGGeometryNode {
            private:
                QSGGeometry m_geometry;
                LightMaterial m_material;

            public:
                Node();
                void preprocess();

                inline LightMaterial* material() { return &m_material; }
                inline QSGGeometry* geometry() { return &m_geometry; }

                void updateGeometry(qreal radius);
                void synchronize(Light* light);
        };

        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        Light(Item* = nullptr);

        inline QVector3D attenuation() const { return m_attenuation; }
        void setAttenuation(QVector3D);

        inline qreal radius() const { return m_radius; }
        void setRadius(qreal);

        inline qreal renderFraction() const { return m_renderFraction; }
        void setRenderFraction(qreal);

        inline QColor color() const { return m_color; }
        void setColor(QColor);

        inline LightSystem* lightSystem() const { return m_lightSystem; }
        inline void setLightSystem(LightSystem* l) { m_lightSystem = l; }

        QRectF boundingRect() const;
        QRectF renderRect() const;

        bool read(const QJsonObject &);
        bool write(QJsonObject &) const;

        inline bool enabled() const { return false; }
};

#endif // LIGHT_HPP
