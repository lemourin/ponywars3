#ifndef ADDBODY_HPP
#define ADDBODY_HPP
#include "MapEditorAction.hpp"
#include "AddPolygon.hpp"
#include "AddCircle.hpp"
#include "AddRectangle.hpp"

class AddBody: public MapEditorAction {
    private:
        Q_OBJECT

        friend class AddFixture;

        Q_PROPERTY(AddPolygon* addPolygon READ addPolygon CONSTANT)
        Q_PROPERTY(AddCircle* addCircle READ addCircle CONSTANT)
        Q_PROPERTY(AddRectangle* addRectangle READ addRectangle CONSTANT)
        Q_PROPERTY(QString currentAction READ currentAction NOTIFY currentActionChanged)

        AddPolygon m_addPolygon;
        AddCircle m_addCircle;
        AddRectangle m_addRectangle;
        QString m_currentAction;
        std::vector<QFixture*> m_fixtures;
        std::vector<Action*> m_action;

        void finishedSubaction(AddFixture*);
        void actionFocusChanged(AddFixture* action);
        void setCurrentAction(QString);

    protected:
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

    public:
        explicit AddBody(MapEditor*);
        void reset();

        void finished();

        inline AddPolygon* addPolygon() { return &m_addPolygon; }
        inline AddCircle* addCircle() { return &m_addCircle; }
        inline AddRectangle* addRectangle() { return &m_addRectangle; }
        inline QString currentAction() const { return m_currentAction; }


    signals:
        void currentActionChanged();
};

#endif // ADDBODY_HPP
