#ifndef TEXTUREDITEM_HPP
#define TEXTUREDITEM_HPP

#include <QQuickItem>
#include <QMatrix4x4>
#include "SceneGraph/Item.hpp"

class QSGTexture;

class TexturedItem: public SceneGraph::Item {
    private:
        QString m_source;

        QSize m_textureSize;

    protected:
        QSGTexture* texture() const;

    public:
        explicit TexturedItem(SceneGraph::Item* = nullptr);

        inline const QString& source() const { return m_source; }
        void setSource(const QString&);
};

#endif // TEXTUREDITEM_HPP
