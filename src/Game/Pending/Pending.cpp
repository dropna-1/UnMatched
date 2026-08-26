#include "Game/Pending/Pending.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Game.hpp"
#include "Game/Player/player.hpp"
#include "Game/Characters/InvisibleMan.hpp"
using namespace std;

RequestType PendingAction::getType() const{
    return type;
}
/*-----------------------------------------------------------------*/
MoveAction::MoveAction(Character* current, Character* other, 
    MoveMode mode, int value) :
currentCharacter(current), otherCharacter(other), 
mode(mode), range(value) {type = RequestType::Move;}


vector<int> MoveAction::getOption(Game& game){
    if(mode == MoveMode::Range){
        spaces = game.getAvailableMoves(currentCharacter, range);
        return spaces;
    }
    if(mode == MoveMode::AnySpace){
        spaces = game.getAllSpaces();
        return spaces;
    }
    if(mode == MoveMode::Zone){
        spaces = game.getSidekickPlacement(currentCharacter);
        return spaces;
    }
    spaces = game.getFreeSpacesNearby(otherCharacter);
    return spaces;
}


void MoveAction::submit(Game& game, int choice){
    game.getPendingCombat().get()->selection.destination = choice;
    game.completePendingAction();
}

Character* MoveAction::getCurrentCharacter() const{return currentCharacter;}
Character* MoveAction::getOtherCharacter() const{return otherCharacter;}
MoveMode MoveAction::getMode() const{return mode;}
int MoveAction::getRange() const{return range;}
/*-----------------------------------------------------------------*/
RaveningAction::RaveningAction(Game& game){
    if(allCharacters.empty()){
        for(auto i : game.getCurrentPlayer()->getAllCharacters())
            allCharacters.push_back(i);
        for(auto i : game.getOtherPlayer()->getAllCharacters())
            allCharacters.push_back(i);
    }
    type = RequestType::RaveningST1;
}

std::vector<int> RaveningAction::getOption(Game& game){
    if(stage == 0){
        vector<int> options;
        for(Character* c : allCharacters)
            options.push_back(c->getPosition());
        return options;
    } 
    return game.getAvailableMoves(selected, 2);;
}

void RaveningAction::submit(Game& game, int choice){
    if(stage == 0){
        for(Character* c : allCharacters)
            if(c->getPosition() == choice)
                selected = c;
        type = RequestType::RaveningST2;
        stage = 1;
    }
    else{
        game.getPendingCombat().get()->selection.character = selected;
        game.getPendingCombat().get()->selection.destination = choice;
        game.completePendingAction();
    }
}

Character* RaveningAction::getSelected() const{return selected;}
int RaveningAction::getStage() const{return stage;}

void RaveningAction::restoreState(Character* c, const int& state){
    stage = state;
    selected = c;

    if(stage == 0)
        type = RequestType::RaveningST1;
    else
        type = RequestType::RaveningST2;
}
/*-----------------------------------------------------------------*/
ChooseCardAction::ChooseCardAction(Player* player, int min, int max, Game& game) : 
selected(player), minCards(min), maxCards(max) {
    if(player == game.getCurrentPlayer())
        type = RequestType::CardFromCurrent;
    else 
        type = RequestType::CardFromOther;
}

std::vector<int> ChooseCardAction::getOption(Game& game){
    vector<int> options;

    auto hand = selected->getHero()->getDeck()->getHand();
    for(int id = 0; id < hand.size(); id++){
        bool selected = false;

        for(int card : selectedCards){
            if(card == id){selected = true; break;}
        }
        if(!selected)
            options.push_back(id);
    }
    if(!game.getPendingCombat()->selection.canFinish && selectedCards.size() >= minCards)
        game.getPendingCombat()->selection.canFinish = true;

    return options;
}

void ChooseCardAction::submit(Game& game, int choice){
    if(choice == -1){
        game.getPendingCombat()->selection.canFinish = false;
        game.getPendingCombat()->selection.showHand = true;
        game.getPendingCombat().get()->selection.cards = selectedCards;
        game.completePendingAction();
        return;
    }
    selectedCards.push_back(choice);
    if(selectedCards.size() == maxCards){
        game.getPendingCombat()->selection.canFinish = false;
        game.getPendingCombat()->selection.showHand = true;
        game.getPendingCombat().get()->selection.cards = selectedCards;
        game.completePendingAction();
    }
}

Player* ChooseCardAction::getSelectedPlayer() const{return selected;}
int ChooseCardAction::getMinCards() const{return minCards;}
int ChooseCardAction::getMaxCards() const{return maxCards;}
const std::vector<int>& ChooseCardAction::getSelectedCards() const{return selectedCards;}

void ChooseCardAction::restoreState(const std::vector<int>& cards){
    selectedCards = cards;
}
/*-----------------------------------------------------------------*/
ShowCardAction::ShowCardAction(Player* player) : selected(player) 
{type = RequestType::ShowCard;}

