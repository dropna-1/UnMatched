#include "Screens/MatchScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "screens/MainScreen.hpp"
#include "Game/Game.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Enums/TypeEnums.hpp"
#include "raygui.h"

MatchScreen::MatchScreen(ScreenManager* mgr) {
    this->manager = mgr;

    actions.setGame(&mgr->GetGame());

    int monitor = GetCurrentMonitor();
    int monitorWidth  = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    SetWindowSize(monitorWidth, monitorHeight);
    SetWindowPosition(0, 0);

    font = LoadFontEx("external/font/GermaniaOne-Regular.ttf", 64, 0, 0);

    float btnW = GetScreenWidth()/12;
    float btnH = 40;
    btnHome = {
        2.0f,
        2.0f,
        btnW,
        btnH
    };
    btnSave = {
        btnW*11-2,
        2.0f,
        btnW,
        btnH
    };

    FillMessage();
}

MatchScreen::~MatchScreen() {
    UnloadFont(font);
    if (background.id != 0)
        UnloadTexture(background);
}

void MatchScreen::HandleInput() {}
void MatchScreen::Update() {}

void MatchScreen::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background,
            {0, 0, (float)background.width, (float)background.height},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
    }
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 255});

    Game& game = manager->GetGame();

    float x = GetScreenWidth();
    float y = GetScreenHeight();
    float boardW = (x*4)/6;
    float boardH = (y*2)/3;
    Rectangle m = {x/6+10, 20, boardW-20, boardH};
    board.Draw(game.getBoard(), m, *game.getCurrentPlayer(), *game.getOtherPlayer());

    if(stage == Stage::SideKickPlacementP1 || stage == Stage::SideKickPlacementP2){
        board.HighlightSpaces(
            game.getSidekickPlacement(game.getCurrentPlayer()->getHero().get()), 
            HighlightType::Selected
        );
        HandleSidekickPlacement(game);
    }
    else if(stage == Stage::SelectManeuverCharacter){
        std::vector<int> characterPlaces;
        for(auto c : game.getCurrentPlayer()->getAllCharacters())
            if(!game.getFreeSpacesNearby(c).empty())
                characterPlaces.push_back(c->getPosition());
        board.HighlightSpaces(characterPlaces, HighlightType::None);
        HandleCharacterSelect(game);
    }
    else if(stage == Stage::ChoiceNode){
        board.HighlightSpaces(
            game.getAvailableMoves(selected, selected->getMovement()), 
            HighlightType::Move
        );
        HandleMove(game);
    }
        
    status.DrawPlayerPanel(*game.getCurrentPlayer(), 2, 46, x/6, boardH-26);
    status.DrawPlayerPanel(*game.getOtherPlayer(), (x*5)/6-2, 46, x/6, boardH-26);

    Rectangle h = {(x*2)/3, boardH+20, x/3, y/3-30-20};
    hand.Draw(*game.getCurrentPlayer()->getHero()->getDeck(), h);
    // وقتی می‌خوای کارت‌ها رو قابل کلیک کنی:
    hand.HighlightCards({0, 2, 4}, HighlightType::Move);   // مثال

    // تو حلقه‌ی اصلی بازی:
    int clickedIndex = hand.GetClickedCard(*game.getCurrentPlayer()->getHero()->getDeck(), h);

    if (clickedIndex != -1)
    {
        // فقط وقتی وارد می‌شه که روی کارت هایلایت‌شده کلیک شده باشه
        // ...
}

    Rectangle b = {2, y*7/9, x/4-2, y/4-30-30-10};
    actions.Draw(b, &stage);

    DrawRectangleRoundedLines({x/4+10, (y*7/9) + (y/4-30-30-10) - (30), x/3, 30}, 0.1f, 1, WHITE);
    DrawTextEx(font, message[stage].c_str(), (Vector2){x/4+17, (y*7/9) + (y/4-100)}, 30, 0.7f, GOLD);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
    GuiSetFont(font);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(GOLD));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);

    if (GuiButton(btnHome, "HOME")) {
        SetWindowSize(890, 500);

    int monitor = GetCurrentMonitor();
    SetWindowPosition(
        (GetMonitorWidth(monitor) - 890) / 2,
        (GetMonitorHeight(monitor) - 500) / 2
    );
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
    }
    if(GuiButton(btnSave, "Save")){
        return;
    }
}

void MatchScreen::HandleSidekickPlacement(Game& game){
    int space = board.GetClickedSpace(); 
    if(space != -1)
        for(auto& side : game.getCurrentPlayer()->getHero()->getSidekicks())
            if(side->getPosition() == -1){
                side->setPosition(space); 
                break;
            }
        bool canChange = true;
        for(auto& side : game.getCurrentPlayer()->getHero()->getSidekicks())
            if(side->getPosition() == -1){canChange = false; break;}

        if(canChange == true){
            if(stage == Stage::SideKickPlacementP1)
                stage = Stage::SideKickPlacementP2;
            else
                stage = Stage::None;
            board.ClearHighlightedSpaces();
            game.changeTurn();
        }
}

void MatchScreen::HandleCharacterSelect(Game& game){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        for(const auto& c : game.getCurrentPlayer()->getAllCharacters()){
            if(c->getPosition() == space){
                selected = c;
                board.ClearHighlightedSpaces();
                stage = Stage::ChoiceNode;
                break;
            }
        }
    }
}

void MatchScreen::HandleMove(Game& game){
    int space = board.GetClickedSpace(); 
        if(space != -1){
            game.performManeuver(selected, space);
            board.ClearHighlightedSpaces();
            selected = nullptr;
            stage = Stage::None;
        }
}


void MatchScreen::FillMessage(){
    message.insert({Stage::SideKickPlacementP1, 
        manager->GetGame().getCurrentPlayer()->getName() + ", Set a Location for your sidekick/s"});
    message.insert({Stage::SideKickPlacementP2, 
        manager->GetGame().getOtherPlayer()->getName() + ", Set a Location for your sidekick/s"});
    message.insert({Stage::None, 
        manager->GetGame().getCurrentPlayer()->getName() + ", Make your move"});
    message.insert({Stage::SelectManeuverCharacter,
        manager->GetGame().getCurrentPlayer()->getName() + ", Choose the character you want to move"});
    message.insert({Stage::ChoiceNode,
        manager->GetGame().getCurrentPlayer()->getName() + ", Choose the place you want to go"});
}