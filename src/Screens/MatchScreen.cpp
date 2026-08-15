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
    btnBoost = {
        (float)GetScreenWidth()/6+14,
        (float)GetScreenHeight()*2/3-24,
        (float)GetScreenWidth()/12,
        40,
    };
}

MatchScreen::~MatchScreen() {
    UnloadFont(font);
    if (background.id != 0)
        UnloadTexture(background);
}

void MatchScreen::HandleInput() {}
void MatchScreen::Update() {}

void MatchScreen::DrawSkip(){
    if(stage == Stage::Pending){
        btnSkip = {
            (float)GetScreenWidth()/6+14,
            (float)GetScreenHeight()*2/3-24,
            (float)GetScreenWidth()/12,
            40,
        };
        if (GuiButton(btnSkip, "SKIP")){
            game->completePendingAction();
            board.ClearHighlightedSpaces();
            stage = Stage::None;
        }
    }
    else if(stage == Stage::SelectDefenseCard){
        btnSkip = {
            (float)GetScreenWidth()*2/3,
            (float)GetScreenHeight()*2/3+20,
            (float)GetScreenWidth()/3,
            40,
        };
        if (GuiButton(btnSkip, "SKIP")){
            hand.ClearHighlightedCards();
            game->combat(option, AttackCardIndex, std::nullopt);
            if(game->hasPendingAction())
                stage = Stage::Combat;
            else {stage = Stage::None;}
        }
    }
    else if(stage == Stage::Combat && game->getPendingCombat() != nullptr && 
    game->getPendingCombat()->selection.canFinish){
        btnSkip = {
            (float)GetScreenWidth()/2-356,
            (float)GetScreenHeight()/2+200,
            712,
            40,
        };
        if (GuiButton(btnSkip, "SKIP")){
            game->currentPendingAction()->submit(*game, -1);
            hand.ClearHighlightedCards();
            if(stage == Stage::Combat){
                game->continueCombat();
                if(!game->hasPendingAction())
                    stage = Stage::None;
            }
        }
    }
    else if(stage == Stage::End){
        btnSkip = {
            ((float)GetScreenWidth()-(float)GetScreenWidth()/3)/2,
            (float)GetScreenHeight()/3 + 70,
            (float)GetScreenWidth()/3,
            70,
        };
        GuiSetStyle(DEFAULT, TEXT_SIZE, 60);
        if (GuiButton(btnSkip, "OK")){
            CloseWindow();
        }
        GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
    }
}

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
    if(stage == Stage::SelectDefenseCard || 
    (game->hasPendingAction() && 
    game->currentPendingAction()->getType() == RequestType::DeleteFromOther))
    {
        hand.Draw(*game->getOtherPlayer()->getHero()->getDeck(), h);
    }
    else{
        hand.Draw(*game->getCurrentPlayer()->getHero()->getDeck(), h);
    }

    Rectangle b = {2, y*7/9, x/4-2, y/4-30-30-10};
    actions.Draw(b, &stage);

    Rectangle t = {x/4+4, y*7/9-30, x/4-2, y/8};
    tip.Draw(t, &stage, *game, font);

    if(game->checkWinner() != nullptr)
        stage = Stage::End;

    if(game->hasPendingAction() && stage != Stage::End){
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
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(GOLD));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);

    DrawSkip();

    if (GuiButton(btnHome, "HOME")) {
        manager->BackToHome(890, 500);
        manager->ChangeScreen(std::make_unique<MenuScreen>(manager));
    }
    if(GuiButton(btnSave, "Save")){
        return;
    }
    if(canBoost && stage == Stage::ChoiceNode && GuiButton(btnBoost, "Boost")){
        stage = Stage::Boost;
        board.ClearHighlightedSpaces();
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
            if(!game->getAvailableMoves(c, c->getMovement()).empty())
                characterPlaces.push_back(c->getPosition());
        board.HighlightSpaces(characterPlaces, HighlightType::None);
        HandleCharacterSelect();
        break;
    }
    case Stage::Boost:
    {
        std::vector<int> cardIndexes;
        for(int i = 0; i < game->getCurrentPlayer()->getHero()->getDeck()->getHand().size(); i++)
            cardIndexes.push_back(i);
        hand.HighlightCards(cardIndexes);
        HandleBoost();
        break;
    }
    case Stage::ChoiceNode:
    {
        if(Movement == -1){
            Movement = selected->getMovement();
            canBoost = true;
        }
        board.HighlightSpaces(
            game->getAvailableMoves(selected, Movement), 
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
            if(!game->getPlayableAttackCard(a.attacker).empty())
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
        board.HighlightSpaces(targets, HighlightType::Attack);
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
            if(game->hasPendingAction())
                stage = Stage::Combat;
            else {stage = Stage::None;}
        }else{
            hand.HighlightCards(playableCards);
            HandlePlayCombatCard();
        }
        break;
    }
    case Stage::End:
    {
        int x = GetScreenWidth();
        int y = GetScreenHeight();
        DrawRectangle(0, 0, x, y, {0, 0, 0, 170});
        const char* text = TextFormat("%s IS WINNER", game->checkWinner()->getName().c_str());
        Vector2 titleSize = MeasureTextEx(font, text, 64, 1.0f);
        DrawTextEx(font, text, (Vector2){(x - titleSize.x)/2, (float)y/3}
        , 64, 1.0f, GOLD);
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
void MatchScreen::HandleBoost(){
    int handIndex = hand.GetClickedCard(*game->getCurrentPlayer()->getHero()->getDeck());
    if(handIndex != -1){
        Movement = game->boost(selected, handIndex);
        hand.ClearHighlightedCards();
        stage = Stage::ChoiceNode;
        canBoost = false;
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandleMove(){
    int space = board.GetClickedSpace(); 
    if(space != -1){
        board.ClearHighlightedSpaces();
        game->performManeuver(selected, space);
        selected = nullptr;
        Movement = -1;
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
    int handIndex = -1;
    if(stage == Stage::SelectAttackCard)
        handIndex = hand.GetClickedCard(*game->getCurrentPlayer()->getHero()->getDeck());
    else if(stage == Stage::SelectDefenseCard)
        handIndex = hand.GetClickedCard(*game->getOtherPlayer()->getHero()->getDeck());

    if(handIndex != -1){
        hand.ClearHighlightedCards();
        if(stage == Stage::SelectAttackCard){
            AttackCardIndex = handIndex;
            stage = Stage::SelectDefenseCard;
        }
        else if(stage == Stage::SelectDefenseCard){
            DefenseCardIndex = handIndex;
            game->combat(option, AttackCardIndex, DefenseCardIndex);
            if(game->hasPendingAction())
                stage = Stage::Combat;
            else {stage = Stage::None;}
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
        Rectangle c = {(float)(GetScreenWidth()-712)/2, (float)(GetScreenHeight()-400)/2, 712, 400};
        hand.Draw(*game->getCurrentPlayer()->getHero()->getDeck(), c);
        hand.HighlightCards(action->getOption(*game));
        HandlePendingChooseCard();
        break;
    }
    case RequestType::CardFromOther:
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
        Rectangle c = {(float)(GetScreenWidth()-712)/2, (float)(GetScreenHeight()-400)/2, 712, 400};
        hand.Draw(*game->getOtherPlayer()->getHero()->getDeck(), c);
        hand.HighlightCards(action->getOption(*game));
        HandlePendingChooseCard();
        break;
    }
    case RequestType::ShowCard:
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
        Rectangle c = {(float)(GetScreenWidth()-712)/2, (float)(GetScreenHeight()-400)/2, 712, 400};
        hand.Draw(*game->getOtherPlayer()->getHero()->getDeck(), c);
        game->currentPendingAction()->getOption(*game);
        break;
    }
    case RequestType::RaveningST1:
    case RequestType::Character:
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
    case RequestType::DeleteFromCurrent:
    case RequestType::DeleteFromOther:
    {
        hand.HighlightCards(action->getOption(*game));
        HandlePendingChooseCard();
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
            if(!game->hasPendingAction())
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
            if(!game->hasPendingAction())
                game->continueCombat();
            if(!game->hasPendingAction())
                stage = Stage::None;
        }
    }
}
// ------------------------------------------------------------------------------------------
void MatchScreen::HandlePendingChooseCard(){
    int handIndex = -1;
    switch (game->currentPendingAction()->getType())
    {
    case RequestType::CardFromCurrent:
    case RequestType::DeleteFromCurrent:
        handIndex = hand.GetClickedCard(*game->getCurrentPlayer()->getHero()->getDeck());
        break;
    case RequestType::CardFromOther:
    case RequestType::DeleteFromOther:
        handIndex = hand.GetClickedCard(*game->getOtherPlayer()->getHero()->getDeck());
        break;
    default:
        break;
    }

    if(handIndex != -1){
        game->currentPendingAction()->submit(*game, handIndex);
        hand.ClearHighlightedCards();
        if(stage == Stage::SelectSchemeCard && !game->hasPendingAction()){
            game->continuePlayScheme();
            stage = Stage::None;
        }
        if(stage == Stage::Combat){
            if(!game->hasPendingAction())
                game->continueCombat();
            if(!game->hasPendingAction())
                stage = Stage::None;
        }
    }
}
// ------------------------------------------------------------------------------------------