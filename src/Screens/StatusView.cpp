#include "Screens/StatusView.hpp"

#include "Game/Characters/Character.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Characters/Hero.hpp"
#include "raylib.h"



static void DrawOutlinedText(Font font,
                             const std::string& text,
                             Vector2 position,
                             float fontSize,
                             float spacing,
                             Color textColor,
                             Color outlineColor,
                             int thickness = 2)
{
    for(int x = -thickness; x <= thickness; x++)
    {
        for(int y = -thickness; y <= thickness; y++)
        {
            if(x == 0 && y == 0)
                continue;

            DrawTextEx(
                font,
                text.c_str(),
                {position.x + x, position.y + y},
                fontSize,
                spacing,
                outlineColor
            );
        }
    }

    DrawTextEx(
        font,
        text.c_str(),
        position,
        fontSize,
        spacing,
        textColor
    );
}

static Color PanelBackground(const Hero* hero)
{
    if(hero->getname() == "Dracula")
        return Color{45,18,24,255};   
    if(hero->getname() == "Sherlock")
        return Color{48,42,18,255};
    return Color{28,32,42,255};       
}

static Color AccentColor(const Hero* hero)
{
    if(hero->getname() == "Dracula")
        return Color{185,40,45,255};
    if(hero->getname() == "Sherlock")
        return Color{220,185,55,255} ;
    return Color{28,32,42,255}; 
}

static Color SidekickBackground(const Character* character)
{
    if(character->getname().find("Sister") != std::string::npos)
        return Color{50, 24, 30, 255};   // زرشکی تیره

    if(character->getname() == "Dr.Watson")
        return Color{58, 50, 22, 255};   // طلایی تیره

    return Color{45,45,45,255};
}

static Color SidekickPortraitBackground(const Character* character)
{
    if(character->getname().find("Sister") != std::string::npos)
        return Color{22,22,24,255};      // تقریبا مشکی

    if(character->getname() == "Dr.Watson")
        return Color{28,24,16,255};      // قهوه‌ای خیلی تیره

    return Color{25,25,25,255};
}

static Color CharacterColor(const std::string& name)
{
    if(name=="Sherlock")
        return GOLD;

    if(name=="Dracula")
        return RED;

    if(name=="Dr.Watson")
        return BEIGE;

    if(name.find("Sister")!=std::string::npos)
        return PINK;

    return WHITE;
}

const Texture2D& StatusView::GetBackground(const Hero* hero) const
{
    if(hero->getname()=="Dracula")
        return draculaBackground;

    return sherlockBackground;
}

static std::string ShortName(const std::string& name)
{
    if(name=="Dr.Watson")
        return "Watson";

    if(name=="Sherlock")
        return "SH";

    if(name=="Dracula")
        return "DR";

    if(name=="Sister 1")
        return "Agatha";

    if(name=="Sister 2")
        return "Lucy";

    if(name=="Sister 3")
        return "Mina";

    return name;
}

void StatusView::DrawHpBar(int hp,
                           int maxHp,
                           Rectangle area , Layout layout) const
{
    float ratio =
        (float)hp / maxHp;

    Color color = GREEN;

    if(ratio < 0.3f)
        color = RED;
    else if(ratio < 0.7f)
        color = YELLOW;

    DrawRectangleRounded(
        area,
        0.4f,
        8,
        DARKGRAY);

    Rectangle fill = area;
    fill.width *= ratio;

    DrawRectangleRounded(
        fill,
        0.4f,
        8,
        color);

    
    /*DrawTextEx(
        font ,
        TextFormat("%d / %d",hp,maxHp),
        {area.x, area.y+12}  ,
        22,
        1,
        WHITE);*/
    DrawOutlinedText(
        font,
        TextFormat("%d / %d", hp, maxHp),
        {area.x, area.y + 12},
        layout.S(18),
        1,
        WHITE,
        BLACK,
        1
    );
}

