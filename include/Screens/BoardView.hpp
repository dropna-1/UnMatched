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

class BoardView
{
    public:
        BoardView();
        ~BoardView();

        void Draw(const Board& board,
                Rectangle area) const;

    private:
        Texture2D background;
        Texture2D node;
        void DrawBackground(Rectangle area) const;
        void DrawConnections(const Board& board,
                            Rectangle area) const;
        void DrawSpace(Vector2 pos, int id , const std::vector<Color>& colors, Direction direct) const ;
        void DrawNode(Vector2 pos , const std::vector<Color>& colors, Direction direct) const ;
        void DrawSpaces(const Board& board ,Rectangle area) const ;
        void DrawFrame(Rectangle area) const;
        Font font ;
};