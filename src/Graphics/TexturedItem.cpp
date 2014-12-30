#include "TexturedItem.hpp"
#include "Utility/Window.hpp"
#include <QSGTexture>

TexturedItem::TexturedItem(Item* parent):
    SceneGraph::Item(parent) {
}

void TexturedItem::setSource(const QString& path) {
    m_source = path;
    /*if (m_source.isEmpty())
        setFlag(ItemHasContents, false);
    else
        setFlag(ItemHasContents, true);
    update();*/
}

QSGTexture* TexturedItem::texture() const {
    return static_cast<Window*>(window())->texture(source().toStdString());
}


