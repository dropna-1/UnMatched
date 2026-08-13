#include "Screens/TipView.hpp"
#include "Game/Game.hpp"
#include "Game/Player/player.hpp"

void TipView::Draw(Rectangle t, Stage* stage, Game& game, Font& font){
    std::string text;
    if(game.hasPendingAction())
        text = pendingMessage[game.currentPendingAction()->getType()];
    else{text = message[*stage];}
    
    Vector2 titleSize = MeasureTextEx(font, text.c_str(), 22, 0.2f);
    DrawTextEx(font, text.c_str(), (Vector2){(t.width - titleSize.x) / 2, t.y}, 
    22, 0.2f, GOLD);
}

void TipView::FillMessage(Player& pc, Player& po){
    message.insert({Stage::SideKickPlacementP1, 
        pc.getName() + ", Set a Location for your sidekick/s"});
    message.insert({Stage::SideKickPlacementP2, 
        po.getName() + ", Set a Location for your sidekick/s"});
    message.insert({Stage::None, 
        pc.getName() + ", Make your move"});
    message.insert({Stage::SelectManeuverCharacter,
        pc.getName() + ", Choose the character you want to move"});
    message.insert({Stage::ChoiceNode,
        pc.getName() + ", Choose the place you want to go"});
    message.insert({Stage::SelectSchemeCharacter,
        pc.getName() + ", Choose the character you want to play as"});
    message.insert({Stage::Combat, 
        pc.getName() + " VS " + po.getName()});
    message.insert({Stage::SelectAttackCharacter,
        pc.getName() + ", Select the character you want to attack with"});
    message.insert({Stage::SelectDefenseCharacter,
        pc.getName() + ", Select the character you want to attack"});
    message.insert({Stage::SelectAttackCard,
        pc.getName() + ", Choose a card to attack"});
    message.insert({Stage::SelectDefenseCard,
        pc.getName() + ", Choose a card to defense"});
    // ----------------------------------------------------------------------------------
    pendingMessage.insert({RequestType::Move,
        "Choose the place you want to go"});
    pendingMessage.insert({RequestType::Character,
        "Choose the character you want to play as"});
    pendingMessage.insert({RequestType::RaveningST1,
        "Choose the character you want to move"});
    pendingMessage.insert({RequestType::RaveningST2,
        "Select the location you want it to move to"});
    pendingMessage.insert({RequestType::Dracula,
        "Perform dracula`s ability on a character"});
    pendingMessage.insert({RequestType::ShowCard,
        "Click on one of the cards to skip"});
    pendingMessage.insert({RequestType::CardFromCurrent,
        pc.getName() + ", Choose a specific number of cards"});
    pendingMessage.insert({RequestType::CardFromOther,
        po.getName() + ", Choose a specific number of cards"});
}