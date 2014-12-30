#ifndef ADDFIXTURE_HPP
#define ADDFIXTURE_HPP
#include "Action.hpp"

class QFixture;
class AddBody;

class AddFixture: public Action {
    private:
        AddBody* m_addBody;

    protected:
        void focusChanged();

    public:
        AddFixture(AddBody*);
        void finished();

        virtual QFixture* fixture() const = 0;
};

#endif // ADDFIXTURE_HPP
