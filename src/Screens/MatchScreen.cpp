#include "Screens/MatchScreen.hpp"
#include "Screens/ScreenManager.hpp"
#include "screens/MainScreen.hpp"
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

    float x = GetScreenWidth();
    float y = GetScreenHeight();
    float boardW = (x*4)/6;
    float boardH = (y*2)/3;
    Rectangle m = {x/6+10, 20, boardW-20, boardH};
    board.Draw(game->getBoard(), m, *game->getCurrentPlayer(), *game->getOtherPlayer());

    status.DrawPlayerPanel(*game->getCurrentPlayer(), 2, 46, x/6, boardH-26);
    status.DrawPlayerPanel(*game->getOtherPlayer(), (x*5)/6-2, 46, x/6, boardH-26);

    Rectangle h = {(x*2)/3, boardH+20, x/3, y/3-30-20};
    if(stage == Stage::SelectDefenseCard)
        hand.Draw(*game->getOtherPlayer()->getHero()->getDeck(), h);
    else{
        hand.Draw(*game->getCurrentPlayer()->getHero()->getDeck(), h);
    }

    Rectangle b = {2, y*7/9, x/4-2, y/4-30-30-10};
    actions.Draw(b, &stage);

    Rectangle t = {x/4+4, y*7/9, x/4-2, y/8};
    tip.Draw(t, &stage, *game, font);

    if(game->hasPendingAction()){
        if(game->currentPendingAction()->getType() == RequestType::Dracula)
            stage = Stage::Pending;
        HandlePendingActionInput();
    }
    else{
        HandleStageInput();
    }

    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
    GuiSetFont(font);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(GOLD));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);

    if (GuiButton(btnHome, "HOME")) {
        manager->BackToHome(890, 500);
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
    }
    if(GuiButton(btnSave, "Save")){
        return;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleStageInput(){
    switch (stage)
    {
    case Stage::SideKickPlacementP1:
    case Stage::SideKickPlacementP2:
    {
        board.HighlightSpaces(
            game->getSidekickPlacement(game->getCurrentPlayer()->getHero().get()), 
            HighlightType::Selected
        );
        HandleSidekickPlacement();
        break;
    }
    case Stage::SelectManeuverCharacter:
    {
        std::vector<int> characterPlaces;
        for(auto c : game->getCurrentPlayer()->getAllCharacters())
            if(!game->getFreeSpacesNearby(c).empty())
                characterPlaces.push_back(c->getPosition());
        board.HighlightSpaces(characterPlaces, HighlightType::None);
        HandleCharacterSelect();
        break;
    }
    case Stage::ChoiceNode:
    {
        board.HighlightSpaces(
            game->getAvailableMoves(selected, selected->getMovement()), 
            HighlightType::Move
        );
        HandleMove();
        break;
    }
    case Stage::SelectSchemeCharacter:
    {
        std::vector<int> characterPlaces;
        for(auto c : game->getCurrentPlayer()->getAllCharacters())
            if(!game->getSchemeCards(c).empty())
                characterPlaces.push_back(c->getPosition());
        board.HighlightSpaces(characterPlaces, HighlightType::None);
        HandleCharacterSelect();
        break;
    }
    case Stage::SelectSchemeCard:
    {
        hand.HighlightCards(game->getSchemeCards(selected));
        HandlePlaySchemeCard();
        break;
    }
    case Stage::SelectAttackCharacter:
    {
        std::vector<int> attakers;
        for(auto& a : game->getAttackableTargets())
            attakers.push_back(a.attacker->getPosition());
        board.HighlightSpaces(attakers, HighlightType::Selected);
        HandleCharacterSelect();
        break;
    }
    case Stage::SelectDefenseCharacter:
    {
        std::vector<int> targets;
        for(auto& a : game->getAttackableTargets())
            if(option.attacker->getPosition() == a.attacker->getPosition())
                targets.push_back(a.target->getPosition());
        board.HighlightSpaces(targets, HighlightType::Selected);
        HandleCharacterSelect();
        break;
    }
    case Stage::SelectAttackCard:
    {
        hand.HighlightCards(game->getPlayableAttackCard(option.attacker));
        HandlePlayCombatCard();
        break;
    }
    case Stage::SelectDefenseCard:
    {
        std::vector<int> playableCards = game->getPlayableDefenseCard(option.target);
        if(playableCards.empty()){
            game->combat(option, AttackCardIndex, nullopt);
            stage = Stage::Combat;
        }else{
            hand.HighlightCards(game->getPlayableDefenseCard(option.target));
            HandlePlayCombatCard();
        }
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
        board.ClearHighlightedSpaces();
        bool canChange = true;
        for(auto& side : game->getCurrentPlayer()->getHero()->getSidekicks())
            if(side->getPosition() == -1){canChange = false; break;}

        if(canChange == true){
            if(stage == Stage::SideKickPlacementP1)
                stage = Stage::SideKickPlacementP2;
            else {
                stage = Stage::None;
                game->setCanUseAbility(true);
            }
            game->changeTurn();
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleCharacterSelect(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        board.ClearHighlightedSpaces();
        for(const auto& c : game->getCurrentPlayer()->getAllCharacters()){
            if(c->getPosition() == space){
                if(stage == Stage::SelectManeuverCharacter){
                    stage = Stage::ChoiceNode;
                    selected = c;
                }
                else if(stage == Stage::SelectSchemeCharacter){
                    stage = Stage::SelectSchemeCard;
                    selected = c;
                }
                else if(stage == Stage::SelectAttackCharacter){
                    stage = Stage::SelectDefenseCharacter;
                    option.attacker = c;
                }
                break;
            }
        }
        if(stage == Stage::SelectDefenseCharacter)
            for(const auto& c : game->getOtherPlayer()->getAllCharacters()){
                if(c->getPosition() == space){
                    stage = Stage::SelectAttackCard;
                    option.target = c;
                    break;
                }
            }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleMove(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        board.ClearHighlightedSpaces();
        game->performManeuver(selected, space);
        selected = nullptr;
        stage = Stage::None;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleAbility(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        game->currentPendingAction()->submit(*game, space);
        board.ClearHighlightedSpaces();
        stage = Stage::None;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePlaySchemeCard(){
    int handIndex = hand.GetClickedCard(*game->getCurrentPlayer()->getHero()->getDeck());
    if(handIndex != -1){
        hand.ClearHighlightedCards();
        game->playScheme(selected, handIndex);
        if(!game->hasPendingAction()){
            selected = nullptr;
            stage = Stage::None;
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePlayCombatCard(){
    int handIndex = hand.GetClickedCard(*game->getCurrentPlayer()->getHero()->getDeck());
    if(handIndex != -1){
        hand.ClearHighlightedCards();
        if(stage == Stage::SelectAttackCard){
            AttackCardIndex = handIndex;
            stage = Stage::SelectDefenseCard;
        }
        else if(stage == Stage::SelectDefenseCard){
            DefenseCardIndex = handIndex;
            game->combat(option, AttackCardIndex, DefenseCardIndex);
            stage = Stage::Combat;
        }
    }
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePendingActionInput(){
    PendingAction* action = game->currentPendingAction();
    switch (action->getType())
    {
    case RequestType::Dracula:
    {
        std::vector<int> neighboors = game->currentPendingAction()->getOption(*game);
        if(!neighboors.empty()){
            board.HighlightSpaces(neighboors, HighlightType::Ability);
            HandleAbility();
        } else {stage = Stage::None; game->completePendingAction();}
        break;
    }
    case RequestType::Move:
    {
        board.HighlightSpaces(action->getOption(*game), HighlightType::Move);
        HandlePendingMove();
        break;
    }
    case RequestType::CardFromCurrent:
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
        Rectangle c = {(float)(GetScreenWidth()-890)/2, (float)(GetScreenHeight()-500)/2, 890, 500};
        pendingHand.Draw(*game->getCurrentPlayer()->getHero()->getDeck(), c);
        pendingHand.HighlightCards(action->getOption(*game));
        HandlePendingChooseCard();
        break;
    }
    case RequestType::CardFromOther:
    case RequestType::ShowCard:
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
        Rectangle c = {(float)(GetScreenWidth()-890)/2, (float)(GetScreenHeight()-500)/2, 890, 500};
        pendingHand.Draw(*game->getOtherPlayer()->getHero()->getDeck(), c);
        pendingHand.HighlightCards(action->getOption(*game));
        HandlePendingChooseCard();
        break;
    }
    case RequestType::RaveningST1:
    {
        board.HighlightSpaces(action->getOption(*game), HighlightType::Selected);
        HandlePendingChooseCharacter();
        break;
    }
    case RequestType::RaveningST2:
    {
        board.HighlightSpaces(action->getOption(*game), HighlightType::Selected);
        HandlePendingMove();
        break;
    }
    default:
        break;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePendingMove(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        game->currentPendingAction()->submit(*game, space);
        board.ClearHighlightedSpaces();
        if(stage == Stage::SelectSchemeCard && !game->hasPendingAction()){
            game->continuePlayScheme();
            stage = Stage::None;
        }
        if(stage == Stage::Combat){
            game->continueCombat();
            if(!game->hasPendingAction())
                stage = Stage::None;
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePendingChooseCharacter(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        game->currentPendingAction()->submit(*game, space);
        board.ClearHighlightedSpaces();
        if(stage == Stage::SelectSchemeCard && !game->hasPendingAction()){
            game->continuePlayScheme();
            stage = Stage::None;
        }
        if(stage == Stage::Combat){
            game->continueCombat();
            if(!game->hasPendingAction())
                stage = Stage::None;
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePendingChooseCard(){
    int handIndex = pendingHand.GetClickedCard(*game->getOtherPlayer()->getHero()->getDeck());
    if(handIndex != -1){
        game->currentPendingAction()->submit(*game, handIndex);
        board.ClearHighlightedSpaces();
        if(stage == Stage::SelectSchemeCard && !game->hasPendingAction()){
            game->continuePlayScheme();
            stage = Stage::None;
        }
        if(stage == Stage::Combat){
            game->continueCombat();
            if(!game->hasPendingAction())
                stage = Stage::None;
        }
    }
}
// ------------------------------------------------------------------------------------------