std::vector<int> ShowCardAction::getOption(Game& game){
    game.getPendingCombat()->selection.canFinish = true;
    return std::vector<int>{};
}

void ShowCardAction::submit(Game& game, int choice){
    game.getPendingCombat()->selection.canFinish = false;
    game.getPendingCombat()->selection.showHand = true;
    game.completePendingAction();
}

Player* ShowCardAction::getSelectedPlayer() const{return selected;}
/*-----------------------------------------------------------------*/
ChooseCharacterAction::ChooseCharacterAction(SelectionMode mode, Character* c) 
: mode(mode), pc(c) {type = RequestType::Character;}

/*std::vector<int> ChooseCharacterAction::getOption(Game& game){
    std::vector<int> characterPositions;
    if(mode == SelectionMode::Neighboors){
        for(auto ch : game.getEnemiesNearby(pc))
            characterPositions.push_back(ch->getPosition());
        return characterPositions;
    }
    if(mode == SelectionMode::Current || mode == SelectionMode::All){
        for(Character* c : game.getCurrentPlayer()->getAllCharacters()){
            if(c->getPosition() == pc->getPosition())
                continue;
            characters.push_back(c);
            characterPositions.push_back(c->getPosition());
        }
    }
    if(mode == SelectionMode::Other || mode == SelectionMode::All){
        for(Character* c : game.getOtherPlayer()->getAllCharacters()){
            if(c->getPosition() == pc->getPosition())
                continue;
            characters.push_back(c);
            characterPositions.push_back(c->getPosition());
        }
    }
    return characterPositions;
}*/

std::vector<int> ChooseCharacterAction::getOption(Game& game)
{
    std::vector<int> characterPositions;

    characters.clear();

    if(mode == SelectionMode::Neighboors)
    {
        for(auto ch : game.getEnemiesNearby(pc))
            characterPositions.push_back(ch->getPosition());

        return characterPositions;
    }

    if(mode == SelectionMode::Current || mode == SelectionMode::All)
    {
        for(Character* c : game.getCurrentPlayer()->getAllCharacters())
        {
            if(pc != nullptr && c->getPosition() == pc->getPosition())
                continue;

            characters.push_back(c);
            characterPositions.push_back(c->getPosition());
        }
    }

    if(mode == SelectionMode::Other || mode == SelectionMode::All)
    {
        for(Character* c : game.getOtherPlayer()->getAllCharacters())
        {
            if(pc != nullptr && c->getPosition() == pc->getPosition())
                continue;

            characters.push_back(c);
            characterPositions.push_back(c->getPosition());
        }
    }

    return characterPositions;
}

void ChooseCharacterAction::submit(Game& game, int choice){
    for(Character* c : characters)
        if(choice == c->getPosition()){
            game.getPendingCombat()->selection.character = c;
            game.completePendingAction();
        }
}

SelectionMode ChooseCharacterAction::getMode() const{return mode;}
Character* ChooseCharacterAction::getCharacter() const{return pc;}
void ChooseCharacterAction::restoreState(Character* c, SelectionMode mode){
    this->pc = c;
    this->mode = mode;
}
/*-----------------------------------------------------------------*/
DeleteCardAction::DeleteCardAction(Game& game, Player* player) : selected(player){
    if(player == game.getCurrentPlayer())
        type = RequestType::DeleteFromCurrent;
    else 
        type = RequestType::DeleteFromOther;
}

std::vector<int> DeleteCardAction::getOption(Game& game){
    vector<int> options;
    for(int id = 0; id < selected->getHero()->getDeck()->getHand().size(); id++)
        options.push_back(id);
    return options;
}

void DeleteCardAction::submit(Game& game, int choice){
    selected->getHero()->getDeck()->discardFromHand(choice);
    if(selected->getHero()->getDeck()->getHandSize() <= 7){
        game.completePendingAction();
        game.useAction();
    }
}

Player* DeleteCardAction::getSelectedPlayer() const{return selected;}
void DeleteCardAction::restoreState(Player* s){selected = s;}
/*-----------------------------------------------------------------*/
DraculaAction::DraculaAction(){type = RequestType::Dracula;}

std::vector<int> DraculaAction::getOption(Game& game){
    vector<int> options;
    neighboors = game.getEnemiesNearby(game.getDracula().get());
    for(Character* c : neighboors)
        options.push_back(c->getPosition());
    return options;
}

void DraculaAction::submit(Game& game, int choice){
    for(Character* c : neighboors)
        if(c->getPosition() == choice){
            game.getDracula().get()->getAbility().get()->execute(
                c, game.getDracula().get()
            );
            break;
        }
    game.completePendingAction();
}
/*-----------------------------------------------------------------*/
FogMoveAction::FogMoveAction( const int& range, bool emptySpaceOnly, Fog* restricted, Fog* excluded)
    : restricted(restricted), excluded(excluded), range(range), emptySpaceOnly(emptySpaceOnly)
{
    type = RequestType::FogST1;
}

