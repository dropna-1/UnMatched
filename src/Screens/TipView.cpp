#include "Screens/TipView.hpp"
#include "Game/Game.hpp"
#include "Game/Player/player.hpp"

void TipView::Draw(Rectangle t, Stage* stage, Font& font){
    Vector2 titleSize = MeasureTextEx(font, message[*stage].c_str(), 22, 0.2f);
    DrawTextEx(font, message[*stage].c_str(), (Vector2){(t.width - titleSize.x) / 2, t.y}, 
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
}