void StatusView::DrawHeroSection(Player& player,
                                 Layout layout) const
{
    auto hero = player.getHero();

    /*DrawTextEx(
        font , 
        player.getName().c_str(),
        {area.x+20,area.y+10} ,
        30,
        1 ,
        LIGHTGRAY);*/
    DrawOutlinedText(
        font,
        player.getName(),
        {layout.X(20) , layout.Y(10)},
        layout.S(34),
        1,
        LIGHTGRAY,
        BLACK,
        2
    );
    float portraitSize = layout.S(250) ;
    Rectangle portrait =
    {
        layout.X(16) ,
        layout.Y(40) ,
        portraitSize , 
        portraitSize 
    };

    const Texture2D& tex = GetPortrait(hero.get());

    DrawTexturePro(
        tex,
        {
            0,
            0,
            (float)tex.width ,
            (float)tex.height 
        },
        portrait,
        {0,0},
        0,
        WHITE
    );

    DrawRectangleLinesEx(
        portrait,
        2 ,
        AccentColor(hero.get()));

    /*DrawTextEx(
        font , 
        hero->getname().c_str(),
        {area.x+25,area.y + 295} ,
        36,
        1 , 
        CharacterColor(hero->getname()));*/
    DrawOutlinedText(
        font,
        hero->getname(),
        {layout.X(25), layout.Y(300)},
        layout.S(36),
        1,
        CharacterColor(hero->getname()),
        BLACK,
        2   
    );

    DrawHpBar(
        hero->getHp(),
        hero->getMaxhp(),
        {
            layout.X(25),
            layout.Y(335),
            layout.W(250) , 
            layout.H(4)
        } , layout);
}

void StatusView::DrawCardsSection(Player& player,
                                  Layout layout) const
{
    auto deck = player.getHero()->getDeck();

    DrawTextEx(
        font , 
        "CARDS",
        {layout.X(20) , layout.Y(380)} ,
        layout.S(32) ,
        1, 
        LIGHTGRAY);

    DrawLine(
        layout.X(20),
        layout.Y(410),
        layout.X(280),
        layout.Y(410),
        BLACK);

    DrawTextEx(
        font , 
        TextFormat("Hand : %i",
                   deck->getHand().size()),
        {layout.X(30), layout.Y(425)} ,
        layout.S(30) ,
        1 ,
        WHITE);

    DrawTextEx(
        font , 
        TextFormat("Deck : %i",
                   deck->getDrawPileSize()),
        {layout.X(30) ,layout.Y(455)},
        layout.S(30) ,
        1 , 
        WHITE);

    DrawTextEx(
        font , 
        TextFormat("Discard : %i",
                   deck->getDiscardPileSize()),
        {layout.X(30), layout.Y(485)} ,
        layout.S(30) ,
        1 , 
        WHITE);
}

