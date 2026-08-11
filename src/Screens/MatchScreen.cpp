#include "Screens/MatchScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "screens/MainScreen.hpp"
#include "Game/Game.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Enums/TypeEnums.hpp"
#include "Game/Pending/Pending.hpp"
#include "raygui.h"

MatchScreen::MatchScreen(ScreenManager* mgr) {
    this->manager = mgr;
    this->game = &mgr->GetGame();

    actions.setGame(&mgr->GetGame());
    tip.FillMessage(*game->getCurrentPlayer(), *game->getOtherPlayer());

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

    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
    GuiSetFont(font);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(GOLD));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
}

MatchScreen::~MatchScreen() {
    UnloadFont(font);
    if (background.id != 0)
        UnloadTexture(background);
}

void MatchScreen::HandleInput() {}
void MatchScreen::Update() {HandleStage();}

void MatchScreen::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background,
            {0, 0, (float)background.width, (float)background.height},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
    }
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 255});

    float x = GetScreenWidth();
    float y = GetScreenHeight();
    float boardW = (x*4)/6;
    float boardH = (y*2)/3;
    Rectangle m = {x/6+10, 20, boardW-20, boardH};
    board.Draw(game->getBoard(), m, *game->getCurrentPlayer(), *game->getOtherPlayer());

    status.DrawPlayerPanel(*game->getCurrentPlayer(), 2, 46, x/6, boardH-26);
    status.DrawPlayerPanel(*game->getOtherPlayer(), (x*5)/6-2, 46, x/6, boardH-26);

    Rectangle h = {(x*2)/3, boardH+20, x/3, y/3-30-20};
    hand.Draw(*game->getCurrentPlayer()->getHero()->getDeck(), h);

    Rectangle b = {2, y*7/9, x/4-2, y/4-30-30-10};
    actions.Draw(b, &stage);

    Rectangle t = {x/4+4, y*7/9, x/4-2, y/8};
    tip.Draw(t, &stage, font);

    HandleStage(h);

    if (GuiButton(btnHome, "HOME")) {
        manager->BackToHome(890, 500);
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
    }
    if(GuiButton(btnSave, "Save")){
        return;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleStage(Rectangle h){
    switch (stage)
    {
    case Stage::SideKickPlacementP1:
    case Stage::SideKickPlacementP2:
    {
        HandleSidekickPlacement();
        break;
    }
    case Stage::ChoiceNeighboor:
    {
        HandleAbility();
        break;
    }
    case Stage::SelectManeuverCharacter:
    {
        HandleCharacterSelect();
        break;
    }
    case Stage::ChoiceNode:
    {
        HandleMove();
        break;
    }
    case Stage::SelectSchemeCharacter:
    {
        HandleCharacterSelect();
        break;
    }
    case Stage::SelectSchemeCard:
    {
        HandlePlaycard(h);
        break;
    }
    default:
        break;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::DrawStageHighlight(){
    switch (stage)
    {
    case Stage::SideKickPlacementP1:
    case Stage::SideKickPlacementP2:
    {
        board.HighlightSpaces(
            game->getSidekickPlacement(game->getCurrentPlayer()->getHero().get()), 
            HighlightType::Selected
        );
    }
    case Stage::None:
    {
        if(game->hasPendingAction()){
            if(dynamic_cast<DraculaAction*>(game->currentPendingAction()) != nullptr)
                stage = Stage::ChoiceNeighboor;
        }
        break;
    }
    case Stage::ChoiceNeighboor:
    {
        std::vector<int> neighboors = game->currentPendingAction()->getOption(*game);
        if(!neighboors.empty()){
            board.HighlightSpaces(neighboors, HighlightType::None);
        } 
        else {stage = Stage::None;}
        break;
    }
    case Stage::SelectManeuverCharacter:
    {
        std::vector<int> characterPlaces;
        for(auto c : game->getCurrentPlayer()->getAllCharacters())
            if(!game->getFreeSpacesNearby(c).empty())
                characterPlaces.push_back(c->getPosition());
        board.HighlightSpaces(characterPlaces, HighlightType::None);
        break;
    }
    case Stage::ChoiceNode:
    {
        board.HighlightSpaces(
            game->getAvailableMoves(selected, selected->getMovement()), 
            HighlightType::Move
        );
        break;
    }
    case Stage::SelectSchemeCharacter:
    {
        std::vector<int> characterPlaces;
        for(auto c : game->getCurrentPlayer()->getAllCharacters())
            characterPlaces.push_back(c->getPosition());
        board.HighlightSpaces(characterPlaces, HighlightType::None);
        break;
    }
    case Stage::SelectSchemeCard:
    {
        hand.HighlightCards(game->getSchemeCards(selected), HighlightType::Selected);
        break;
    }
    default:
        break;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleSidekickPlacement(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        for(auto& side : game->getCurrentPlayer()->getHero()->getSidekicks())
            if(side->getPosition() == -1){
                side->setPosition(space); 
                break;
            }
        bool canChange = true;
        for(auto& side : game->getCurrentPlayer()->getHero()->getSidekicks())
            if(side->getPosition() == -1){canChange = false; break;}

        if(canChange == true){
            if(stage == Stage::SideKickPlacementP1)
                stage = Stage::SideKickPlacementP2;
            else{
                stage = Stage::None;
            }
            board.ClearHighlightedSpaces();
            game->changeTurn();
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleCharacterSelect(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        for(const auto& c : game->getCurrentPlayer()->getAllCharacters()){
            if(c->getPosition() == space){
                selected = c;
                board.ClearHighlightedSpaces();
                if(stage == Stage::SelectManeuverCharacter)
                    stage = Stage::ChoiceNode;
                else if(stage == Stage::SelectSchemeCharacter)
                    stage = Stage::SelectSchemeCard;
                break;
            }
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleMove(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        game->performManeuver(selected, space);
        board.ClearHighlightedSpaces();
        if(stage != Stage::SelectSchemeCard){
            selected = nullptr;
            stage = Stage::None;
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleAbility(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        PendingAction* action = game->currentPendingAction();
        for(int& c : action->getOption(*game)){
            if(c == space){
                action->submit(*game, space);
                break;
            }
        }
        board.ClearHighlightedSpaces();
        stage = Stage::None;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePlaycard(Rectangle& h){
    int handIndex = hand.GetClickedCard(*game->getCurrentPlayer()->getHero()->getDeck(), h);
    if(handIndex != -1){
        game->playScheme(selected, handIndex);
        hand.ClearHighlightedCards();
        if(game->hasPendingAction()){
            HandlePendingAction(); 
            cout << "pending\n";
        }
        else {
            selected = nullptr;
            stage = Stage::None;
        }
    }
}

void MatchScreen::HandlePendingAction(){
    PendingAction* action = game->currentPendingAction();
    if(dynamic_cast<MoveAction*>(action) != nullptr){
        board.HighlightSpaces(action->getOption(*game), HighlightType::Move);
        pStage = PendingStage::Move;
    }
    if(dynamic_cast<ChooseCardAction*>(action) != nullptr){
        hand.HighlightCards(action->getOption(*game), HighlightType::None);
    }
}
