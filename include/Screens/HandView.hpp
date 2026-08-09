#pragma once

#include "layout.hpp"
#include <vector>
#include <unordered_set>
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
        void HighlightCards(const std::vector<int>& indices);

        void ClearHighlightedCards();

        bool IsHighlighted(int index) const;

        int GetClickedCard(
            const Deck& deck,
            Rectangle area
        ) const;
    private:

        void DrawCard(
            const Card& card,
            const CardTransform& t,
            bool playable
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
        std::unordered_set<int> highlightedCards;
        

};