#include "Game/Characters/InvisibleMan.hpp"
#include <stdexcept>

using namespace std ;

InvisibleMan::InvisibleMan()
    : Hero(
        "Invisible Man",
        15,
        2,
        AttackType::Melee,
        HeroType::Invisibleman
    )
{
}

array<Fog, InvisibleMan::FogCount>& InvisibleMan::getFogs()
{
    return fogs;
}

const array<Fog, InvisibleMan::FogCount>& InvisibleMan::getFogs() const
{
    return fogs;
}

Fog& InvisibleMan::getFog(size_t index)
{
    if (index >= FogCount)
        throw out_of_range("Invalid fog index");

    return fogs[index];
}

const Fog& InvisibleMan::getFog(size_t index) const
{
    if (index >= FogCount)
        throw out_of_range("Invalid fog index");

    return fogs[index];
}

bool InvisibleMan::isOnFog() const
{
    for (const Fog& fog : fogs)
    {
        if (fog.isPlaced() &&
            fog.getPosition() == getPosition())
        {
            return true;
        }
    }

    return false;
}

void InvisibleMan::updateStartedTurnOnFog()
{
    startedTurnOnFogFlag = isOnFog();
}

bool InvisibleMan::startedTurnOnFog() const
{
    return startedTurnOnFogFlag;
}