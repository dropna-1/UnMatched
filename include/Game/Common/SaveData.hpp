#pragma once
#include <string>
#include <vector>
#include <optional>
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

struct FogSave {
    int position = -1;
};

struct HeroSave {
    HeroType type = HeroType::Sherlock;
    int HP = -1;
    int position = -1;
    std::vector<SidekickSave> sidekicks;
    std::vector<FogSave> fogs;
};

struct PlayerSave {
    std::string name;
    int age = -1;
    HeroSave hero;
    DeckSave deck;
};

struct CharacterRef
{
    int player = -1;
    int index = -1;
};

struct PendingSave
{
    RequestType type = RequestType::None;

    CharacterRef currentCharacter;
    CharacterRef otherCharacter;
    int mode = 0;
    int range = 0;

    int stage = 0;

    CharacterRef selectedCharacter;
    int destination = -1;

    int player = -1;
    int minCards = 0;
    int maxCards = 0;
    std::vector<int> selectedCards;

    int fogIndex = -1;
};

struct PendingSelectionSave
{
    CharacterRef character;
    std::vector<int> cards;
    int destination = -1;
    int fog = -1;
    bool showHand = false;
    bool canFinish = false;
};

struct PendingCombatSave
{
    CharacterRef attacker;
    CharacterRef target;

    std::string attackCardId;
    std::string defenseCardId;

    int stage = -1;

    PendingSelectionSave selection;
};

struct MatchScreenSave
{
    int stage = -1;

    CharacterRef selected;

    CharacterRef attacker;
    CharacterRef target;

    int attackCardIndex = -1;
    int defenseCardIndex = -1;

    int movement = -1;
    bool canBoost = false;
};

struct GameSave {
    PlayerSave players[2];

    int currentPlayer = -1;
    int remainingAction = -1;

    bool canUseAbility = false;
    MatchScreenSave matchScreen;

    std::vector<PendingSave> pendingActions;
    std::optional<PendingCombatSave> pendingCombat;
};
// ------------------------------------------------------------------------------------------
inline std::string heroTypeToString(HeroType type){
    switch(type)
    {
        case Sherlock:
            return "Sherlock";
        case Dracula:
            return "Dracula";
        case Invisibleman:
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
        return Invisibleman;

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
inline void to_json(json& j, const FogSave& s)
{
    j = json{
        {"position", s.position}
    };
}

inline void from_json(const json& j, FogSave& s)
{
    j.at("position").get_to(s.position);
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
        {"sidekicks", h.sidekicks},
        {"fogs", h.fogs}
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
    j.at("fogs").get_to(h.fogs);
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
inline void to_json(json& j, const CharacterRef& c)
{
    j = json{
        {"player", c.player},
        {"index", c.index}
    };
}

inline void from_json(const json& j, CharacterRef& c)
{
    j.at("player").get_to(c.player);
    j.at("index").get_to(c.index);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const PendingSave& p)
{
    j = json{
        {"type", p.type},

        {"currentCharacter", p.currentCharacter},
        {"otherCharacter", p.otherCharacter},

        {"mode", p.mode},
        {"range", p.range},

        {"stage", p.stage},

        {"selectedCharacter", p.selectedCharacter},
        {"destination", p.destination},

        {"player", p.player},
        {"minCards", p.minCards},
        {"maxCards", p.maxCards},
        {"selectedCards", p.selectedCards},

        {"fogIndex", p.fogIndex}
    };
}

inline void from_json(const json& j, PendingSave& p)
{
    j.at("type").get_to(p.type);

    j.at("currentCharacter").get_to(p.currentCharacter);
    j.at("otherCharacter").get_to(p.otherCharacter);

    j.at("mode").get_to(p.mode);
    j.at("range").get_to(p.range);

    j.at("stage").get_to(p.stage);

    j.at("selectedCharacter").get_to(p.selectedCharacter);
    j.at("destination").get_to(p.destination);

    j.at("player").get_to(p.player);
    j.at("minCards").get_to(p.minCards);
    j.at("maxCards").get_to(p.maxCards);
    j.at("selectedCards").get_to(p.selectedCards);

    j.at("fogIndex").get_to(p.fogIndex);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const PendingSelectionSave& s)
{
    j = json{
        {"character", s.character},
        {"cards", s.cards},
        {"destination", s.destination},
        {"showHand", s.showHand},
        {"canFinish", s.canFinish},
        {"fog", s.fog}
    };
}

inline void from_json(const json& j, PendingSelectionSave& s)
{
    j.at("character").get_to(s.character);
    j.at("cards").get_to(s.cards);
    j.at("destination").get_to(s.destination);
    j.at("showHand").get_to(s.showHand);
    j.at("canFinish").get_to(s.canFinish);
    j.at("fog").get_to(s.fog);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const PendingCombatSave& s)
{
    j = json{
        {"attacker", s.attacker},
        {"target", s.target},
        {"attackCardId", s.attackCardId},
        {"defenseCardId", s.defenseCardId},
        {"stage", s.stage},
        {"selection", s.selection}
    };
}

inline void from_json(const json& j, PendingCombatSave& s)
{
    j.at("attacker").get_to(s.attacker);
    j.at("target").get_to(s.target);
    j.at("attackCardId").get_to(s.attackCardId);
    j.at("defenseCardId").get_to(s.defenseCardId);
    j.at("stage").get_to(s.stage);
    j.at("selection").get_to(s.selection);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const MatchScreenSave& s)
{
    j = json{
        {"stage", s.stage},
        {"selected", s.selected},
        {"attacker", s.attacker},
        {"target", s.target},
        {"attackCardIndex", s.attackCardIndex},
        {"defenseCardIndex", s.defenseCardIndex},
        {"movement", s.movement},
        {"canBoost", s.canBoost}
    };
}

inline void from_json(const json& j, MatchScreenSave& s)
{
    j.at("stage").get_to(s.stage);
    j.at("selected").get_to(s.selected);
    j.at("attacker").get_to(s.attacker);
    j.at("target").get_to(s.target);
    j.at("attackCardIndex").get_to(s.attackCardIndex);
    j.at("defenseCardIndex").get_to(s.defenseCardIndex);
    j.at("movement").get_to(s.movement);
    j.at("canBoost").get_to(s.canBoost);
}
// ------------------------------------------------------------------------------------------
inline void to_json(json& j, const GameSave& g)
{
    j = json{
        {"players", json::array({g.players[0], g.players[1]})},
        {"currentPlayer", g.currentPlayer},
        {"remainingAction", g.remainingAction},
        {"canUseAbility", g.canUseAbility},
        {"pendingActions", g.pendingActions},
        {"pendingCombat", g.pendingCombat},
        {"matchScreen", g.matchScreen}
    };
}

inline void from_json(const json& j, GameSave& g)
{
    j.at("players")[0].get_to(g.players[0]);
    j.at("players")[1].get_to(g.players[1]);
    j.at("currentPlayer").get_to(g.currentPlayer);
    j.at("remainingAction").get_to(g.remainingAction);
    j.at("canUseAbility").get_to(g.canUseAbility);
    j.at("pendingActions").get_to(g.pendingActions);
    j.at("matchScreen").get_to(g.matchScreen);
    if(j.contains("pendingCombat") && !j.at("pendingCombat").is_null())
        g.pendingCombat = j.at("pendingCombat").get<PendingCombatSave>();
    else{
        g.pendingCombat.reset();
    }
}