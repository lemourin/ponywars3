#ifndef ADDPOLYGON_HPP
#define ADDPOLYGON_HPP
#include <vector>
#include "AddFixture.hpp"

class AddBody;

class AddPolygon: public AddFixture {
    private:
        ActionObject m_object;

        std::vector<QPointF> m_pts;

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        AddPolygon(AddBody*);

        void reset();
        QFixture* fixture() const;

        inline QString name() const { return "AddPolygon"; }

};

#endif // ADDPOLYGON_HPP
