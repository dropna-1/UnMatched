#pragma once
#include "Game/Player/player.hpp"
#include "layout.hpp"

class Character ;

constexpr float DESIGN_WIDTH  = 300.0f;
constexpr float DESIGN_HEIGHT = 700.0f;




class StatusView
{
    public:
        StatusView()
        {
            dracula  = LoadTexture("external/images/dracula/DracArt.png");
            sherlock = LoadTexture("external/images/sherlock/holmsArt.png");
            watson   = LoadTexture("external/images/sherlock/drwatson.png");
            agatha  = LoadTexture("external/images/dracula/sis1.png");
            lucy  = LoadTexture("external/images/dracula/sis2.png");
            mina = LoadTexture("external/images/dracula/sis3.png");
            font = LoadFontEx("external/font/GermaniaOne-Regular.ttf" , 32 , nullptr , 0) ;
            draculaBackground = LoadTexture("external/images/dracula/draculatexture.png");
            sherlockBackground = LoadTexture("external/images/sherlock/sherlocktexture.png") ;
            invisBackground = LoadTexture("external/images/invisibleMan/invisback.png"); 
            invisMan = LoadTexture("external/images/invisibleMan/invArt.png") ;
        }
        void DrawPlayerPanel(Player& player,
                            int x,
                            int y,
                            int width,
                            int height) const;
       ~StatusView() ;

    private:
        void DrawHeroSection(Player& player, Layout layout) const;
        void DrawCardsSection(Player& player, Layout layout) const;
        void DrawSidekickSection(Player& player, Layout layout) const;
        void DrawCharacterCard(const Character* character, Rectangle card,  const Layout& layout) const;
        void DrawHpBar(int hp, int maxHp, Rectangle area , Layout layout) const;
        const Texture2D& GetPortrait(const Character* character) const;
        const Texture2D& GetBackground(const Hero* hero) const ;
        Texture2D dracula;
        Texture2D sherlock;
        Texture2D watson;
        Texture2D agatha;
        Texture2D lucy;
        Texture2D mina;
        Texture2D invisMan ;
        Font font ;
        Texture2D draculaBackground;
        Texture2D sherlockBackground;   
        Texture2D invisBackground ;
};

/*mm
*/