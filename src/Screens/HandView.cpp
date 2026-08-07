#include "Screens/HandView.hpp"
#include "cmath" 
#include "Game/Cards/Deck.hpp"
#include "Game/Cards/Cards.hpp"

HandView::HandView()
{
}

void HandView::Draw(
    const Deck& deck,
    Rectangle area)
{
    Layout layout;

    layout.panel = area;

    layout.sx = area.width / DESIGN_HAND_WIDTH;
    layout.sy = area.height / DESIGN_HAND_HEIGHT;

    layout.s = std::min(layout.sx, layout.sy);

    DrawRectangleLinesEx(layout.panel, layout.S(3), BLACK);

    const auto& hand = deck.getHand();

    int count = (int)hand.size();

    int hovered = GetHoveredCard(deck, layout);

    for (int i = 0; i < count; i++)
    {
        if (i == hovered)
            continue;

        DrawCard(
            *hand[i],
            GetCardTransform(
                i,
                count,
                layout,
                false
            )
        );
    }

    if (hovered != -1)
    {
        DrawCard(
            *hand[hovered],
            GetCardTransform(
                hovered,
                count,
                layout,
                true
            )
        );
    }
}

bool PointInCard(Vector2 mouse,const CardTransform& t)
{
    Vector2 p;

    p.x = mouse.x - t.pivot.x;
    p.y = mouse.y - t.pivot.y;

    float rad = -t.rotation * DEG2RAD;

    float cs = cosf(rad);
    float sn = sinf(rad);

    float lx = p.x * cs - p.y * sn;
    float ly = p.x * sn + p.y * cs;

    lx += t.width / 2;
    ly += t.height;

    return
        lx >= 0 &&
        lx <= t.width &&
        ly >= 0 &&
        ly <= t.height;
}



int HandView::GetHoveredCard(
    const Deck& deck,
    const Layout& layout
) const
{
    
    Vector2 mouse = GetMousePosition();

    const auto& hand = deck.getHand();

    for(int i = hand.size()-1; i>=0; --i)
    {
        CardTransform t =
            GetCardTransform(
                i,
                hand.size(),
                layout,
                false
            );

        if(PointInCard(mouse,t))
            return i;
    }

    return -1;
}


void HandView::DrawCard(
    const Card& card,
    const CardTransform& t
) const
{
    Texture2D tex = cardView.getCardView(card);

    Rectangle src =
    {
        0,
        0,
        (float)tex.width,
        (float)tex.height
    };

    Rectangle dst =
    {
        t.pivot.x,
        t.pivot.y,
        t.width,
        t.height
    };

    Vector2 origin =
    {
        t.width / 2,
        t.height
    };

    DrawTexturePro(
        tex,
        src,
        dst,
        origin,
        t.rotation,
        WHITE
    );
}


CardTransform HandView::GetCardTransform(
    int index,
    int count,
    const Layout& layout,
    bool hovered
) const
{
    CardTransform t;

    constexpr float DESIGN_CARD_WIDTH  = 250.0f;
    constexpr float DESIGN_CARD_HEIGHT = 350.0f;
    constexpr float CARD_RATIO = DESIGN_CARD_HEIGHT / DESIGN_CARD_WIDTH;

    float baseW = layout.W(DESIGN_CARD_WIDTH);
    float baseH = baseW * CARD_RATIO;

    t.scale = hovered ? 1.08f : 1.0f;

    t.width  = baseW * t.scale;
    t.height = baseH * t.scale;

    float availableWidth = layout.panel.width * 0.90f;
    float desiredSpacing = t.width * 0.42f;   

    float totalDesired = t.width + desiredSpacing * (count - 1);

    float spacing;
    if (count <= 1 || totalDesired <= availableWidth)
    {
        spacing = desiredSpacing;               
    }
    else
    {
        spacing = (availableWidth - t.width) / (count - 1);
        spacing = std::max(spacing, t.width * 0.18f);
    }

    float totalWidth = t.width + spacing * (count - 1);

    float startX =
        layout.panel.x +
        (layout.panel.width - totalWidth) * 0.5f;

    t.pivot.x =
        startX +
        index * spacing +
        t.width * 0.5f;

    t.pivot.y =
        layout.panel.y +
        layout.panel.height -
        layout.H(20);

    t.rotation = 0;

    if (hovered)
        t.pivot.y -= layout.H(45);

    return t;
}