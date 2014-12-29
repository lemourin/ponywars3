#ifndef TEXTUREDITEM_HPP
#define TEXTUREDITEM_HPP

#include <QQuickItem>
#include <QMatrix4x4>
#include "SceneGraph/Item.hpp"

class QSGTexture;

class TexturedItem: public SceneGraph::Item {
    private:
        QString m_source;
        QMatrix4x4 m_matrix;

        QSize m_textureSize;

    protected:
        QSGTexture* texture() const;

    public:
        explicit TexturedItem(QQuickItem* = nullptr);

        inline const QString& source() const { return m_source; }
        void setSource(const QString&);

        inline const QMatrix4x4& matrix() const { return m_matrix; }
        void setMatrix(const QMatrix4x4&);

};

#endif // TEXTUREDITEM_HPP
