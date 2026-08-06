#pragma once

#include "layout.hpp"
#include "Screens/CardView.hpp"

class Deck;

constexpr float DESIGN_HAND_WIDTH  = 1100.0f;
constexpr float DESIGN_HAND_HEIGHT = 380.0f;

struct CardTransform
{
    Vector2 pivot;     // محل لولای کارت
    float width;
    float height;
    float rotation;
    float scale;
};

class HandView
{
    public:

        HandView();

        void Draw(
            const Deck& deck,
            Rectangle area
        );

        int GetHoveredCard(
            const Deck& deck,
            const Layout& layout
        ) const ;
        

    private:

        void DrawCard(
            const Card& card,
            const CardTransform& transform
        ) const;


        CardView cardView;

        float cardScale = 0.45f;
        float rotationStep = 6.0f;

        CardTransform GetCardTransform(
            int index,
            int count,
            const Layout&,
            bool hovered
        ) const;
};