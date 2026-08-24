#pragma once
#include <array>
#include "Game/Characters/Hero.hpp"

class Fog
{
    private:
        int position;
    public:
        Fog()
            : position(-1)
        {
        }
        int getPosition() const
        {
            return position;
        }
        void setPosition(int position)
        {
            this->position = position;
        }
        bool isPlaced() const
        {
            return position != -1;
        }
};


class InvisibleMan : public Hero
{
    private:
        static constexpr std::size_t FogCount = 3;
        std::array<Fog, FogCount> fogs;

    public:
        InvisibleMan();

        std::array<Fog, FogCount>& getFogs();
        const std::array<Fog, FogCount>& getFogs() const;
        Fog& getFog(std::size_t index);
        const Fog& getFog(std::size_t index) const;
        bool isOnFog() const;
        bool startedTurnOnFogFlag = false;
        void updateStartedTurnOnFog();
        bool startedTurnOnFog() const;
};