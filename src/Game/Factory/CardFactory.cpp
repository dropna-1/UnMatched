#include "Game/Factory/CardFactory.hpp"
#include "Game/Effects/Effects.hpp"
#include "Game/Effects/Conditions.hpp"
using namespace std ;

shared_ptr<Card> CardFactory::createCard(
    const string & name ,
    CardType type , 
    FighterType fighter , 
    TriggerType trigger ,
    int value , 
    int boost , 
    const string& description
)
{
    return make_shared<Card>(
        name,
        type,
        fighter,
        trigger,
        value,
        boost,
        description
    );
}

void CardFactory::addCopies(
    std::shared_ptr<Deck> deck,
    int count,
    const std::shared_ptr<Card>& card)
{
    for (int i = 0; i < count; i++)
    {
        deck->addcard(
            std::make_shared<Card>(*card)
        );
    }
}
shared_ptr<Deck> CardFactory::createSherlockDeck()
{
    auto deck = make_shared<Deck>() ;

    auto AdministerAid = createCard(
        "Administer Aid" ,
        CardType::Scheme , 
        FighterType::Sidekick , 
        TriggerType::None , 
        0 , 
        2 ,
        "Place Dr.Watson in a space adjacent to Holmes. Holmes recovers 1 Health. Draw 1 Card." 
    ) ;
    AdministerAid->addEffect(
        TriggerType::None , EffectTarget::FriendlyHero , nullptr , make_shared<HealEffect>(1)
    );
    AdministerAid->addEffect(
        TriggerType::None , EffectTarget::currentPlayer , nullptr  ,make_shared<DrawCardEffect>(1)
    );
    AdministerAid->addEffect(TriggerType::None , EffectTarget::FriendlySidekicks, nullptr, make_shared<MoveToAdjacentEffect>());
    addCopies(deck ,2 , AdministerAid) ;
    
    auto ServiceRevolver = createCard(
        "Service Revolver" , 
        CardType::Attack , 
        FighterType::Sidekick ,     
        TriggerType::None ,  
        5 , 
        3 ,
        "No description."
    ) ;
    addCopies(deck , 2 , ServiceRevolver) ;

    auto FPIACA = createCard(
        "Fixed Point In A Changing Age" , 
        CardType::Versalite ,
        FighterType::Sidekick , 
        TriggerType::AfterCombat , 
        3 , 
        1 ,
        "AFTER COMBAT: If Dr.watson is adjacent to Holmes, they each recover 1 health."
    );
    FPIACA->addEffect(
        TriggerType::AfterCombat , EffectTarget::FriendlyCharacters , make_shared<AdjacentCondition>(
            ConditionTarget::FriendlyHero , ConditionTarget::FriendlySidekicks) , make_shared<HealEffect>(1));
    addCopies(deck , 2 , FPIACA) ;

    auto CounterPunch = createCard(
        "Counter Punch" ,
        CardType::Versalite , 
        FighterType::Hero , 
        TriggerType::AfterCombat , 
        3 , 
        1 , 
        "AFTER COMBAT: If Holmes is Adjacent to the opsing fighter, deal 2 Damage to that Fighter."
    );
    CounterPunch->addEffect(TriggerType::AfterCombat , EffectTarget::EnemyHero , 
        make_shared<AdjacentCondition>(ConditionTarget::FriendlyHero , ConditionTarget::EnemyHero) , make_shared<DamageEffect>(2));
    addCopies(deck , 3 , CounterPunch) ;

    auto EducationNeverEnds = createCard(
        "Education Never Ends" ,
        CardType::Versalite , 
        FighterType::Any , 
        TriggerType::AfterCombat , 
        3 , 
        1 , 
        "AFTER COMBAT: If you won the combat your opponet draws 1 card, if you lost thecombat you draw 2 cards."
    );
    EducationNeverEnds->addEffect(TriggerType::AfterCombat , EffectTarget::EnemyHero , 
        make_shared<WonBattleCondition>(ConditionTarget::FriendlyHero) , make_shared<DrawCardEffect>(1) ) ;
    EducationNeverEnds->addEffect(TriggerType::AfterCombat , EffectTarget::EnemyHero , 
        make_shared<LossBattleCondition>(ConditionTarget::FriendlyHero) , make_shared<DrawCardEffect>(2) ) ;
    addCopies(deck , 2 , EducationNeverEnds) ;

    auto EliminateTheImpossible = createCard(
        "Eliminate The Impossible" ,
        CardType::Scheme , 
        FighterType::Hero ,
        TriggerType::None , 
        0 , 
        2 ,
        "Choose one opponet, look at their card hand and choose 1 card for them to discard." 
    );
    //EliminateTheImpossible->addRequest({RequestType::Card , EffectTarget::EnemyPlayer});
    EliminateTheImpossible->addEffect(TriggerType::None , EffectTarget::EnemyHero ,
        nullptr , make_shared<DiscardCardEffect>(1)) ;
    addCopies(deck , 2 , EliminateTheImpossible);

    auto Feint = createCard(
        "Feint" ,
        CardType::Versalite ,
        FighterType::Any ,
        TriggerType::Immediately , 
        2 ,
        1 ,
        "IMMEDIATELY: Cancel all effects on your opponet's card."
    );
    Feint->addEffect(TriggerType::Immediately , EffectTarget::EnemyHero , nullptr , make_shared<CancelEffectsEffect>());
    addCopies(deck , 3 , Feint); 

    auto MasterOfDisguise = createCard(
        "Master Of Disguise" ,
        CardType::Scheme , 
        FighterType::Hero , 
        TriggerType::None ,
        0 , 
        2 , 
        "Choose an Opponet, Holmes Swaps to"
    );
    MasterOfDisguise->addEffect(TriggerType::None , EffectTarget::FriendlyHero , nullptr , make_shared<SwapEffect>()) ;
    addCopies(deck , 2 , MasterOfDisguise); 


    auto TheGameIsAfoot = createCard(
        "The Game Is AFoot" , 
        CardType::Attack ,
        FighterType::Hero ,
        TriggerType::AfterCombat ,
        5 , 
        2 ,
        "AFTER COMBAT: Move Holmes up to 3 spaces."
    );
    TheGameIsAfoot->addEffect(TriggerType::AfterCombat , EffectTarget::FriendlyHero, nullptr , make_shared<MoveEffect>(3));
    addCopies(deck , 2 , TheGameIsAfoot);
    
    auto DeduceStrategy = createCard(
        "Deduce Strategy" ,
        CardType::Versalite ,
        FighterType::Hero , 
        TriggerType::DuringCombat , 
        3 , 
        1 , 
        "DURING COMBAT: You may change the printed value of the opponet's card to its BOOST value.(if a card does not have a BOOST value, it is treated as 0)"
    );
    DeduceStrategy->addEffect(TriggerType::DuringCombat, EffectTarget::EnemyHero, nullptr, make_shared<DeduceEffect>()); 
    addCopies(deck , 3 , DeduceStrategy) ;

    auto StudyMethods = createCard(
        "Study Methods" , 
        CardType::Versalite ,
        FighterType::Any , 
        TriggerType::AfterCombat , 
        3 , 
        2 , 
        "AFTER COMBAT: If you won the combat, look at your opponet's hand."
    );
    StudyMethods->addEffect(TriggerType::AfterCombat, EffectTarget::EnemyHero, make_shared<WonBattleCondition>(
        ConditionTarget::FriendlyHero) ,
        make_shared<ShowHandEffect>()) ;
    addCopies(deck , 2 , StudyMethods) ;

    deck->shuffleDeck();
    return deck ;
}