void StatusView::DrawCharacterCard(const Character* character,
                                   Rectangle card,
                                   const Layout& layout) const
{
    DrawRectangleRounded(
        card,
        0.12f,
        8,
        SidekickBackground(character));

    DrawRectangleRoundedLinesEx(
        card,
        0.12f,
        8,
        2,
        CharacterColor(character->getname()));

    //---------------------------------
    // Layout
    //---------------------------------

    float padding = layout.H(6);

    float hpHeight = layout.H(4);

    float bottomPadding = layout.H(8);

    int fontSize = layout.S(18);

    float nameHeight = (float)fontSize;

    float gap = layout.H(6);

    //---------------------------------
    // Portrait Height
    //---------------------------------

    float portraitHeight =
        card.height
        - padding
        - gap
        - nameHeight
        - gap
        - hpHeight
        - bottomPadding;

    if (portraitHeight < 40)
        portraitHeight = 40;

    Rectangle portrait
    {
        card.x + layout.W(6),
        card.y + padding,
        card.width - layout.W(12),
        portraitHeight
    };

    const Texture2D& tex = GetPortrait(character);

    if(tex.id != 0)
    {
        DrawRectangleRounded(
            portrait,
            0.08f,
            8,
            SidekickPortraitBackground(character));

        DrawTexturePro(
            tex,
            {
                0,
                0,
                (float)tex.width,
                (float)tex.height
            },
            portrait,
            {0,0},
            0,
            WHITE);
    }

    //---------------------------------
    // Name
    //---------------------------------

    std::string name = ShortName(character->getname());

    Vector2 size =
        MeasureTextEx(
            font,
            name.c_str(),
            fontSize,
            1);

    DrawOutlinedText(
        font,
        name,
        {
            card.x + (card.width-size.x)/2,
            portrait.y + portrait.height + gap
        },
        fontSize,
        1,
        WHITE,
        BLACK,
        1);

    //---------------------------------
    // HP
    //---------------------------------

    Rectangle hpBar
    {
        card.x + layout.W(8),
        card.y + card.height - bottomPadding - hpHeight,
        card.width - layout.W(16),
        hpHeight
    };

    DrawHpBar(
        character->getHp(),
        character->getMaxhp(),
        hpBar,
        layout);
}
void StatusView::DrawSidekickSection(Player& player,
                                     Layout layout) const
{
    DrawTextEx(
        font,
        "COMPANIONS",
        { layout.X(20), layout.Y(520) },
        layout.S(32),
        1,
        LIGHTGRAY);

    DrawLine(
        layout.X(20),
        layout.Y(550),
        layout.X(280),
        layout.Y(550),
        BLACK);

    auto& sidekicks = player.getHero()->getSidekicks();

    if(sidekicks.empty())
        return;

    //---------------------------------
    // Card Layout
    //---------------------------------

    float cardWidth  = layout.W(68);
    float cardHeight = layout.H(120) ;

    float gap = layout.W(8);

    float totalWidth =
        sidekicks.size() * cardWidth +
        (sidekicks.size() - 1) * gap;

    float startX =
        layout.X(20) +
        (layout.W(260) - totalWidth) / 2;

    //---------------------------------
    // Draw Cards
    //---------------------------------

    for(auto& sidekick : sidekicks)
    {
        Rectangle card =
        {
            startX,
            layout.Y(565),
            cardWidth,
            cardHeight
        };

        DrawCharacterCard(
            sidekick.get(),
            card,
            layout);

        startX += cardWidth + gap;
    }
}

void StatusView::DrawPlayerPanel(Player& player,
                                 int x,
                                 int y,
                                 int width,
                                 int height) const
{
    auto hero = player.getHero();

    Rectangle panel =
    {
        (float)x,
        (float)y,
        (float)width,
        (float)height
    };

    Layout layout;

    layout.panel = panel;

    layout.sx = panel.width  / DESIGN_WIDTH;
    layout.sy = panel.height / DESIGN_HEIGHT;

    layout.s = std::min(layout.sx, layout.sy);

    DrawRectangleRounded(
        panel,
        0.05f,
        10,
        PanelBackground(hero.get()));

    DrawRectangleRoundedLinesEx(
        panel,
        0.05f,
        10,
        2,
        AccentColor(hero.get()));

    //--------------------------------- 

    const Texture2D& bg = GetBackground(hero.get());

    DrawTexturePro(
        bg,
        {
            0,
            0,
            (float)bg.width,
            (float)bg.height
        },
        panel,
        {0,0},
        0,
        Fade(WHITE,0.4f)
    );

    DrawRectangleRounded(
        panel,
        0.05f,
        10,
        Fade(BLACK,0.18f)
    );

    DrawHeroSection(
        player,layout);

    //---------------------------------

    DrawCardsSection(
        player, layout);

    //---------------------------------

    DrawSidekickSection(
        player, layout);
}
const Texture2D& StatusView::GetPortrait(const Character* character) const
{
        if(character->getname()=="Dracula")
            return dracula;
        if(character->getname()=="Sherlock")
            return sherlock;
        if(character->getname()=="Dr.Watson")
            return watson;
        if(character->getname()=="Sister 1")
            return agatha;
        if(character->getname()=="Sister 2")
            return lucy ;
        if(character->getname()=="Sister 3")
            return mina ;
        return dracula ;
}
    

StatusView::~StatusView()
{
    UnloadTexture(dracula);
    UnloadTexture(sherlock);
    UnloadTexture(watson);
    UnloadTexture(agatha);
    UnloadTexture(lucy);
    UnloadTexture(mina);
    UnloadFont(font);
    UnloadTexture(draculaBackground);
    UnloadTexture(sherlockBackground);
}