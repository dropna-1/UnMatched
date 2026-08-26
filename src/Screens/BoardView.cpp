#include "Screens/BoardView.hpp"

#include "Game/Board/board.hpp"
#include "Game/Player/player.hpp"
#include "Game/Characters/Character.hpp"
#include "Game/Enums/TypeEnums.hpp"
#include "Game/Characters/InvisibleMan.hpp"
#include <memory>
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
    else if(character.getname() == "Dracula")
    {
        return dractoken ;
    }
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
    else if(character.getname() == "Invisible Man")
    {
        return InvToken ;
    }
    return lucytoken ;

}

Color GetZoneColor(int zone)
{
    switch(zone)
    {
        case 1:
            return {0,0,0,255};
        case 2:
            return {124,0,140,255};
        case 3:
            return {0,120,0,255};
        case 4:
            return {220,120,0,255};
        case 5:
            return {160,130,0,255};
        case 6:
            return {124,0,0,255};
        case 7:
            return {0,0,210,255};

        default:
            return WHITE;
    }
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
        return {120, 0, 0, 255};
    if(name == "Invisible Man")
    {
        return Color{0, 255 , 255, 255} ;
    }
    return WHITE;
}

BoardView::BoardView()
{
    background = LoadTexture(
        "external/images/Board/BoardBack.png");
    node = LoadTexture(
        "external/images/Board/node6.png") ;
    font = LoadFontEx(
        "external/font/GermaniaOne-Regular.ttf" , 40 , nullptr, 0 ) ;
    dractoken = LoadTexture("external/images/Board/dractos.png") ;
    watsontoken = LoadTexture("external/images/Board/watsono.png");
    shertoken = LoadTexture("external/images/Board/shero.png") ;
    agathatoken = LoadTexture("external/images/Board/agho3.png");
    lucytoken = LoadTexture("external/images/Board/l2.png") ;
    minatoken = LoadTexture("external/images/Board/mina4.png") ;
    InvToken = LoadTexture("external/images/Board/invo1.png") ;
    SetTextureFilter(node , TEXTURE_FILTER_BILINEAR) ;
    SetTextureFilter(minatoken , TEXTURE_FILTER_BILINEAR) ;
    SetTextureFilter(lucytoken , TEXTURE_FILTER_BILINEAR) ;
    SetTextureFilter(agathatoken , TEXTURE_FILTER_BILINEAR) ;
    secretIcon = LoadTexture("external/images/Board/secret1.png");
    fogToken = LoadTexture(
    "external/images/Board/fogg.png");
    SetTextureFilter( secretIcon, TEXTURE_FILTER_BILINEAR);
}

void BoardView::HighlightSpaces(
    const std::vector<int>& spaces,
    HighlightType type)
{
    for(int id : spaces)
        highlightedSpaces[id] = type;
}

void BoardView::ClearHighlightedSpaces()
{
    highlightedSpaces.clear();
}

bool BoardView::IsHighlighted(int id) const
{
    return highlightedSpaces.find(id) != highlightedSpaces.end();
}

HighlightType BoardView::GetHighlightType(int id) const
{
    auto it = highlightedSpaces.find(id);

    if (it == highlightedSpaces.end())
        return HighlightType::None;

    return it->second;
}


void BoardView::Draw(const Board& board,
                     Rectangle area , Player& first , Player& second) const
{
    Layout layout;

    layout.panel = area;

    layout.sx = area.width  / DESIGN_BOARD_WIDTH;
    layout.sy = area.height / DESIGN_BOARD_HEIGHT;

    layout.s  = std::min(layout.sx, layout.sy);

    DrawBackground(layout);
    DrawConnections(board, layout);
    DrawSpaces(board,layout);
    DrawCharacters(first, second, layout);
    DrawFogs(first, second, layout) ;
    DrawSecretPassages(board, layout);
    DrawFrame(layout);
    
}

void BoardView::DrawBackground(const Layout&  layout) const
{
    DrawTexturePro(
        background,
        {
            0,
            0,
            (float)background.width,
            (float)background.height
        },
        layout.panel,
        {0,0},
        0,
        WHITE);
}

