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
                           Rectangle area) const
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
        18,
        1,
        WHITE,
        BLACK,
        1
    );
}

void StatusView::DrawHeroSection(Player& player,
                                 Rectangle area) const
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
        {area.x+20, area.y+10},
        34,
        1,
        LIGHTGRAY,
        BLACK,
        2
    );

    Rectangle portrait =
    {
        area.x+16,
        area.y+10,
        area.width - 32,
        area.width - 32 ,
    };

    const Texture2D& tex = GetPortrait(hero.get());

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
        WHITE
    );

    DrawRectangleLinesEx(
        portrait,
        2,
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
        {area.x+25, area.y+300},
        36,
        1,
        CharacterColor(hero->getname()),
        BLACK,
        2   
    );

    DrawHpBar(
        hero->getHp(),
        hero->getMaxhp(),
        {
            area.x+25,
            area.y+ 335,
            area.width - 50 ,
            4
        });
}

void StatusView::DrawCardsSection(Player& player,
                                  Rectangle area) const
{
    auto deck = player.getHero()->getDeck();

    DrawTextEx(
        font , 
        "CARDS",
        {area.x,area.y} ,
        32,
        1, 
        LIGHTGRAY);

    DrawLine(
        area.x,
        area.y + 30,
        area.x + area.width,
        area.y + 30,
        BLACK);

    DrawTextEx(
        font , 
        TextFormat("Hand : %i",
                   deck->getHand().size()),
        {area.x + 10, area.y + 45} ,
        30,
        1 ,
        WHITE);

    DrawTextEx(
        font , 
        TextFormat("Deck : %i",
                   deck->getDrawPileSize()),
        {area.x + 10,area.y + 75},
        30,
        1 , 
        WHITE);

    DrawTextEx(
        font , 
        TextFormat("Discard : %i",
                   deck->getDiscardPileSize()),
        {area.x + 10, area.y + 105} ,
        30,
        1 , 
        WHITE);
}

void StatusView::DrawCharacterCard(const Character* character,
                                   Rectangle area) const
{
    /*DrawRectangleRounded(
        area,
        0.12f,
        8,
        Color{55,60,75,255});*/
    DrawRectangleRounded(
    area,
    0.12f,
    8,
    SidekickBackground(character));

    DrawRectangleRoundedLinesEx(
        area,
        0.12f,
        8,
        2,
        CharacterColor(character->getname()));

    //---------------------------------
    // Portrait
    //---------------------------------

    Rectangle portrait =
    {
        area.x + 6,
        area.y + 6,
        area.width - 12,
        78
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
                WHITE
            );
                    DrawLineEx(
            {portrait.x, portrait.y + portrait.height},
            {portrait.x + portrait.width, portrait.y + portrait.height},
            2,
            Fade(WHITE,0.15f));
        }
    //---------------------------------
    // Name
    //---------------------------------

    std::string name = ShortName(character->getname());

    int fontSize = 20;

    Vector2 size = MeasureTextEx(
        font,
        name.c_str(),
        fontSize,
        1
    );

    float textWidth = size.x;

    /*DrawTextEx(
        font , 
        name.c_str(),
        {area.x + (area.width-textWidth)/2, portrait.y + portrait.height + 6}, 
        fontSize,
        1 ,
        WHITE);*/
    
    DrawOutlinedText(
        font,
        name,
        {
            area.x + (area.width-textWidth)/2,
            portrait.y + portrait.height + 6
        },
        fontSize,
        1,
        WHITE,
        BLACK,
        1
    );

    //---------------------------------
    // HP Bar
    //---------------------------------

    DrawHpBar(
        character->getHp(),
        character->getMaxhp(),
        {
            area.x + 8,
            portrait.y + portrait.height + 28,
            area.width - 16,
            4
        });
}

void StatusView::DrawSidekickSection(Player& player,
                                     Rectangle area) const
{
    DrawTextEx(
        font ,
        "COMPANIONS",
        {area.x, area.y} ,
        32,
        1 ,
        LIGHTGRAY);

    DrawLine(
        area.x,
        area.y + 30,
        area.x + area.width,
        area.y + 30,
        BLACK);

    auto& sidekicks =
        player.getHero()->getSidekicks();

    float cardWidth = 82;
    float gap = 15;

    float totalWidth =
        sidekicks.size()*cardWidth +
        (sidekicks.size()-1)*gap;

    float startX =
        area.x +
        (area.width-totalWidth)/2;

    for(auto& sidekick : sidekicks)
    {
        Rectangle card =
        {
            startX,
            area.y + 45,
            cardWidth,
            145
        };

        DrawCharacterCard(
            sidekick.get(),
            card);

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
        player,
        {
            panel.x,
            panel.y,
            panel.width,
            360
        });

    //---------------------------------

    DrawCardsSection(
        player,
        {
            panel.x + 20,
            panel.y + 380   ,
            panel.width - 40,
            140
        });

    //---------------------------------

    DrawSidekickSection(
        player,
        {
            panel.x + 20,
            panel.y + 520,
            panel.width - 40,
            170
        });
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