#include "Game/Ability/InvisiblemanAbility.hpp"
#include "Game/Characters/InvisibleMan.hpp"

std::string InvisibleManAbility::getName() const
{
    return "Invisible Man Ability";
}

std::string InvisibleManAbility::getDescription() const
{
    return
        "While defending on a space with a fog token, "
        "Invisible Man's defense value is increased by 1. "
        "This increase is not a card effect and cannot be canceled. "
        "Invisible Man can also move directly between spaces "
        "containing fog tokens.";
}

bool InvisibleManAbility::HasAbilityOnStart()
{
    return false;
}

bool InvisibleManAbility::allowCancel(
    Card* card,
    GameContext& context)
{
    return true;
}

int InvisibleManAbility::getDefenseBonus(
    Character* defender,
    GameContext& context)
{
    auto* invisibleMan = dynamic_cast<InvisibleMan*>(defender);

    if(invisibleMan == nullptr)
        return 0;

    if(invisibleMan->isOnFog())
        return 1;

    return 0;
}