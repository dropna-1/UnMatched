#include "Screens/BoardView.hpp"

#include "Game/Board/Board.hpp"
#include "Game/Player/player.hpp"
#include "Game/Characters/Character.hpp"

enum class ZoneColor
{
    Red,
    Blue,
    Green,
    Yellow
};

Texture2D BoardView::getCharacterPic(const Character& character) const
{
    if(character.getname() == "Sherlock")
        return shertoken ;
    /*else if(character.getname() == "Dracula")
    {
        return 
    }*/
    else if(character.getname() == "Dr.Watson")
    {
        return watsontoken ;
    }
    else if(character.getname() == "Sister 1")
    {
        return agathatoken ;
    }
    else if(character.getname() == "Sister 2")
    {
        return lucytoken ;
    }
    else if(character.getname() == "Sister 3")
    {
        return minatoken ;
    }
    return lucytoken ;

}

Color GetZoneColor(int zone)
{
    switch(zone)
    {
        case 1:
            return {120,165,190,255} ;
        case 2:
            return {103,66,40,255};

        case 3:
            return {184,163,107,255};

        case 4:
            return {34,86,118,255};

        case 5:
            return {142,108,170,255};

        case 6:
            return {98,150,104,255};

        case 7:
            return {160,164,170,255};

        default:
            return WHITE;
    }
}

BoardView::BoardView()
{
    background = LoadTexture(
        "external/images/Board/BoardBack.png");
    
    node = LoadTexture(
        "external/images/Board/node6.png") ;
    font = LoadFontEx(
        "external/font/GermaniaOne-Regular.ttf" , 40 , nullptr, 0 ) ;
    //dractoken = LoadTexture("external/images/dracula/")
    watsontoken = LoadTexture("external/images/Board/watsonAa.png");
    shertoken = LoadTexture("external/images/Board/sherlockA2.png") ;
    agathatoken = LoadTexture("external/images/Board/sis1.png");
    lucytoken = LoadTexture("external/images/Board/lucyA.png") ;
    minatoken = LoadTexture("external/images/dracula/sis3.png") ;
    SetTextureFilter(node , TEXTURE_FILTER_BILINEAR) ;
    //std::cout <<node.width << endl ; 
}

void BoardView::Draw(const Board& board,
                     Rectangle area , Player& first , Player& second) const
{
    DrawBackground(area);
    DrawConnections(board,area);
    DrawSpaces(board,area);
    DrawCharacters(first , second ,area) ;
    DrawFrame(area) ;
    
}

void BoardView::DrawBackground(Rectangle area) const
{
    DrawTexturePro(
        background,
        {
            0,
            0,
            (float)background.width,
            (float)background.height
        },
        area,
        {0,0},
        0,
        WHITE);
}

void BoardView::DrawNode(Vector2 pos, const std::vector<Color>& colors, Direction direct) const
{
    float size = 70;

    float radius = size * 0.34f;
    if(colors.empty())
        return;

    Rectangle source =
    {
        0,
        0,
        (float)node.width,
        (float)node.height
    };

    Rectangle dest =
    {
        pos.x-size/2,
        pos.y-size/2,
        size,
        size
    };

    DrawTexturePro(
        node,
        source,
        dest,
        {0,0},
        0,
        WHITE);

    if(colors.size()==1)
    {
        DrawCircleV(
            pos,
            radius,
            Fade(colors[0],0.8f));
    }
    else if(colors.size()==2)
    {
        if(direct == Direction::UpDown) 
        {
            DrawCircleSector(
                pos,
                radius,
                0,
                180,
                40,
                Fade(colors[1],0.8f));

            DrawCircleSector(
                pos,
                radius,
                180,
                360,
                40,
                Fade(colors[0],0.8f));
        }
        else 
        {
            DrawCircleSector(
                pos,
                radius,
                -90,
                90,
                40,
                Fade(colors[1],0.8f));

            DrawCircleSector(
                pos,
                radius,
                90,
                270,
                40,
                Fade(colors[0],0.8f));
        }
    }
    else if(colors.size()==3)
    {
        float angle=-210 ;

        for(int i=0;i<3;i++)
        {
            DrawCircleSector(
                pos,
                radius,
                angle,
                angle+120,
                40,
                Fade(colors[i],0.80f));

            angle+=120;
        }
    }
}

void BoardView::DrawSpace(Vector2 pos, int id , const std::vector<Color>& colors , Direction direct) const
{
    DrawNode(pos , colors, direct);

    std::string text = std::to_string(id);

    int size = 30;

    int w = MeasureText(text.c_str(), size);

    DrawTextEx(
        font ,
        text.c_str(),
        {pos.x - w/2, pos.y - size/2},
        size, 
        1 ,
        WHITE);
}

struct spacePosition
{
    Vector2 pos;
    int id;
    Direction direct= Direction::None ;
};

