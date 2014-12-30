#ifndef SHADERSOURCE_HPP
#define SHADERSOURCE_HPP

#include "SceneGraph/Item.hpp"
#include <QQmlComponent>
#include <QColor>

class QSGDynamicTexture;

class ShaderSource: public SceneGraph::Item {
    private:
        class StaticData {
            private:
                QQmlComponent m_component;

                StaticData();

            public:
                static StaticData* instance();
                inline QQmlComponent* component() { return &m_component; }
        };

        //QQuickItem* m_item;
        //QQuickItem* m_sourceItem;
        QRectF m_sourceRect;
        QSize m_textureSize;
        QColor m_background;

        QMetaProperty m_sourceRectProperty;
        QMetaProperty m_textureSizeProperty;
        QMetaProperty m_backgroundProperty;

    protected:
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

    public:
        explicit ShaderSource(SceneGraph::Item *parent = nullptr);

        //inline QQuickItem* shaderSourceItem() const { return m_item; }
        //inline QQuickItem* sourceItem() const { return m_sourceItem; }

        inline QRectF sourceRect() const { return m_sourceRect; }
        void setSourceRect(QRectF);

        inline QSize textureSize() const { return m_textureSize; }
        void setTextureSize(QSize);

        inline QColor background() const { return m_background; }
        void setBackground(QColor);

        QSGDynamicTexture* texture() const;

};

#endif // SHADERSOURCE_HPP
