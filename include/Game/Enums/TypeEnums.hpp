#pragma once

enum HeroType
{
    Sherlock , 
    Dracula 
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
    None ,
};