static const spacePosition spaces[] =
{
    {{80,100},0},

    {{220,80},1},

    {{80,230},2},

    {{180,210},3},

    {{270,250},4},

    {{350,90},5},

    {{515,85},6},

    {{428,170},7},

    {{582,190},8},

    {{652,85},9},

    {{727,174},10}, 

    {{798,100},11},

    {{265,333},12, Direction::UpDown},

    {{80,395},13},

    {{170,330},14},

    {{723,278},15},

    {{794,343},16},

    {{582,380},17},

    {{700,380},18},

    {{475,305},19},

    {{395,385},20},

    {{305,455},21},

    {{160,450},22, Direction::UpDown},

    {{160,565},23},

    {{245,590},24},

    {{330,565},25},

    {{430,510},26},

    {{400,630},27},

    {{535,545},28},

    {{620,545},29},

    {{710,600},30},

    {{794,500},31, Direction::UpDown}
};

void BoardView::DrawSpaces(const Board& board,
                           Rectangle area) const
{
    for(const auto& s : spaces)
    {
        Vector2 screenPos =
        {
            area.x + s.pos.x,
            area.y + s.pos.y
        };

        const Space& space = board.getSpace(s.id);

        std::vector<Color> colors;

        for(int zone : space.zone)
        {
            colors.push_back(GetZoneColor(zone));
        }

        DrawSpace(
            screenPos,
            s.id + 1,
            colors,
            s.direct
        );
    }
}

void BoardView::DrawFrame(Rectangle area) const
{
    const float border = 4;

    // Border
    DrawRectangleLinesEx(area, border, DARKBROWN);

    // Inner Border
    Rectangle inner =
    {
        area.x + 6,
        area.y + 6,
        area.width - 12,
        area.height - 12
    };

    DrawRectangleLinesEx(inner, 2, BROWN);

    //-----------------------------
    // Title
    //-----------------------------

    std::string title = "Baskervile manor";

    int fontSize = 40;
    int padding = 18;
    Vector2 size = MeasureTextEx(
    font,
    title.c_str(),
    40,
    1
    );
    float titleWidth = size.x + padding * 2;
    Rectangle titleRect =
    {
        area.x + (area.width - titleWidth) / 2.0f,
        area.y - 18,
        titleWidth,
        52
    };

    DrawRectangleRounded(titleRect, 0.25f, 8, BROWN);

    DrawRectangleRoundedLines(titleRect, 0.25f, 8, DARKBROWN);

    DrawTextEx(
        font,
        title.c_str(),
        {
            titleRect.x + (titleRect.width - size.x) / 2.0f,
            titleRect.y + (titleRect.height - size.y) / 2.0f
        },
        fontSize,
        1,
        WHITE
    );
}

void BoardView::DrawConnections(const Board& board,
                                Rectangle area) const
{
    for(int i = 0; i < board.size(); i++)
    {
        Vector2 start =
        {
            area.x + spaces[i].pos.x,
            area.y + spaces[i].pos.y
        };

        const Space& space = board.getSpace(i);

        for(int neighbor : space.neighbors)
        {
            if(neighbor < i)
                continue;

            Vector2 end =
            {
                area.x + spaces[neighbor].pos.x,
                area.y + spaces[neighbor].pos.y
            };

            DrawLineEx(
                start,
                end,
                6,
                BROWN
            );
        }
    }
}

Vector2 BoardView::GetSpacePosition(int id, Rectangle area) const
{
    return
    {
        area.x + spaces[id].pos.x,
        area.y + spaces[id].pos.y
    };
}

void BoardView::DrawCharacter(const Character& character,
                              Rectangle area) const
{
    if(!character.isAlive())
        return;

    Vector2 pos =
        GetSpacePosition(character.getPosition(), area);

    const float size = 56;
    auto charToken = getCharacterPic(character);  //token ;;
    Rectangle source =
    {
        0,
        0,
        (float)charToken.width,
        (float)charToken.height
    };

    Rectangle dest =
    {
        pos.x - size/2,
        pos.y - size/2,
        size,
        size
    };

    DrawTexturePro(
        charToken,
        source,
        dest,
        {0,0},
        0,
        WHITE
    );
}

void BoardView::DrawCharacters(
    Player& first,
    Player& second,
    Rectangle area) const
{
    std::vector<Character*> characters = first.getAllCharacters();

    for (Character* character : second.getAllCharacters())
        characters.push_back(character);

    for (Character* character : characters)
    {
        if(character == nullptr)
            continue;

        if(!character->isAlive())
            continue;

        DrawCharacter(*character, area);
    }
}


BoardView::~BoardView()
{
    UnloadTexture(background);
    UnloadTexture(node) ;
    UnloadFont(font) ;
    UnloadTexture(shertoken) ;
    UnloadTexture(lucytoken) ;
    UnloadTexture(watsontoken) ;
    UnloadTexture(minatoken); 
    UnloadTexture(agathatoken) ;
}


