#pragma once
#include "raylib.h"
#include <vector>

class Board;

enum class Direction
{
    LeftRight , 
    UpDown ,
    None 
}; 

class Character ;
class Player ;
class BoardView
{
    public:
        BoardView();
        ~BoardView();

        void Draw(const Board& board,
                Rectangle area , Player& ,Player& ) const;

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
        void DrawBackground(Rectangle area) const;
        void DrawConnections(const Board& board,
                            Rectangle area) const;
        void DrawSpace(Vector2 pos, int id , const std::vector<Color>& colors, Direction direct) const ;
        void DrawNode(Vector2 pos , const std::vector<Color>& colors, Direction direct) const ;
        void DrawSpaces(const Board& board ,Rectangle area) const ;
        void DrawFrame(Rectangle area) const;
        Font font ;
        void DrawCharacter(const Character& character, Rectangle area) const;
        Vector2 GetSpacePosition(int id, Rectangle area) const;
        void DrawCharacters(Player& first,Player& second,Rectangle area) const;
        Texture2D getCharacterPic(const Character& character) const;
};