#pragma once

#include "layout.hpp"
#include <unordered_map>
#include <string>

class Card;

class CardView
{
public:
    CardView();
    ~CardView();

    Texture2D getCardView(const Card& card) const;

private:
    std::unordered_map<std::string, Texture2D> textures;

    void LoadCard(const std::string& path);
};