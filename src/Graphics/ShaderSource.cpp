#include "ShaderSource.hpp"
#include "Utility/Utility.hpp"
#include <QSGTextureProvider>
#include <QSGDynamicTexture>
#include <cassert>
#define QML(code) #code

ShaderSource::ShaderSource(Item* parent):
    SceneGraph::Item(parent),
    /*m_item(static_cast<QQuickItem*>(StaticData::instance()->component()->create())),
    m_sourceItem(m_item->property("sourceItem").value<QQuickItem*>()),*/
    m_background(Qt::white) {

    /*shaderSourceItem()->setParent(this);
    shaderSourceItem()->setParentItem(this);

    int srcRect = m_item->metaObject()->indexOfProperty("sourceRect");
    m_sourceRectProperty = m_item->metaObject()->property(srcRect);

    int txtSize = m_item->metaObject()->indexOfProperty("textureSize");
    m_textureSizeProperty = m_item->metaObject()->property(txtSize);

    int bgColor = sourceItem()->metaObject()->indexOfProperty("color");
    m_backgroundProperty = sourceItem()->metaObject()->property(bgColor);

    shaderSourceItem()->setProperty("hideSource", true);
    shaderSourceItem()->setVisible(false);*/

    setBackground(Qt::transparent);
}

void ShaderSource::setSourceRect(QRectF rect) {
    m_sourceRect = rect;
    //m_sourceRectProperty.write(shaderSourceItem(), m_sourceRect);
}

void ShaderSource::setTextureSize(QSize size) {
    m_textureSize = size;
    //m_textureSizeProperty.write(shaderSourceItem(), m_textureSize);
}

void ShaderSource::setBackground(QColor color) {
    m_background = color;
    //m_backgroundProperty.write(sourceItem(), m_background);
}

QSGDynamicTexture* ShaderSource::texture() const {
    //QSGTexture* t = shaderSourceItem()->textureProvider()->texture();
    //return static_cast<QSGDynamicTexture*>(t);
}

void ShaderSource::geometryChanged(const QRectF& newGeometry,
                                   const QRectF& oldGeometry) {
    //QQuickItem::geometryChanged(newGeometry, oldGeometry);
    //shaderSourceItem()->setPosition(newGeometry.topLeft());
    //shaderSourceItem()->setSize(newGeometry.size());
}

ShaderSource::StaticData::StaticData():
    m_component(Utility::qmlEngine()) {
    const char* code = QML(
        import QtQuick 2.0;
        ShaderEffectSource {
            sourceItem: Rectangle { }
        }
    );
    m_component.setData(code, QUrl());
}

ShaderSource::StaticData* ShaderSource::StaticData::instance() {
    static StaticData data;
    return &data;
}
