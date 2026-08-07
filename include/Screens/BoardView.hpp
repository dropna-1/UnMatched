#pragma once
#include "layout.hpp"
#include "Game/Enums/TypeEnums.hpp"
#include <vector>
#include <unordered_map>

class Board;

enum class Direction
{
    LeftRight , 
    UpDown ,
    None 
}; 

constexpr float DESIGN_BOARD_WIDTH = 900.0f ; 
constexpr float DESIGN_BOARD_HEIGHT = 680.0f ;

class Character ;
class Player ;
class BoardView
{
    public:
        BoardView();
        ~BoardView();

        void Draw(const Board& board,
                Rectangle area , Player& ,Player&) const;

        void HighlightSpaces(const std::vector<int>& spaces , HighlightType type);
        void ClearHighlightedSpaces();

    private:
        Texture2D background;
        Texture2D node;
        Texture2D shertoken ;
        Texture2D dractoken ;
        Texture2D token ;
        Texture2D watsontoken ;
        Texture2D agathatoken ;
        Texture2D lucytoken ;
        Texture2D minatoken ;
        void DrawBackground(const Layout& layout) const;
        void DrawConnections(const Board&, const Layout& layout) const;
        void DrawSpace(Vector2 pos, int id , const std::vector<Color>& colors, Direction direct ,bool highlighted,
            HighlightType type,  const Layout& layout) const ;
        void DrawNode(Vector2 pos , const std::vector<Color>& colors, Direction direct ,bool highlighted, 
            HighlightType type, const Layout& layout) const ;
        void DrawSpaces(const Board&, const Layout& layout) const ;
        void DrawFrame(const Layout& layout ) const;
        Font font ;
        void DrawCharacter(const Character&, const Layout& layout) const;
        Vector2 GetSpacePosition(int id, const Layout& layout) const;
        void DrawCharacters(Player&,Player&,const Layout& layout) const;
        Texture2D getCharacterPic(const Character& character) const;
        std::unordered_map<int, HighlightType> highlightedSpaces;
        bool IsHighlighted(int id) const;
        HighlightType GetHighlightType(int id) const;
};  