shared_ptr<Deck> CardFactory::createDraculaDeck()
{
    auto deck = std::make_shared<Deck>();

    auto FeedingFrenzy = createCard(
        "Feeding Frenzy" , 
        CardType::Attack , 
        FighterType::Hero , 
        TriggerType::DuringCombat , 
        2 , 
        3 ,
        "DURING COMBAT: This card's value +1 for each Sister in the same zone as the opposing fighter."
    );
    FeedingFrenzy->addEffect(TriggerType::DuringCombat, EffectTarget::FriendlySidekicks, nullptr, 
        make_shared<FeedingFrenzyEffect>());
    addCopies(deck , 2 , FeedingFrenzy) ;
        
    auto Feint = createCard(
        "Feint" ,
        CardType::Versalite ,
        FighterType::Any ,
        TriggerType::Immediately , 
        2 ,
        2 ,
        "IMMEDIATELY: Cancel all effects on your opponet's card."
    );
    Feint->addEffect(TriggerType::Immediately , EffectTarget::EnemyHero , nullptr , make_shared<CancelEffectsEffect>());
    addCopies(deck , 3 , Feint); 

    auto Dash = createCard(
        "Dash" , 
        CardType::Versalite ,
        FighterType::Any ,
        TriggerType::AfterCombat ,
        3 , 
        1 ,
        "AFTER COMBAT: Move your fighter up to 3 spaces."
    );
    Dash->addEffect(TriggerType::AfterCombat , EffectTarget::FriendlyCharacters , nullptr , make_shared<MoveEffect>(3));
    addCopies(deck , 2 , Dash);

    auto ThirstForSustenance = createCard(
        "Thirst for Sustenance" , 
        CardType::Attack , 
        FighterType::Sidekick , 
        TriggerType::AfterCombat ,
        3 ,
        3 , 
        "AFTER COMBAT: If you won the combat, place Dracula in any space Adjacent to the Opposing Fighter" 
    );
    ThirstForSustenance->addEffect(TriggerType::AfterCombat, EffectTarget::FriendlyHero, 
        make_shared<WonBattleCondition>(ConditionTarget::FriendlyHero),make_shared<ThirstEffect>());
    addCopies(deck , 3 , ThirstForSustenance);

    auto Exploit = createCard(
        "Exploit" , 
        CardType::Versalite ,
        FighterType::Any , 
        TriggerType::AfterCombat ,
        4 , 
        1 , 
        "AFTER COMBAT: Draw 1 card." 
    ); 
    Exploit->addEffect(TriggerType::AfterCombat, EffectTarget::FriendlyHero, nullptr , make_shared<DrawCardEffect>(1) );
    addCopies(deck ,3 , Exploit);

    auto BaptismOfBlood = createCard(
        "Baptism Of Blood" , 
        CardType::Scheme ,
        FighterType::Hero , 
        TriggerType::None ,
        0 , 
        2 , 
        "Recover 2 health. Return any defeated Sister(if any) to any space in Dracula zone."
    );
    BaptismOfBlood->addEffect(TriggerType::None, EffectTarget::FriendlyHero, nullptr , make_shared<HealEffect>(2)) ;
    BaptismOfBlood->addEffect(TriggerType::None, EffectTarget::FriendlySidekicks, nullptr, make_shared<ReviveSister>()) ;
    addCopies(deck ,2 , BaptismOfBlood) ;

    auto Ambush = createCard(
        "Ambush" , 
        CardType::Attack , 
        FighterType::Any , 
        TriggerType::DuringCombat , 
        2 , 
        2 , 
        "DURING COMBAT: Your opponet discards 1 random card. add its BOOST's value to this card's attacks value." 
    );
    Ambush->addEffect(TriggerType::DuringCombat, EffectTarget::FriendlyHero, nullptr, make_shared<AmbushEffect>()) ;
    addCopies(deck, 2 , Ambush) ;

    auto Mistform = createCard(
        "Mistform",
        CardType::Scheme,
        FighterType::Hero,
        TriggerType::None,
        0,
        2,
        "Place Dracula in any space. Gain 1 action."
    );
    Mistform->addEffect(TriggerType::None, EffectTarget::FriendlyHero, nullptr , make_shared<MoveEffect>(-1)) ;
    Mistform->addEffect(TriggerType::None, EffectTarget::FriendlyHero, nullptr , make_shared<GainActionEffect>()) ;
    addCopies(deck , 2 , Mistform) ;

    auto PreyUpon = createCard(
        "Prey Upon" , 
        CardType::Scheme,
        FighterType::Hero , 
        TriggerType::None,
        0 , 
        4 , 
        "Deal 1 Damage to all opposing fighters adjacent to Dracula, Dracula recover 1 health fo reach damage dealt" 
    );
    PreyUpon->addEffect(TriggerType::None, EffectTarget::EnemyCharacters,nullptr , make_shared<PreyUponEffect>()) ;
    addCopies(deck , 2 , PreyUpon) ;

    auto LookIntoMyEyes = createCard(
        "Look Into My Eyes" , 
        CardType::Defend , 
        FighterType::Hero,
        TriggerType::DuringCombat, 
        1 ,
        2 ,
        "DURING COMBAT: Add the BOOSt value from your opponet's card to the defense value of this card" 
    );
    LookIntoMyEyes->addEffect(TriggerType::DuringCombat, EffectTarget::FriendlyHero, nullptr , make_shared<LookIntoMyEyesEffect>());
    addCopies(deck , 3 , LookIntoMyEyes) ; 

    auto RaveningSeduction = createCard(
        "Ravening Seduction" , 
        CardType::Scheme , 
        FighterType::Sidekick , 
        TriggerType::None ,
        0 , 
        2 , 
        "Move any Fighter up to 2 spaces.After Moving deal 1 damage to the moved fighter for each sister adjacent to them."
    );
    RaveningSeduction->addEffect(TriggerType::None , EffectTarget::FriendlySidekicks , nullptr , make_shared<RaveningEffect>()) ;
    addCopies(deck , 3 , RaveningSeduction) ;

    auto BeastForm = createCard(
        "Beast Form" , 
        CardType::Attack ,
        FighterType::Hero ,
        TriggerType::DuringCombat , 
        6 , 
        4 ,
        "DURING COMBAT: You may discard any number of your cards from your hand. This card's value is +1 for each card you discard."
    );
    BeastForm->addEffect(TriggerType::DuringCombat, EffectTarget::currentPlayer, nullptr, make_shared<BeastFormEffect>()) ;
    addCopies(deck ,2 , BeastForm) ;
    

    deck->shuffleDeck();
    return deck;
}