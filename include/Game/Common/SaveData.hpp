#pragma once
#include <string>
#include <vector>
#include "Game/Enums/TypeEnums.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SidekickSave {
    std::string name;
    int HP = -1;
    int position = -1;
    int index = -1;
};

struct DeckSave {
    std::vector<std::string> drawPile;
    std::vector<std::string> hand;
    std::vector<std::string> discardPile;
};

struct HeroSave {
    HeroType type = HeroType::Sherlock;
    int HP = -1;
    int position = -1;
    std::vector<SidekickSave> sidekicks;
};

struct PlayerSave {
    std::string name;
    int age = -1;
    HeroSave hero;
    DeckSave deck;
};

struct GameSave {
    PlayerSave players[2];

    int currentPlayer = -1;
    int remainingAction = -1;

    bool canUseAbility = false;
};
// ------------------------------------------------------------------------------------------
inline std::string heroTypeToString(HeroType type){
    switch(type)
    {
        case Sherlock:
            return "Sherlock";
        case Dracula:
            return "Dracula";
        case InvisibleMan:
            return "InvisibleMan";
    }
    return "Unknown";
}


inline HeroType stringToHeroType(const std::string& type)
{
    if(type == "Sherlock")
        return Sherlock;

    if(type == "Dracula")
        return Dracula;

    if(type == "InvisibleMan")
        return InvisibleMan;

    return Sherlock;
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const SidekickSave& s)
{
    j = json{
        {"name", s.name},
        {"HP", s.HP},
        {"position", s.position},
        {"index", s.index}
    };
}


inline void from_json(const json& j, SidekickSave& s)
{
    j.at("name").get_to(s.name);
    j.at("HP").get_to(s.HP);
    j.at("position").get_to(s.position);
    j.at("index").get_to(s.index);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const DeckSave& d)
{
    j = json{
        {"drawPile", d.drawPile},
        {"hand", d.hand},
        {"discardPile", d.discardPile}
    };
}


inline void from_json(const json& j, DeckSave& d)
{
    j.at("drawPile").get_to(d.drawPile);
    j.at("hand").get_to(d.hand);
    j.at("discardPile").get_to(d.discardPile);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const HeroSave& h)
{
    j = json{
        {"type", heroTypeToString(h.type)},
        {"HP", h.HP},
        {"position", h.position},
        {"sidekicks", h.sidekicks}
    };
}


inline void from_json(const json& j, HeroSave& h)
{
    std::string type;
    j.at("type").get_to(type);
    h.type = stringToHeroType(type);
    j.at("HP").get_to(h.HP);
    j.at("position").get_to(h.position);
    j.at("sidekicks").get_to(h.sidekicks);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const PlayerSave& p)
{
    j = json{
        {"name", p.name},
        {"age", p.age},
        {"hero", p.hero},
        {"deck", p.deck}
    };
}


inline void from_json(const json& j, PlayerSave& p)
{
    j.at("name").get_to(p.name);
    j.at("age").get_to(p.age);
    j.at("hero").get_to(p.hero);
    j.at("deck").get_to(p.deck);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const GameSave& g)
{
    j = json{
        {"players", json::array({g.players[0], g.players[1]})},
        {"currentPlayer", g.currentPlayer},
        {"remainingAction", g.remainingAction},
        {"canUseAbility", g.canUseAbility}
    };
}


inline void from_json(const json& j, GameSave& g)
{
    j.at("players")[0].get_to(g.players[0]);
    j.at("players")[1].get_to(g.players[1]);
    j.at("currentPlayer").get_to(g.currentPlayer);
    j.at("remainingAction").get_to(g.remainingAction);
    j.at("canUseAbility").get_to(g.canUseAbility);
}