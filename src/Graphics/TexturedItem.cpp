#include "TexturedItem.hpp"
#include "Utility/Window.hpp"
#include <QSGTexture>

TexturedItem::TexturedItem(QQuickItem *parent):
    QQuickItem(parent) {
}

void TexturedItem::setSource(const QString& path) {
    m_source = path;
    if (m_source.isEmpty())
        setFlag(ItemHasContents, false);
    else
        setFlag(ItemHasContents, true);
    update();
}

void TexturedItem::setMatrix(const QMatrix4x4& matrix) {
    m_matrix = matrix;
    update();
}

QSGTexture* TexturedItem::texture() const {
    return static_cast<Window*>(window())->texture(source().toStdString());
}