std::vector<int> FogMoveAction::getOption(Game& game)
{
    InvisibleMan* inv = game.getInvisibleMan();

    if(inv == nullptr)
        return {};

    if(stage == 0)
    {
        if(restricted != nullptr)
        {
            if(!restricted->isPlaced() ||
               restricted == excluded)
                return {};

            return { restricted->getPosition() };
        }

        std::vector<int> fogs;

        for(auto& fog : inv->getFogs())
        {
            if(!fog.isPlaced())
                continue;

            if(&fog == excluded)
                continue;

            fogs.push_back(fog.getPosition());
        }

        return fogs;
    }

    if(selected == nullptr)
        return {};

    return game.getFogMoves(
        selected,
        range,
        emptySpaceOnly
    );
}

void FogMoveAction::submit(Game& game, int choice)
{
    if(stage == 0)
    {
        if(restricted != nullptr)
        {
            if(choice != restricted->getPosition() ||
               restricted == excluded)
                return;

            selected = restricted;
        }
        else
        {
            for(auto& fog :
                game.getInvisibleMan()->getFogs())
            {
                if(choice == fog.getPosition() &&
                   &fog != excluded)
                {
                    selected = &fog;
                    break;
                }
            }
        }

        if(selected == nullptr)
            return;

        stage = 1;
        type = RequestType::FogST2;

        return;
    }
    game.getPendingCombat()->selection.fog = selected;
    game.getPendingCombat()->selection.destination = choice;
    game.completePendingAction();
}

Fog* FogMoveAction::getSelected() const{return selected;}
int FogMoveAction::getRange() const{return range;}
int FogMoveAction::getStage() const{return stage;}
//added recently
Fog* FogMoveAction::getRestricted() const{return restricted;}
Fog* FogMoveAction::getExcluded() const{return excluded;}
bool FogMoveAction::getEmptySpaceOnly() const{return emptySpaceOnly;}
void FogMoveAction::restoreState( Fog& fog, const int& stage, Fog* restricted, Fog* excluded)
{
    selected = &fog;
    this->stage = stage;
    this->restricted = restricted;
    this->excluded = excluded;
    if(stage == 0)
        type = RequestType::FogST1;
    else
        type = RequestType::FogST2;
}
/*-----------------------------------------------------------------*/
LurkingAction::LurkingAction(){type = RequestType::LurST1;}

vector<int> LurkingAction::getOption(Game& game)
{
    InvisibleMan* inv = game.getInvisibleMan();
    if(inv == nullptr)
        return {};
    std::vector<int> fogs;
    for(auto& fog : inv->getFogs())
    {
        if(!fog.isPlaced())
            continue;
        bool canPlace = true;
        for(auto c : game.getOtherPlayer()->getAllCharacters())
        {
            if(c != nullptr &&
               c->getPosition() == fog.getPosition())
            {
                canPlace = false;
                break;
            }
        }
        if(canPlace)
            fogs.push_back(fog.getPosition());
    }
    if(stage == 0)
    {
        std::vector<int> all;
        if(!fogs.empty())
            all.push_back(inv->getPosition());
        for(auto& fog : inv->getFogs())
        {
            if(fog.isPlaced())
                all.push_back(fog.getPosition());
        }
        return all;
    }
    if(selectedInv != nullptr)
        return fogs;
    if(selectedFog == nullptr)
        return {};
    return game.getFogMoves(
        selectedFog,
        3
    );
}

void LurkingAction::submit(Game& game, int choice){
    if(stage == 0){
        InvisibleMan* inv = game.getInvisibleMan();
        if(choice == inv->getPosition())
            selectedInv = inv;
        else{
            for(auto& fog : inv->getFogs())
                if(choice == fog.getPosition()){
                    selectedFog = &fog;
                    break;
                }
        }
        stage = 1;
        type = RequestType::LurST2;
    }
    else{
        if(selectedFog != nullptr)
            game.getPendingCombat()->selection.fog = selectedFog;
        else if(selectedInv != nullptr)
            game.getPendingCombat()->selection.character = selectedInv;
        game.getPendingCombat()->selection.destination = choice;
        game.completePendingAction();
    }
}

Fog* LurkingAction::getSelectedFog() const{return selectedFog;}
Character* LurkingAction::getSelectedInv() const{return selectedInv;}
int LurkingAction::getStage() const{return stage;}
void LurkingAction::restoreState(Fog* fog, Character* inv , const int& stage){
    this->stage = stage;
    selectedFog = fog;
    selectedInv = inv;
}