void BoardView::DrawNode(Vector2 pos, const std::vector<Color>& colors, Direction direct , bool highlighted,
    HighlightType type, const Layout& layout) const
{
    float size = layout.S(90);

    float radius = size * 0.34f;

    bool hovered = false;

    if(highlighted)
    {
        hovered = CheckCollisionPointCircle(
            GetMousePosition(),
            pos,
            radius
        );

        if(hovered)
        {
            size *= 1.10f;
            radius *= 1.10f;
        }
    }
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

    if(highlighted)
    {
        Color glowColor;
        switch(type)
        {
        case HighlightType::Move:
            glowColor = SKYBLUE;
            break;

        case HighlightType::Attack:
            glowColor = RED;
            break;

        case HighlightType::Ability:
            glowColor = PURPLE;
            break;

        case HighlightType::Selected:
            glowColor= GOLD;
            break;

        default:
            glowColor = YELLOW;
            break;
        }
        Color glow =
        hovered ?
        Fade(glowColor, 0.70f)
        :
        Fade(glowColor, 0.55f);


        DrawCircleV(
            pos,
            radius * 1.40f,
            glow
        );
    }
    DrawTexturePro(
        node,
        source,
        dest,
        {0,0},
        0,
        WHITE);
    
    float alpha = hovered ? 1.0f : 0.8f;

    if(colors.size()==1)
    {
        DrawCircleV(
            pos,
            radius,
            Fade(colors[0],alpha));
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
                Fade(colors[1],alpha));

            DrawCircleSector(
                pos,
                radius,
                180,
                360,
                40,
                Fade(colors[0],alpha));
        }
        else 
        {
            DrawCircleSector(
                pos,
                radius,
                -90,
                90,
                40,
                Fade(colors[1],alpha));

            DrawCircleSector(
                pos,
                radius,
                90,
                270,
                40,
                Fade(colors[0],alpha));
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
                Fade(colors[i],alpha));

            angle+=120;
        }
    }
}

void BoardView::DrawSpace(Vector2 pos, int id , const std::vector<Color>& colors , Direction direct , bool highlighted ,
    HighlightType type,const Layout& layout) const
{
    DrawNode(pos , colors, direct ,highlighted, type , layout);

    std::string text = std::to_string(id);

    int size = layout.S(30);
    Vector2 textSize =
    MeasureTextEx(
        font,
        text.c_str(),
        size,
        1);

    DrawTextEx(
        font,
        text.c_str(),
        {
            pos.x - textSize.x / 2,
            pos.y - textSize.y / 2
        },
        size,
        1,
        WHITE
    );
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

    {{270,200},4},

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

    {{430,510},26 , Direction::UpDown},

    {{400,630},27},

    {{535,545},28},

    {{620,545},29},

    {{710,600},30},

    {{794,500},31, Direction::UpDown}
};

void BoardView::DrawSpaces(const Board& board,
const Layout& layout) const
{
    clickZones.clear();


    for(const auto& s : spaces)
    {
        Vector2 screenPos =
        {
            layout.X(s.pos.x),
            layout.Y(s.pos.y)
        };

        clickZones.push_back(
        {
            s.id,
            screenPos,
            layout.S(90) * 0.34f
        });

        const Space& space = board.getSpace(s.id);

        std::vector<Color> colors;

        for(int zone : space.zone)
        {
            colors.push_back(GetZoneColor(zone));
        }


        bool highlighted = IsHighlighted(s.id);
        HighlightType type = GetHighlightType(s.id);


        DrawSpace(
            screenPos,
            s.id + 1,
            colors,
            s.direct,
            highlighted,
            type,
            layout
        );
    }
}

void BoardView::DrawFrame( const Layout& layout ) const
{
    const float border = layout.S(4);

    // Border
    DrawRectangleLinesEx(layout.panel, border, GRAY);

    // Inner Border
    /*Rectangle inner =
    {
        layout.panel.x + layout.S(6),
        layout.panel.y + layout.S(6),
        layout.panel.width - layout.S(6),
        layout.panel.height - layout.S(6)
    };

    DrawRectangleLinesEx(inner, 2, GRAY);*/

    //-----------------------------
    // Title
    //-----------------------------

    std::string title = "Baskervile manor";

    int fontSize = layout.S(40);
    int padding = layout.S(18);
    Vector2 size = MeasureTextEx(
    font,
    title.c_str(),
    layout.S(40),
    1
    );
    float titleWidth = size.x + padding * 2;
    Rectangle titleRect =
    {
        layout.panel.x + (layout.panel.width - titleWidth) / 2.0f,
        layout.panel.y - layout.S(18),
        titleWidth,
        layout.S(52)
    };

    DrawRectangleRounded(titleRect, 0.25f, 8, BLACK);

    DrawRectangleRoundedLines(titleRect, 0.25f, 8, GRAY);

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
                                const Layout& layout) const
{
    for(int i = 0; i < board.size(); i++)
    {
        Vector2 start =
        {
            layout.X(spaces[i].pos.x),
            layout.Y(spaces[i].pos.y)
        };

        const Space& space = board.getSpace(i);

        for(int neighbor : space.neighbors)
        {
            if(neighbor < i)
                continue;

            Vector2 end =
            {
                layout.X(spaces[neighbor].pos.x),
                layout.Y(spaces[neighbor].pos.y)
            };

            DrawLineEx(
                start,
                end,
                layout.S(3),
                GRAY
            );
        }
    }
}

