#include "Screens/CardView.hpp"
#include "Game/Cards/Cards.hpp"

CardView::CardView()
{
    // Sherlock
    LoadCard("external/images/cards/holms/administer-aid.png");
    LoadCard("external/images/cards/holms/service-revolver.png"); //
    LoadCard("external/images/cards/holms/fixed-point-in-a-changing-age.png"); //
    LoadCard("external/images/cards/holms/counterpunch.png"); //
    LoadCard("external/images/cards/holms/education-never-ends.png"); //
    LoadCard("external/images/cards/holms/eliminate-the-impossible.png"); //
    LoadCard("external/images/cards/holms/feintS.png"); //
    LoadCard("external/images/cards/holms/master-of-disguise.png"); //
    LoadCard("external/images/cards/holms/the-game-is-afoot.png"); //
    LoadCard("external/images/cards/holms/deduce-strategy.png"); //
    LoadCard("external/images/cards/holms/study-methods.png"); //

    // Dracula
    LoadCard("external/images/cards/dracula/feeding-frenzy.png");
    LoadCard("external/images/cards/dracula/feint (1).png");
    LoadCard("external/images/cards/dracula/dash.png");
    LoadCard("external/images/cards/dracula/thirst-for-sustenance.png");
    LoadCard("external/images/cards/dracula/exploit.png");
    LoadCard("external/images/cards/dracula/baptism-of-blood.png");
    LoadCard("external/images/cards/dracula/ambush.png");
    LoadCard("external/images/cards/dracula/mistform.png");
    LoadCard("external/images/cards/dracula/prey-upon.png");
    LoadCard("external/images/cards/dracula/look-into-my-eyes.png");
    LoadCard("external/images/cards/dracula/ravening-seduction.png");
    LoadCard("external/images/cards/dracula/beastform.png");
}

void CardView::LoadCard(const std::string& path)
{
    textures[path] = LoadTexture(path.c_str());
}

Texture2D CardView::getCardView(const Card& card) const
{
    auto it = textures.find(card.getId());

    if(it != textures.end())
        return it->second;

    return Texture2D{};
}

CardView::~CardView()
{
    for(auto& [path, texture] : textures)
    {
        UnloadTexture(texture);
    }
}