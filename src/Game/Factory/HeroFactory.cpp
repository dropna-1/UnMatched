#include "Game/Factory/HeroFactory.hpp"
#include "Game/Characters/InvisibleMan.hpp"
#include "Game/Characters/Hero.hpp"
#include "Game/Characters/SideKick.hpp"
#include "Game/Factory/CardFactory.hpp"
#include "Game/Ability/SherlockAbility.hpp"
#include "Game/Ability/DraculaAbility.hpp"
#include "Game/Ability/InvisibleManAbility.hpp"

using namespace std ;

shared_ptr<Hero> HeroFactory::createSherlock()
{
    auto hero = make_shared<Hero>("Sherlock" , 16, 2 , AttackType::Melee , HeroType::Sherlock ) ;
    hero->setDeck(CardFactory::createSherlockDeck()) ;
    hero->setAbility(std::make_shared<SherlockAbility>()) ;
    hero->addSidekick(make_shared<SideKick>("Dr.Watson" , 13  , 2 ,AttackType::Ranged)) ;
    return hero ;
}

shared_ptr<Hero> HeroFactory::createDracula()
{
    auto hero = make_shared<Hero>("Dracula" , 13 , 2, AttackType::Melee , HeroType::Dracula) ;
    hero->setDeck(CardFactory::createDraculaDeck()) ;
    hero->setAbility(std::make_shared<DraculaAbility>()) ;
    hero->addSidekick(make_shared<SideKick>("Sister 1" , 1 , 2 , AttackType::Ranged)) ;
    hero->addSidekick(make_shared<SideKick>("Sister 2" , 1 , 2 , AttackType::Ranged)) ;
    hero->addSidekick(make_shared<SideKick>("Sister 3" , 1 , 2 , AttackType::Ranged)) ;
    return hero ;
}

shared_ptr<Hero> HeroFactory::createInvisibleMan()
{
    auto hero = make_shared<InvisibleMan>();
    hero->setDeck(CardFactory::createInvisibleManDeck());
    hero->setAbility(make_shared<InvisibleManAbility>()); 
    return hero;

}