Vector2 BoardView::GetSpacePosition(int id, const Layout& layout) const
{
    return
    {
        layout.X(spaces[id].pos.x),
        layout.Y(spaces[id].pos.y)
    };
}

void BoardView::DrawCharacter(const Character& character,
                              const Layout& layout) const
{
    if(!character.isAlive() || character.getPosition() < 0)
        return;
    int charpos = character.getPosition() ; 
    if(charpos < 0 || charpos > 31)
    {
        return ;
    }
    Vector2 pos =
        GetSpacePosition(character.getPosition(), layout);

    const float size = layout.S(80);
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
    auto color = CharacterColor(character.getname()) ;
    DrawCircleLinesV(pos, size/2, Fade(BLACK, 0.55f));
    DrawCircleLinesV(pos, size/2 - 1.0f, Fade(color , 0.8f));
}

void BoardView::DrawCharacters(
    Player& first,
    Player& second,
    const Layout& layout) const
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
        DrawCharacter(*character, layout);
    }
}

int BoardView::GetClickedSpace() const
{
    if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return -1;


    Vector2 mouse = GetMousePosition();


    for(const auto& zone : clickZones)
    {
        if(CheckCollisionPointCircle(
            mouse,
            zone.center,
            zone.radius
        ))
        {
            if(IsHighlighted(zone.id))
            {
                            return zone.id;
            }
            return -1 ;
        }
    }


    return -1;
}

void BoardView::DrawSecretPassages(
    const Board& board,
    const Layout& layout
) const
{
    for(const auto& s : spaces)
    {
        const Space& space = board.getSpace(s.id);

        // این خانه Secret Passage ندارد
        if(space.secret.empty())
            continue;

        Vector2 pos =
        {
            layout.X(s.pos.x),
            layout.Y(s.pos.y)
        };

        // اندازه Node
        float nodeSize = layout.S(90);
        float radius = nodeSize * 0.34f;

        // اندازه آیکون
        float iconSize = layout.S(22);

        // مرکز آیکون:
        // بخشی داخل Node و بخشی خارج آن
        Vector2 iconPos =
        {
            pos.x + radius * 0.15f,
            pos.y - radius * 0.75f
        };

        Rectangle source =
        {
            0,
            0,
            (float)secretIcon.width,
            (float)secretIcon.height
        };

        Rectangle dest =
        {
            iconPos.x - iconSize / 2,
            iconPos.y - iconSize / 2,
            iconSize,
            iconSize
        };

        DrawTexturePro(
            secretIcon,
            source,
            dest,
            {
                iconSize / 2,
                iconSize / 2
            },
            0,
            WHITE
        );
    }
}

void BoardView::DrawFogs(
    Player& first,
    Player& second,
    const Layout& layout
) const
{
    Player* players[] =
    {
        &first,
        &second
    };

    for(Player* player : players)
    {
        if(player == nullptr)
            continue;

        auto hero = player->getHero();

        if(!hero)
            continue;

        auto invisibleMan =
            std::dynamic_pointer_cast<InvisibleMan>(hero);

        if(!invisibleMan)
            continue;

        for(const Fog& fog : invisibleMan->getFogs())
        {
            if(!fog.isPlaced())
                continue;

            int position = fog.getPosition();

            if(position < 0 || position >= 32)
                continue;

            Vector2 pos =
                GetSpacePosition(position, layout);

            const float size = layout.S(80);

            Rectangle source =
            {
                0,
                0,
                (float)fogToken.width,
                (float)fogToken.height
            };

            Rectangle dest =
            {
                pos.x - size / 2,
                pos.y - size / 2,
                size,
                size
            };

            DrawTexturePro(
                fogToken,
                source,
                dest,
                {0, 0},
                0,
                Fade(WHITE , 0.5f) 
            );
        }
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
    UnloadTexture(secretIcon) ;
    UnloadTexture(InvToken) ;
    UnloadTexture(fogToken);
}


