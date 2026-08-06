#pragma once 
#include "raylib.h"

struct Layout
{
    Rectangle panel;

    float sx;
    float sy;
    float s;

    float X(float x) const
    {
        return panel.x + x * sx;
    }

    float Y(float y) const
    {
        return panel.y + y * sy;
    }

    float W(float w) const
    {
        return w * sx;
    }

    float H(float h) const
    {
        return h * sy;
    }

    float S(float value) const
    {
        return value * s;
    }
};