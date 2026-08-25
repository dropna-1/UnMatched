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
    const string& description, 
    const string& pathID
)
{
    return make_shared<Card>(
        name,
        type,
        fighter,
        trigger,
        value,
        boost,
        description, 
        pathID
    );
}

void CardFactory::addCopies(
    std::shared_ptr<Deck> deck,
    int count,
    const std::shared_ptr<Card>& card)
{
    for (int i = 0; i < count; ++i)
    {
        auto copy = std::make_shared<Card>(*card);
        for (auto& entry : copy->getEffects())
        {
            if (entry.effect)
                entry.effect = entry.effect->clone();
        }
        deck->addcard(copy);
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
        "Place Dr.Watson in a space adjacent to Holmes. Holmes recovers 1 Health. Draw 1 Card." , 
        "external/images/cards/holms/administer-aid.png"
    ) ;
    AdministerAid->addEffect(TriggerType::None , EffectTarget::FriendlySidekicks, nullptr, make_shared<MoveToAdjacentEffect>());
    AdministerAid->addEffect(
        TriggerType::None , EffectTarget::FriendlyHero , nullptr , make_shared<HealEffect>(1)
    );
    AdministerAid->addEffect(
        TriggerType::None , EffectTarget::currentPlayer , nullptr  ,make_shared<DrawCardEffect>(1)
    );
    addCopies(deck ,2 , AdministerAid) ;
    auto ServiceRevolver = createCard(
        "Service Revolver" , 
        CardType::Attack , 
        FighterType::Sidekick ,     
        TriggerType::None ,  
        5 , 
        3 ,
        "No description.", 
        "external/images/cards/holms/service-revolver.png"
    ) ;
    addCopies(deck , 2 , ServiceRevolver) ;

    auto FPIACA = createCard(
        "Fixed Point In A Changing Age" , 
        CardType::Versalite ,
        FighterType::Sidekick , 
        TriggerType::AfterCombat , 
        3 , 
        1 ,
        "AFTER COMBAT: If Dr.watson is adjacent to Holmes, they each recover 1 health." , 
        "external/images/cards/holms/fixed-point-in-a-changing-age.png"
    );
    auto condition =
    make_shared<AdjacentCondition>(
        ConditionTarget::FriendlyHero,
        ConditionTarget::FriendlySidekicks
    );
    auto ownerAndSidekickAdjacent =
    make_shared<AdjacentCondition>(
        ConditionTarget::CardOwner,
        ConditionTarget::CardOwnerSidekicks
    );
    FPIACA->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::CardOwner,
        ownerAndSidekickAdjacent,
        make_shared<HealEffect>(1)
    );

    FPIACA->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::CardOwnerSidekicks,
        make_shared<AdjacentCondition>(
            ConditionTarget::CardOwner,
            ConditionTarget::CardOwnerSidekicks
        ),
        make_shared<HealEffect>(1)
    );
    addCopies(deck , 2 , FPIACA) ;

    auto CounterPunch = createCard(
        "Counter Punch" ,
        CardType::Versalite , 
        FighterType::Hero , 
        TriggerType::AfterCombat , 
        3 , 
        1 , 
        "AFTER COMBAT: If Holmes is Adjacent to the opsing fighter, deal 2 Damage to that Fighter.", 
        "external/images/cards/holms/counterpunch.png"
    );
    CounterPunch->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::CardOpponentFighter,
        make_shared<AdjacentCondition>(
            ConditionTarget::CardOwner,
            ConditionTarget::CardOpponentFighter
        ),
        make_shared<DamageEffect>(2)
    );
    addCopies(deck , 3 , CounterPunch) ;

    auto EducationNeverEnds = createCard(
        "Education Never Ends" ,
        CardType::Versalite , 
        FighterType::Any , 
        TriggerType::AfterCombat , 
        3 , 
        1 , 
        "AFTER COMBAT: If you won the combat your opponet draws 1 card, if you lost thecombat you draw 2 cards.", 
        "external/images/cards/holms/education-never-ends.png"
    );
    EducationNeverEnds->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::CardOpponent,
        make_shared<WonBattleCondition>(
            ConditionTarget::CardOwner
        ),
        make_shared<DrawCardEffect>(1)
    );

    EducationNeverEnds->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::CardOwner,
        make_shared<LossBattleCondition>(
            ConditionTarget::CardOwner
        ),
        make_shared<DrawCardEffect>(2)
    );
    addCopies(deck , 2 , EducationNeverEnds) ;

    auto EliminateTheImpossible = createCard(
        "Eliminate The Impossible" ,
        CardType::Scheme , 
        FighterType::Hero ,
        TriggerType::None , 
        0 , 
        2 ,
        "Choose one opponet, look at their card hand and choose 1 card for them to discard." ,
        "external/images/cards/holms/eliminate-the-impossible.png"
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
        "IMMEDIATELY: Cancel all effects on your opponet's card." ,
        "external/images/cards/holms/feintS.png"
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
        "Choose an Opponet, Holmes Swaps to", 
        "external/images/cards/holms/master-of-disguise.png"
    );
    MasterOfDisguise->addEffect(TriggerType::None , EffectTarget::FriendlyHero , nullptr , make_shared<SwapEffect>()) ;
    MasterOfDisguise->addEffect(
        TriggerType::None,
        EffectTarget::SelectedCharacter,
        nullptr,
        make_shared<DamageEffect>(1)
    );
    addCopies(deck , 2 , MasterOfDisguise); 


    auto TheGameIsAfoot = createCard(
        "The Game Is AFoot" , 
        CardType::Attack ,
        FighterType::Hero ,
        TriggerType::AfterCombat ,
        5 , 
        2 ,
        "AFTER COMBAT: Move Holmes up to 3 spaces." , 
        "external/images/cards/holms/the-game-is-afoot.png"
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
        "DURING COMBAT: You may change the printed value of the opponet's card to its BOOST value.(if a card does not have a BOOST value, it is treated as 0)", 
        "external/images/cards/holms/deduce-strategy.png"
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
        "AFTER COMBAT: If you won the combat, look at your opponet's hand." , 
        "external/images/cards/holms/study-methods.png"
    );
    StudyMethods->addEffect(TriggerType::AfterCombat, EffectTarget::EnemyHero, make_shared<WonBattleCondition>(
        ConditionTarget::Attacker) , make_shared<ShowHandEffect>()) ;
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
        "DURING COMBAT: This card's value +1 for each Sister in the same zone as the opposing fighter." , 
        "external/images/cards/dracula/feeding-frenzy.png"
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
        "IMMEDIATELY: Cancel all effects on your opponet's card.", 
        "external/images/cards/dracula/feint (1).png"
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
        "AFTER COMBAT: Move your fighter up to 3 spaces.", 
        "external/images/cards/dracula/dash.png"
    );
    Dash->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::CardOwnerCharacters,
        nullptr,
        make_shared<MoveEffect>(3)
    );
    addCopies(deck , 2 , Dash);

    auto ThirstForSustenance = createCard(
        "Thirst for Sustenance" , 
        CardType::Attack , 
        FighterType::Sidekick , 
        TriggerType::AfterCombat ,
        3 ,
        3 , 
        "AFTER COMBAT: If you won the combat, place Dracula in any space Adjacent to the Opposing Fighter" , 
        "external/images/cards/dracula/thirst-for-sustenance.png"
    );
    ThirstForSustenance->addEffect(TriggerType::AfterCombat, EffectTarget::FriendlyHero, 
        make_shared<WonBattleCondition>(ConditionTarget::Attacker),make_shared<ThirstEffect>());
    addCopies(deck , 3 , ThirstForSustenance);

    auto Exploit = createCard(
        "Exploit" , 
        CardType::Versalite ,
        FighterType::Any , 
        TriggerType::AfterCombat ,
        4 , 
        1 , 
        "AFTER COMBAT: Draw 1 card." , 
        "external/images/cards/dracula/exploit.png"
    ); 
    Exploit->addEffect(TriggerType::AfterCombat, EffectTarget::CardOwner, nullptr , make_shared<DrawCardEffect>(1) );
    addCopies(deck ,3 , Exploit);

    auto BaptismOfBlood = createCard(
        "Baptism Of Blood" , 
        CardType::Scheme ,
        FighterType::Hero , 
        TriggerType::None ,
        0 , 
        2 , 
        "Recover 2 health. Return any defeated Sister(if any) to any space in Dracula zone.", 
        "external/images/cards/dracula/baptism-of-blood.png"
    );
    BaptismOfBlood->addEffect(TriggerType::None, EffectTarget::FriendlySidekicks, nullptr, make_shared<ReviveSister>()) ;
    BaptismOfBlood->addEffect(TriggerType::None, EffectTarget::FriendlyHero, nullptr , make_shared<HealEffect>(2)) ;
    addCopies(deck ,2 , BaptismOfBlood) ;

    auto Ambush = createCard(
        "Ambush" , 
        CardType::Attack , 
        FighterType::Any , 
        TriggerType::DuringCombat , 
        2 , 
        2 , 
        "DURING COMBAT: Your opponet discards 1 random card. add its BOOST's value to this card's attacks value." , 
        "external/images/cards/dracula/ambush.png"
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
        "Place Dracula in any space. Gain 1 action." , 
        "external/images/cards/dracula/mistform.png"
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
        "Deal 1 Damage to all opposing fighters adjacent to Dracula, Dracula recover 1 health fo reach damage dealt" ,
        "external/images/cards/dracula/prey-upon.png"
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
        "DURING COMBAT: Add the BOOSt value from your opponet's card to the defense value of this card" ,
        "external/images/cards/dracula/look-into-my-eyes.png"
    );
    LookIntoMyEyes->addEffect(
        TriggerType::DuringCombat,
        EffectTarget::CardOwner,
        nullptr,
        make_shared<LookIntoMyEyesEffect>()
    );
    addCopies(deck , 3 , LookIntoMyEyes) ; 

    auto RaveningSeduction = createCard(
        "Ravening Seduction" , 
        CardType::Scheme , 
        FighterType::Sidekick , 
        TriggerType::None ,
        0 , 
        2 , 
        "Move any Fighter up to 2 spaces.After Moving deal 1 damage to the moved fighter for each sister adjacent to them." ,
        "external/images/cards/dracula/ravening-seduction.png"
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
        "DURING COMBAT: You may discard any number of your cards from your hand. This card's value is +1 for each card you discard.", 
        "external/images/cards/dracula/beastform.png"
    );
    BeastForm->addEffect(TriggerType::DuringCombat, EffectTarget::currentPlayer, nullptr, make_shared<BeastFormEffect>()) ;
    addCopies(deck ,2 , BeastForm) ;
    

    deck->shuffleDeck();
    return deck;
}

