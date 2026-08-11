#include "Game/Pending/Pending.hpp"
#include "Game/Cards/Deck.hpp"
#include "Game/Game.hpp"
#include "Game/Player/player.hpp"
using namespace std;

bool PendingAction::isFinished() const {
    return finished;
}
/*-----------------------------------------------------------------*/
MoveAction::MoveAction(Character* current, Character* other, 
    MoveMode mode, int value) :
currentCharacter(current), otherCharacter(other), 
mode(mode), range(value) {}


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
    game.getPendingCombat().get()->selection.destination = spaces.at(choice);
    finished = true;
    game.completePendingAction();
}
/*-----------------------------------------------------------------*/
RaveningAction::RaveningAction(Game& game){
    if(allCharacters.empty()){
        for(auto i : game.getCurrentPlayer()->getAllCharacters())
            allCharacters.push_back(i);
        for(auto i : game.getOtherPlayer()->getAllCharacters())
            allCharacters.push_back(i);
    }
}

std::vector<int> RaveningAction::getOption(Game& game){
    if(stage == 0){
        vector<int> options;
        for(int id = 0; id < allCharacters.size(); id++)
            options.push_back(id);
        return options;
    }
    spaces = game.getAvailableMoves(selected, 2);
    return spaces;
}


void RaveningAction::submit(Game& game, int choice){
    if(stage == 0){
        selected = allCharacters.at(choice);
        stage = 1;
    }
    else{
        game.getPendingCombat().get()->selection.character = selected;
        game.getPendingCombat().get()->selection.destination = spaces.at(choice);
        finished = true;
        game.completePendingAction();
    }
}
/*-----------------------------------------------------------------*/
ChooseCardAction::ChooseCardAction(Player* player, int min, int max) : 
selected(player), minCards(min), maxCards(max) {}

std::vector<int> ChooseCardAction::getOption(Game& game){
    vector<int> options;

    auto hand = selected->getHero().get()->getDeck().get()->getHand();
    for(int id = 0; id < hand.size(); id++){
        bool selected = false;

        for(int card : selectedCards){
            if(card == id){selected = true; break;}
        }
        if(!selected)
            options.push_back(id);
    }
    // if(selectedCards.size() >= minCards)
    //     options.push_back(-1);

    return options;
}

void ChooseCardAction::submit(Game& game, int choice=-1){
    if(choice != -1){
        finished = true;
        game.getPendingCombat().get()->selection.cards = selectedCards;
        return;
    }
    auto hand = selected->getHero().get()->getDeck().get()->getHand();
    selectedCards.push_back(choice);
    if(selectedCards.size() == maxCards){
        finished = true;
        game.getPendingCombat().get()->selection.cards = selectedCards;
        game.completePendingAction();
    }
}
/*-----------------------------------------------------------------*/
ShowCardAction::ShowCardAction(Player* player) : selected(player) {}

std::vector<int> ShowCardAction::getOption(Game& game){
    vector<int> options;

    auto hand = selected->getHero().get()->getDeck().get()->getHand();
    for(int id = 0; id < hand.size(); id++)
        options.push_back(id);

    options.push_back(-1);
    return options;
}

void ShowCardAction::submit(Game& game, int choice){
    game.getPendingCombat()->selection.showHand = true;
    finished = true;
    game.completePendingAction();
}
/*-----------------------------------------------------------------*/
std::vector<int> DraculaAction::getOption(Game& game){
    vector<int> options;
    neighboors = game.getEnemiesNearby();
    for(Character* c : neighboors)
        options.push_back(c->getPosition());
    return options;
}

void DraculaAction::submit(Game& game, int choice){
    if(choice != -1){
        for(Character* c : neighboors)
            if(c->getPosition() == choice){
                game.getDracula().get()->getAbility().get()->execute(
                    c, game.getDracula().get()
                );
                break;
            }
    }
    finished = true;
    game.completePendingAction();
}

