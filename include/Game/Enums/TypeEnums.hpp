#pragma once

enum HeroType
{
    Sherlock , 
    Dracula , 
    InvisibleMan
};

enum CardType
{
    Attack , 
    Defend , 
    Scheme ,
    Versalite
};

enum class TriggerType
{
    Immediately,
    DuringCombat,
    AfterCombat , 
    None 
};

enum class FighterType
{
    Hero,
    Sidekick,
    Any
};

enum class AttackType
{
    Melee , 
    Ranged 
};

enum class EffectTarget
{
    currentPlayer , 
    EnemyPlayer ,
    FriendlyHero , 
    FriendlySidekicks , 
    FriendlyCharacters , 
    EnemyHero , 
    EenmySidekicks , 
    EnemyCharacters ,
    Attacker , 
    Defender ,
    None
};

enum class ConditionTarget
{
    FriendlyHero,
    EnemyHero,
    FriendlySidekicks,
    EnemySidekicks,
    FriendlyCharacters,
    EnemyCharacters,
    Attacker,
    Defender
};

enum class RequestType
{
    Move, 
    Character , 
    Card ,
    Ravening , 
    Dracula ,
    ShowCard ,
    None ,
};

enum class HighlightType
{
    None,
    Move,
    Attack,
    Ability,
    Selected
};

enum class Stage {
    SideKickPlacementP1,
    SideKickPlacementP2,
    SelectManeuverCharacter,
    ChoiceNode,
    SelectAttackCharacter,
    SelectSchemeCharacter,
    SelectAttackCard,
    SelectDefenseCard,
    SelectSchemeCard,
    None
};