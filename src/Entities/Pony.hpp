#ifndef PONY_HPP
#define PONY_HPP
#include "Creature.hpp"
#include "PonyAnimation.hpp"
#include "Lighting/StaticLight.hpp"

class Pony: public Creature {
    private:
        PonyAnimation m_body;
        PonyAnimation m_wings;

        std::string m_bodySource;
        std::string m_wingsSource;

    protected:
        void currentStateChanged(unsigned added, unsigned removed);

    public:
        Pony(Item* = nullptr);
        void initialize();

        void setCurrentSprite(const char*);

        void jumpRequested();
        void punchRequested();

        bool read(const QJsonObject &);
        bool write(QJsonObject &) const;

};

#endif // PONY_HPP