shared_ptr<Deck> CardFactory::createInvisibleManDeck()
{
    auto deck = make_shared<Deck>();

    // =========================================================
    // 1. Slip Away
    // 3x Attack - 3 - Boost 2
    // =========================================================

    auto SlipAway = createCard(
        "Slip Away",
        CardType::Attack,
        FighterType::Hero,
        TriggerType::AfterCombat,
        3,
        2,
        "AFTER COMBAT: Move 1 fog token to a space without a fighter, "
        "then place Invisible Man on that space.",
        "external/images/cards/InvisibleMan/slip-away.png"
    );
    SlipAway->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::None,
        nullptr,
        make_shared<SlipAwayEffect>()
    );
    addCopies(deck, 3, SlipAway);


    // =========================================================
    // 2. Surprise Attack
    // 2x Attack - 5 - Boost 1
    // =========================================================

    /*auto SurpriseAttack = createCard(
        "Surprise Attack",
        CardType::Attack,
        FighterType::Hero,
        TriggerType::Immediately,
        5,
        1,
        "IMMEDIATELY: Cancel all effects on your opponent's card. "
        "AFTER COMBAT: If Invisible Man is on a space with a fog token, "
        "move that fog token to another space.",
        "external/images/cards/InvisibleMan/surprise-attack.png"
    );

    addCopies(deck, 2, SurpriseAttack);*/


    // =========================================================
    // 3. Emerge From Mist
    // 2x Attack - 3 - Boost 2
    // =========================================================

    auto EmergeFromMist = createCard(
        "Emerge From Mist",
        CardType::Attack,
        FighterType::Hero,
        TriggerType::DuringCombat,
        3,
        2,
        "DURING COMBAT: If Invisible Man started this turn on "
        "a space with a fog token, this card's value is 5 instead.",
        "external/images/cards/InvisibleMan/emerge-from-mist.png"
    );
    EmergeFromMist->addEffect(
        TriggerType::DuringCombat,
        EffectTarget::None,
        nullptr,
        make_shared<EmergeFromMistEffect>()
    );
    addCopies(deck, 2, EmergeFromMist);



    // =========================================================
    // 4. Coded Notes
    // 2x Defense - 3 - Boost 2
    // =========================================================

    auto CodedNotes = createCard(
        "Coded Notes",
        CardType::Defend,
        FighterType::Hero,
        TriggerType::AfterCombat,
        3,
        2,
        "AFTER COMBAT: Draw 3 cards, then choose 2 cards from "
        "your hand and put them on top of your deck in any order.",
        "external/images/cards/InvisibleMan/coded-notes.png"
    );

    addCopies(deck, 2, CodedNotes);


    // =========================================================
    // 5. Into Thin Air
    // 2x Defense - 4 - Boost 1
    // =========================================================

    auto IntoThinAir = createCard(
        "Into Thin Air",
        CardType::Defend,
        FighterType::Hero,
        TriggerType::AfterCombat,
        4,
        1,
        "AFTER COMBAT: Move Invisible Man up to 1 space. "
        "Your opponent then moves a fog token up to 3 spaces.",
        "external/images/cards/InvisibleMan/into-thin-air.png"
    );
    IntoThinAir->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::None,
        nullptr,
        make_shared<IntoThinAirEffect>()
    );
    addCopies(deck, 2, IntoThinAir);


    // =========================================================
    // 6. Lurking
    // 2x Defense - 2 - Boost 2
    // =========================================================

    auto Lurking = createCard(
        "Lurking",
        CardType::Defend,
        FighterType::Hero,
        TriggerType::AfterCombat,
        2,
        2,
        "AFTER COMBAT: Draw 1 card and choose 1 effect: "
        "move Invisible Man to a space with a fog token, "
        "or move 1 fog token up to 3 spaces.",
        "external/images/cards/InvisibleMan/lurking.png"
    );
    Lurking->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::None,
        nullptr,
        make_shared<LurkingEffect>()
    );
    addCopies(deck, 2, Lurking);


    // =========================================================
    // 7. Rolling Fog
    // 2x Scheme - Boost 1
    // =========================================================

    auto RollingFog = createCard(
        "Rolling Fog",
        CardType::Scheme,
        FighterType::Hero,
        TriggerType::None,
        0,
        1,
        "Move 1 fog token to another space. Gain 1 action.",
        "external/images/cards/InvisibleMan/rolling-fog.png"
    );
    RollingFog->addEffect(
        TriggerType::None,
        EffectTarget::None,
        nullptr,
        make_shared<RollingFogEffect>()
    );

    addCopies(deck, 2, RollingFog);


    // =========================================================
    // 8. Reign of Terror
    // 2x Scheme - Boost 1
    // =========================================================

    auto ReignOfTerror = createCard(
        "Reign of Terror",
        CardType::Scheme,
        FighterType::Hero,
        TriggerType::None,
        0,
        1,
        "If Invisible Man is on a space with a fog token, "
        "deal 2 damage to any one opposing fighter.",
        "external/images/cards/InvisibleMan/reign-of-terror.png"
    );

    ReignOfTerror->addEffect(
        TriggerType::None,
        EffectTarget::None,
        nullptr,
        make_shared<ReignOfTerrorEffect>()
    );

    addCopies(deck, 2, ReignOfTerror);


    // =========================================================
    // 9. Vanish
    // 2x Scheme - Boost 3
    // =========================================================

    /*auto Vanish = createCard(
        "Vanish",
        CardType::Scheme,
        FighterType::Hero,
        TriggerType::None,
        0,
        3,
        "Recover 1 health. Remove Invisible Man from the board. "
        "At the start of your next turn, place Invisible Man in any space. "
        "(If you played this as your first action, end your turn.)",
        "external/images/cards/InvisibleMan/vanish.png"
    );

    addCopies(deck, 2, Vanish);*/


    // =========================================================
    // 10. Step Lightly
    // 2x Scheme - Boost 1
    // =========================================================

    auto StepLightly = createCard(
        "Step Lightly",
        CardType::Scheme,
        FighterType::Hero,
        TriggerType::None,
        0,
        1,
        "Deal 1 damage to one adjacent fighter. "
        "If Invisible Man is on a space with a fog token, "
        "deal 3 damage instead. "
        "Your opponent then moves a fog token up to 2 spaces.",
        "external/images/cards/InvisibleMan/step-lightly.png"
    );
    StepLightly->addEffect(
        TriggerType::None,
        EffectTarget::None,
        nullptr,
        make_shared<StepLightlyEffect>()
    );
    addCopies(deck, 2, StepLightly);


    // =========================================================
    // 11. Covert Preparation
    // 3x Versatile - 2 - Boost 1
    // =========================================================

    /*auto CovertPreparation = createCard(
        "Covert Preparation",
        CardType::Versalite,
        FighterType::Hero,
        TriggerType::AfterCombat,
        2,
        1,
        "AFTER COMBAT: Draw 1 card. Move 1 fog token up to 2 spaces, "
        "then your opponent moves a different fog token up to 2 spaces.",
        "external/images/cards/InvisibleMan/covert-preparation.png"
    );

    addCopies(deck, 3, CovertPreparation);*/


    // =========================================================
    // 12. Impossible to See
    // 2x Versatile - 2 - Boost 2
    // =========================================================

    auto ImpossibleToSee = createCard(
        "Impossible to See",
        CardType::Versalite,
        FighterType::Hero,
        TriggerType::Immediately,
        2,
        2,
        "IMMEDIATELY: The value of your opponent's attack or defense "
        "is 0 and cannot be changed by card effects. "
        "(Other card effects still happen.)",
        "external/images/cards/InvisibleMan/impossible-to-see.png"
    );

    ImpossibleToSee->addEffect(
        TriggerType::Immediately,
        EffectTarget::None,
        nullptr,
        make_shared<ImpossibleToSeeEffect>()
    );

    addCopies(deck, 2, ImpossibleToSee);


    // =========================================================
    // 13. Confound
    // 2x Versatile - 3 - Boost 2
    // =========================================================

    /*auto Confound = createCard(
        "Confound",
        CardType::Versalite,
        FighterType::Hero,
        TriggerType::AfterCombat,
        3,
        2,
        "AFTER COMBAT: Your opponent may choose to discard 1 card. "
        "If they do not, you may move each fog token to any other space.",
        "external/images/cards/InvisibleMan/confound.png"
    );

    addCopies(deck, 2, Confound);*/


    // =========================================================
    // 14. Dreaming of Revenge
    // 2x Versatile - 3 - Boost 1
    // =========================================================

    auto DreamingOfRevenge = createCard(
        "Dreaming of Revenge",
        CardType::Versalite,
        FighterType::Hero,
        TriggerType::AfterCombat,
        3,
        1,
        "AFTER COMBAT: If Invisible Man is on a space with a fog token, "
        "all opposing fighters on spaces with fog tokens take 1 damage.",
        "external/images/cards/InvisibleMan/dreaming-of-revenge.png"
    );

    DreamingOfRevenge->addEffect(
        TriggerType::AfterCombat,
        EffectTarget::None,
        nullptr,
        make_shared<DreamingOfRevengeEffect>()
    );

    addCopies(deck, 2, DreamingOfRevenge);


    // =========================================================
    // Shuffle
    // =========================================================

    deck->shuffleDeck();
    return deck;
}