#ifndef FILEACTION_HPP
#define FILEACTION_HPP
#include "SubAction.hpp"

class MainAction;

class FileAction: public SubAction {
    private:
        ActionObject m_object;

    public:
        FileAction(MainAction*);
        ~FileAction();

        inline QString name() const { return "FileAction"; }
};

#endif // FILEACTION_HPP
