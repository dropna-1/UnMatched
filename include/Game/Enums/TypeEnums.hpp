#pragma once

enum HeroType
{
    Sherlock , 
    Dracula , 
    Invisibleman
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
    currentPlayer,
    EnemyPlayer,

    FriendlyHero,
    FriendlySidekicks,
    FriendlyCharacters,

    EnemyHero,
    EenmySidekicks,
    EnemyCharacters,

    CardOwner,
    CardOwnerSidekicks,
    CardOwnerCharacters,

    CardOpponent,
    CardOpponentSidekicks,
    CardOpponentCharacters,
    CardOpponentFighter,

    Attacker,
    Defender,

    SelectedCharacter,
    
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

    CardOwner,
    CardOwnerSidekicks,
    CardOwnerCharacters,

    CardOpponent,
    CardOpponentSidekicks,
    CardOpponentCharacters,
    CardOpponentFighter,

    Attacker,
    Defender
};

enum class RequestType
{
    Move, 
    Character , 
    CardFromCurrent ,
    CardFromOther ,
    RaveningST1 , 
    RaveningST2 ,
    FogST1 ,
    FogST2 ,
    DeleteFromCurrent ,
    DeleteFromOther ,
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

enum class MoveMode
{
    Range,
    Zone,
    AnySpace,
    Neighboor
};

enum class SelectionMode
{
    Current,
    Other,
    All
};

enum class Stage {
    SideKickPlacementP1,
    SideKickPlacementP2,
    SelectManeuverCharacter,
    SelectAttackCharacter,
    SelectDefenseCharacter,
    SelectSchemeCharacter,
    SelectAttackCard,
    SelectDefenseCard,
    SelectSchemeCard,
    ChoiceNode,
    Pending,
    Combat,
    Boost,
    None,
    End
};